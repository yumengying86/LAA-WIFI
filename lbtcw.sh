#! /bin/bash

echo "" > data7.txt
value=7
echo -e "\nCwmin of laa : ${value}" >> data7.txt
taskset -c 22 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data7.txt &

echo "" > data15.txt
value=15
echo -e "\nCwmin of laa : ${value}" >> data15.txt
taskset -c 15 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data15.txt &

echo "" > data31.txt
value=31
echo -e "\nCwmin of laa : ${value}" >> data31.txt
taskset -c 16 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data31.txt &

echo "" > data63.txt
value=63
echo -e "\nCwmin of laa : ${value}" >> data63.txt
taskset -c 17 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data63.txt &

echo "" > data127.txt
value=127
echo -e "\nCwmin of laa : ${value}" >> data127.txt
taskset -c 18 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data127.txt &

echo "" > data255.txt
value=255
echo -e "\nCwmin of laa : ${value}" >> data255.txt
taskset -c 19 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data255.txt &

echo "" > data511.txt
value=511
echo -e "\nCwmin of laa : ${value}" >> data511.txt
taskset -c 20 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data511.txt &

echo "" > data1023.txt
value=1023
echo -e "\nCwmin of laa : ${value}" >> data1023.txt
taskset -c 21 nohup ./waf --run "scratch/test --laaMinCw=${value} --d1=10 --d2=10 --duration=15 --numA=1 --numB=1  --lbtTxop=3 --udpPacketSize=1000  --laaMaxM=6"  >> data1023.txt &







