#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <filesystem>

#include "ns3/log.h"
#include "ns3/core-module.h"
#include "ns3/config-store.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/spectrum-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/nr-u-module.h"
#include "ns3/nr-module.h"
#include "ns3/propagation-module.h"
#include "ns3/wifi-module.h"
#include "ns3/config-store-module.h"
#include "ns3/antenna-module.h"

#include "ns3/multi-model-spectrum-channel.h"
#include "simulation-helper.h"

#include "ns3/seq-ts-size-frag-header.h"
#include "ns3/bursty-helper.h"
#include "ns3/burst-sink-helper.h"
#include "ns3/three-gpp-ftp-m1-helper.h"
#include "ns3/three-gpp-http-client.h"
#include "ns3/three-gpp-http-helper.h"
#include "ns3/three-gpp-http-server.h"
#include "ns3/three-gpp-http-variables.h"
#include "ns3/traffic-generator-ngmn-ftp-multi.h"
#include "ns3/traffic-generator-ngmn-gaming.h"
#include "ns3/traffic-generator-ngmn-video.h"
#include "ns3/traffic-generator-ngmn-voip.h"
#include "ns3/v4ping-helper.h"

#include "ns3/ai-module.h"
#include "nr-env.h"

NS_LOG_COMPONENT_DEFINE ("nr-ai");

bool g_connect = false;
bool g_customEpisode = false;
uint32_t g_numgNbs = 6;
Ptr<NrEnv> g_env;
uint32_t maxNodes = 6;


Time g_timeStep = MilliSeconds(100);
std::vector<L2Setup*> nr(maxNodes);

uint32_t g_nrPairs;


ApplicationContainer g_nrServerNodes;
ApplicationContainer g_nrClientNodes;

