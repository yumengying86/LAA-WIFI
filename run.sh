#! /bin/bash
# rm laa_wifi_simple_default_phy_log laa_wifi_simple_default_* Dl* Ul*
./waf --run "scratch/test --lbtTxop=1 --logPhyArrivals=true"


# --shutA=true --udpRate=30000000 --udpPacketSize=9000 --shutB=true --lbtTxop=3

# int=1
# while(( $int<=8 ))
# do
#     ./waf --run "scratch/test --lbtTxop=$int --logPhyArrivals=true --logTxops=true"  
#     mv laa_wifi_simple_default_txop_log txop${int}
#     let "int++"
# done

# for i in 10000 15000 20000 250000
# do	
# 	echo $i
# 	sed -i "2429c double interval = static_cast<double> (packetSize * 8) / (bitRate*${i});"  ./src/laa-wifi-coexistence/helper/scenario-helper.cc
# 	./waf --run "test --logPhyArrivals=true"
# done	
