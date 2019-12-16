#! /bin/bash

# --shutA=true --udpRate=30000000 --udpPacketSize=9000 --cellConfigA=Lte --shutB=true   --lbtTxop=1

# test
# sed -i "446c       dcf->SetTxopLimit (MicroSeconds (0));"  src/wifi/model/wifi-mac.cc
# ./waf --run "scratch/test --logPhyArrivals=true"

# # only lte
# ./waf --run "scratch/test --shutB=true --cellConfigA=Lte --d2=500 --logPhyArrivals=true" 

# # only laa and wifi, no impact 8
# sed -i "446c       dcf->SetTxopLimit (MicroSeconds (0));"  src/wifi/model/wifi-mac.cc
# ./waf --run "scratch/test --d2=500 --logPhyArrivals=true" 

# # only laa and wifi, no impact 3
# sed -i "446c       dcf->SetTxopLimit (MicroSeconds (3008));"  src/wifi/model/wifi-mac.cc
# ./waf --run "scratch/test --d2=500 --lbtTxop=3 --logPhyArrivals=true" 

# int=1
# while(( $int<=8 ))
# do
#     ./waf --run "scratch/test --lbtTxop=${int} --logPhyArrivals=true"  
#     mv laa_wifi_simple_default_phy_log laatxop${int}.txt
#     let "int++"
# done

# for int in 992 2016 3008 4000 4992 6016 7008 8000
# do	
# 	sed -i "446c       dcf->SetTxopLimit (MicroSeconds (${int}));"  src/wifi/model/wifi-mac.cc
# 	./waf --run "scratch/test --lbtTxop=3 --logPhyArrivals=true"
#     mv laa_wifi_simple_default_phy_log wifitxop${int}.txt
# done	
