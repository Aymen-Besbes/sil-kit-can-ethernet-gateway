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

void SendFrame(ICanController* controller, ILogger* logger,std::vector<uint8_t> payloadBytes)
{
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    CanFrame canFrame {};
    canFrame.canId = 3;
    canFrame.flags |= static_cast<CanFrameFlagMask>(CanFrameFlag::Fdf) // FD Format Indicator
                      | static_cast<CanFrameFlagMask>(CanFrameFlag::Brs); // Bit Rate Switch (for FD Format only)

    static int msgId = 0;
    const auto currentMessageId = msgId++;
    canFrame.dataField = payloadBytes;
    canFrame.dlc = static_cast<uint16_t>(canFrame.dataField.size());

    std::cout << ">> Sending Can Frame to Gateway" << canFrame.dataField.size()
           << " Bytes" << std::endl;
    for (const unsigned char &byte : canFrame.dataField)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ' ';
    }
    std::cout << std::dec << std::endl;
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    
    void* const userContext = reinterpret_cast<void *>(static_cast<intptr_t>(currentMessageId));
    
    controller->SendFrame(std::move(canFrame), userContext);
    std::stringstream buffer;
    buffer << "<< CAN frame sent with userContext=" << userContext;
    logger->Info(buffer.str());
}


/**************************************************************************************************
 * Main Function
 **************************************************************************************************/

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Missing arguments! Start demo with: " << argv[0]
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl;
        return -1;
    }

    if (argc > 4)
    {
        std::cerr << "Too many arguments! Start demo with: " << argv[0]
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl;
        return -1;
    }

    std::string participantName = "CanWriter";

    try
    {
        std::string participantConfigurationFilename(argv[1]);

        std::string registryUri = "silkit://localhost:8500";

        bool runSync = true;

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
                registryUri = arg;
            }
        }

        auto participantConfiguration = SilKit::Config::ParticipantConfigurationFromFile(participantConfigurationFilename);
        auto sleepTimePerTick = 1000ms;

        std::cout << "Creating participant '" << participantName << "' with registry " << registryUri << std::endl;

        auto participant = SilKit::CreateParticipant(participantConfiguration, participantName, registryUri);

        auto* logger = participant->GetLogger();
        auto* canController = participant->CreateCanController("CAN1", "CAN");

        canController->AddFrameTransmitHandler(
            [logger](ICanController* /*ctrl*/, const CanFrameTransmitEvent& ack) {
                FrameTransmitHandler(ack, logger);
            });
        

        auto operationMode = (runSync ? OperationMode::Coordinated : OperationMode::Autonomous);

        auto* lifecycleService = participant->CreateLifecycleService({operationMode});

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
                    std::vector<uint8_t> payloadBytes = {0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xbb,0xaa,0x11,0x22,0x33,0x11,0x11,0x11,0x22,0x22,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xbb,0xaa,0x11,0x22,0x33,0x11,0x11,0x11,0x22,0x22};
                    SendFrame(canController, logger,payloadBytes);
                    std::vector<uint8_t> payloadBytes2 = {0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xbb,0xaa};
                    SendFrame(canController, logger,payloadBytes2);
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
