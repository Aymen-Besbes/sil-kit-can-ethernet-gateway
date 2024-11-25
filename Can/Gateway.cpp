/* This is a silkit simulation presenting three participant:
-1st particiant:CanWriter sending a can frame 
-2nd participant:Gateway a canReader to get the frame from the canWriter,converting the can frame into ethernet frame and a EthernetWriter to send the ethernet frame
-3rd participant: EthernetReader to get the ethernet frame from the gateway*/

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
#include "silkit/SilKitVersion.hpp"

#include "silkit/services/all.hpp"
#include "silkit/services/can/string_utils.hpp"

using namespace SilKit::Services::Ethernet;
using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Can;
using namespace SilKit::Services::Logging;

using namespace std::chrono_literals;
// Field in a frame that can indicate the protocol, payload size, or the start of a VLAN tag
using EtherType = uint16_t;
using EthernetMac = std::array<uint8_t, 6>;

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


//shared memmory
void* memory = NULL;

uint32_t calculateFCS(std::vector<unsigned char>& data) {
    // Example FCS calculation
    uint32_t fcs = 0xFFFFFFFF;
    for (auto byte : data) {
        fcs ^= byte;
    }
    return fcs;
}

void CANFrameTransmitHandler(const CanFrameTransmitEvent& ack, ILogger* logger)
{
    std::stringstream buffer;
    buffer << ">> " << ack.status << " for CAN frame with timestamp=" << ack.timestamp
           << " and userContext=" << ack.userContext;
    logger->Info(buffer.str());
}

void CANFrameHandler(const CanFrameEvent& frameEvent, ILogger* logger)
{
    std::string payload(frameEvent.frame.dataField.begin(), frameEvent.frame.dataField.end());
    std::stringstream buffer;
    buffer << ">> CAN frame: canId=" << frameEvent.frame.canId << " timestamp=" << frameEvent.timestamp << " \""
           << payload << "\"";
    logger->Info(buffer.str());
}

