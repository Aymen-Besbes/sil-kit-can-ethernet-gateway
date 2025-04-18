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
#include "silkit/SilKit.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/orchestration/all.hpp"
#include "silkit/services/orchestration/string_utils.hpp"
#include "silkit/services/can/all.hpp"
#include "silkit/services/can/string_utils.hpp"
#include <mutex>
#include <queue>
#include <condition_variable>
using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Can;
using namespace SilKit::Services::Logging;
using namespace std::chrono_literals;
using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Ethernet;
using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Ethernet;


// Field in a frame that can indicate the protocol, payload size, or the start of a VLAN tag
using EtherType = uint16_t;
using EthernetMac = std::array<uint8_t, 6>;


//Initialization for the buffer use
std::mutex mtx; // Mutex for synchronization
std::condition_variable cond_var; // Condition variable for producer-consumer signaling
std::queue<std::vector<uint8_t>> buffer;const unsigned int MAX_BUFFER_SIZE = 10; // Maximum size of the buffer





std::string GetPayloadStringFromFrame(const EthernetFrame& frame)
{
    const size_t FrameHeaderSize = 2 * sizeof(EthernetMac) + sizeof(EtherType);

    std::vector<uint8_t> payload;
    payload.insert(payload.end(), frame.raw.begin() + FrameHeaderSize, frame.raw.end());
    std::string payloadString(payload.begin(), payload.end());
    return payloadString;
}
/**
 * @brief 
 * 
 * @param frameEvent 
 * @param logger 
 */


void FrameHandler(IEthernetController* /*controller*/, const EthernetFrameEvent& frameEvent)
{
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    auto frame = frameEvent.frame;
    //auto payload = GetPayloadStringFromFrame(frame);
    std::cout << ">> Ethernet frame is received from [EthernetWriter] with " << frame.raw.size()
           << " Bytes" << std::endl;
    for (const unsigned char &byte : frame.raw)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ' ';
    }
    std::cout << std::dec << std::endl;
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    const size_t FrameHeaderSize = 2 * sizeof(EthernetMac) + sizeof(EtherType);
    std::vector<uint8_t> GlobalPayload;
    // Insert the payload from the raw Ethernet frame, skipping the header
    GlobalPayload.insert(GlobalPayload.end(), 
        frame.raw.begin() + 42,  
        frame.raw.end()               
    );
    // Lock the mutex
    std::unique_lock<std::mutex> lock(mtx); 
    // Wait until there's space in buffer
    cond_var.wait(lock, [] { return buffer.size() < MAX_BUFFER_SIZE; });
    // Add payload to the buffer
    buffer.push(GlobalPayload); 
    std::cout << "Producing Payload " << std::endl; 
    // Display buffer size after producing
    std::cout << "Buffer size after producing: " << buffer.size() << std::endl << std::endl; 
    // Unlock the mutex
    lock.unlock(); 
    cond_var.notify_one(); // Notify one waiting thread
}

/* ************************************************ */

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


void SendFrame(ICanController* controller, ILogger* logger)
{
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    CanFrame canFrame {};
    
    canFrame.canId = 3;
    canFrame.flags |= static_cast<CanFrameFlagMask>(CanFrameFlag::Fdf) // FD Format Indicator
                      | static_cast<CanFrameFlagMask>(CanFrameFlag::Brs); // Bit Rate Switch (for FD Format only)

    static int msgId = 0;
    const auto currentMessageId = msgId++;
    
    canFrame.dlc = static_cast<uint16_t>(canFrame.dataField.size());
    
    void* const userContext = reinterpret_cast<void *>(static_cast<intptr_t>(currentMessageId));
    
    std::unique_lock<std::mutex> lock(mtx); 
    cond_var.wait(lock, [] { return buffer.size() > 0; }); 
    std::vector<uint8_t> framePayload = buffer.front(); 
    buffer.pop(); 
    canFrame.dataField = framePayload;
    controller->SendFrame(std::move(canFrame), userContext);
    std::stringstream buffer2;
    buffer2 << "<< CAN frame sent with userContext=" << userContext;
    logger->Info(buffer2.str());
    std::cout << "Consuming Payload"<< std::endl; 
    std::cout << "Buffer size after consuming: " << buffer.size() << std::endl << std::endl;           
    lock.unlock();
    cond_var.notify_one();                                      
    std::cout << "<< ETH Frame sent with userContext=" << userContext << std::endl;
    std::cout << ">> Sending Can Frame from Gateway to CanReader " << canFrame.dataField.size()
           << " Bytes" << std::endl;

    for (const unsigned char &byte : canFrame.dataField)
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
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl;
        return -1;
    }

    if (argc > 4)
    {
        std::cerr << "Too many arguments! Start demo with: " << argv[0]
                  << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl;
        return -1;
    }

    std::string participantName = "Gateway";

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
        std::cout<<"--********---"<< participantConfiguration<<std::endl;
        auto sleepTimePerTick = 1000ms;

        std::cout << "Creating participant Gateway with registry " << registryUri << std::endl;

        auto participant = SilKit::CreateParticipant(participantConfiguration, "Gateway", registryUri);
        auto* logger = participant->GetLogger();

        auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");
        auto* canController = participant->CreateCanController("CAN1", "CAN");
        
        canController->AddFrameTransmitHandler(
            [logger](ICanController* /*ctrl*/, const CanFrameTransmitEvent& ack) {
                FrameTransmitHandler(ack, logger);
            });
        ethernetController->AddFrameHandler(&FrameHandler);
        
        auto operationMode = (runSync ? OperationMode::Coordinated : OperationMode::Autonomous);
        auto* lifecycleService = participant->CreateLifecycleService({operationMode});
       
        lifecycleService->SetStopHandler([]() {
            std::cout << "Stop handler called" << std::endl;
        });
        lifecycleService->SetShutdownHandler([]() {
            std::cout << "Shutdown handler called" << std::endl;
        });
        lifecycleService->SetAbortHandler([](auto lastState) {
            std::cout << "Abort handler called while in state " << lastState << std::endl;
        });
        std::cout<<"Hi from can lifecycle"<<std::endl;
        std::atomic<bool> isStopRequested = {false};
        std::thread workerThread;
        std::promise<void> promiseObj;
        std::future<void> futureObj = promiseObj.get_future();
            
        lifecycleService->SetCommunicationReadyHandler([&]() {
            std::cout << "Communication ready for  Gateway" << std::endl;
            canController->SetBaudRate(10'000, 1'000'000, 2'000'000);

            workerThread = std::thread{[&]() {
                futureObj.get();
                while (lifecycleService->State() == ParticipantState::ReadyToRun ||
                        lifecycleService->State() == ParticipantState::Running)
                {       
                        
                        SendFrame(canController,logger);
                        std::cout<<"Frame sent from Ethernet Writer to Ethernet Reader..."<<std::endl;
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
