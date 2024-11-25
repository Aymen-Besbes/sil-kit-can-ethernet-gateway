/* Copyright (c) 2022 Vector Informatik GmbH

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include <algorithm>
#include <cstring>
#include <future>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <string>
#ifdef SILKIT_HOURGLASS
#include "silkit/hourglass/SilKit.hpp"
#include "silkit/hourglass/config/IParticipantConfiguration.hpp"
#else
#include "silkit/SilKit.hpp"
#endif
#include "silkit/SilKitVersion.hpp"

#include "silkit/services/all.hpp"
#include "silkit/services/orchestration/all.hpp"
#include "silkit/services/orchestration/string_utils.hpp"


using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Ethernet;

using namespace std::chrono_literals;
#include "silkit/SilKit.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/orchestration/all.hpp"
#include "silkit/services/orchestration/string_utils.hpp"
#include "silkit/services/can/all.hpp"
#include "silkit/services/can/string_utils.hpp"

using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Can;
using namespace SilKit::Services::Logging;

using namespace std::chrono_literals;

namespace std {
namespace chrono {
std::ostream& operator<<(std::ostream& out, nanoseconds timestamp)
{
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(timestamp);
    out << seconds.count() << "s";
    return out;
}
} // namespace chrono
} // namespace std


std::string GlobalPayload="";
void FrameHandler(const CanFrameEvent& frameEvent, ILogger* logger)
{
    std::string payload(frameEvent.frame.dataField.begin(), frameEvent.frame.dataField.end());
    std::stringstream buffer;
    buffer << ">> CAN frame: canId=" << frameEvent.frame.canId
           << " timestamp=" << frameEvent.timestamp
           << " \"" << payload << "\"";
    logger->Info(buffer.str());
    GlobalPayload = payload;
}




/************************************************* */

using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Ethernet;

using namespace std::chrono_literals;

// Field in a frame that can indicate the protocol, payload size, or the start of a VLAN tag
using EtherType = uint16_t;
using EthernetMac = std::array<uint8_t, 6>;

std::ostream& operator<<(std::ostream& out, std::chrono::nanoseconds timestamp)
{
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(timestamp);
    out << seconds.count() << "s";
    return out;
}

std::vector<uint8_t> CreateFrame(const EthernetMac& destinationAddress, const EthernetMac& sourceAddress,
                                 const std::vector<uint8_t>& payload)
{
    const uint16_t etherType = 0x0000;  // no protocol

    //std::vector<uint8_t> raw;
    std::vector<uint8_t> raw = {0x01, 0x00, 0x5e, 0x60, 0xe0, 0xf5, 0x8c, 0x16, 0x45, 0x04, 0x10, 0xa4, 0x08, 0x00, 0x45, 0x00,
                                               0x00, 0x64, 0x2b, 0x77, 0x40, 0x00, 0x01, 0x11, 0xcc, 0x28, 0xac, 0x10, 0x14, 0x03, 0xe0, 0xe0,
                                               0xe0, 0xf5, 0x77, 0x1a, 0x77, 0x1a, 0x00, 0x50, 0x82, 0x4b};

    // std::copy(destinationAddress.begin(), destinationAddress.end(), std::back_inserter(raw));
    // std::copy(sourceAddress.begin(), sourceAddress.end(), std::back_inserter(raw));
    // auto etherTypeBytes = reinterpret_cast<const uint8_t*>(&etherType);
    // raw.push_back(etherTypeBytes[1]);  // We assume our platform to be little-endian
    // raw.push_back(etherTypeBytes[0]);
    std::copy(payload.begin(), payload.end(), std::back_inserter(raw));

    return raw;
}

std::string GetPayloadStringFromFrame(const EthernetFrame& frame)
{
    const size_t FrameHeaderSize = 2 * sizeof(EthernetMac) + sizeof(EtherType);

    std::vector<uint8_t> payload;
    payload.insert(payload.end(), frame.raw.begin() + FrameHeaderSize, frame.raw.end());
    std::string payloadString(payload.begin(), payload.end());
    return payloadString;
}

