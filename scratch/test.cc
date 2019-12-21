//Running simulation for 1 sec of data transfer; 5 sec overall
//Operator A: LAA; number of cells 1; number of UEs 1
//Operator B: Wi-Fi; number of cells 1; number of UEs 1
//Total txop duration: 0 seconds.
//Total phy arrivals duration: 0 seconds.
//--------monitorA----------
//Flow 1 (1.0.0.2:49153 -> 7.0.0.2:9) proto UDP
//  Tx Packets: 7303
//  Tx Bytes:   7507484
//  TxOffered:  60.0599 Mbps
//  Rx Bytes:   7507484
//  Throughput: 71.4195 Mbps
//  Mean delay:  27.8461 ms
//  Mean jitter:  0.201836 ms
//  Rx Packets: 7303
//--------monitorB----------
//Flow 1 (12.0.0.1:49153 -> 18.0.0.2:9) proto UDP
//  Tx Packets: 5197
//  Tx Bytes:   5342516
//  TxOffered:  42.7401 Mbps
//  Rx Bytes:   5342516
//  Throughput: 77.0265 Mbps
//  Mean delay:  0.509075 ms
//  Mean jitter:  0.0645998 ms
//  Rx Packets: 5197
//--- List of associations (time and node id) ---
//0.600294 3 associate 00:00:00:00:00:06
//
//  In this case, both LAA and Wi-Fi obtain good throughput and low latency.
//

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/lte-module.h>
#include <ns3/wifi-module.h>
#include <ns3/config-store-module.h>
#include <ns3/spectrum-module.h>
#include <ns3/applications-module.h>
#include <ns3/flow-monitor-module.h>
#include <ns3/propagation-module.h>
#include <ns3/scenario-helper.h>
#include <ns3/laa-wifi-coexistence-helper.h>
#include <ns3/lbt-access-manager.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LaaWifiCoexistenceSimple");

static ns3::GlobalValue g_numA("numA",
                                  "the number of cell A",
                                  ns3::IntegerValue(3),
                                  ns3::MakeIntegerChecker<int>());
static ns3::GlobalValue g_numB("numB",
                                  "the number of cell B",
                                  ns3::IntegerValue(3),
                                  ns3::MakeIntegerChecker<int>());
static ns3::GlobalValue g_laaDeferX("laaDeferX",
                                        "x of LAA's defer time",
                                        ns3::IntegerValue(3),
                                        ns3::MakeIntegerChecker<int>());
static ns3::GlobalValue g_laaMinCw("laaMinCw",
                                        "LAA's min cw",
                                        ns3::IntegerValue(15),
                                        ns3::MakeIntegerChecker<int>());
static ns3::GlobalValue g_laaMaxM("laaMaxM",
                                        "LAA's max value of M",
                                        ns3::IntegerValue(6),
                                        ns3::MakeIntegerChecker<int>());

// Global Values are used in place of command line arguments so that these
// values may be managed in the ns-3 ConfigStore system.
static ns3::GlobalValue g_d1 ("d1",
                              "intra-cell separation (e.g. AP to STA)",
                              ns3::DoubleValue (10),
                              ns3::MakeDoubleChecker<double> ());

static ns3::GlobalValue g_d2 ("d2",
                              "inter-cell separation",
                              ns3::DoubleValue (10),
                              ns3::MakeDoubleChecker<double> ());

static ns3::GlobalValue g_duration ("duration",
                                    "Data transfer duration (seconds)",
                                    ns3::DoubleValue (1),
                                    ns3::MakeDoubleChecker<double> ());

static ns3::GlobalValue g_cellConfigA ("cellConfigA",
                                       "Laa, Lte, or Wifi",
                                        ns3::EnumValue (LAA),
                                        ns3::MakeEnumChecker (WIFI, "Wifi",
                                                              LTE, "Lte",
                                                              LAA, "Laa"));
static ns3::GlobalValue g_cellConfigB ("cellConfigB",
                                       "Laa, Lte, or Wifi",
                                        ns3::EnumValue (WIFI),
                                        ns3::MakeEnumChecker (WIFI, "Wifi",
                                                              LTE, "Lte",
                                                              LAA, "Laa"));

static ns3::GlobalValue g_channelAccessManager ("ChannelAccessManager",
                                                "Default, DutyCycle, Lbt",
                                                ns3::EnumValue (Lbt),
                                                ns3::MakeEnumChecker (Default, "Default",
                                                                      DutyCycle, "DutyCycle",
                                                                      Lbt, "Lbt"));

static ns3::GlobalValue g_lbtTxop ("lbtTxop",
                                   "TxOp for LBT devices (ms)",
                                   ns3::DoubleValue (8.0),
                                   ns3::MakeDoubleChecker<double> (0.0, 20.0));

