#! /bin/bash
#rm laa_wifi_simple_default_phy_log laa_wifi_simple_default_operator* Dl* Ul*
#  --udpRate=30000000 --udpPacketSize=9000
int=1
while(( $int<=8 ))
do
    ./waf --run "laa-wifi-simple --d2=10  --lbtTxop=$int --logPhyArrivals=true" 
    let "int++"
done