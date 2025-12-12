#include "ns3/antenna-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/config-store.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ideal-beamforming-algorithm.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/log.h"
#include "ns3/network-module.h"
#include "ns3/nr-helper.h"
#include "ns3/nr-mac-scheduler-tdma-rr.h"
#include "ns3/nr-module.h"
#include "ns3/nr-point-to-point-epc-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/point-to-point-module.h"
#include <fstream>

#include "ns3/ai-module.h"
#include "nr-rl-env.h"

NS_LOG_COMPONENT_DEFINE("NrRlExample");

using namespace ns3;

Ptr<OpenGymInterface> g_openGymInterface;
Ptr<NrRlEnv> g_env;
uint32_t g_numAps = 6;
uint32_t maxNodes = 6;
Time g_timeStep = MilliSeconds(100);
std::vector<double> g_airTime (maxNodes);
std::vector<Time> g_nrOccupancyOld (maxNodes);
std::vector<Time> g_nrOccupancy (maxNodes);
std::vector<bool> g_nrIsOccupying (maxNodes);
std::vector<bool> g_firstIterationUdp(maxNodes);
std::vector<bool> g_firstIterationBurst(maxNodes);
std::vector<double> g_ueRxPower(maxNodes);

bool g_connect = false;


//Rewards
std::vector<std::pair<uint32_t, double>> g_throughput(maxNodes);
std::vector<std::pair<uint32_t, double>> g_delay(maxNodes);
std::vector<std::pair<uint32_t, double>> g_jitter(maxNodes);

std::vector<double> g_throughputDiff(maxNodes);
std::vector<double> g_jitterDiff(maxNodes);
std::vector<double> g_delayDiff(maxNodes);


std::vector<uint32_t> g_mcs(maxNodes);
std::vector<double> g_txPower(maxNodes);
std::vector<uint32_t> g_numerology(maxNodes);


void 
CreateEnv()
{
    // std::cout << "Debug CreateEnv()" << std::endl;
    Ptr<NrRlTimeStepEnv> env;
    env = CreateObject<NrRlTimeStepEnv>(g_numAps);
    g_env = env;
}

void ScheduleNextStateRead ()
{
    g_env->ScheduleNextStateRead();
}

void
GiveAirTime ()
{
    for (uint32_t i = 0; i < g_airTime.size(); i++)
    {
        g_airTime[i] = g_nrOccupancy[i].GetMilliSeconds() - g_nrOccupancyOld[i].GetMilliSeconds();
        g_env->GiveAirTime (g_airTime[i],i);
        g_airTime[i] = 0; //reset
    }
    g_nrOccupancyOld = g_nrOccupancy;

    Simulator::Schedule (g_timeStep, &GiveAirTime);
}

void
ChangeRlTypeAlt()
{
  if (!g_connect)
  {
    g_connect = true;
    CreateEnv();
  }
  for (uint16_t i = 0; i < g_airTime.size(); i++)
  {
    // std::cout << "ChangeMacType() new Mac parameters with iteration = i " << i << ": " << std::endl;
    double newTxPower = g_env->ChangeTxPower(g_txPower[i],i);
    uint32_t newMcs = g_env->ChangeMcs(g_mcs[i],i);
    uint16_t newNumerology = g_env->ChangeNumerology(g_numerology[i],i);
    g_txPower[i] = newTxPower;
    g_mcs[i] = newMcs;
    g_numerology[i] = newNumerology;
  }
  
  Simulator::Schedule (g_timeStep, &ChangeRlTypeAlt);
}

