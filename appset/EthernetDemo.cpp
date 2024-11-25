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

#include "silkit/SilKit.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/orchestration/all.hpp"
#include "silkit/services/orchestration/string_utils.hpp"
#include "silkit/services/can/all.hpp"
#include "silkit/services/can/string_utils.hpp"
#include "silkit/services/ethernet/all.hpp"

using namespace SilKit::Services::Ethernet;
using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Can;
using namespace SilKit::Services::Logging;

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

void FrameTransmitHandler(const CanFrameTransmitEvent& ack, ILogger* logger)
{
    std::stringstream buffer;
    buffer << ">> " << ack.status
           << " for CAN frame with timestamp=" << ack.timestamp
           << " and userContext=" << ack.userContext;
    logger->Info(buffer.str());
}

void FrameHandler(const CanFrame& frameEvent, ILogger* logger, IEthernetController* ethernetController)
{
    // Extract payload from CAN frame
    std::string payload = GetPayload(frameEvent);  // Get payload string
    
    // Log the CAN frame details
    std::stringstream buffer;
    buffer << ">> CAN frame: canId=" << frameEvent.frame.canId
           << " timestamp=" << frameEvent.timestamp
           << " \"" << payload << "\"";
    logger->Info(buffer.str());

    // Now, send the extracted payload via Ethernet
    EthernetMac fromMac("00:11:22:33:44:55");  // Example MAC address
    EthernetMac toMac("FF:FF:FF:FF:FF:FF");    // Broadcast MAC address
    SendEthernetFrame(ethernetController, fromMac, toMac, payload);
}

std::string GetPayload(const CanFrame& canFrame)
{
    return std::string(canFrame.dataField.begin(), canFrame.dataField.end());
}

void EthernetFrameHandler(IEthernetController* /*controller*/, const EthernetFrameEvent& frameEvent)
{
    auto frame = frameEvent.frame;
    auto payload = GetPayloadStringFromFrame(frame);
    std::cout << ">> Ethernet frame received: \"" << payload << "\"" << std::endl;
}

void SendEthernetFrame(IEthernetController* controller, const EthernetMac& from, const EthernetMac& to, const std::string& payloadString)
{
    static int frameId = 0;
    std::stringstream stream;
    stream << "From CAN to Ethernet! (frameId =" << frameId++ << ")"
              "----------------------------------------------------";

    // Combine the static part of the message with the payloadString
    std::string finalPayload = stream.str() + payloadString;

    // Prepare the payload for Ethernet frame (ensure it's null-terminated)
    std::vector<uint8_t> payload(finalPayload.begin(), finalPayload.end());

    // Construct Ethernet frame
    auto frame = CreateFrame(to, from, payload);
    
    // Send the Ethernet frame using the controller
    const auto userContext = reinterpret_cast<void *>(static_cast<intptr_t>(frameId));
    controller->SendFrame(EthernetFrame{frame}, userContext);
    std::cout << "<< Ethernet frame sent with userContext=" << userContext << std::endl;
}



/**************************************************************************************************
 * Main Function
 **************************************************************************************************/

int main(int argc, char** argv)
{
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

    if (participantName != "CanWriter" && participantName != "CanReader")
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
        auto* canController = participant->CreateCanController("CAN1", "CAN1");

        auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");

        ethernetController->AddFrameHandler(&FrameHandler);

        canController->AddFrameHandler([logger, ethernetController](ICanController* /*ctrl*/, const CanFrameEvent& frameEvent) {
            FrameHandler(frameEvent, logger, ethernetController);
        });

        auto operationMode = (runSync ? OperationMode::Coordinated : OperationMode::Autonomous);

        auto* lifecycleService = participant->CreateLifecycleService({operationMode});
        auto* lifecycleServiceEthernet = participant->CreateLifecycleService({operationMode});

        lifecycleService->SetStopHandler([]() {
            std::cout << "Stop handler called" << std::endl;
        });
        lifecycleService->SetShutdownHandler([]() {
            std::cout << "Shutdown handler called" << std::endl;
        });
        lifecycleService->SetAbortHandler([](auto lastState) {
            std::cout << "Abort handler called while in state " << lastState << std::endl;
        });

        lifecycleServiceEthernet->SetStopHandler([]() {
            std::cout << "Stop handler called" << std::endl;
        });
        lifecycleServiceEthernet->SetShutdownHandler([]() {
            std::cout << "Shutdown handler called" << std::endl;
        });
        lifecycleServiceEthernet->SetAbortHandler([](auto lastState) {
            std::cout << "Abort handler called while in state " << lastState << std::endl;
        });

        if (runSync)
        {
            auto* timeSyncService = lifecycleService->CreateTimeSyncService();
            auto* timeSyncServiceEthernet = lifecycleServiceEthernet->CreateTimeSyncService();
            lifecycleServiceEthernet->SetCommunicationReadyHandler([&participantName, ethernetController]() {
                std::cout << "Communication ready handler called for " << participantName << std::endl;
                ethernetController->Activate();
            });
            
            lifecycleService->SetCommunicationReadyHandler([canController, &participantName]() {
                std::cout << "Communication ready for " << participantName << std::endl;
                canController->SetBaudRate(10'000, 1'000'000, 2'000'000);
                canController->Start();
            });

            timeSyncService->SetSimulationStepHandler(
                [sleepTimePerTick](std::chrono::nanoseconds now, std::chrono::nanoseconds duration) {
                    std::cout << "Simulation step of length " << duration << " ns has elapsed at " << now << std::endl;
                    std::this_thread::sleep_for(sleepTimePerTick);
                });
            lifecycleService->StartLifecycle();
            lifecycleServiceEthernet->StartLifecycle();
        }
        else
        {
            lifecycleService->SetCommunicationReadyHandler([]() {
                std::cout << "Communication ready handler called" << std::endl;
            });

            lifecycleService->StartLifecycle();
            lifecycleServiceEthernet->StartLifecycle();
        }

        std::cout << "Press enter to end the process..." << std::endl;
        std::cin.ignore();

        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
        return -1;
    }
}
