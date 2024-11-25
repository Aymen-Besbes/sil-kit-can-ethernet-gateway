// Gateway.cpp

#include "silkit/SilKit.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/can/all.hpp"
#include "silkit/services/ethernet/all.hpp"
#include "silkit/services/orchestration/all.hpp"
#include <iostream>
#include <vector>
#include <thread>

using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Can;
using namespace SilKit::Services::Ethernet;
using namespace SilKit::Services::Logging;

void FrameHandler(const CanFrameEvent& frameEvent, ILogger* logger, IEthernetController* ethernetController)
{
    std::vector<uint8_t> ethernetFrameData = frameEvent.frame.dataField; // Directly use the CAN data as Ethernet payload
    EthernetFrame ethernetFrame{};
    ethernetFrame.destinationAddress = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Broadcast MAC address
    ethernetFrame.frameData = ethernetFrameData;

    ethernetController->SendFrame(ethernetFrame);
    logger->Info("CAN frame converted to Ethernet and sent");
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Missing arguments! Start demo with: " << argv[0] << " <ParticipantConfiguration.yaml|json> Gateway" << std::endl;
        return -1;
    }

    std::string participantConfigurationFilename(argv[1]);
    auto participant = SilKit::CreateParticipant(participantConfigurationFilename, "Gateway", "silkit://localhost:8500");

    auto* logger = participant->GetLogger();
    auto* canController = participant->CreateCanController("CAN1", "CAN1");
    auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");

    auto* lifecycleService = participant->CreateLifecycleService({OperationMode::Coordinated});
    lifecycleService->SetCommunicationReadyHandler([canController, ethernetController, logger]() {
        std::cout << "Communication ready for Gateway" << std::endl;
        canController->SetBaudRate(10'000, 1'000'000, 2'000'000);
        canController->Start();
        ethernetController->Start();
    });

    canController->AddFrameHandler([logger, ethernetController](ICanController* /*ctrl*/, const CanFrameEvent& frameEvent) {
        FrameHandler(frameEvent, logger, ethernetController); // Convert CAN frame to Ethernet
    });

    lifecycleService->StartLifecycle().get();
    return 0;
}
