/*This code is an implementation of a Can to Ethernet Gateway demo 
to ensure the reception of a can frame from a CanWriter and send it 
to an EthernetReader using SilKit */

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
//Define the source and the destination addresses to use it into the Ethernet Frame
const std::array<uint8_t, 6> sourceAddress = {0xF6, 0x04, 0x68, 0x71, 0xAA, 0xC1};
const std::array<uint8_t, 6> destinationAddress = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Mutex for synchronization
std::mutex mtx; 
// Condition variable for producer-consumer signaling
std::condition_variable cond_var; 
// Maximum size of the buffer
std::queue<std::vector<uint8_t>> buffer;const unsigned int MAX_BUFFER_SIZE = 10; 

namespace std {
namespace chrono {
std::ostream& operator<<(std::ostream& out, nanoseconds timestamp)
{
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(timestamp);
    out << seconds.count() << "s";
    return out;
}
}
}

void FrameHandler(const CanFrameEvent& frameEvent, ILogger* logger)
{   
    /**
     * @brief This function is designed to receive Can Frames from CanWriter and extract the payload and save it into buffer.
     * 
     * @param frameEvent The event of an incoming CAN frame including transmit ID, timestamp and the actual frame.
     * @param logger 
     */
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    std::cout<<"Reading frame from CanReader.........."<<std::endl;
    std::stringstream buffer2;
    buffer2 << ">> CAN frame: canId=" << frameEvent.frame.canId
           << " timestamp=" << frameEvent.timestamp
           << " \"" ;
    std::cout << ">> Receiving Can Frame from CanWriter" << frameEvent.frame.dataField.size()
           << "  Bytes:" << std::endl;
    // Print the frame
    for (const unsigned char &byte : frameEvent.frame.dataField)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ' ';
    }
    std::cout << std::dec << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    logger->Info(buffer2.str());
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    // Extract the payload of the receiving can frame
    std::vector<uint8_t> GlobalPayload = std::vector<uint8_t>(frameEvent.frame.dataField.begin(), frameEvent.frame.dataField.end());
    // Lock the mutex
    std::unique_lock<std::mutex> lock(mtx); 
    // Wait until there's space in buffer
    cond_var.wait(lock, [] { return buffer.size() < MAX_BUFFER_SIZE; }); 
    // Add payload to the buffer
    buffer.push(GlobalPayload); 
    std::cout << "Producing Payload " << std::endl; 
    std::cout << "Buffer size after producing: " << buffer.size() << std::endl << std::endl; 
    //Unlock the mutex
    lock.unlock();
    //Notify the waiting thread 
    cond_var.notify_one(); 
    
    
}

std::ostream& operator<<(std::ostream& out, std::chrono::nanoseconds timestamp)
{
    auto seconds = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(timestamp);
    out << seconds.count() << "s";
    return out;
}

std::vector<uint8_t> CreateFrame(const std::vector<uint8_t>& payload)
{   
    /**
     * @brief This function is designed to encapsulate the payload into an Ethernet Frame
     * @param payload a vector representing the data to encapsulatye into an Ethernet Frame
     * 
     */

    // Define the Ethertype as IPv4
    const uint16_t etherType = 0x0800; 
    // Initialize the frame
    std::vector<uint8_t> raw={};
    // Add the destination address
    std::copy(destinationAddress.begin(), destinationAddress.end(), std::back_inserter(raw));
    // Add the source address
    std::copy(sourceAddress.begin(), sourceAddress.end(), std::back_inserter(raw));
    // Add the etherTypeBytes
    auto etherTypeBytes = reinterpret_cast<const uint8_t*>(&etherType);
    // Add the payload
    std::copy(payload.begin(), payload.end(), std::back_inserter(raw));
    // Add the Frame Check Sequence
    raw.push_back(etherTypeBytes[1]);  // We assume our platform to be little-endian
    raw.push_back(etherTypeBytes[0]);
    return raw;
}

void FrameTransmitHandler(IEthernetController* /*controller*/, const EthernetFrameTransmitEvent& frameTransmitEvent)
{   
    /**
     * @brief 
     * 
     */
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

void SendFrame(IEthernetController* controller)
{
    /**
     * @brief This function is designed to ensure the get of the payload from the buffer and send it into an Ethernet Frame to the EthernetReader
     * 
     *@param controller the Ethernet Controller
     */
    std::cout << "--------------------------------------- \n ---------------------------------------" << std::endl;
    static int frameId = 0;
    frameId++;
    const auto userContext = reinterpret_cast<void *>(static_cast<intptr_t>(frameId));
    // Lock the mutex
    std::unique_lock<std::mutex> lock(mtx); 
    // Wait until there's content in the buffer
    cond_var.wait(lock, [] { return buffer.size() > 0; }); 
    // Get the first element from the buffer
    std::vector<uint8_t> framePayload = buffer.front(); 
    // Delete the payload from the buffer
    buffer.pop();
    // Encapsulate the extracted payload into an Ethernet frame 
    auto frame = CreateFrame(framePayload);
    // Send the Ethernet Frame 
    controller->SendFrame(EthernetFrame{frame}, userContext);
    std::cout << "Consuming Payload"<< std::endl;
    std::cout << "Buffer size after consuming: " << buffer.size() << std::endl << std::endl;
    // Unlock the mutex
    lock.unlock();
    // Notify one waiting thread
    cond_var.notify_one(); 
    std::cout << "<< ETH Frame sent with userContext=" << userContext << std::endl;
    std::cout << ">> Sending Ethernet Frame from Gateway to Ethernet Reader " << frame.size()
           << " Bytes" << std::endl;
    // Print the frame sent to EthernetReader 
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
        auto sleepTimePerTick = 1000ms;

        std::cout << "Creating participant Gateway with registry " << registryUri << std::endl;

        auto participant = SilKit::CreateParticipant(participantConfiguration, "Gateway", registryUri);
        auto* logger = participant->GetLogger();

        auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");
        auto* canController = participant->CreateCanController("CAN1", "CAN");
        
        ethernetController->AddFrameTransmitHandler(&FrameTransmitHandler);
        canController->AddFrameHandler(
            [logger](ICanController* /*ctrl*/, const CanFrameEvent& frameEvent) {
                FrameHandler(frameEvent, logger);
            });
        
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
                        
                        SendFrame(ethernetController);
                        std::cout<<"Frame sent from Ethernet Writer to Ethernet Reader..."<<std::endl;
                        //std::this_thread::sleep_for(sleepTimePerTick);
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
        std::cin.ignore();
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