void SendFrame(ICanController* controller, ILogger* logger)
{
    CanFrame canFrame{};
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

    void* const userContext = reinterpret_cast<void*>(static_cast<intptr_t>(currentMessageId));

    controller->SendFrame(std::move(canFrame), userContext);
    std::stringstream buffer;
    buffer << "<< CAN frame sent with userContext=" << userContext;
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



// Function to convert a CAN frame to an Ethernet frame
std::vector<uint8_t> convertCANtoEthernet(const CanFrameEvent& frameEvent) {
    const CanFrame& canFrame = frameEvent.frame;
    std::vector<uint8_t> ethernetFrame;

    // Example MAC addresses
    uint8_t destinationMAC[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t sourceMAC[6] = {0xF6, 0x04, 0x68, 0x71, 0xAA, 0xC1};

    // Reserve space for Ethernet frame (14 bytes for header + payload + 4 bytes for FCS)
    ethernetFrame.reserve(14 + canFrame.dataField.size() + 4);

    // Copy MAC addresses to the Ethernet frame
    ethernetFrame.insert(ethernetFrame.end(), std::begin(destinationMAC), std::end(destinationMAC));
    ethernetFrame.insert(ethernetFrame.end(), std::begin(sourceMAC), std::end(sourceMAC));

    // EtherType for IPv4 (0x0800)
    ethernetFrame.push_back(0x08);
    ethernetFrame.push_back(0x00);

    // Copy CAN frame payload to Ethernet frame
    ethernetFrame.insert(ethernetFrame.end(), canFrame.dataField.begin(), canFrame.dataField.end());

    // Calculate and append FCS (Frame Check Sequence)
    uint32_t fcs = calculateFCS(ethernetFrame);
    ethernetFrame.push_back((fcs >> 24) & 0xFF);
    ethernetFrame.push_back((fcs >> 16) & 0xFF);
    ethernetFrame.push_back((fcs >> 8) & 0xFF);
    ethernetFrame.push_back(fcs & 0xFF);

    return ethernetFrame;
}


void SendFrameGateway(IEthernetController* controller, const CanFrameEvent& frameEvent){
    static int frameId = 0;
    std::vector<uint8_t> ethernetFrame = convertCANtoEthernet(frameEvent.frame);
    auto payload = GetPayloadStringFromFrame(ethernetFrame);
    std::stringstream stream;
    stream <<Payload << frameId++ << ")"
              "----------------------------------------------------"; // ensure that the payload is long enough to constitute a valid Ethernet frame
    const auto userContext = reinterpret_cast<void *>(static_cast<intptr_t>(frameId));

    controller->SendFrame(EthernetFrame{ethernetFrame}, userContext);
    std::cout << "<< ETH Frame sent with userContext=" << userContext << std::endl;
}







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

    if (participantName != "EhthernetWriter" && participantName != "CanReader")
    {
        std::cout << "Wrong participant name provided. Use either \"CanWriter\" or \"CanReader\"." << std::endl;
        return -1;
    }
    
    try
    {
        std::string participantConfigurationFilename(argv[1]);
        std::string registryUri{"silkit://localhost:8500"};

        bool runSync = true;

        // skip argv[0] and collect all arguments
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
                registryUri = arg.c_str();
            }
        }

        using SilKit::CreateParticipant;
        using SilKit::Config::ParticipantConfigurationFromFile;

        auto participantConfiguration = ParticipantConfigurationFromFile(participantConfigurationFilename);
        auto sleepTimePerTick = 1000ms;

        std::cout << "Creating participant '" << participantName << "' with registry " << registryUri << std::endl;

        auto participant = CreateParticipant(participantConfiguration, participantName, registryUri);
        auto* logger = participant->GetLogger();
        auto* canController = participant->CreateCanController("CAN1", "CAN1");
        auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");

        ethernetController->AddFrameHandler(&FrameHandler);
        ethernetController->AddFrameTransmitHandler(&FrameTransmitHandler);

        canController->AddFrameTransmitHandler([logger](ICanController* /*ctrl*/, const CanFrameTransmitEvent& ack) {
            FrameTransmitHandler(ack, logger);
        });
        canController->AddFrameHandler(
            [logger](ICanController* /*ctrl*/, const CanFrameEvent& frameEvent) { FrameHandler(frameEvent, logger); });

        auto operationMode = (runSync ? OperationMode::Coordinated : OperationMode::Autonomous);

        auto* lifecycleService = participant->CreateLifecycleService({operationMode});

         // Observe state changes
        lifecycleService->SetStopHandler([]() { std::cout << "Stop handler called" << std::endl; });
        lifecycleService->SetShutdownHandler([]() { std::cout << "Shutdown handler called" << std::endl; });
        lifecycleService->SetAbortHandler(
            [](auto lastState) { std::cout << "Abort handler called while in state " << lastState << std::endl; });

        if (runSync)
        {
            lifecycleService->SetCommunicationReadyHandler([canController, &participantName]() {
                std::cout << "Communication ready for " << participantName << std::endl;
                canController->SetBaudRate(10'000, 1'000'000, 2'000'000);
                canController->Start();
            });

            auto* timeSyncService = lifecycleService->CreateTimeSyncService();

            // Set a CommunicationReady Handler for can
            lifecycleService->SetCommunicationReadyHandler([&participantName, ethernetController]() {
                std::cout << "Communication ready handler called for " << participantName << std::endl;
                ethernetController->Activate();
            });
            if (participantName == "CanReaderEthernetWriter"){
            //Can
                timeSyncService->SetSimulationStepHandler(
                        [sleepTimePerTick](std::chrono::nanoseconds now, std::chrono::nanoseconds duration) {
                        std::cout << "now=" << now << ", duration=" << duration << std::endl;
                        std::this_thread::sleep_for(sleepTimePerTick);
                    }, 5ms);
            //Ether
                timeSyncService->SetSimulationStepHandler(
                        [ethernetController, WriterMacAddr, destinationAddress = BroadcastMacAddr](
                            std::chrono::nanoseconds now, std::chrono::nanoseconds /*duration*/) {
                        std::cout << "now=" << std::chrono::duration_cast<std::chrono::milliseconds>(now).count() << "ms"
                                  << std::endl;
                        SendFrameGateway(ethernetController, frameEvent);
                        std::this_thread::sleep_for(300ms);
                    },
                        1ms);
            }
            else if (participantName == "CanWriter"){
                    timeSyncService->SetSimulationStepHandler(
                        [canController, logger, sleepTimePerTick](std::chrono::nanoseconds now,
                                                              std::chrono::nanoseconds duration) {
                        std::cout << "now=" << now << ", duration=" << duration << std::endl;
                        SendFrame(canController, logger);
                        std::this_thread::sleep_for(sleepTimePerTick);
                    },
                        5ms);
            }
            else{
                timeSyncService->SetSimulationStepHandler(
                    [](std::chrono::nanoseconds now, std::chrono::nanoseconds /*duration*/) {
                    std::cout << "now=" << std::chrono::duration_cast<std::chrono::milliseconds>(now).count() << "ms"
                              << std::endl;
                    std::this_thread::sleep_for(300ms);
                }, 1ms);
            }
            auto finalStateFuture = lifecycleService->StartLifecycle();
            auto finalState = finalStateFuture.get();

            std::cout << "Simulation stopped. Final State: " << finalState << std::endl;
            std::cout << "Press enter to end the process..." << std::endl;
            std::cin.ignore();
        }
        else
        {
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
                        if (participantName == "EthernetReader")
                        {
                            std::cout<<"received"<<std::endl;
                        }
                        std::this_thread::sleep_for(1s);
                        else if (participantName == "CanWriter"){
                            SendFrame(canController, logger);
                        }
                        else{
                            SendFrameGateway(ethernetController, frameEvent);
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
             lifecycleService->SetStartingHandler([&]() { promiseObj.set_value(); });

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
