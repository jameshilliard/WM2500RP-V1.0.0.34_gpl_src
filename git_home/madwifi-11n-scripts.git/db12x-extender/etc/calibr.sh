#!/bin/sh

#When the device /dev/caldata is not set up correctly, It will abord the calibration process,
#which require the device file is created.
FLAG=`hexdump -s 4096 -n 2  -e '2/2 "%02X, "' -e '"\n"' /dev/caldata |cut -d, -f 1`

#avoid FLAG be some wild character ,such as *
if [ ${#FLAG} -eq 1 ]; then
    FLAG="A"
fi

echo FLAG: ${FLAG}
if [ ! -z ${FLAG} ] && [ "$FLAG" != "A55A" ] && [ "$FLAG" != "5AA5" ];then
#use AND command list to execute all of the cmd after last cmd is executed successfully.
        ifconfig eth1 192.168.0.1 \
        && cd /tmp/ \
        && echo "Begin Transfer a.out from 192.168.0.100" \
        &&  tftp -gr mdk_client.out 192.168.0.100 \
        && echo "Begin Transfer athr.ko from 192.168.0.100" \
        && tftp -gr art.ko 192.168.0.100 \
        && echo "Insmod atheros wifi driver:athr.ko" \
        && insmod /tmp/art.ko \
        && echo "Begin Calibration" \
        && chmod +x /tmp/mdk_client.out \
        && /tmp/mdk_client.out  \
        && echo "It will reboot after 10 second" \
        && sleep 10  && reboot
else
    if [ -z ${FLAG} ];then
        echo There are some error in the environment, can not be calibrated 
    else
        echo "Already calibrated"
    fi
fi


