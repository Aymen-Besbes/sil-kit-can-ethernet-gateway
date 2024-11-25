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
using namespace silkit::Services::ethernet;

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

void FrameTransmitHandler(const CanFrameTransmitEvent& ack, ILogger* logger)
{
    std::stringstream buffer;
    buffer << ">> " << ack.status
           << " for CAN frame with timestamp=" << ack.timestamp
           << " and userContext=" << ack.userContext;
    logger->Info(buffer.str());
}

void FrameHandler(const CanFrameEvent& frameEvent, ILogger* logger)
{
    std::string payload(frameEvent.frame.dataField.begin(), frameEvent.frame.dataField.end());
    std::stringstream buffer;
    buffer << ">> CAN frame: canId=" << frameEvent.frame.canId
           << " timestamp=" << frameEvent.timestamp
           << " \"" << payload << "\"";
    logger->Info(buffer.str());
}

void EthernetFrameHandler(IEthernetController* /*controller*/, const EthernetFrameEvent& frameEvent)
{
    auto frame = frameEvent.frame;
    auto payload = GetPayloadStringFromFrame(frame);
    std::cout << ">> Ethernet frame: \""
              << payload
              << "\"" << std::endl;
}

void SendEthernetFrame(IEthernetController* controller, const EthernetMac& from, const EthernetMac& to)
{
    static int frameId = 0;
    std::stringstream stream;
    stream << "Hello from Ethernet writer! (frameId =" << frameId++ << ")"
              "----------------------------------------------------"; // ensure that the payload is long enough to constitute a valid Ethernet frame

    auto payloadString = stream.str();
    std::vector<uint8_t> payload(payloadString.size() + 1);
    memcpy(payload.data(), payloadString.c_str(), payloadString.size() + 1);

    const auto userContext = reinterpret_cast<void *>(static_cast<intptr_t>(frameId));

    auto frame = CreateFrame(to, from, payload);
    controller->SendFrame(EthernetFrame{frame}, userContext);
    std::cout << "<< ETH Frame sent with userContext=" << userContext << std::endl;
}

void SendFrame(ICanController* controller, ILogger* logger)
{
    CanFrame canFrame {};
    canFrame.canId = 3;
    canFrame.flags |= static_cast<CanFrameFlagMask>(CanFrameFlag::Fdf) // FD Format Indicator
                      | static_cast<CanFrameFlagMask>(CanFrameFlag::Brs); // Bit Rate Switch (for FD Format only)

    static int msgId = 0;
    const auto currentMessageId = msgId++;

    std::stringstream payloadBuilder;
    payloadBuilder << "CAN " << (currentMessageId % 100);
    auto payloadStr = payloadBuilder.str();

    std::vector<uint8_t> payloadBytes;
    payloadBytes.resize(payloadStr.size());
    std::copy(payloadStr.begin(), payloadStr.end(), payloadBytes.begin());

    canFrame.dataField = payloadBytes;
    canFrame.dlc = static_cast<uint16_t>(canFrame.dataField.size());

    void* const userContext = reinterpret_cast<void *>(static_cast<intptr_t>(currentMessageId));

    controller->SendFrame(std::move(canFrame), userContext);
    std::stringstream buffer;
    buffer << "<< CAN frame sent with userContext=" << userContext;
    logger->Info(buffer.str());
}

