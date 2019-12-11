#! /bin/bash

# --shutA=true --udpRate=30000000 --udpPacketSize=9000 --shutB=true   --lbtTxop=1

# only lte
./waf --run "scratch/test --shutB=true --cellConfigA=Lte --logPhyArrivals=true" 
mv laa_wifi_simple_default_phy_log lte.txt

# only laa
./waf --run "scratch/test  --shutB=true --logPhyArrivals=true" 
mv laa_wifi_simple_default_phy_log laa.txt

# only wifi
./waf --run "scratch/test --shutA=true --logPhyArrivals=true" 
mv laa_wifi_simple_default_phy_log wifi.txt

int=1
while(( $int<=8 ))
do
    ./waf --run "scratch/test --lbtTxop=${int} --logPhyArrivals=true"  
    mv laa_wifi_simple_default_phy_log laatxop${int}.txt
    let "int++"
done

# int=1
# while(( $int<=8 ))
# do
#     size=`expr $int \* 1500`
#     ./waf --run "scratch/test --udpPacketSize=${size} --logPhyArrivals=true"  
#     mv laa_wifi_simple_default_phy_log wifitxop${int}.txt
#     rm laa_wifi_simple_default_* Dl* Ul*
#     let "int++"
# done

# for i in 10000 15000 20000 250000
# do	
# 	echo $i
# 	sed -i "2429c double interval = static_cast<double> (packetSize * 8) / (bitRate*${i});"  ./src/laa-wifi-coexistence/helper/scenario-helper.cc
# 	./waf --run "test --logPhyArrivals=true"
# done	
