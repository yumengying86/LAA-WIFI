#! /bin/bash
#rm laa_wifi_simple_default_phy_log laa_wifi_simple_default_operator* Dl* Ul*
#  --udpRate=30000000 --udpPacketSize=9000
# int=1
# while(( $int<=8 ))
# do
#     ./waf --run "laa-wifi-simple --d2=10  --lbtTxop=$int --logPhyArrivals=true" 
#     let "int++"
# done

for i in 250000 # 10000 15000 20000 250000
do	
	echo $i
	sed -i "2429c double interval = static_cast<double> (packetSize * 8) / (bitRate*${i});"  ./src/laa-wifi-coexistence/helper/scenario-helper.cc
	./waf --run "test --logPhyArrivals=true"
done	