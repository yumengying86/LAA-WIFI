/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Benjamin Cizdziel <ben.cizdziel@gmail.com>
 */

#include <iostream>
#include <ns3/wifi-module.h>

using namespace ns3;

int main (int argc, char *argv[])
{
  Ptr<FreqSelectiveErrorRateModel> errorModel = CreateObject<FreqSelectiveErrorRateModel> (); // initializes to 1 RX antenna
  // parameters are WifiMode, WifiTxVector, SNR, and number of bits
  WifiTxVector txVector; 
  txVector.SetChannelWidth (20);
  txVector.SetNss (1);
  errorModel->SetNumRxAntennas (2); 
  WifiMode mode = WifiPhy::GetHtMcs1 (); //mcs 1
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 10, 1) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 4.25, 457) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 1, 1) << std::endl;
  errorModel->SetNumRxAntennas (2); //change to 1x2 error rates
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 10, 1) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 4.25, 457) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 1, 1) << std::endl;
  mode = WifiPhy::GetHtMcs3 (); //mcs 3
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 10, 1) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 4.25, 457) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 1, 1) << std::endl;
  mode = WifiPhy::GetHtMcs9 (); //mcs 9
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 10, 1) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 4.25, 457) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 10, 1) << std::endl;
  mode = WifiPhy::GetHtMcs15 (); //mcs 15
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 3981.071706, 1) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 100000, 1) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 11220.18454, 78) << std::endl;
  errorModel->SetNumRxAntennas (1); //go back to 1x1 error rates
  mode = WifiPhy::GetHtMcs1 (); //mcs 1
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 10, 1) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 4.25, 457) << std::endl;
  std::cout << errorModel->GetChunkSuccessRate (mode, txVector, 1, 1) << std::endl;
}
