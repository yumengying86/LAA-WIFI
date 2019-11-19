/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 MIRKO BANCHI
 * Copyright (c) 2015 University of Washington
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Mirko Banchi <mk.banchi@gmail.com>
 *          Sebastien Deronne <sebastien.deronne@gmail.com>
 *
 * Modified from ht-wifi-network.cc
 */
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/spectrum-module.h"

// This is a simple example of an IEEE 802.11n Wi-Fi network.
//
// The main use case is to enable and test SpectrumWifiPhy vs YansWifiPhy 
// under saturation conditions (for max throughput)
//
// Network topology:
//
//  Wifi 192.168.1.0
//
//         AP
//    *    *
//    |    |
//    n1   n2
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiSpectrumSaturationExample");

int main (int argc, char *argv[])
{
  // Enable aggregation for AC_BE; see bug 2471 in tracker
  Config::SetDefault ("ns3::RegularWifiMac::BE_BlockAckThreshold", UintegerValue (2));

  bool udp = true;
  double simulationTime = 10; //seconds
  std::string wifiType = "ns3::SpectrumWifiPhy";
  CommandLine cmd;
  cmd.AddValue ("simulationTime", "Simulation time in seconds", simulationTime);
  cmd.AddValue ("udp", "UDP if set to 1, TCP otherwise", udp);
  cmd.AddValue ("wifiType", "select Spectrum or Yans WifiPhy", wifiType);
  cmd.Parse (argc,argv);

  std::cout << "WifiType=" << wifiType << '\n';
  std::cout << "DataRate" << "\t" << "RX/TX" << "\t" << "Throughput" << '\n';

  std::string mimo;
  for (int i = 0; i <= 63; i++)
    {
      uint32_t payloadSize;  // target of 1500 byte IP packets
      if (udp)
        {
          payloadSize = 1472; 
        }
      else
        {
          payloadSize = 1448; 
          Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (payloadSize));
        }

      NodeContainer wifiStaNode;
      wifiStaNode.Create (1);
      NodeContainer wifiApNode;
      wifiApNode.Create (1);

      YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
      SpectrumWifiPhyHelper spectrumPhy = SpectrumWifiPhyHelper::Default ();

      if (wifiType == "ns3::YansWifiPhy")
        {
          YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
          phy.SetChannel (channel.Create ());
    
          if (i <= 7)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (false));
              phy.Set ("ChannelWidth", UintegerValue (20));
              mimo = "1x1";
            }
          else if (i > 7 && i <= 15)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (true));
              phy.Set ("ChannelWidth", UintegerValue (20));
              mimo = "1x1";
            }
          else if (i > 15 && i <= 23)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (false));
              phy.Set ("ChannelWidth", UintegerValue (40));
              mimo = "1x1";
            }
          else if (i > 23 && i <= 31)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (true));
              phy.Set ("ChannelWidth", UintegerValue (40));
              mimo = "1x1";
            }
          else if (i > 31 && i <= 39)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (false));
              phy.Set ("ChannelWidth", UintegerValue (20));
              phy.Set ("Receivers", UintegerValue (2));
              phy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
          else if (i > 39 && i <= 47)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (true));
              phy.Set ("ChannelWidth", UintegerValue (20));
              phy.Set ("Receivers", UintegerValue (2));
              phy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
          else if (i > 47 && i <= 55)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (false));
              phy.Set ("ChannelWidth", UintegerValue (40));
              phy.Set ("Receivers", UintegerValue (2));
              phy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
          else //if (i > 55 && i <= 63)
            {
              phy.Set ("ShortGuardEnabled", BooleanValue (true));
              phy.Set ("ChannelWidth", UintegerValue (40));
              phy.Set ("Receivers", UintegerValue (2));
              phy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
        }
      else if (wifiType == "ns3::SpectrumWifiPhy")
        {
          Ptr<SingleModelSpectrumChannel> spectrumChannel
            = CreateObject<SingleModelSpectrumChannel> ();
          Ptr<LogDistancePropagationLossModel> lossModel
            = CreateObject<LogDistancePropagationLossModel> ();
          spectrumChannel->AddPropagationLossModel (lossModel);

          Ptr<ConstantSpeedPropagationDelayModel> delayModel
            = CreateObject<ConstantSpeedPropagationDelayModel> ();
          spectrumChannel->SetPropagationDelayModel (delayModel);

          spectrumPhy.SetChannel (spectrumChannel);
          spectrumPhy.SetChannelNumber (36); 
          spectrumPhy.SetErrorRateModel ("ns3::FreqSelectiveErrorRateModel");

          if (i <= 7)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (false));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (20));
              mimo = "1x1";
            }
          else if (i > 7 && i <= 15)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (true));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (20));
              mimo = "1x1";
            }
          else if (i > 15 && i <= 23)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (false));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (40));
              mimo = "1x1";
            }
          else if (i > 23 && i <= 31)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (true));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (40));
              mimo = "1x1";
            }
          else if (i > 31 && i <= 39)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (false));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (20));
              spectrumPhy.Set ("Receivers", UintegerValue (2));
              spectrumPhy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
          else if (i > 39 && i <= 47)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (true));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (20));
              spectrumPhy.Set ("Receivers", UintegerValue (2));
              spectrumPhy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
          else if (i > 47 && i <= 55)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (false));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (40));
              spectrumPhy.Set ("Receivers", UintegerValue (2));
              spectrumPhy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
          else //if (i > 55 && i <= 63)
            {
              spectrumPhy.Set ("ShortGuardEnabled", BooleanValue (true));
              spectrumPhy.Set ("ChannelWidth", UintegerValue (40));
              spectrumPhy.Set ("Receivers", UintegerValue (2));
              spectrumPhy.Set ("Transmitters", UintegerValue (2));
              mimo = "2x2";
            }
        }
      else
        {
          NS_FATAL_ERROR ("Unsupported WiFi type " << wifiType);
        }

      WifiHelper wifi;
      wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
      WifiMacHelper mac;

      Ssid ssid = Ssid ("ns380211n");

      double datarate = 0;
      StringValue DataRate;
      if (i == 0)
        {
          DataRate = StringValue ("HtMcs0");
          datarate = 6.5;
        }
      else if (i == 1)
        {
          DataRate = StringValue ("HtMcs1");
          datarate = 13;
        }
      else if (i == 2)
        {
          DataRate = StringValue ("HtMcs2");
          datarate = 19.5;
        }
      else if (i == 3)
        {
          DataRate = StringValue ("HtMcs3");
          datarate = 26;
        }
      else if (i == 4)
        {
          DataRate = StringValue ("HtMcs4");
          datarate = 39;
        }
      else if (i == 5)
        {
          DataRate = StringValue ("HtMcs5");
          datarate = 52;
        }
      else if (i == 6)
        {
          DataRate = StringValue ("HtMcs6");
          datarate = 58.5;
        }
      else if (i == 7)
        {
          DataRate = StringValue ("HtMcs7");
          datarate = 65;
        }
      else if (i == 8)
        {
          DataRate = StringValue ("HtMcs0");
          datarate = 7.2;
        }
      else if (i == 9)
        {
          DataRate = StringValue ("HtMcs1");
          datarate = 14.4;
        }
      else if (i == 10)
        {
          DataRate = StringValue ("HtMcs2");
          datarate = 21.7;
        }
      else if (i == 11)
        {
          DataRate = StringValue ("HtMcs3");
          datarate = 28.9;
        }
      else if (i == 12)
        {
          DataRate = StringValue ("HtMcs4");
          datarate = 43.3;
        }
      else if (i == 13)
        {
          DataRate = StringValue ("HtMcs5");
          datarate = 57.8;
        }
      else if (i == 14)
        {
          DataRate = StringValue ("HtMcs6");
          datarate = 65;
        }
      else if (i == 15)
        {
          DataRate = StringValue ("HtMcs7");
          datarate = 72.2;
        }
      else if (i == 16)
        {
          DataRate = StringValue ("HtMcs0");
          datarate = 13.5;
        }
      else if (i == 17)
        {
          DataRate = StringValue ("HtMcs1");
          datarate = 27;
        }
      else if (i == 18)
        {
          DataRate = StringValue ("HtMcs2");
          datarate = 40.5;
        }
      else if (i == 19)
        {
          DataRate = StringValue ("HtMcs3");
          datarate = 54;
        }
      else if (i == 20)
        {
          DataRate = StringValue ("HtMcs4");
          datarate = 81;
        }
      else if (i == 21)
        {
          DataRate = StringValue ("HtMcs5");
          datarate = 108;
        }
      else if (i == 22)
        {
          DataRate = StringValue ("HtMcs6");
          datarate = 121.5;
        }
      else if (i == 23)
        {
          DataRate = StringValue ("HtMcs7");
          datarate = 135;
        }
      else if (i == 24)
        {
          DataRate = StringValue ("HtMcs0");
          datarate = 15;
        }
      else if (i == 25)
        {
          DataRate = StringValue ("HtMcs1");
          datarate = 30;
        }
      else if (i == 26)
        {
          DataRate = StringValue ("HtMcs2");
          datarate = 45;
        }
      else if (i == 27)
        {
          DataRate = StringValue ("HtMcs3");
          datarate = 60;
        }
      else if (i == 28)
        {
          DataRate = StringValue ("HtMcs4");
          datarate = 90;
        }
      else if (i == 29)
        {
          DataRate = StringValue ("HtMcs5");
          datarate = 120;
        }
      else if (i == 30)
        {
          DataRate = StringValue ("HtMcs6");
          datarate = 135;
        }
      else if (i == 31)
        {
          DataRate = StringValue ("HtMcs7");
          datarate = 150;
        }
      else if (i == 32)
        {
          DataRate = StringValue ("HtMcs8");
          datarate = 13;
        }
      else if (i == 33)
        {
          DataRate = StringValue ("HtMcs9");
          datarate = 26;
        }
      else if (i == 34)
        {
          DataRate = StringValue ("HtMcs10");
          datarate = 39;
        }
      else if (i == 35)
        {
          DataRate = StringValue ("HtMcs11");
          datarate = 52;
        }
      else if (i == 36)
        {
          DataRate = StringValue ("HtMcs12");
          datarate = 78;
        }
      else if (i == 37)
        {
          DataRate = StringValue ("HtMcs13");
          datarate = 104;
        }
      else if (i == 38)
        {
          DataRate = StringValue ("HtMcs14");
          datarate = 117;
        }
      else if (i == 39)
        {
          DataRate = StringValue ("HtMcs15");
          datarate = 130;
        }
      else if (i == 40)
        {
          DataRate = StringValue ("HtMcs8");
          datarate = 14.4;
        }
      else if (i == 41)
        {
          DataRate = StringValue ("HtMcs9");
          datarate = 28.9;
        }
      else if (i == 42)
        {
          DataRate = StringValue ("HtMcs10");
          datarate = 43.3;
        }
      else if (i == 43)
        {
          DataRate = StringValue ("HtMcs11");
          datarate = 57.8;
        }
      else if (i == 44)
        {
          DataRate = StringValue ("HtMcs12");
          datarate = 86.7;
        }
      else if (i == 45)
        {
          DataRate = StringValue ("HtMcs13");
          datarate = 115.6;
        }
      else if (i == 46)
        {
          DataRate = StringValue ("HtMcs14");
          datarate = 130.3;
        }
      else if (i == 47)
        {
          DataRate = StringValue ("HtMcs15");
          datarate = 144.4;
        }
      else if (i == 48)
        {
          DataRate = StringValue ("HtMcs8");
          datarate = 27;
        }
      else if (i == 49)
        {
          DataRate = StringValue ("HtMcs9");
          datarate = 54;
        }
      else if (i == 50)
        {
          DataRate = StringValue ("HtMcs10");
          datarate = 81;
        }
      else if (i == 51)
        {
          DataRate = StringValue ("HtMcs11");
          datarate = 108;
        }
      else if (i == 52)
        {
          DataRate = StringValue ("HtMcs12");
          datarate = 162;
        }
      else if (i == 53)
        {
          DataRate = StringValue ("HtMcs13");
          datarate = 216;
        }
      else if (i == 54)
        {
          DataRate = StringValue ("HtMcs14");
          datarate = 243;
        }
      else if (i == 55)
        {
          DataRate = StringValue ("HtMcs15");
          datarate = 270;
        }
      else if (i == 56)
        {
          DataRate = StringValue ("HtMcs8");
          datarate = 30;
        }
      else if (i == 57)
        {
          DataRate = StringValue ("HtMcs9");
          datarate = 60;
        }
      else if (i == 58)
        {
          DataRate = StringValue ("HtMcs10");
          datarate = 90;
        }
      else if (i == 59)
        {
          DataRate = StringValue ("HtMcs11");
          datarate = 120;
        }
      else if (i == 60)
        {
          DataRate = StringValue ("HtMcs12");
          datarate = 180;
        }
      else if (i == 61)
        {
          DataRate = StringValue ("HtMcs13");
          datarate = 240;
        }
      else if (i == 62)
        {
          DataRate = StringValue ("HtMcs14");
          datarate = 270;
        }
      else //if (i == 63)
        {
          DataRate = StringValue ("HtMcs15");
          datarate = 300;
        }

      wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", DataRate,
                                    "ControlMode", DataRate);

      NetDeviceContainer staDevice;
      NetDeviceContainer apDevice;
      if (wifiType == "ns3::YansWifiPhy")
        {
          mac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid),
                       "ActiveProbing", BooleanValue (false));

          staDevice = wifi.Install (phy, mac, wifiStaNode);
          mac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid));
          apDevice = wifi.Install (phy, mac, wifiApNode);
        }
      else if (wifiType == "ns3::SpectrumWifiPhy")
        {
          mac.SetType ("ns3::StaWifiMac",
                       "Ssid", SsidValue (ssid),
                       "ActiveProbing", BooleanValue (false));
          staDevice = wifi.Install (spectrumPhy, mac, wifiStaNode);
          mac.SetType ("ns3::ApWifiMac",
                       "Ssid", SsidValue (ssid));
          apDevice = wifi.Install (spectrumPhy, mac, wifiApNode);
        }

      // mobility.
      MobilityHelper mobility;
      Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

      positionAlloc->Add (Vector (0.0, 0.0, 0.0));
      positionAlloc->Add (Vector (1.0, 0.0, 0.0));
      mobility.SetPositionAllocator (positionAlloc);

      mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

      mobility.Install (wifiApNode);
      mobility.Install (wifiStaNode);

      /* Internet stack*/
      InternetStackHelper stack;
      stack.Install (wifiApNode);
      stack.Install (wifiStaNode);

      Ipv4AddressHelper address;

      address.SetBase ("192.168.1.0", "255.255.255.0");
      Ipv4InterfaceContainer staNodeInterface;
      Ipv4InterfaceContainer apNodeInterface;

      staNodeInterface = address.Assign (staDevice);
      apNodeInterface = address.Assign (apDevice);

      /* Setting applications */
      ApplicationContainer serverApp, sinkApp;
      if (udp)
        {
          //UDP flow
          UdpServerHelper myServer (9);
          serverApp = myServer.Install (wifiStaNode.Get (0));
          serverApp.Start (Seconds (0.0));
          serverApp.Stop (Seconds (simulationTime+1));

          UdpClientHelper myClient (staNodeInterface.GetAddress (0), 9);
          myClient.SetAttribute ("MaxPackets", UintegerValue (4294967295u));
          myClient.SetAttribute ("Interval", TimeValue (Time ("0.00002"))); //packets/s
          myClient.SetAttribute ("PacketSize", UintegerValue (payloadSize));

          ApplicationContainer clientApp = myClient.Install (wifiApNode.Get (0));
          clientApp.Start (Seconds (1.0));
          clientApp.Stop (Seconds (simulationTime+1));
        }
      else
        {
          //TCP flow
          uint16_t port = 50000;
          Address apLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
          PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", apLocalAddress);
          sinkApp = packetSinkHelper.Install (wifiStaNode.Get (0));

          sinkApp.Start (Seconds (0.0));
          sinkApp.Stop (Seconds (simulationTime+1));

          OnOffHelper onoff ("ns3::TcpSocketFactory",Ipv4Address::GetAny ());
          onoff.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
          onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
          onoff.SetAttribute ("PacketSize", UintegerValue (payloadSize));
          onoff.SetAttribute ("DataRate", DataRateValue (1000000000)); //bit/s
          ApplicationContainer apps;

          AddressValue remoteAddress (InetSocketAddress (staNodeInterface.GetAddress (0), port));
          onoff.SetAttribute ("Remote", remoteAddress);
          apps.Add (onoff.Install (wifiApNode.Get (0)));
          apps.Start (Seconds (1.0));
          apps.Stop (Seconds (simulationTime+1));
        }

      Simulator::Stop (Seconds (simulationTime+1));
      Simulator::Run ();
      Simulator::Destroy ();

      double throughput = 0;
      if (udp)
        {
          //UDP
          uint32_t totalPacketsThrough = DynamicCast<UdpServer> (serverApp.Get (0))->GetReceived ();
          throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0); //Mbit/s
        }
      else
        {
          //TCP
          uint32_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApp.Get (0))->GetTotalRx ();
          throughput = totalPacketsThrough * 8 / (simulationTime * 1000000.0); //Mbit/s
        }
      std::cout << datarate << "\t\t" << mimo << "\t" << throughput << " Mbit/s" << std::endl;
    }
  return 0;
}
