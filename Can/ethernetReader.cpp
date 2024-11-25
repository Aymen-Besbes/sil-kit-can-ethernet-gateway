// EthernetReader.cpp

#include "silkit/SilKit.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/ethernet/all.hpp"
#include "silkit/services/orchestration/all.hpp"
#include <iostream>
#include <vector>

using namespace SilKit::Services::Orchestration;
using namespace SilKit::Services::Ethernet;
using namespace SilKit::Services::Logging;

void FrameHandler(const EthernetFrameEvent& ethernetFrameEvent, ILogger* logger)
{
    std::string payload(ethernetFrameEvent.frame.frameData.begin(), ethernetFrameEvent.frame.frameData.end());
    logger->Info("Received Ethernet frame: " + payload);
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Missing arguments! Start demo with: " << argv[0] << " <ParticipantConfiguration.yaml|json> EthernetReader" << std::endl;
        return -1;
    }

    std::string participantConfigurationFilename(argv[1]);
    auto participant = SilKit::CreateParticipant(participantConfigurationFilename, "EthernetReader", "silkit://localhost:8500");

    auto* logger = participant->GetLogger();
    auto* ethernetController = participant->CreateEthernetController("Eth1", "Eth1");

    auto* lifecycleService = participant->CreateLifecycleService({OperationMode::Coordinated});
    lifecycleService->SetCommunicationReadyHandler([ethernetController, logger]() {
        std::cout << "Communication ready for EthernetReader" << std::endl;
        ethernetController->Start();
    });

    ethernetController->AddFrameHandler([logger](IEthernetController* /*ctrl*/, const EthernetFrameEvent& ethernetFrameEvent) {
        FrameHandler(ethernetFrameEvent, logger); // Handle received Ethernet frame
    });

    lifecycleService->StartLifecycle().get();
    return 0;
}
