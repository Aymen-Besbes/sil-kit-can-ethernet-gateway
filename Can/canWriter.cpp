// CanWriter.cpp

#include "silkit/SilKit.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/can/all.hpp"
#include "silkit/services/ethernet/all.hpp"
#include "silkit/services/orchestration/all.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <thread>

using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Can;
using namespace SilKit::Services::Ethernet;
using namespace SilKit::Services::Logging;

void SendFrame(ICanController* controller, ILogger* logger)
{
    CanFrame canFrame{};
    canFrame.canId = 1; // Unique CAN ID
    canFrame.dlc = 8; // Data length
    canFrame.dataField = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}; // Sample data

    controller->SendFrame(std::move(canFrame), nullptr); // Sending the CAN frame

    logger->Info("CAN frame sent");
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Missing arguments! Start demo with: " << argv[0] << " <ParticipantConfiguration.yaml|json> CanWriter" << std::endl;
        return -1;
    }

    std::string participantConfigurationFilename(argv[1]);
    auto participant = SilKit::CreateParticipant(participantConfigurationFilename, "CanWriter", "silkit://localhost:8500");

    auto* logger = participant->GetLogger();
    auto* canController = participant->CreateCanController("CAN1", "CAN1");

    auto* lifecycleService = participant->CreateLifecycleService({OperationMode::Coordinated});
    lifecycleService->SetCommunicationReadyHandler([canController, logger]() {
        std::cout << "Communication ready for CanWriter" << std::endl;
        canController->SetBaudRate(10'000, 1'000'000, 2'000'000);
        canController->Start();
    });

    auto* timeSyncService = lifecycleService->CreateTimeSyncService();
    timeSyncService->SetSimulationStepHandler([canController, logger](std::chrono::nanoseconds now, std::chrono::nanoseconds duration) {
        SendFrame(canController, logger); // Send CAN frame on every tick
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }, 100ms);

    lifecycleService->StartLifecycle().get();
    return 0;
}
