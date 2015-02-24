#!/bin/sh
#
#    WFA-start.sh : Start script for Wi-Fi Direct Testing.
#

#
#    Parameters
#
KVER=`uname -r | cut -f 1 -d '-'`
MODULE_PATH=/lib/modules/$KVER/offload
BUS=`cat ${MODULE_PATH}/hif_type`
BIN_PATH=/sbin
WFD_PATH=/usr/bin


#
#    Install Driver
#
echo "=============Install Driver..."
${BIN_PATH}/insmod $MODULE_PATH/adf.ko
${BIN_PATH}/insmod $MODULE_PATH/hif_${BUS}.ko
${BIN_PATH}/insmod $MODULE_PATH/fwd.ko
${BIN_PATH}/insmod $MODULE_PATH/atd.ko
sleep 2

i=0
WIFI_FOUND=0

while [ "$i" != "30" ]
do
    cat /proc/net/dev | grep wifi0 > /dev/null
    if [ "$?" = "0" ]; then
#        echo wifi0 detected...
        WIFI_FOUND=1
        break
    fi
    sleep 1
    i=$(($i+1))
done
if [ "$WIFI_FOUND" != "1" ]; then
    echo Fail to detect WLAN...
    exit 1
fi



#
#    Create VAPs
#
echo "=============Create VAPs..."
WLANDEV_STA=`${BIN_PATH}/wlanconfig wlan create wlandev wifi0 wlanmode sta`
WLANDEV_P2P=`${BIN_PATH}/wlanconfig wlan create wlandev wifi0 wlanmode p2pdev`

ifconfig ${WLANDEV_STA} up
ifconfig ${WLANDEV_P2P} up
sleep 1

#
#    Start wpa_supplicant
#
echo "=============Start wpa_supplicant..."
if [ ! -e /tmp/p2pdev.conf ]; then
        echo Using default p2pdev.conf
        cp /etc/ath/p2p/p2pdev.conf.def /tmp/p2pdev.conf
fi

if [ ! -e /tmp/wpa-sta.conf ]; then
        cp /etc/ath/p2p/wpa-sta.conf /tmp/wpa-sta.conf
fi
wpa_supplicant -i${WLANDEV_STA} -c /tmp/wpa-sta.conf -N -i${WLANDEV_P2P} -c /tmp/p2pdev.conf &
sleep 2

#
#    Other actions before testing
#
echo "=============Action before testing..."
echo 60 > /proc/sys/net/ipv4/neigh/default/base_reachable_time
echo 0 > /proc/sys/net/ipv4/neigh/default/ucast_solicit
iwpriv ${WLANDEV_P2P} shortgi 0

#
#    Start WFD automation & wpa_cli action control
#
P2P_ACT_FILE=/home/atheros/Atheros-P2P/scripts/p2p-action-pb44.sh
WLAN_ACT_FILE=/home/atheros/Atheros-P2P/scripts/wlan-action.sh
if [ -e ${WLAN_ACT_FILE} ] && [ -e ${P2P_ACT_FILE} ]; then
	echo "=============Start wpa_cli action control..."
	${BIN_PATH}/wpa_cli -i ${WLANDEV_STA} -a ${WLAN_ACT_FILE} &
	sleep 1
	${BIN_PATH}/wpa_cli -i ${WLANDEV_P2P} -a ${P2P_ACT_FILE} &
	sleep 1
fi
if [ -e ${WFD_PATH}/wfa_dut ] && [ -e ${WFD_PATH}/wfa_ca ]; then
	echo "=============Start WFD automation..."
	${WFD_PATH}/wfa_dut lo 8000 &
	sleep 1
	export WFA_ENV_AGENT_IPADDR=127.0.0.1
	export WFA_ENV_AGENT_PORT=8000
	${WFD_PATH}/wfa_ca lo 9000 &
	sleep 1
fi

echo "=============Done!"



