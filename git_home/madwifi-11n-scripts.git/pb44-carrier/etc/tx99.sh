#!/bin/sh
#set -x

# how to use
# Setp 1: load the modules,            >> tx99.sh mod
# Step 2: 
#       - change the max power.        >> tx99.sh pow
#       - Turn on the receive mode.    >> tx99.sh recv
# Step 3: adjust tx99 params and max power by edit this script.
# Step 4: unload the modules.          >> tx99.sh rmmod
#
# These parameters could enlarge the maxpower and send pkts successfully.
#   For 11n device, freq=5180, rate=13000bps, HT20
#                   max power: power_mode=rateHt20_1, max_power=36
#
# This configuration could continusous TX on pb42 + Nala.
#   For MB55(Nala), freq=2437, txmode=3, rate=54000, htmode=0, htext=0
#

freq=5180     # Channel/Frequency
txmode=0      # wireless mode, 11NG(8), auto-select(0)
power=20      # dBm, drv-power=${power} * 2
rate=13000    # data rate
htmode=0      # HT40(2), HT20(0)
htext=0       # HT extension channel, 0(none), 1(plus), 2(minus)
type=0        # 0:frame data, 1:single carrier
bus=pci       # options: ahb, pci

# adjust the max power level for the specific mode/channel
# consult madwifi/hal/main/ar5416/ar5416.h
# Availabe options:
#   rateHt20_0, rateHt20_1, rateHt20_2, rateHt20_3, rateHt20_4, rateHt20_5, rateHt20_6, rateHt20_7
#   rateHt40_0, rateHt40_1, rateHt40_2, rateHt40_3, rateHt40_4, rateHt40_5, rateHt40_6, rateHt40_7
#   rate6mb, rate9mb, rate12mb, rate18mb, rate24mb, rate36mb, rate48mb, rate54mb
#   rate11l, rate11s, rate1l, rate2l, rate2s, rate5_5l, rate5_5s
#   rateDupCck, rateDupOfdm, rateExtCck, rateExtOfdm, rateXr
power_mode=rateHt20_1
max_power=36 # power per rate, in 0.5 dBm unit

# options, 
#  mod:load module
#  rmmod:unload module
#  pow:change max power
#  recv: Turn on receive mode
arg=${1} 

iface=wifi0
mod_path=/lib/modules/2.6.15/net/11n

load_modules()
{
	insmod ${mod_path}/wlan.ko
	insmod ${mod_path}/ath_hal.ko
	insmod ${mod_path}/ath_rate_atheros.ko
	insmod ${mod_path}/ath_dfs.ko

	if [ ${bus} = "pci" ]; then
		insmod ${mod_path}/ath_pci.ko use_eeprom=1
	fi
	if [ ${bus} = "ahb" ]; then
		insmod ${mod_path}/ath_ahb.ko
	fi

	insmod ${mod_path}/ath_tx99.ko device=${iface}
}

unload_modules()
{
	rmmod athconf
	rmmod ath_tx99
	rmmod ath_${bus}
	rmmod ath_dfs
	rmmod ath_rate_atheros
	rmmod ath_hal
	rmmod wlan
}

change_tx99_params()
{
	echo ${freq}    > /proc/sys/dev/wifi0/tx99freq
	echo ${txmode}  > /proc/sys/dev/wifi0/tx99txmode
	echo ${power}   > /proc/sys/dev/wifi0/tx99power
	echo ${rate}    > /proc/sys/dev/wifi0/tx99rate
	echo ${htmode}  > /proc/sys/dev/wifi0/tx99htmode
	echo ${htext}   > /proc/sys/dev/wifi0/tx99htext
	echo ${type}    > /proc/sys/dev/wifi0/tx99type
}

dump_tx99_params()
{
	echo ""
	echo " frequency=${freq}"
	echo " tx-mode=${txmode}"
	echo " power(dBm)=${power}"
	echo " rate(bps)=${rate}"
	echo " HT-mode=${htmode}"
	echo " HT-ext=${htext}"
	echo " tx99type=${type}"
}

adjust_max_power()
{
	mod_conf=`lsmod |grep athconf`
	if [ "${mod_conf}" = "" ]; then
		insmod ${mod_path}/athconf.ko
	fi

	echo ${max_power} > /proc/sys/athconf/${iface}/powerperrate/${power_mode}
	echo 1 > /proc/sys/athconf/${iface}/powerperrate/write
	echo 1 > /proc/sys/athconf/${iface}/powerperrate/print
}

if [ "${arg}Z" = "modZ" ]; then
	load_modules
	echo ""
	echo "${0}: load module completely. exit!"
	echo ""
fi 

if [ "${arg}Z" = "rmmodZ" ]; then
	unload_modules
	echo ""
	echo "${0}: Unload module completely. exit!"
	return
fi 

if [ "${arg}Z" = "powZ" ]; then
	adjust_max_power
	echo ""
	echo "${0}: adjust max power completely. exit!"
	echo ""
fi 

if [ "${arg}Z" = "recvZ" ]; then
	echo 1 > /proc/sys/dev/wifi0/tx99recv
fi 

ifconfig ${iface} down
change_tx99_params
ifconfig ${iface} up
dump_tx99_params

