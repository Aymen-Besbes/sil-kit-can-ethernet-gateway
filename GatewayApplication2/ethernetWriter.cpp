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
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "silkit/SilKit.hpp"
#include "silkit/SilKitVersion.hpp"

#include "silkit/services/all.hpp"
#include "silkit/services/orchestration/all.hpp"
#include "silkit/services/orchestration/string_utils.hpp"


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
    std::vector<uint8_t> raw = {0x01, 0x00, 0x5e, 0x60, 0xe0, 0xf5, 0x8c, 0x16, 0x45, 0x04, 0x10, 0xa4, 0x08, 0x00, 0x45, 0x00,
                                               0x00, 0x64, 0x2b, 0x77, 0x40, 0x00, 0x01, 0x11, 0xcc, 0x28, 0xac, 0x10, 0x14, 0x03, 0xe0, 0xe0,
                                               0xe0, 0xf5, 0x77, 0x1a, 0x77, 0x1a, 0x00, 0x50, 0x82, 0x4b};
    std::copy(payload.begin(), payload.end(), std::back_inserter(raw));
    return raw;
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


void SendFrame(IEthernetController* controller, const EthernetMac& from, const EthernetMac& to,std::vector<uint8_t> framePayload)
{
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    static int frameId = 0;
    std::stringstream stream;
    stream <<"frameid:" << frameId++ << ")"
            "----------------------------------------------------"; // ensure that the payload is long enough to constitute a valid Ethernet frame
    auto payloadString = stream.str();
    std::vector<uint8_t> payload(payloadString.size() + 1);
    memcpy(payload.data(), payloadString.c_str(), payloadString.size() + 1);
    const auto userContext = reinterpret_cast<void *>(static_cast<intptr_t>(frameId));
    auto frame = CreateFrame(to, from, framePayload);
    //std::lock_guard<std::mutex> lock(payloadMutex);
    controller->SendFrame(EthernetFrame{frame}, userContext);
    std::cout << "<< ETH Frame sent with userContext=" << userContext << std::endl;
    std::cout << ">> Sending Ethernet Frame from Gateway to Ethernet Reader " << frame.size()
           << " Bytes" << std::endl;

    for (const unsigned char &byte : frame)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ' ';
    }
    std::cout << std::dec << std::endl;
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;

    

}

/**************************************************************************************************
 * Main Function
 **************************************************************************************************/

int main(int argc, char** argv)
{
    EthernetMac WriterMacAddr = {0xF6, 0x04, 0x68, 0x71, 0xAA, 0xC1};
    EthernetMac BroadcastMacAddr = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    if (argc < 2)
    {
        std::cerr << "Missing arguments! Start demo with: " << argv[0]
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl
                  << "Use \"EthernetWriter\" or \"EthernetReader\" as <ParticipantName>." << std::endl;
        return -1;
    }

    if (argc > 4)
    {
        std::cerr << "Too many arguments! Start demo with: " << argv[0]
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl
                  << "Use \"EthernetWriter\" or \"EthernetReader\" as <ParticipantName>." << std::endl;
        return -1;
    }

    std::string participantName ="EthernetWriter";
    std::cout << "SIL Kit Version: " << SilKit::Version::String() << std::endl;
    try
    {
        std::string participantConfigurationFilename(argv[1]);
        std::string registryUri{"silkit://localhost:8500"};

        bool runSync = true;

        // skip argv[0] and collect all arguments
        std::vector<std::string> args;
        std::copy((argv + 2), (argv + argc), std::back_inserter(args));

        for (auto arg : args)
        {
            if (arg == "--async")
            {
                runSync = false;
            }
            else
            {
                registryUri = arg.c_str();
            }
        }

        using SilKit::CreateParticipant;
        using SilKit::Config::ParticipantConfigurationFromFile;

        auto participantConfiguration = ParticipantConfigurationFromFile(participantConfigurationFilename);

        std::cout << "Creating participant '" << participantName << "' with registry " << registryUri << std::endl;
        auto participant = CreateParticipant(participantConfiguration, participantName, registryUri);
        auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");

        ethernetController->AddFrameTransmitHandler(&FrameTransmitHandler);

        auto operationMode = (runSync ? OperationMode::Coordinated : OperationMode::Autonomous);
        auto* lifecycleService = participant->CreateLifecycleService({operationMode});

        // Observe state changes
        lifecycleService->SetStopHandler([]() { std::cout << "Stop handler called" << std::endl; });
        lifecycleService->SetShutdownHandler([]() { std::cout << "Shutdown handler called" << std::endl; });
        lifecycleService->SetAbortHandler(
            [](auto lastState) { std::cout << "Abort handler called while in state " << lastState << std::endl; });

    
    
        std::promise<void> startHandlerPromise;
        auto startHandlerFuture = startHandlerPromise.get_future();
        std::atomic<bool> isStopRequested = {false};
        std::thread workerThread;

        lifecycleService->SetCommunicationReadyHandler([&]() {
            std::cout << "Communication ready handler called for " << participantName << std::endl;
            workerThread = std::thread{[&]() {
                startHandlerFuture.get();
                while (lifecycleService->State() == ParticipantState::ReadyToRun
                       || lifecycleService->State() == ParticipantState::Running)
                {
                    std::vector<uint8_t> framePayload = {0xaa,0xbb,0xcc,0xdd,0xaa,0xbb,0xcc,0xdd};
                    SendFrame(ethernetController, WriterMacAddr, BroadcastMacAddr,framePayload);
                    std::this_thread::sleep_for(1s);
                }
                if (!isStopRequested)
                {
                        std::cout << "Press enter to end the process..." << std::endl;
                }
            }};
            ethernetController->Activate();
        });

        lifecycleService->SetStartingHandler([&]() { startHandlerPromise.set_value(); });

        lifecycleService->StartLifecycle();
        std::cout << "Press enter to leave the simulation..." << std::endl;
        std::cin.ignore();

        isStopRequested = true;
        if (lifecycleService->State() == ParticipantState::Running
            || lifecycleService->State() == ParticipantState::Paused)
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