void
GiveThroughputAlt (FlowMonitorHelper *flowHelper, Ptr<FlowMonitor> flowMonitor, uint32_t numNrPairs)
{
    if(!g_connect)
    {
        g_connect = true;
        CreateEnv();
    }
    
    flowMonitor->SetAttribute("DelayBinWidth", DoubleValue(0.001));
    flowMonitor->SetAttribute("JitterBinWidth", DoubleValue(0.001));
    flowMonitor->SetAttribute("PacketSizeBinWidth", DoubleValue(20));

    flowMonitor->CheckForLostPackets();

    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowHelper->GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats ();

    uint32_t vecNum = 0;
    std::vector<std::pair<uint32_t, double>> newThroughput(numNrPairs);
    std::vector<std::pair<uint32_t, double>> newJitter(numNrPairs);
    std::vector<std::pair<uint32_t, double>> newDelay(numNrPairs);

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
        if (vecNum >= numNrPairs) break; // prevent overflow
    auto t = classifier->FindFlow(i->first);
    if (i->second.rxPackets > 1)
    {
        double throughputMbps = i->second.rxBytes * 8.0 / g_timeStep.GetSeconds() / 1e6;
        double delayMs = i->second.delaySum.GetMilliSeconds() / i->second.rxPackets;
        newThroughput[vecNum] = {t.destinationAddress.Get(), throughputMbps};
        newDelay[vecNum] = {t.destinationAddress.Get(), delayMs};
        newJitter[vecNum] = {t.destinationAddress.Get(), 0.0};
    }
    vecNum++;
}
newThroughput.resize(vecNum);
newDelay.resize(vecNum);
newJitter.resize(vecNum);
    
    double throughputDiff;
    double delayDiff;
    double jitterDiff;

    for (uint32_t k = 0; k < g_airTime.size(); k++)
    {
        for (uint32_t j = 0; j < newThroughput.size(); j++)
        {
            uint32_t throughputIp = 117440514 + k;
            if (newThroughput[j].first == throughputIp)
            {
                if (g_firstIterationUdp[k])
                {
                    throughputDiff = newThroughput[j].second;
                    delayDiff = newDelay[j].second;
                    jitterDiff = newJitter[j].second;
                    g_throughputDiff[k] = throughputDiff;
                    g_delayDiff[k] = delayDiff;
                    g_jitterDiff[k] = jitterDiff;
                }
                else 
                {
                    for (uint32_t i = 0; i < g_throughput.size(); i++)
                    {
                        if (g_throughput[i].first == throughputIp)
                        {
                            throughputDiff = newThroughput[j].second - g_throughput[i].second;
                            delayDiff = newDelay[j].second - g_delay[i].second;
                            if (delayDiff < 0)
                            {
                                delayDiff = newDelay[j].second;
                            }
                            jitterDiff = newJitter[j].second - g_jitter[i].second;
                            g_throughputDiff[k] = throughputDiff;
                            g_delayDiff[k] = delayDiff;
                            g_jitterDiff[k] = jitterDiff;
                        }
                    }
                }
                g_firstIterationUdp[k] = false;
            }
        }
    } 


    for (uint32_t i = 0; i < 6; i++)
    {    
        g_env->GiveThroughput (g_throughputDiff[i],i);
        g_env->GiveDelay (g_delayDiff[i],i);
        g_throughputDiff[i] = 0; //reset
        g_delayDiff[i] = 0;
        g_jitterDiff[i] = 0;
    }
    g_throughput = newThroughput;
    g_delay = newDelay;
    g_jitter = newJitter;


    Simulator::Schedule (g_timeStep, &GiveThroughputAlt, flowHelper, flowMonitor, numNrPairs);
}

void
GiveRxPower(NodeContainer gNbNodes, NodeContainer ueNodes, Ptr<ThreeGppUmaPropagationLossModel> lossModel)
{
    double pathloss = 0.0;
    double rxPower = 0.0;

    // Loop over all gNBs and UEs
    for (uint16_t i = 0; i < gNbNodes.GetN(); i++)
    {
        for (uint16_t j = 0; j < gNbNodes.GetN(); j++)
        {
            Ptr<MobilityModel> gnbMob = gNbNodes.Get(i)->GetObject<MobilityModel>();
            Ptr<MobilityModel> ueMob  = ueNodes.Get(j)->GetObject<MobilityModel>();

            double txPowerDbm = g_txPower[i]; // or your gNB’s configured Tx power
            double rxPowerDbm = lossModel->CalcRxPower(txPowerDbm, gnbMob, ueMob);

            // Store and feed into RL environment
            g_ueRxPower[j] = rxPowerDbm;
            g_env->GiveRxPower(rxPowerDbm, i, j);

        }
    }

    // Reschedule this function to run again after g_timeStep
    Simulator::Schedule(g_timeStep, &GiveRxPower, gNbNodes, ueNodes, lossModel);
}

void
UpdateRlParameters (NetDeviceContainer enbNetDev, Ptr<NrHelper> nrHelper, bool baselineMode)
{
    // std::cout << "Debug UpdateMacParameters()" << std::endl;
    if(!baselineMode)
    {
        for (uint32_t i = 0; i < g_airTime.size(); i++)
        {
            // change mcs
            Ptr<NrMacScheduler> sched = nrHelper->GetScheduler(enbNetDev.Get(i), 0);
            sched->SetAttribute("StartingMcsDl", UintegerValue(g_mcs[i]));
            
            // change tx power
            double gnbX = pow(10, g_txPower[i] / 10);
            Ptr<NrGnbPhy> phy = nrHelper->GetGnbPhy(enbNetDev.Get(i), 0);
            phy->SetTxPower(10 * log10(gnbX));
            
            // change numerology
            //nrHelper->GetGnbPhy(enbNetDev.Get(i), 0)
              //      ->SetAttribute("Numerology", UintegerValue(g_numerology[i]));


            std::cout << "Time " << Simulator::Now().GetSeconds()
                    << "s | gNB " << i
                    << " TxPower=" << g_txPower[i]
                    << " dBm, MCS=" << g_mcs[i]
                    << ", Numerology=" << g_numerology[i]
                    << std::endl;

        }
        Simulator::Schedule (g_timeStep, &UpdateRlParameters, enbNetDev, nrHelper, baselineMode);
    }
}