void FrameTransmitHandler(IEthernetController* /*controller*/, const EthernetFrameTransmitEvent& frameTransmitEvent)
{
    if (frameTransmitEvent.status == EthernetTransmitStatus::Transmitted)
    {
        std::cout << ">> ACK for Ethernet frame with userContext=" << frameTransmitEvent.userContext << std::endl;
    }
    else
    {
        std::cout << ">> NACK for Ethernet frame with userContext=" << frameTransmitEvent.userContext;
        switch (frameTransmitEvent.status)
        {
        case EthernetTransmitStatus::Transmitted:
            break;
        case EthernetTransmitStatus::InvalidFrameFormat:
            std::cout << ": InvalidFrameFormat";
            break;
        case EthernetTransmitStatus::ControllerInactive:
            std::cout << ": ControllerInactive";
            break;
        case EthernetTransmitStatus::LinkDown:
            std::cout << ": LinkDown";
            break;
        case EthernetTransmitStatus::Dropped:
            std::cout << ": Dropped";
            break;
        }

        std::cout << std::endl;
    }
}

void SendFrame(IEthernetController* controller, const EthernetMac& from, const EthernetMac& to,const char* framePayload)
{
    static int frameId = 0;
    std::stringstream stream;
    stream << framePayload<<"frameid:" << frameId++ << ")"
              "----------------------------------------------------"; // ensure that the payload is long enough to constitute a valid Ethernet frame

    auto payloadString = stream.str();
    std::vector<uint8_t> payload(payloadString.size() + 1);
    memcpy(payload.data(), payloadString.c_str(), payloadString.size() + 1);

    const auto userContext = reinterpret_cast<void *>(static_cast<intptr_t>(frameId));

    auto frame = CreateFrame(to, from, payload);
    controller->SendFrame(EthernetFrame{frame}, userContext);
    std::cout << "<< ETH Frame sent with userContext=" << userContext << std::endl;
}
/************************************************* */

/**************************************************************************************************
 * Main Function
 **************************************************************************************************/