std::string GetPayload(const CanFrame& canFrame)
{
    return std::string(canFrame.dataField.begin(), canFrame.dataField.end());
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
        ethernetController->AddFrameTransmitHandler(&FrameTransmitHandler);

        canController->AddFrameTransmitHandler(
            [logger](ICanController* /*ctrl*/, const CanFrameTransmitEvent& ack) {
                FrameTransmitHandler(ack, logger);
            });
        canController->AddFrameHandler(
            [logger](ICanController* /*ctrl*/, const CanFrameEvent& frameEvent) {
                FrameHandler(frameEvent, logger);
            });

        auto operationMode = (runSync ? OperationMode::Coordinated : OperationMode::Autonomous);

        auto* lifecycleService = participant->CreateLifecycleService({operationMode});
        auto* lifecycleServiceEthernet = participant->CreateLifecycleService({operationMode});

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
            // Set a CommunicationReady Handler
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
                    std::cout << "now=" << now << ", duration=" << duration << std::endl;
                    std::this_thread::sleep_for(sleepTimePerTick);
                }, 5ms);

            timeSyncServiceEthernet->SetSimulationStepHandler(
                    [ethernetController, WriterMacAddr, destinationAddress = BroadcastMacAddr](
                        std::chrono::nanoseconds now, std::chrono::nanoseconds /*duration*/) {
                    std::cout << "now=" << std::chrono::duration_cast<std::chrono::milliseconds>(now).count() << "ms"
                              << std::endl;
                    SendFrame(ethernetController, WriterMacAddr, destinationAddress);
                    std::this_thread::sleep_for(300ms);
                },
                    1ms);
            

            auto finalStateFuture = lifecycleService->StartLifecycle();
            auto finalState = finalStateFuture.get();

            std::cout << "Simulation stopped. Final State: " << finalState << std::endl;
            std::cout << "Press enter to end the process..." << std::endl;
            std::cin.ignore();

            auto finalStateFutureEthernet = lifecycleServiceEthernet->StartLifecycle();
            auto finalStateFutureEthernet = finalStateFutureEthernet.get();

            std::cout << "Simulation stopped. Final State: " << finalState << std::endl;
            std::cout << "Press enter to end the process..." << std::endl;
            std::cin.ignore();

            std::cout << "Simulation stopped. Final State Ethernet: " << finalStateEthernet << std::endl;
            std::cout << "Press enter to end the process..." << std::endl;
            std::cin.ignore();
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
                        if (participantName == "CanWriter")
                        {
                            SendFrame(canController, logger);
                        }
                        std::this_thread::sleep_for(sleepTimePerTick);
                    }
                    if (!isStopRequested)
                    {
                        std::cout << "Press enter to end the process..." << std::endl;
                    }
                }};
                canController->Start();
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

            std::promise<void> startHandlerPromise;
            auto startHandlerFutureEthernet = startHandlerPromise.get_future();
            std::atomic<bool> isStopRequestedEthernet = {false};
            std::thread workerThreadEthernet;

            lifecycleServiceEthernet->SetCommunicationReadyHandler([&]() {
                std::cout << "Communication ready handler called for " << participantName << std::endl;
                workerThreadEthernet = std::thread{[&]() {
                    startHandlerFutureEthernet.get();
                    while (lifecycleServiceEthernet->State() == ParticipantState::ReadyToRun
                           || lifecycleServiceEthernet->State() == ParticipantState::Running)
                    
                        SendFrame(ethernetController, WriterMacAddr, BroadcastMacAddr);
                        std::this_thread::sleep_for(1s);
                    }
                    if (!isStopRequested)
                    {
                        std::cout << "Press enter to end the process..." << std::endl;
                    }
                }};
                ethernetController->Activate();
            });

            lifecycleServiceEthernet->SetStartingHandler([&]() { startHandlerPromise.set_value(); });

            lifecycleServiceEthernet->StartLifecycle();
            std::cout << "Press enter to leave the simulation..." << std::endl;
            std::cin.ignore();

            isStopRequestedEthernet = true;
            if (lifecycleServiceEthernet->State() == ParticipantState::Running
                || lifecycleServiceEthernet->State() == ParticipantState::Paused)
            {
                std::cout << "User requested to stop in state " << lifecycleServiceEthernet->State() << std::endl;
                lifecycleServiceEthernet->Stop("User requested to stop");
            }
            if (workerThreadEthernet.joinable())
            {
                workerThreadEthernet.join();
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