static ns3::GlobalValue g_useReservationSignal ("useReservationSignal",
                                                "Defines whether reservation signal will be used when used channel access manager at LTE eNb",
                                                ns3::BooleanValue (true),
                                                ns3::MakeBooleanChecker ());

static ns3::GlobalValue g_laaEdThreshold ("laaEdThreshold",
                                   "CCA-ED threshold for channel access manager (dBm)",
                                   ns3::DoubleValue (-72.0),
                                   ns3::MakeDoubleChecker<double> (-100.0, -50.0));

static ns3::GlobalValue g_pcap ("pcapEnabled",
                                "Whether to enable pcap trace files for Wi-Fi",
                                ns3::BooleanValue (false),
                                ns3::MakeBooleanChecker ());

static ns3::GlobalValue g_ascii ("asciiEnabled",
                                "Whether to enable ascii trace files for Wi-Fi",
                                ns3::BooleanValue (false),
                                ns3::MakeBooleanChecker ());

static ns3::GlobalValue g_transport ("transport",
                                     "whether to use 3GPP Ftp, Udp, or Tcp",
                                     ns3::EnumValue (UDP),
                                     ns3::MakeEnumChecker (FTP, "Ftp",
                                                           UDP, "Udp",
                                                           TCP, "Tcp"));

static ns3::GlobalValue g_lteDutyCycle ("lteDutyCycle",
                                    "Duty cycle value to be used for LTE",
                                    ns3::DoubleValue (1),
                                    ns3::MakeDoubleChecker<double> (0.0, 1.0));

static ns3::GlobalValue g_generateRem ("generateRem",
                                       "if true, will generate a REM and then abort the simulation;"
                                       "if false, will run the simulation normally (without generating any REM)",
                                       ns3::BooleanValue (false),
                                       ns3::MakeBooleanChecker ());

static ns3::GlobalValue g_simTag ("simTag",
                                  "tag to be appended to output filenames to distinguish simulation campaigns",
                                  ns3::StringValue ("default"),
                                  ns3::MakeStringChecker ());

static ns3::GlobalValue g_outputDir ("outputDir",
                                     "directory where to store simulation results",
                                     ns3::StringValue ("./"),
                                     ns3::MakeStringChecker ());

static ns3::GlobalValue g_cwUpdateRule ("cwUpdateRule",
                                         "Rule that will be used to update contention window of LAA node",
                                         ns3::EnumValue (LbtAccessManager::NACKS_80_PERCENT),
                                         ns3::MakeEnumChecker (ns3::LbtAccessManager::ALL_NACKS, "all",
                                        		 	 	 	   ns3::LbtAccessManager::ANY_NACK, "any",
                                        		 	 	 	   ns3::LbtAccessManager::NACKS_10_PERCENT, "nacks10",
                                        		 	 	 	   ns3::LbtAccessManager::NACKS_80_PERCENT, "nacks80"));

// Higher lambda means faster arrival rate; values [0.5, 1, 1.5, 2, 2.5]
// recommended
static ns3::GlobalValue g_ftpLambda ("ftpLambda",
                                    "Lambda value for FTP model 1 application",
                                    ns3::DoubleValue (0.5),
                                    ns3::MakeDoubleChecker<double> ());

static ns3::GlobalValue g_voiceEnabled ("voiceEnabled",
                                       "Whether to enable voice",
                                       ns3::BooleanValue (false),
                                       ns3::MakeBooleanChecker ());

static ns3::GlobalValue g_indoorLossModel ("indoorLossModel",
                                           "TypeId string of indoor propagation loss model",
                                            ns3::StringValue ("ns3::LogDistancePropagationLossModel"),
                                            ns3::MakeStringChecker ());

// Global variables for use in callbacks.
double g_signalDbmAvg[2];
double g_noiseDbmAvg[2];
uint32_t g_samples[2];
uint16_t g_channelNumber[2];
uint32_t g_rate[2];