int main(int argc, char** argv)
{
    EthernetMac WriterMacAddr = {0xF6, 0x04, 0x68, 0x71, 0xAA, 0xC1};
    EthernetMac BroadcastMacAddr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    if (argc < 3)
    {
        std::cerr << "Missing arguments! Start demo with: " << argv[0]
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl
                  << "Use \"CanWriter\" or \"CanReader\" as <ParticipantName>." << std::endl;
        return -1;
    }

    if (argc > 5)
    {
        std::cerr << "Too many arguments! Start demo with: " << argv[0]
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl
                  << "Use \"CanWriter\" or \"CanReader\" as <ParticipantName>." << std::endl;
        return -1;
    }

    std::string participantName(argv[2]);

    if (participantName != "EthernetWriter" && participantName != "CanReader")
    {
        std::cout << "Wrong participant name provided. Use either \"CanWriter\" or \"CanReader\"." << std::endl;
        return -1;
    }

    try
    {
        std::string participantConfigurationFilename(argv[1]);

        std::string registryUri = "silkit://localhost:8500";

        bool runSync = true;

        std::vector<std::string> args;
        std::copy((argv + 3), (argv + argc), std::back_inserter(args));

        for (auto arg : args)
        {
            if (arg == "--async")
            {
                runSync = false;
            }
            else
            {
                registryUri = arg;
            }
        }

        auto participantConfiguration = SilKit::Config::ParticipantConfigurationFromFile(participantConfigurationFilename);
        auto sleepTimePerTick = 1000ms;

        std::cout << "Creating participant '" << participantName << "' with registry " << registryUri << std::endl;

        auto participant = SilKit::CreateParticipant(participantConfiguration, participantName, registryUri);

        auto* logger = participant->GetLogger();
        std::cout<<"logger::::::"<<logger<<std::endl;
        auto* canController = participant->CreateCanController("CAN1", "CAN1");
        auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");

        ethernetController->AddFrameTransmitHandler(&FrameTransmitHandler);
        canController->AddFrameHandler(
            [logger](ICanController* /*ctrl*/, const CanFrameEvent& frameEvent) {
                FrameHandler(frameEvent, logger);
            });
        
        auto operationMode = (runSync ? OperationMode::Coordinated : OperationMode::Autonomous);

        auto* lifecycleService = participant->CreateLifecycleService({operationMode});
        //auto* lifecycleService2 = participant->CreateLifecycleService({operationMode});
        // Observe state changes
        lifecycleService->SetStopHandler([]() {
            std::cout << "Stop handler called" << std::endl;
        });
        lifecycleService->SetShutdownHandler([]() {
            std::cout << "Shutdown handler called" << std::endl;
        });
        lifecycleService->SetAbortHandler([](auto lastState) {
            std::cout << "Abort handler called while in state " << lastState << std::endl;
        });


        // Observe state changes
        //lifecycleService2->SetStopHandler([]() { std::cout << "Stop handler called" << std::endl; });
        //lifecycleService2->SetShutdownHandler([]() { std::cout << "Shutdown handler called" << std::endl; });
        //lifecycleService2->SetAbortHandler(
        //    [](auto lastState) { std::cout << "Abort handler called while in state " << lastState << std::endl; });

        if (runSync)
        {
            
        }
        else
        {
            std::atomic<bool> isStopRequested = {false};
            std::thread workerThread;

            std::promise<void> promiseObj;
            std::future<void> futureObj = promiseObj.get_future();
            
            lifecycleService->SetCommunicationReadyHandler([&]() {
                std::cout << "Communication ready for " << participantName << std::endl;
                canController->SetBaudRate(10'000, 1'000'000, 2'000'000);

                workerThread = std::thread{[&]() {
                    futureObj.get();
                    while (lifecycleService->State() == ParticipantState::ReadyToRun ||
                           lifecycleService->State() == ParticipantState::Running)
                    {
                        if (participantName == "EthernetWriter")
                        {
                            SendFrame(ethernetController, WriterMacAddr, BroadcastMacAddr,GlobalPayload.c_str());
                            std::cout<<"global:::::"<<GlobalPayload<<"global.cstr"<<GlobalPayload.c_str()<<std::endl;
                        }
                        std::this_thread::sleep_for(sleepTimePerTick);
                    }
                    if (!isStopRequested)
                    {
                        std::cout << "Press enter to end the process..." << std::endl;
                    }
                }};
                canController->Start();
                ethernetController->Activate();
            });

            lifecycleService->SetStartingHandler([&]() {
                promiseObj.set_value();
            });

            lifecycleService->StartLifecycle();
            std::cout << "Press enter to leave the simulation..." << std::endl;
            std::cin.ignore();

            isStopRequested = true;
            if (lifecycleService->State() == ParticipantState::Running || 
                lifecycleService->State() == ParticipantState::Paused)
            {
                std::cout << "User requested to stop in state " << lifecycleService->State() << std::endl;
                lifecycleService->Stop("User requested to stop");
            }

            if (workerThread.joinable())
            {
                workerThread.join();
            }
            std::cout << "The participant has shut down and left the simulation" << std::endl;
        }
    }
    catch (const SilKit::ConfigurationError& error)
    {
        std::cerr << "Invalid configuration: " << error.what() << std::endl;
        std::cout << "Press enter to end the process..." << std::endl;
        std::cin.ignore();
        return -2;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Something went wrong: " << error.what() << std::endl;
        std::cout << "Press enter to end the process..." << std::endl;
        std::cin.ignore();
        return -3;
    }

    return 0;
}
