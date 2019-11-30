#! /bin/bash
# rm laa_wifi_simple_default_* Dl* Ul*
# --shutA=true --udpRate=30000000 --udpPacketSize=9000 --shutB=true   --lbtTxop=1

./waf --run "scratch/test --udpPacketSize=4500 --lbtTxop=1 --logPhyArrivals=true" 
# int=1
# while(( $int<=8 ))
# do
#     # size=`expr $int \* 1500`
#     ./waf --run "scratch/test --udpPacketSize=4500 --lbtTxop=${int} --logPhyArrivals=true"  
#     rm laa_wifi_simple_default_* Dl* Ul*
#     let "int++"
# done

# for i in 10000 15000 20000 250000
# do	
# 	echo $i
# 	sed -i "2429c double interval = static_cast<double> (packetSize * 8) / (bitRate*${i});"  ./src/laa-wifi-coexistence/helper/scenario-helper.cc
# 	./waf --run "test --logPhyArrivals=true"
# done	