int
main(int argc, char* argv[])
{

    std::cout << "Starting NR RL example..." << std::endl;
    for (uint32_t i = 0; i < maxNodes; i++)
    {
        g_nrIsOccupying[i] = false;
    }    

    uint16_t gNbNum = 3;
    uint16_t ueNumPergNb = 5;
    uint16_t numFlowsUe = 1;
    uint32_t numNrPairs = gNbNum * ueNumPergNb;
    g_numAps = gNbNum;
    uint32_t timeStep = 100;


    uint8_t numBands = 1;
    double centralFrequencyBand = 28e9;
    double bandwidthBand = 3e9;

    bool contiguousCc = true;
    bool baselineMode = false;

    uint16_t numerology = 1; // for contiguous case
    uint8_t mcsValue = 28;
    double totalTxPower = 70;


    // non-contiguous case
    double centralFrequencyCc0 = 28e9;
    double centralFrequencyCc1 = 29e9;
    double bandwidthCc0 = 400e6;
    double bandwidthCc1 = 100e6;
    uint16_t numerologyCc0Bwp0 = 3;
    uint16_t numerologyCc0Bwp1 = 4;
    uint16_t numerologyCc1Bwp0 = 3;

    std::string pattern =
        "F|F|F|F|F|F|F|F|F|F|"; // Pattern can be e.g. "DL|S|UL|UL|DL|DL|S|UL|UL|DL|"

    bool cellScan = false;
    double beamSearchAngleStep = 10.0;

    bool udpFullBuffer = false;
    uint32_t udpPacketSizeUll = 100;
    uint32_t udpPacketSizeBe = 1252;
    uint32_t lambdaUll = 10000;
    uint32_t lambdaBe = 1000;

    bool logging = false;

    bool disableDl = false;
    bool disableUl = true;

    std::string simTag = "default";
    std::string outputDir = "./";

    double simTime = 0.5;           // seconds
    double udpAppStartTime = 0.1; // seconds

    

// cmd user inputs
//
//
// ...
//
//
    std::string envNumber;
    uint32_t simRound = 0;
    bool customEpisode = false;
    uint32_t numAgents = 0;
    std::string trafficType;
    uint32_t packetSize = 0;
    uint32_t fragmentSize = 0;
    uint32_t udpLambda1 = 0;
    uint32_t udpLambda2 = 0;
    uint32_t udpLambda3 = 0;
    uint32_t udpLambda4 = 0;
    uint32_t udpLambda5 = 0;
    uint32_t udpLambda6 = 0;
    bool isEvaluation = false;

    // Set up ns-3 CommandLine parser
    CommandLine cmd;
    cmd.AddValue("envNumber", "Shared memory segment suffix (PID)", envNumber);
    cmd.AddValue("simTime", "Simulation time (seconds)", simTime);
    cmd.AddValue("simRound", "Simulation run ID", simRound);
    cmd.AddValue("customEpisode", "Use custom traffic parameters per episode", customEpisode);
    cmd.AddValue("numAgents", "Number of DRL agents", numAgents);
    cmd.AddValue("trafficType", "Traffic type (UDP_CBR or BURST)", trafficType);
    cmd.AddValue("packetSize", "Packet size in bytes", packetSize);
    cmd.AddValue("fragmentSize", "Fragment size in bytes", fragmentSize);
    cmd.AddValue("udpLambda1", "Lambda for UDP interval (AP1)", udpLambda1);
    cmd.AddValue("udpLambda2", "Lambda for UDP interval (AP2)", udpLambda2);
    cmd.AddValue("udpLambda3", "Lambda for UDP interval (AP3)", udpLambda3);
    cmd.AddValue("udpLambda4", "Lambda for UDP interval (AP4)", udpLambda4);
    cmd.AddValue("udpLambda5", "Lambda for UDP interval (AP5)", udpLambda5);
    cmd.AddValue("udpLambda6", "Lambda for UDP interval (AP6)", udpLambda6);
    cmd.AddValue("isEvaluation", "Run in evaluation mode", isEvaluation);
    cmd.AddValue("baselineMode", "Run in baseline (no RL) mode", baselineMode);

    // Parse the actual arguments
    cmd.Parse(argc, argv);

    NS_ABORT_IF(numBands < 1);
    NS_ABORT_MSG_IF(disableDl == true && disableUl == true, "Enable one of the flows");

    // ConfigStore inputConfig;
    // inputConfig.ConfigureDefaults ();

    // enable logging or not
    if (logging)
    {
        LogComponentEnable("Nr3gppPropagationLossModel", LOG_LEVEL_ALL);
        LogComponentEnable("Nr3gppBuildingsPropagationLossModel", LOG_LEVEL_ALL);
        LogComponentEnable("Nr3gppChannel", LOG_LEVEL_ALL);
        LogComponentEnable("UdpClient", LOG_LEVEL_INFO);
        LogComponentEnable("UdpServer", LOG_LEVEL_INFO);
        LogComponentEnable("LtePdcp", LOG_LEVEL_INFO);
    }

    Config::SetDefault("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue(999999999));


    // create base stations and mobile terminals
    NodeContainer gNbNodes;
    NodeContainer ueNodes;
    MobilityHelper mobility;

    g_openGymInterface = OpenGymInterface::Get(envNumber);

    double gNbHeight = 10;
    double ueHeight = 1.5;
    g_timeStep =  MilliSeconds(timeStep);
    g_airTime.resize(gNbNum);
    g_nrOccupancy.resize(gNbNum);
    g_nrOccupancyOld.resize(gNbNum);
    g_nrIsOccupying.resize(gNbNum);
    g_firstIterationUdp.resize(gNbNum, true);
    g_firstIterationBurst.resize(gNbNum, true);
    g_ueRxPower.resize(gNbNum);
    g_throughput.resize(gNbNum);
    g_delay.resize(gNbNum);
    g_jitter.resize(gNbNum);
    g_throughputDiff.resize(gNbNum);
    g_delayDiff.resize(gNbNum);
    g_jitterDiff.resize(gNbNum);
    g_mcs.resize(gNbNum);
    g_txPower.resize(gNbNum);
    g_numerology.resize(gNbNum);

    for (uint32_t i = 0; i < gNbNum; i++)
    {
        g_firstIterationUdp[i] = true;
        g_firstIterationBurst[i] = true;
    }

    gNbNodes.Create(gNbNum);
    ueNodes.Create(ueNumPergNb * gNbNum);

    Ptr<ListPositionAllocator> apPositionAlloc = CreateObject<ListPositionAllocator>();
    Ptr<ListPositionAllocator> staPositionAlloc = CreateObject<ListPositionAllocator>();

    Ptr<UniformRandomVariable> randX = CreateObject<UniformRandomVariable>();
    Ptr<UniformRandomVariable> randY = CreateObject<UniformRandomVariable>();

    // Define the area bounds for gNBs
    double gNbXmin = -2500.0, gNbXmax = 2500.0;
    double gNbYmin = -2500.0, gNbYmax = 2500.0;

    for (uint32_t i = 0; i < gNbNodes.GetN(); ++i)
    {
        double gnbX = randX->GetValue(gNbXmin, gNbXmax);
        double gnbY = randY->GetValue(gNbYmin, gNbYmax);

        apPositionAlloc->Add(Vector(gnbX, gnbY, gNbHeight));

        // Place UEs around this gNB
        for (uint32_t j = 0; j < ueNumPergNb; ++j)
        {
            double dx = randX->GetValue(-500.0, 500.0);   // UE offset in X
            double dy = randY->GetValue(-500.0, 500.0);  // UE offset in Y

            staPositionAlloc->Add(Vector(gnbX + dx, gnbY + dy, ueHeight));
        }
    }


    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.SetPositionAllocator(apPositionAlloc);
    mobility.Install(gNbNodes);

    mobility.SetPositionAllocator(staPositionAlloc);
    mobility.Install(ueNodes);


    // setup the nr simulation
    Ptr<NrPointToPointEpcHelper> epcHelper = CreateObject<NrPointToPointEpcHelper>();
    Ptr<IdealBeamformingHelper> idealBeamformingHelper = CreateObject<IdealBeamformingHelper>();
    Ptr<NrHelper> nrHelper = CreateObject<NrHelper>();

    nrHelper->SetBeamformingHelper(idealBeamformingHelper);
    nrHelper->SetEpcHelper(epcHelper);

    BandwidthPartInfoPtrVector allBwps;
    CcBwpCreator ccBwpCreator;

    OperationBandInfo band;

    // For the case of manual configuration of CCs and BWPs
    std::unique_ptr<ComponentCarrierInfo> cc0(new ComponentCarrierInfo());
    std::unique_ptr<BandwidthPartInfo> bwp0(new BandwidthPartInfo());
    std::unique_ptr<BandwidthPartInfo> bwp1(new BandwidthPartInfo());

    std::unique_ptr<ComponentCarrierInfo> cc1(new ComponentCarrierInfo());
    std::unique_ptr<BandwidthPartInfo> bwp2(new BandwidthPartInfo());

    if (contiguousCc == true)
    {
        /*
         * CC band configuration n257F (NR Release 15): four contiguous CCs of
         * 400MHz at maximum. In this automated example, each CC contains a single
         * BWP occupying the whole CC bandwidth.
         *
         * The configured spectrum division is:
         * ----------------------------- Band --------------------------------
         * ------CC0------|------CC1-------|-------CC2-------|-------CC3-------
         * ------BWP0-----|------BWP0------|-------BWP0------|-------BWP0------
         */

        const uint8_t numContiguousCcs = 4; // 4 CCs per Band

        // Create the configuration for the CcBwpHelper
        CcBwpCreator::SimpleOperationBandConf bandConf(centralFrequencyBand,
                                                       bandwidthBand,
                                                       numContiguousCcs,
                                                       BandwidthPartInfo::UMa_LoS);

        bandConf.m_numBwp = 1; // 1 BWP per CC

        // By using the configuration created, it is time to make the operation band
        band = ccBwpCreator.CreateOperationBandContiguousCc(bandConf);
    }
    else
    {
        /*
         * The configured spectrum division is:
         * ----------------------------- Band ---------------------------------
         * ---------------CC0--------------|----------------CC1----------------
         * ------BWP0------|------BWP1-----|----------------BWP0---------------
         */
        
    }
    /*else
      {
        nrHelper->SetAttribute ("UseCa", BooleanValue (false));
      }*/

    // NS_ABORT_MSG_IF (ccId < 1,"No CC created");

    nrHelper->SetPathlossAttribute("ShadowingEnabled", BooleanValue(false));
    epcHelper->SetAttribute("S1uLinkDelay", TimeValue(MilliSeconds(0)));
    nrHelper->SetSchedulerTypeId(TypeId::LookupByName("ns3::NrMacSchedulerTdmaRR"));
    // Beamforming method
    if (cellScan)
    {
        idealBeamformingHelper->SetAttribute("BeamformingMethod",
                                             TypeIdValue(CellScanBeamforming::GetTypeId()));
        idealBeamformingHelper->SetBeamformingAlgorithmAttribute("BeamSearchAngleStep",
                                                                 DoubleValue(beamSearchAngleStep));
    }
    else
    {
        idealBeamformingHelper->SetAttribute("BeamformingMethod",
                                             TypeIdValue(DirectPathBeamforming::GetTypeId()));
    }

    nrHelper->InitializeOperationBand(&band);
    allBwps = CcBwpCreator::GetAllBwps({band});

    double x = pow(10, totalTxPower / 10);

    // Antennas for all the UEs
    nrHelper->SetUeAntennaAttribute("NumRows", UintegerValue(2));
    nrHelper->SetUeAntennaAttribute("NumColumns", UintegerValue(4));
    nrHelper->SetUeAntennaAttribute("AntennaElement",
                                    PointerValue(CreateObject<IsotropicAntennaModel>()));

    // Antennas for all the gNbs
    nrHelper->SetGnbAntennaAttribute("NumRows", UintegerValue(4));
    nrHelper->SetGnbAntennaAttribute("NumColumns", UintegerValue(8));
    nrHelper->SetGnbAntennaAttribute("AntennaElement",
                                     PointerValue(CreateObject<IsotropicAntennaModel>()));

    uint32_t bwpIdForLowLat = 0;
    uint32_t bwpIdForVoice = 1;
    uint32_t bwpIdForVideo = 2;
    uint32_t bwpIdForVideoGaming = 3;

    nrHelper->SetGnbBwpManagerAlgorithmAttribute("NGBR_LOW_LAT_EMBB",
                                                 UintegerValue(bwpIdForLowLat));
    nrHelper->SetGnbBwpManagerAlgorithmAttribute("GBR_CONV_VOICE", UintegerValue(bwpIdForVoice));
    nrHelper->SetGnbBwpManagerAlgorithmAttribute("NGBR_VIDEO_TCP_PREMIUM",
                                                 UintegerValue(bwpIdForVideo));
    nrHelper->SetGnbBwpManagerAlgorithmAttribute("NGBR_VOICE_VIDEO_GAMING",
                                                 UintegerValue(bwpIdForVideoGaming));

    // Install and get the pointers to the NetDevices
    NetDeviceContainer enbNetDev = nrHelper->InstallGnbDevice(gNbNodes, allBwps);
    NetDeviceContainer ueNetDev = nrHelper->InstallUeDevice(ueNodes, allBwps);

    
    nrHelper->SetSchedulerAttribute("FixedMcsDl", BooleanValue(true));

    for (uint32_t f = 0; f < enbNetDev.GetN(); ++f)
    {
        for (uint32_t bwpId = 0; bwpId < allBwps.size(); ++bwpId)
        {
        Ptr<NrMacScheduler> sched = nrHelper->GetScheduler(enbNetDev.Get(f), bwpId);
        // Force fixed DL/UL MCS
        sched->SetAttribute("StartingMcsDl", UintegerValue(mcsValue));
        }
    }

    int64_t randomStream = 1;
    randomStream += nrHelper->AssignStreams(enbNetDev, randomStream);
    randomStream += nrHelper->AssignStreams(ueNetDev, randomStream);


    for (uint32_t g = 0; g < enbNetDev.GetN(); ++g)
    {
        if (contiguousCc)
        {
            // Just loop over all BWPs in the contiguous case
            for (uint32_t bwpId = 0; bwpId < allBwps.size(); ++bwpId)
            {
                nrHelper->GetGnbPhy(enbNetDev.Get(g), bwpId)
                    ->SetAttribute("Numerology", UintegerValue(numerology));
                nrHelper->GetGnbPhy(enbNetDev.Get(g), bwpId)
                    ->SetAttribute("TxPower", DoubleValue(10 * log10(0.25 * x)));
                nrHelper->GetGnbPhy(enbNetDev.Get(g), bwpId)
                    ->SetAttribute("Pattern", StringValue(pattern));
            }
        }
        else
        {
            // Non‑contiguous: loop over CCs and BWPs explicitly
            for (uint32_t ccId = 0; ccId < band.m_cc.size(); ++ccId)
            {
                auto &cc = band.m_cc.at(ccId);
                for (uint32_t bwpId = 0; bwpId < cc->m_bwp.size(); ++bwpId)
                {
                    BandwidthPartInfo *bwpInfo = cc->m_bwp.at(bwpId).get();

                    uint16_t num = (bwpInfo->m_bwpId == 0 ? numerologyCc0Bwp0 :
                                (bwpInfo->m_bwpId == 1 ? numerologyCc0Bwp1 : numerologyCc1Bwp0));

                    nrHelper->GetGnbPhy(enbNetDev.Get(g), bwpInfo->m_bwpId)
                        ->SetAttribute("Numerology", UintegerValue(num));
                    nrHelper->GetGnbPhy(enbNetDev.Get(g), bwpInfo->m_bwpId)
                        ->SetAttribute("TxPower",
                            DoubleValue(10 * log10((bwpInfo->m_channelBandwidth / bandwidthBand) * x)));
                    nrHelper->GetGnbPhy(enbNetDev.Get(g), bwpInfo->m_bwpId)
                        ->SetAttribute("Pattern", StringValue(pattern));
                }
            }
        }
    }



    for (auto it = enbNetDev.Begin(); it != enbNetDev.End(); ++it)
    {
        DynamicCast<NrGnbNetDevice>(*it)->UpdateConfig();
    }

    for (auto it = ueNetDev.Begin(); it != ueNetDev.End(); ++it)
    {
        DynamicCast<NrUeNetDevice>(*it)->UpdateConfig();
    }

    // create the internet and install the IP stack on the UEs
    // get SGW/PGW and create a single RemoteHost
    Ptr<Node> pgw = epcHelper->GetPgwNode();
    NodeContainer remoteHostContainer;
    remoteHostContainer.Create(1);
    Ptr<Node> remoteHost = remoteHostContainer.Get(0);
    InternetStackHelper internet;
    internet.Install(remoteHostContainer);

    // connect a remoteHost to pgw. Setup routing too
    PointToPointHelper p2ph;
    p2ph.SetDeviceAttribute("DataRate", DataRateValue(DataRate("100Gb/s")));
    p2ph.SetDeviceAttribute("Mtu", UintegerValue(2500));
    p2ph.SetChannelAttribute("Delay", TimeValue(Seconds(0.000)));
    NetDeviceContainer internetDevices = p2ph.Install(pgw, remoteHost);
    Ipv4AddressHelper ipv4h;
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    ipv4h.SetBase("1.0.0.0", "255.0.0.0");
    Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign(internetDevices);
    Ptr<Ipv4StaticRouting> remoteHostStaticRouting =
        ipv4RoutingHelper.GetStaticRouting(remoteHost->GetObject<Ipv4>());
    remoteHostStaticRouting->AddNetworkRouteTo(Ipv4Address("7.0.0.0"), Ipv4Mask("255.0.0.0"), 1);
    internet.Install(ueNodes);
    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueNetDev));

    Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress(1);

    // Set the default gateway for the UEs
    for (uint32_t j = 0; j < ueNodes.GetN(); ++j)
    {
        Ptr<Ipv4StaticRouting> ueStaticRouting =
            ipv4RoutingHelper.GetStaticRouting(ueNodes.Get(j)->GetObject<Ipv4>());
        ueStaticRouting->SetDefaultRoute(epcHelper->GetUeDefaultGatewayAddress(), 1);
    }

    // attach UEs to the closest eNB before creating the dedicated flows
    nrHelper->AttachToClosestEnb(ueNetDev, enbNetDev);

    // install UDP applications
    uint16_t dlPort = 1234;
    uint16_t ulPort = dlPort + gNbNum * ueNumPergNb * numFlowsUe + 1;
    ApplicationContainer clientApps;
    ApplicationContainer serverApps;

    for (uint32_t u = 0; u < ueNodes.GetN(); ++u)
    {
        for (uint16_t flow = 0; flow < numFlowsUe; ++flow)
        {
            if (!disableDl)
            {
                PacketSinkHelper dlPacketSinkHelper(
                    "ns3::UdpSocketFactory",
                    InetSocketAddress(Ipv4Address::GetAny(), dlPort));
                serverApps.Add(dlPacketSinkHelper.Install(ueNodes.Get(u)));

                UdpClientHelper dlClient(ueIpIface.GetAddress(u), dlPort);
                dlClient.SetAttribute("PacketSize", UintegerValue(udpPacketSizeBe));
                dlClient.SetAttribute("Interval", TimeValue(Seconds(1.0 / lambdaUll)));
                dlClient.SetAttribute("MaxPackets", UintegerValue(0xFFFFFFFF));
                clientApps.Add(dlClient.Install(remoteHost));

                Ptr<EpcTft> tft = Create<EpcTft>();
                EpcTft::PacketFilter dlpf;
                dlpf.localPortStart = dlPort;
                dlpf.localPortEnd = dlPort;
                ++dlPort;
                tft->Add(dlpf);

                enum EpsBearer::Qci q;
                if (flow == 0)
                {
                    q = EpsBearer::NGBR_LOW_LAT_EMBB;
                }
                else if (flow == 1)
                {
                    q = EpsBearer::GBR_CONV_VOICE;
                }
                else if (flow == 2)
                {
                    q = EpsBearer::NGBR_VIDEO_TCP_PREMIUM;
                }
                else if (flow == 3)
                {
                    q = EpsBearer::NGBR_VOICE_VIDEO_GAMING;
                }
                else
                {
                    q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT;
                }
                EpsBearer bearer(q);
                nrHelper->ActivateDedicatedEpsBearer(ueNetDev.Get(u), bearer, tft);
            }

            if (!disableUl)
            {
                PacketSinkHelper ulPacketSinkHelper(
                    "ns3::UdpSocketFactory",
                    InetSocketAddress(Ipv4Address::GetAny(), ulPort));
                serverApps.Add(ulPacketSinkHelper.Install(remoteHost));

                UdpClientHelper ulClient(remoteHostAddr, ulPort);
                ulClient.SetAttribute("PacketSize", UintegerValue(udpPacketSizeBe));
                ulClient.SetAttribute("Interval", TimeValue(Seconds(1.0 / lambdaUll)));
                ulClient.SetAttribute("MaxPackets", UintegerValue(0xFFFFFFFF));
                clientApps.Add(ulClient.Install(ueNodes.Get(u)));

                Ptr<EpcTft> tft = Create<EpcTft>();
                EpcTft::PacketFilter ulpf;
                ulpf.remotePortStart = ulPort;
                ulpf.remotePortEnd = ulPort;
                ++ulPort;
                tft->Add(ulpf);

                enum EpsBearer::Qci q;
                if (flow == 0)
                {
                    q = EpsBearer::NGBR_LOW_LAT_EMBB;
                }
                else if (flow == 1)
                {
                    q = EpsBearer::GBR_CONV_VOICE;
                }
                else if (flow == 2)
                {
                    q = EpsBearer::NGBR_VIDEO_TCP_PREMIUM;
                }
                else if (flow == 3)
                {
                    q = EpsBearer::NGBR_VOICE_VIDEO_GAMING;
                }
                else
                {
                    q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT;
                }
                EpsBearer bearer(q);
                nrHelper->ActivateDedicatedEpsBearer(ueNetDev.Get(u), bearer, tft);
            }
        }
    }

    // start UDP server and client apps
    serverApps.Start(Seconds(udpAppStartTime));
    clientApps.Start(Seconds(udpAppStartTime));
    serverApps.Stop(Seconds(simTime));
    clientApps.Stop(Seconds(simTime));

    // enable the traces provided by the nr module
    //nrHelper->EnableTraces();
    // Config::ConnectWithoutContext(
    //   "/NodeList/*/DeviceList/*/NrGnbPhy/BeamformingTrace",
    // MakeCallback(&BeamTraceHandler));

    FlowMonitorHelper flowmonHelper;
    NodeContainer endpointNodes;
    endpointNodes.Add(remoteHost);
    endpointNodes.Add(ueNodes);

    Ptr<ns3::FlowMonitor> monitor = flowmonHelper.Install(endpointNodes);
    monitor->SetAttribute("DelayBinWidth", DoubleValue(0.001));
    monitor->SetAttribute("JitterBinWidth", DoubleValue(0.001));
    monitor->SetAttribute("PacketSizeBinWidth", DoubleValue(20));

    Ptr<ThreeGppUmaPropagationLossModel> lossModel = CreateObject<ThreeGppUmaPropagationLossModel>(); 

    lossModel->SetAttribute("Frequency", DoubleValue(centralFrequencyBand)); // Hz


    Simulator::Schedule (MilliSeconds(0), &GiveThroughputAlt, &flowmonHelper, monitor, numNrPairs);
    Simulator::Schedule (MilliSeconds(0), &GiveRxPower, gNbNodes, ueNodes, lossModel);
    Simulator::Schedule (MilliSeconds(0), &GiveAirTime);
    Simulator::Schedule (MilliSeconds(0), &ChangeRlTypeAlt);
    Simulator::Schedule (MilliSeconds(0), &ScheduleNextStateRead);
    Simulator::Schedule (MilliSeconds(0), &UpdateRlParameters, enbNetDev, nrHelper, baselineMode);
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();

    /*
     * To check what was installed in the memory, i.e., BWPs of eNb Device, and its configuration.
     * Example is: Node 1 -> Device 0 -> BandwidthPartMap -> {0,1} BWPs -> NrGnbPhy ->
    NrPhyMacCommong-> Numerology, Bandwidth, ... GtkConfigStore config; config.ConfigureAttributes
    ();
    */

    // Print per-flow statistics
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier =
        DynamicCast<Ipv4FlowClassifier>(flowmonHelper.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

    double averageFlowThroughput = 0.0;
    double averageFlowDelay = 0.0;

    std::ofstream outFile;
    std::string filename = outputDir + "/" + simTag;
    outFile.open(filename.c_str(), std::ofstream::out | std::ofstream::trunc);
    if (!outFile.is_open())
    {
        std::cerr << "Can't open file " << filename << std::endl;
        return 1;
    }

    outFile.setf(std::ios_base::fixed);

    std::map<Ipv4Address,double> ueThroughput;

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();
         i != stats.end();
         ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
        std::stringstream protoStream;
        protoStream << (uint16_t)t.protocol;
        if (t.protocol == 6)
        {
            protoStream.str("TCP");
        }
        if (t.protocol == 17)
        {
            protoStream.str("UDP");
        }
        outFile << "Flow " << i->first << " (" << t.sourceAddress << ":" << t.sourcePort << " -> "
                << t.destinationAddress << ":" << t.destinationPort << ") proto "
                << protoStream.str() << "\n";
        outFile << "  Tx Packets: " << i->second.txPackets << "\n";
        outFile << "  Tx Bytes:   " << i->second.txBytes << "\n";
        outFile << "  TxOffered:  "
                << i->second.txBytes * 8.0 / (simTime - udpAppStartTime) / 1000 / 1000 << " Mbps\n";
        outFile << "  Rx Bytes:   " << i->second.rxBytes << "\n";
        if (i->second.rxPackets > 0)
        {
            // Measure the duration of the flow from receiver's perspective
            // double rxDuration = i->second.timeLastRxPacket.GetSeconds () -
            // i->second.timeFirstTxPacket.GetSeconds ();
            double rxDuration = (simTime - udpAppStartTime);
            double thrMbps = i->second.rxBytes * 8.0 / rxDuration / 1000 / 1000; // for visualization
            ueThroughput[t.destinationAddress] += thrMbps;
            averageFlowThroughput += i->second.rxBytes * 8.0 / rxDuration / 1000 / 1000;
            averageFlowDelay += 1000 * i->second.delaySum.GetSeconds() / i->second.rxPackets;

            outFile << "  Throughput: " << i->second.rxBytes * 8.0 / rxDuration / 1000 / 1000
                    << " Mbps\n";
            outFile << "  Mean delay:  "
                    << 1000 * i->second.delaySum.GetSeconds() / i->second.rxPackets << " ms\n";
            // outFile << "  Mean upt:  " << i->second.uptSum / i->second.rxPackets / 1000/1000 << "
            // Mbps \n";
            outFile << "  Mean jitter:  "
                    << 1000 * i->second.jitterSum.GetSeconds() / i->second.rxPackets << " ms\n";
        }
        else
        {
            outFile << "  Throughput:  0 Mbps\n";
            outFile << "  Mean delay:  0 ms\n";
            outFile << "  Mean jitter: 0 ms\n";
        }
        outFile << "  Rx Packets: " << i->second.rxPackets << "\n";
    }

    outFile << "\n\n  Mean flow throughput: " << averageFlowThroughput / stats.size() << "\n";
    outFile << "  Mean flow delay: " << averageFlowDelay / stats.size() << "\n";

    outFile.close();


        // Map visulization_______________________________________________________________
    std::ofstream posFile;
    posFile.open("contrib/ai/examples/nr-rl/positions.csv");
    posFile << "NodeType,ID,X,Y,Z,IPidentifier,ThroughputMbps,CellID\n";  // CSV header   

    // gNB positions
    for (uint32_t i = 0; i < enbNetDev.GetN(); ++i)
    {
        Ptr<MobilityModel> mob = gNbNodes.Get(i)->GetObject<MobilityModel>();
        Vector pos = mob->GetPosition();

        Ptr<NrGnbNetDevice> gnbDev = DynamicCast<NrGnbNetDevice>(enbNetDev.Get(i));
        uint16_t cellId = gnbDev->GetCellId();   // proper gNB cell ID

        Ptr<Ipv4> ipv4 = gNbNodes.Get(i)->GetObject<Ipv4>();
        Ipv4Address gnbIp = ipv4->GetAddress(1,0).GetLocal();  // gNB IP

        posFile << "gNB," << i << "," << pos.x << "," << pos.y << "," << pos.z
                << "," << gnbIp << "," << "" << "," << cellId << "\n";
    }

    // UE positions
    for (uint32_t j = 0; j < ueNetDev.GetN(); ++j)
    {
        Ptr<NrUeNetDevice> ueDev = DynamicCast<NrUeNetDevice>(ueNetDev.Get(j));
        uint16_t cellId = ueDev->GetCellId();   // serving gNB CellId

        Ipv4Address ip = ueIpIface.GetAddress(j);
        Ptr<MobilityModel> mob = ueNodes.Get(j)->GetObject<MobilityModel>();
        Vector pos = mob->GetPosition();

        double thr = 0.0;
        if (ueThroughput.find(ip) != ueThroughput.end())
        {
            thr = ueThroughput[ip];
        }
        // For now throughput is left blank or 0.0 until you compute it from FlowMonitor
        posFile << "UE," << j << "," << pos.x << "," << pos.y << "," << pos.z
                << "," << ip << "," << thr << "," << cellId << "\n";
    }

    posFile.close();

    //_________________________________________________________________________________


    std::ifstream f(filename.c_str());

    if (f.is_open())
    {
        std::cout << f.rdbuf();
    }
    g_openGymInterface->NotifySimulationEnd();

    Simulator::Destroy();
    return 0;
}