int
main (int argc, char *argv[])
{  
  CommandLine cmd;
  cmd.Parse (argc, argv);
  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();
  // parse again so you can override input file default values via command line
  cmd.Parse (argc, argv);

  DoubleValue doubleValue;
  EnumValue enumValue;
  BooleanValue booleanValue;
  StringValue stringValue;
  IntegerValue integerValue;

  GlobalValue::GetValueByName ("d1", doubleValue);
  double d1 = doubleValue.Get ();
  GlobalValue::GetValueByName ("d2", doubleValue);
  double d2 = doubleValue.Get ();
  GlobalValue::GetValueByName ("cellConfigA", enumValue);
  enum Config_e cellConfigA = (Config_e) enumValue.Get ();
  GlobalValue::GetValueByName ("cellConfigB", enumValue);
  enum Config_e cellConfigB = (Config_e) enumValue.Get ();
  GlobalValue::GetValueByName ("ChannelAccessManager", enumValue);
  enum Config_ChannelAccessManager channelAccessManager = (Config_ChannelAccessManager) enumValue.Get ();
  GlobalValue::GetValueByName ("duration", doubleValue);
  double duration = doubleValue.Get ();
  GlobalValue::GetValueByName ("lbtTxop", doubleValue);
  double lbtTxop = doubleValue.Get ();
  GlobalValue::GetValueByName ("laaEdThreshold", doubleValue);
  double laaEdThreshold = doubleValue.Get ();
  GlobalValue::GetValueByName ("transport", enumValue);
  enum Transport_e transport = (Transport_e) enumValue.Get ();
  GlobalValue::GetValueByName ("lteDutyCycle", doubleValue);
  double lteDutyCycle = doubleValue.Get ();
  GlobalValue::GetValueByName ("generateRem", booleanValue);
  bool generateRem = booleanValue.Get ();
  GlobalValue::GetValueByName ("simTag", stringValue);
  std::string simTag = stringValue.Get ();
  GlobalValue::GetValueByName ("outputDir", stringValue);
  std::string outputDir = stringValue.Get ();
  GlobalValue::GetValueByName ("useReservationSignal", booleanValue);
  bool useReservationSignal = booleanValue.Get ();
  GlobalValue::GetValueByName ("cwUpdateRule", enumValue);
  enum  LbtAccessManager::CWUpdateRule_t cwUpdateRule = (LbtAccessManager::CWUpdateRule_t) enumValue.Get ();
  GlobalValue::GetValueByName ("indoorLossModel", stringValue);
  std::string indoorLossModel = stringValue.Get ();

  GlobalValue::GetValueByName ("numA",integerValue);
  int numA = integerValue.Get();
  GlobalValue::GetValueByName ("numB",integerValue);
  int numB = integerValue.Get();
  GlobalValue::GetValueByName("laaDeferX",integerValue);
  int laaDeferX=integerValue.Get();
  GlobalValue::GetValueByName ("laaMinCw",integerValue);
  int laaMinCw = integerValue.Get();
  GlobalValue::GetValueByName ("laaMaxM",integerValue);
  int laaMaxM = integerValue.Get();
  Config::SetDefault ("ns3::ChannelAccessManager::EnergyDetectionThreshold", DoubleValue (laaEdThreshold));
  switch (channelAccessManager)
    {
    case Lbt:
      Config::SetDefault ("ns3::LaaWifiCoexistenceHelper::ChannelAccessManagerType", StringValue ("ns3::LbtAccessManager"));
      Config::SetDefault ("ns3::LbtAccessManager::Txop", TimeValue (Seconds (lbtTxop/1000.0)));
      Config::SetDefault ("ns3::LbtAccessManager::UseReservationSignal", BooleanValue(useReservationSignal));
      Config::SetDefault ("ns3::LbtAccessManager::CwUpdateRule", EnumValue(cwUpdateRule));

      std::cout<<"lbt defer time is "<<(16.0+9.0*laaDeferX)<<"us"<<std::endl<<"lbt mincw is "<<laaMinCw<<std::endl<<"lbt maxcw is "<<(laaMinCw+1)*(1<<laaMaxM)<<std::endl;
      Config::SetDefault ("ns3::LbtAccessManager::DeferTime",TimeValue(Seconds ((16.0+9.0*laaDeferX)/1000000.0)));
      Config::SetDefault ("ns3::LbtAccessManager::MinCw",UintegerValue(static_cast<uint32_t>(laaMinCw)));
      Config::SetDefault ("ns3::LbtAccessManager::MaxCw",UintegerValue(static_cast<uint32_t>(laaMinCw*(1<<laaMaxM))));

      break;
    case DutyCycle:
      Config::SetDefault ("ns3::LaaWifiCoexistenceHelper::ChannelAccessManagerType", StringValue ("ns3::DutyCycleAccessManager"));
      Config::SetDefault ("ns3::DutyCycleAccessManager::OnDuration", TimeValue (MilliSeconds (60)));
      Config::SetDefault ("ns3::DutyCycleAccessManager::OnStartTime",TimeValue (MilliSeconds (0)));
      Config::SetDefault ("ns3::DutyCycleAccessManager::DutyCyclePeriod",TimeValue (MilliSeconds (80)));
      break;
    default:
      //default LTE channel access manager will be used, LTE always transmits
      break;
    } 

  // When logging, use prefixes
  LogComponentEnableAll (LOG_PREFIX_TIME);
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnableAll (LOG_PREFIX_NODE);

  // Create nodes and containers
  NodeContainer bsNodesA, bsNodesB;  // for APs and eNBs
  NodeContainer ueNodesA, ueNodesB;  // for STAs and UEs
  NodeContainer allWirelessNodes;  // container to hold all wireless nodes 
  // Each network A and B gets one type of node each
  bsNodesA.Create (numA);
  bsNodesB.Create (numB);
  ueNodesA.Create (numA);
  ueNodesB.Create (numB);
  allWirelessNodes = NodeContainer (bsNodesA, bsNodesB, ueNodesA, ueNodesB);

  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  double delta_x = 0.0;
  for(int i = 0; i < numA; ++i) // eNB1/AP in cell 0
  {
    positionAlloc->Add (Vector (delta_x, 0.0, 0.0));
    delta_x += 1; 
  }
  for(int i = 0; i < numB; ++i)  // AP in cell 1
  {
    positionAlloc->Add (Vector (d2 - delta_x, d1, 0.0));
    delta_x -= 1; 
  }
  for(int i = 0; i < numA; ++i) // UE1/STA in cell 0
  {
    positionAlloc->Add (Vector (delta_x, d1, 0.0));
      delta_x += 1;
  }
  for(int i = 0; i < numB; ++i) // STA in cell 1
  {
    positionAlloc->Add (Vector (d2 - delta_x, 0, 0.0));
      delta_x -= 1;
  }
  // positionAlloc->Add (Vector (0.0, 0.0, 0.0));   // eNB1/AP in cell 0
  // positionAlloc->Add (Vector (d2, d1, 0.0)); // AP in cell 1
  // positionAlloc->Add (Vector (0.0, d1, 0.0));  // UE1/STA in cell 0
  // positionAlloc->Add (Vector (d2, 0.0, 0.0));  // STA in cell 1
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allWirelessNodes);

  bool disableApps = false;
  Time durationTime = Seconds (duration);


  // REM settings tuned to get a nice figure for this specific scenario
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::OutputFile", StringValue ("laa-wifi-simple.rem"));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::XMin", DoubleValue (-50));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::XMax", DoubleValue (250));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::YMin", DoubleValue (-50));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::YMax", DoubleValue (250));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::XRes", UintegerValue (600));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::YRes", UintegerValue (600));
  Config::SetDefault ("ns3::RadioEnvironmentMapHelper::Z", DoubleValue (1.5));

  // we want deterministic behavior in this simple scenario, so we disable shadowing
  Config::SetDefault ("ns3::Ieee80211axIndoorPropagationLossModel::Sigma", DoubleValue (0));

  // Enable aggregation for AC_BE; see bug 2471 in tracker
  Config::SetDefault ("ns3::RegularWifiMac::BE_BlockAckThreshold", UintegerValue (2));

  // Specify some physical layer parameters that will be used below and
  // in the scenario helper.
  PhyParams phyParams;
  phyParams.m_bsTxGain = 5; // dB antenna gain
  phyParams.m_bsRxGain = 5; // dB antenna gain
  phyParams.m_bsTxPower = 18; // dBm
  phyParams.m_bsNoiseFigure = 5; // dB
  phyParams.m_ueTxGain = 0; // dB antenna gain
  phyParams.m_ueRxGain = 0; // dB antenna gain
  phyParams.m_ueTxPower = 18; // dBm
  phyParams.m_ueNoiseFigure = 9; // dB
  
  // calculate rx power corresponding to d2 for logging purposes
  // note: a separate propagation loss model instance is used, make
  // sure the settings are consistent with the ones used for the
  // simulation 
  const uint32_t earfcn = 255444;
  double dlFreq = LteSpectrumValueHelper::GetCarrierFrequency (earfcn);
  Ptr<PropagationLossModel> plm = CreateObject<Ieee80211axIndoorPropagationLossModel> ();
  plm->SetAttribute ("Frequency", DoubleValue (dlFreq));
  double txPowerFactors = phyParams.m_bsTxGain + phyParams.m_ueRxGain + 
                          phyParams.m_bsTxPower;
  double rxPowerDbmD1 = plm->CalcRxPower (txPowerFactors, 
                                          bsNodesA.Get (0)->GetObject<MobilityModel> (),
                                          ueNodesA.Get (0)->GetObject<MobilityModel> ());
  double rxPowerDbmD2 = plm->CalcRxPower (txPowerFactors, 
                                          bsNodesA.Get (0)->GetObject<MobilityModel> (),
                                          ueNodesB.Get (0)->GetObject<MobilityModel> ());


  std::ostringstream simulationParams;
  simulationParams << d1 << " " << d2 << " " 
                   << rxPowerDbmD1 << " "
                   << rxPowerDbmD2 << " "
                   << lteDutyCycle << " ";
  
  ConfigureAndRunScenario (cellConfigA, cellConfigB, bsNodesA, bsNodesB, ueNodesA, ueNodesB, phyParams, durationTime, transport, indoorLossModel, disableApps, lteDutyCycle, generateRem, outputDir + "/laa_wifi_simple_" + simTag, simulationParams.str ());
  
  return 0;
}
