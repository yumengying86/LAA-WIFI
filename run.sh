#! /bin/bash
# rm laa_wifi_simple_default_* Dl* Ul*
# --shutA=true --udpRate=30000000 --udpPacketSize=9000 --shutB=true   --lbtTxop=1

./waf --run "scratch/test --udpPacketSize=4500 --lbtTxop=3 --duration=5 --shutB=true --logPhyArrivals=true" 
mv laa_wifi_simple_default_phy_log lte${int}_.txt
rm laa_wifi_simple_default_* Dl* Ul*

./waf --run "scratch/test --udpPacketSize=4500 --lbtTxop=3 --duration=5 --shutA=true --logPhyArrivals=true" 
mv laa_wifi_simple_default_phy_log wifi${int}_.txt
rm laa_wifi_simple_default_* Dl* Ul*

int=1
while(( $int<=8 ))
do
    ./waf --run "scratch/test --udpPacketSize=4500 --lbtTxop=${int} --duration=5 --logPhyArrivals=true"  
    mv laa_wifi_simple_default_phy_log ltetxop${int}_.txt
    rm laa_wifi_simple_default_* Dl* Ul*
    let "int++"
done

int=1
while(( $int<=8 ))
do
    size=`expr $int \* 1500`
    ./waf --run "scratch/test --udpPacketSize=${size} --lbtTxop=3 --duration=5 --logPhyArrivals=true"  
    mv laa_wifi_simple_default_phy_log wifitxop${int}_.txt
    rm laa_wifi_simple_default_* Dl* Ul*
    let "int++"
done

# for i in 10000 15000 20000 250000
# do	
# 	echo $i
# 	sed -i "2429c double interval = static_cast<double> (packetSize * 8) / (bitRate*${i});"  ./src/laa-wifi-coexistence/helper/scenario-helper.cc
# 	./waf --run "test --logPhyArrivals=true"
# done	