int
main (int argc, char *argv[])
{

    //Scenario parameters
    uint32_t numAgents = 3;
    uint32_t timeStep = 100;
    std::string outputDirectory = "";

    uint32_t numNrPairs = numAgents;
    bool onlyLOS = true;
    uint32_t scenarioType = 1; //1 - freespace, 2 - factory  
    uint32_t scenarioId = 1; 
    uint32_t simRound = 1;
    uint32_t seed = 1;

    
    //Traffic parameters
    double appStartTime = 0; //seconds
    uint64_t UDP_SATURATION_RATE = 160000000;
    //UintegerValue packetSizeValue = 1474;
    BooleanValue spreadUdpLoad = true;
    DataRateValue dataRateValue = DataRate (UDP_SATURATION_RATE);
    //UintegerValue uintegerValue = UintegerValue(packetSizeValue);
    uint64_t bitRate = dataRateValue.Get().GetBitRate ();
    uint32_t udpLambda = 1000;
    uint32_t packetSize = 1500; // bytes
    double interval = static_cast<double> (packetSize * 8) / bitRate;
    Time udpInterval = Seconds (interval);
    uint16_t operatorPortNr = 1234;

    std::string targetDataRate = "40Mbps";
    uint32_t fragmentSize = 1500;
    std::string trafficType = "UDP_CBR";

    //Distance parameters
    double d1 = 1.0; // meters
    double d2 = 1.0; // meters
    double d3 = 1.0; // meters

    //Simulation parameters
    double simTime = 2.0; // seconds
    double ueX = 1.0; // meters
    std::string simTag = "default";
    bool enableNr = true;
    bool enableWifi = false;
    bool doubleTechnology = false;
    bool positioning = false;

    std::string pathlossDir = "../freespacePL/";

    //Physical parameters
    bool cellScan = true;
    double beamSearchAngleStep = 30.0; //degrees
    uint16_t numerologyBwp = 0;
    double frequency = 5.945e9; //Hz option : 5.945e9 5.975e9
    double bandwidth = 20e6; //Hz option : 20e6 80e6

    std::string rlcModel = "RlcUmAlways";
    std::string errorModel = "ns3::NrEesmIrT1";
    std::string ueCamType = "ns3::NrAlwaysOnAccessManager";
    std::string nodeRate = "150Mbps"; //"500kbps";

    double totalTxPower = 23; // dBm
    double ueTxPower = 23; // dBm
    
    int Duplex = 0;
    uint8_t nruMcs = -1;

    double totalTxPowerVector[maxNodes]; // dBm
    double ueTxPowerVector[maxNodes]; // dBm

    std::string trafficTypeVector [maxNodes];
    double appStartTimeVector[maxNodes];
    uint64_t UDP_SATURATION_RATE_VECTOR[maxNodes];
    uint32_t udpLambdaVector[maxNodes];
    uint32_t packetSizeVector[maxNodes];
    std::string nodeRateVector[maxNodes];
    double frameRateVector[maxNodes];
    std::string targetDataRateVector[maxNodes];
    uint32_t fragmentSizeVector[maxNodes];

    //Setting up defaults
    for (uint32_t i = 0; i < maxNodes; i++)
    { 
        totalTxPowerVector[i] = totalTxPower; // dBm
        ueTxPowerVector[i] = ueTxPower; // dBm

        trafficTypeVector[i] = trafficType;
        UDP_SATURATION_RATE_VECTOR[i] = UDP_SATURATION_RATE;
        udpLambdaVector[i] = udpLambda;
        packetSizeVector[i] = packetSize;
        nodeRateVector[i] = nodeRate;
        frameRateVector[i] = frameRate;
        targetDataRateVector[i] = targetDataRate;
        fragmentSizeVector[i] = fragmentSize;
    }  

    std::stringstream outputDirAutomatic;

    NS_LOG_DEBUG ("Create base stations and mobile terminals");
    NodeContainer allWirelessNodes;
  
    LogComponentEnable ("nr-mac", LOG_LEVEL_ALL);

    nr.resize(numNruPairs);

    DataRateValue dataRateValueVector[maxNodes];
    uint64_t bitRateVector[maxNodes];
    double intervalVector[maxNodes];
    Time udpIntervalVector[maxNodes];

    for (uint32_t i = 0; i < maxNodes; i++)
    {
        dataRateValueVector[i] = DataRate (UDP_SATURATION_RATE_VECTOR[i]);
        bitRateVector[i] = dataRateValueVector[i].Get().GetBitRate ();
        intervalVector[i] = static_cast<double> (packetSizeVector[i] * 8) / bitRateVector[i];
        udpIntervalVector[i] = Seconds (intervalVector[i]);

        double tempDataRate = fragmentSize*8*udpLambdaVector[i]/1e6;
        targetDataRateVector[i] = std::to_string(tempDataRate) + "Mbps";
    }

    NodeContainer ueNodes[numNrPairs];
    NodeContainer gNbNodes[numNrPairs];

    std::stringstream SimTag;
    SimTag << "_" << numWifiPairs << "_" << numNruPairs << "_" << frameAggregation << "_" << simRound << "_" << gnbCamType;
    RngSeedManager::SetSeed (simRound);
    ConfigureDefaultValues (cellScan, beamSearchAngleStep, errorModel, cat2EDThreshold, cat3and4EDThreshold, rlcModel,SimTag.str());

    NS_LOG_DEBUG ("Setting duplex mode");
    std::string duplexMode;
    if(Duplex == 0)
    {
        duplexMode = "FDD";
    }
    else if (Duplex == 1)
    {
        duplexMode = "TDD";
    }

    if (positioning == true)
    {
        // ... positioning related setup ...
    }
        
    else
    {
        Ptr<UniformRandomVariable> co = CreateObject<UniformRandomVariable> ();
        for (uint32_t i = 0; i < numNrPairs; i++)
        {
            // gNB position
            double x_gnb = co->GetValue(0, 500);   // horizontal spread
            double y_gnb = co->GetValue(0, 500);
            double z_gnb = 30;                     // fixed tower height
            positionAlloc->Add(Vector(x_gnb, y_gnb, z_gnb));

            // UE position
            double x_ue = co->GetValue(0, 500);
            double y_ue = co->GetValue(0, 500);
            double z_ue = 1.5;                     // fixed UE antenna height
            positionAlloc->Add(Vector(x_ue, y_ue, z_ue));
        }

    }

    MobilityHelper mobility;
    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.SetPositionAllocator (positionAlloc);

    NS_LOG_DEBUG ("Set path loss configuration");
    Ptr<MatrixPropagationLossModel> propagationPathlossMatrix = CreateObject<MatrixPropagationLossModel> ();
    propagationPathlossMatrix->SetDefaultLoss(200.0); // fallback if no explicit value

    // Example parameters for log-distance model
    double freqHz = 3.5e9;   // 3.5 GHz NR band
    double c = 3e8;          // speed of light
    double d0 = 1.0;         // reference distance (m)
    double PL0 = 20*log10(4*M_PI*d0*freqHz/c); // Friis at 1 m
    double n = 3.5;          // pathloss exponent (urban macro typical)

    // Loop over gNB–UE pairs
    for (uint32_t i = 0; i < gNbNodes.GetN(); i++)
    {
        Vector gnbPos = gNbNodes.Get(i)->GetObject<MobilityModel>()->GetPosition();

        for (uint32_t j = 0; j < ueNodes.GetN(); j++)
        {
            Vector uePos = ueNodes.Get(j)->GetObject<MobilityModel>()->GetPosition();

            // Euclidean distance
            double dx = gnbPos.x - uePos.x;
            double dy = gnbPos.y - uePos.y;
            double dz = gnbPos.z - uePos.z;
            double d = sqrt(dx*dx + dy*dy + dz*dz);

            // Log-distance pathloss model
            double PLdB = PL0 + 10*n*log10(d/d0);

            // Store in matrix
            propagationPathlossMatrix->SetLoss(gNbNodes.Get(i), ueNodes.Get(j), PLdB);
        }
    }

    Packet::EnablePrinting ();

    Ptr<SpectrumChannel> spectrumChannel;
    Ptr<MultiModelSpectrumChannel> channel = CreateObject<MultiModelSpectrumChannel>();
    Ptr<ThreeGppPropagationLossModel> propagation = CreateObject<ThreeGppRmaPropagationLossModel>();
    Ptr<ThreeGppSpectrumPropagationLossModel> spectrumPropagation = CreateObject<ThreeGppSpectrumPropagationLossModel> ();

    propagation->SetAttributeFailSafe("Frequency", DoubleValue(frequency));
    spectrumPropagation->SetChannelModelAttribute("Frequency", DoubleValue(frequency));

    BandwidthPartInfo::Scenario channelScenario = BandwidthPartInfo::RMa_LoS;
    Ptr<ChannelConditionModel> channelConditionModel = CreateObject<ThreeGppRmaChannelConditionModel> ();
    spectrumPropagation->SetChannelModelAttribute ("Scenario", StringValue ("RMa"));
    spectrumPropagation->SetChannelModelAttribute ("ChannelConditionModel", PointerValue (channelConditionModel));
    propagation->SetChannelConditionModel (channelConditionModel);

    channel->AddPropagationLossModel (propagation);
    channel->AddSpectrumPropagationLossModel (spectrumPropagation);

    NS_LOG_DEBUG("Create the internet and remote host");
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create (1);
    Ptr<Node> remoteHost = remoteHostContainer.Get (0);
    InternetStackHelper internet;
    internet.Install (remoteHostContainer);

    NS_LOG_DEBUG("Creating IP adresses for UEs and linking to AP");
    Ipv4InterfaceContainer ueIpIface;
    Ipv4InterfaceContainer ueIpIfaceOperatorWifi, ueIpIfaceOperatorNru[numNruPairs];
    Ipv4InterfaceContainer remoteWifiIface ,remoteNruIface;
    std::unordered_map<uint32_t, NodeContainer> networkWifiMap;
    std::unordered_map<uint32_t, Ptr<Node>> apMap;

    std::unique_ptr<Ipv4AddressHelper> address[numNrPairs]; 









}