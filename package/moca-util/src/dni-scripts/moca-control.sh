#!/bin/sh

. $IPKG_INSTROOT/etc/moca.conf

CONFIG="/bin/config"
IFCONFIG="/sbin/ifconfig"
vlan_tag_mark=$($CONFIG get vlan_tag_enable)
vlan_tag_id_1=$($CONFIG get vlan_tag_id_1)
vlan_tag_id_2=$($CONFIG get vlan_tag_id_2)

stop()
{
	killall clinkd
	killall kclinkd
	sleep 1
	killall -9 clinkd
	killall -9 kclinkd
	rmmod CandDdvr
}

start()
{
	#load Driver
	insmod /etc/moca/CandDdvr.ko
	mknod /dev/jaws31 c 254 31
	#Down/Up Interface to force driver to re-init ethernet MAC
	ifconfig eth0 down
	ifconfig eth0 up
	#mm 0xb8050028 0xae000000
	#start deamon service
	eth0_mac=$(ifconfig eth0 | grep eth0 | sed 's/.*HWaddr//')
	/etc/moca/clinkd -Dvtf/tmp/moca/examples --mac-addr $eth0_mac -i /dev/jaws31 &
	#/etc/moca/clinkd -Dvtf/etc/moca/examples --mac-addr 00:03:7F:11:24:D6 -i /dev/jaws31 &
}

restart()
{
	stop
	start
}
show_netinfo()
{
	echo "MoCA monitoring the status of the connected MoCA modes:"
	/etc/moca/clnkstat -n
}
show_conf()
{
	echo "MoCA SoC Current Configuration/Info:"
	#read configurations
	/etc/moca/clnkcfg --lof
	/etc/moca/clnkcfg --networksearch
	/etc/moca/clnkcfg --channelmask
	/etc/moca/clnkcfg --securitymode
	/etc/moca/clnkcfg --mocapassword
	/etc/moca/clnkstat -d
}

change_password()
{
	/etc/moca/clnkcfg -s --securitymode=$($CONFIG get coax_encry_abled)
	/etc/moca/clnkcfg -s --mocapassword=$($CONFIG get coax_encry_key)
}

change_channel_by_id()
{
	moca_chan=$($CONFIG get coax_channel)
	case $moca_chan in
		15)
			moca_chan_freq=1150
			moca_chan_mask=0x80000000
		;;
		17)
			moca_chan_freq=1200
			moca_chan_mask=0x40000000
		;;
		19)
			moca_chan_freq=1250
			moca_chan_mask=0x20000000
		;;
		21)
			moca_chan_freq=1300
			moca_chan_mask=0x10000000
		;;
		23)
			moca_chan_freq=1350
			moca_chan_mask=0x8000000
		;;
		25)
			moca_chan_freq=1400
			moca_chan_mask=0x4000000
		;;
		27)
			moca_chan_freq=1450
			moca_chan_mask=0x2000000
		;;
		29)
			moca_chan_freq=1500
			moca_chan_mask=0x1000000
		;;
		31)
			moca_chan_freq=1550
			moca_chan_mask=0x800000
		;;
		33)
			moca_chan_freq=1600
			moca_chan_mask=0x400000
		;;
		0|*)	#SCAN
			moca_chan_freq=0
			moca_chan_mask=0xffc00000
		;;
	esac
	$CONFIG set coax_channel_mask=$moca_chan_mask
	$CONFIG commit

	if [ "$moca_chan_mask" = "0xffc00000" ]; then
		/etc/moca/clnkcfg -s --networksearch=1 --channelmask=$moca_chan_mask
	else
		/etc/moca/clnkcfg -s --lof=$moca_chan_freq --networksearch=0 --channelmask=$moca_chan_mask
	fi
}

change_channel_by_mask()
{
	moca_chanmask=$($CONFIG get coax_channel_mask)
	case $moca_chanmask in
		0x80000000)	#1150
			$CONFIG set coax_channel=15
		;;
		0x40000000)	#1200
			$CONFIG set coax_channel=17
		;;
		0x20000000)	#1250
			$CONFIG set coax_channel=19
		;;
		0x10000000)	#1300
			$CONFIG set coax_channel=21
		;;
		0x8000000)	#1350
			$CONFIG set coax_channel=23
		;;
		0x4000000)	#1400
			$CONFIG set coax_channel=25
		;;
		0x2000000)	#1450
			$CONFIG set coax_channel=27
		;;
		0x1000000)	#1500
			$CONFIG set coax_channel=29
		;;
		0x800000)	#1550
			$CONFIG set coax_channel=31
		;;
		0x400000)	#1600
			$CONFIG set coax_channel=33
		;;
		0xffc00000|*)	#SCAN
			$CONFIG set coax_channel=0
		;;
	esac

	#set frequency and commit new config value
	change_channel_by_id
	#reset SoC
	/etc/moca/clnkrst -i jaws31 &
}

#coax_encry_abled - 0:Enable, 1:Disable
#coax_encry_key - default: 99999999988888888
apply_change()
{
	#set frequency
	change_channel_by_id

	#set security
	change_password

	#save configuration files after all change is applied.
	#This function is now called by MoCA LSDK
	#save_config_file

	#reset SoC
	/etc/moca/clnkrst -i jaws31 &
}

#/tmp/moca/examples/clink.conf
update_conf_file()
{
	#Disable this function since it cannot work well for new SDK V1.01.
	#This function can work for old SDK V1.00.
	return

	echo -n > /tmp/moca/examples/clink.conf
	while read LINE
	do
		find_ret1=`echo $LINE | grep -c "mocapassword"`
		find_ret2=`echo $LINE | grep -c "securitymode"`
		find_ret3=`echo $LINE | grep -c "channelmask"`
		if [ "x$find_ret1" == "x1" ]; then
			pwd_len=$(/bin/echo $($CONFIG get coax_encry_key) | wc -L)
			echo "mocapassword $pwd_len $($CONFIG get coax_encry_key)" >> /tmp/moca/examples/clink.conf
		elif [ "x$find_ret2" == "x1" ]; then
			echo "securitymode 1 $($CONFIG get coax_encry_abled)" >> /tmp/moca/examples/clink.conf
		elif [ "x$find_ret3" == "x1" ]; then
			echo "channelmask 10 $($CONFIG get coax_channel_mask)" >> /tmp/moca/examples/clink.conf
		else
			echo $LINE >> /tmp/moca/examples/clink.conf
		fi
	done	< /etc/moca/examples/clink.conf
	#below lines will pend "^@" at end of line
	#sed -i "/mocapassword/c mocapassword $pwd_len $($CONFIG get coax_encry_key)" /tmp/moca/examples/clink.conf
	#sed -i "/securitymode/c securitymode 1 $($CONFIG get coax_encry_abled)" /tmp/moca/examples/clink.conf
	#sed -i "/channelmask/c channelmask 10 $($CONFIG get coax_channel_mask)" /tmp/moca/examples/clink.conf
}

lan_restart() {
	ifconfig $LAN_IF down
	echo -n 5 > /proc/switch_phy
	sleep 8
	ifconfig $LAN_IF up
}

#Prepare MoCA VLAN interface
moca_vlan_if_up() {
	#Prepare for WAN_IF: eth0
	#remove eth0 from br0.
	brctl delif $BR_IF $WAN_IF
	$IFCONFIG $WAN_IF up

	#Attach eth0.wireless (vlan_tag_id_1) to BR_IF: "br0"
	WAN_VLAN1_EXIST=`$IFCONFIG $WAN_IF.$vlan_tag_id_1 2>/dev/zero | grep -c "eth"`
	if [ "x$WAN_VLAN1_EXIST" == "x0" ]; then
		vconfig add $WAN_IF $vlan_tag_id_1
	fi
	brctl addif $BR_IF $WAN_IF.$vlan_tag_id_1
	$IFCONFIG $WAN_IF.$vlan_tag_id_1 up

	#Attach eth0.ethernet (vlan_tag_id_2) to BR_VLAN: br2
	if [ "x$vlan_tag_id_1" != "x$vlan_tag_id_2" ]; then
		WAN_VLAN2_EXIST=`$IFCONFIG $WAN_IF.$vlan_tag_id_2 2>/dev/zero | grep -c "eth"`
		if [ "x$WAN_VLAN2_EXIST" == "x0" ]; then
			vconfig add $WAN_IF $vlan_tag_id_2
		fi
		brctl addif $BR_VLAN $WAN_IF.$vlan_tag_id_2
		$IFCONFIG $WAN_IF.$vlan_tag_id_2 up
	fi
}

moca_vlan_if_down() {
	$IFCONFIG $WAN_IF down
	#Restore BR_IF: br0
	$IFCONFIG $WAN_IF.$vlan_tag_id_1 down
	brctl delif $BR_IF $WAN_IF.$vlan_tag_id_1
	#Restore BR_VLAN: br2
	if [ "x$vlan_tag_id_1" != "x$vlan_tag_id_2" ]; then
		$IFCONFIG $WAN_IF.$vlan_tag_id_2 down
		brctl delif $BR_VLAN $WAN_IF.$vlan_tag_id_2
	fi
	#Restore WAN_IF: eth0
	brctl addif $BR_IF $WAN_IF
	$IFCONFIG $WAN_IF up
}

vlan_tag_disable_without_lan_restart() {
	#Move LAN interface from "br2" back to "br0"
	if [ "x$vlan_tag_id_1" != "x$vlan_tag_id_2" ]; then
		brctl delif $BR_VLAN $LAN_IF
		brctl addif $BR_IF $LAN_IF
	fi

	#Disable MoCA VLAN interface, then enable MoCA interface
	moca_vlan_if_down

	#Disable BR_VLAN (br2) interface
	ifconfig $BR_VLAN down
}

vlan_tag_disable() {
	vlan_tag_disable_without_lan_restart

	#restart LAN interface
	lan_restart
}

#Before running this function, all interfaces should be attached to "br0".
#After this function, "eth1" and "eth0.4094" attaches to "br2".
vlan_tag_enable() {
	#Prepare br2 (BR_VLAN) interface
	#Sometimes, br2 is remvoed by wireles driver, So create it again if necessary.
	BR_VLAN_EXIST=`$IFCONFIG $BR_VLAN 2>/dev/zero | grep -c "br"`
	if [ "x$BR_VLAN_EXIST" == "x0" ]; then
		/etc/rc.d/S16bridge-vlan start
		br_default_mac=`$IFCONFIG $BR_IF | grep "HWaddr" | ask '{print $5}'`
		$IFCONFIG $BR_VLAN hw ether $br_default_mac
	fi
	$IFCONFIG $BR_VLAN up

	#Disable MoCA interface, then enable MoCA VLAN interface
	moca_vlan_if_up

	#Prepare WLAN interfaces: ath0~ath15
	#Nothing to do because all WLAN interfaces are already attached to "br0".

	#Prepare LAN interface: eth1
	if [ "x$vlan_tag_id_1" != "x$vlan_tag_id_2" ]; then
		brctl delif $BR_IF $LAN_IF
		brctl addif $BR_VLAN $LAN_IF
	fi

	#restart LAN interface
	lan_restart
}

test1() {
	killall -9 ntpclient;killall -9 miniupnpd;killall -9 snmpd;killall -9 net-scan;
	killall -9 udhcpd;killall -9 udhcpc;killall -9 dniautoip;
	killall -9 telnetenable;killall -9 utelnetd;
	killall -9 acld;killall -9 crond;killall -9 boxlogin;
	killall -9 syslogd;killall -9 klogd;killall -9 hotplug2;
	#hostapd will auto shut down by 'wlan down'
	/sbin/wlan down;
	killall -9 inetd;killall -9 uhttpd
	#killall -9 button_detect;killall -9 datalib;killall -9 potval;
	#killall -9 detcable;
}

test2() {
	echo 4 > /proc/sys/kernel/printk
}

test3() {
	#restore device from debug mode into normal mode
	$CONFIG set eth_background_control=0
	$CONFIG commit
}

save_config_file() {
	rm -r /tmp/moca/config-to-save.tar.gz config-to-save.tar /tmp/moca/config-to-save/ 1>/dev/mull 2>/dev/null
	[ ! -d /tmp/moca/config-to-save ] && mkdir -p /tmp/moca/config-to-save
	cp /tmp/moca/examples/*.conf /tmp/moca/config-to-save/
	cd /tmp/moca/
	tar cvf config-to-save.tar config-to-save 1>/dev/mull 2>/dev/null
	gzip config-to-save.tar 1>/dev/mull 2>/dev/null #config-to-save.tar.gz
	#get_and_save len into /dev/mtd-moca-config
	#dd if=/tmp/moca/config-to-save.tar.gz of=/dev/mtd-moca-config bs=1 seek=16 count=$(len)
	[ -f config-to-save.tar.gz ] && /etc/moca/dni-mtd save_config_file 1>/dev/mull 2>/dev/null
}

output_config_file() {
	rm -r /tmp/moca/config-to-save.tar.gz config-to-save.tar /tmp/moca/config-to-save/ 1>/dev/mull 2>/dev/null
	#get len from /dev/mtd-moca-config
	#dd if=/dev/mtd-moca-config of=/tmp/moca/config-to-save.tar.gz bs=1 skip=16 count=$(len)
	[ -d /tmp/moca/ ] && /etc/moca/dni-mtd output_config_file 1>/dev/mull 2>/dev/null
	cd /tmp/moca/
	gzip -d config-to-save.tar.gz 1>/dev/mull 2>/dev/null
	tar xvf config-to-save.tar 1>/dev/mull 2>/dev/null
	cp /etc/moca/examples/* /tmp/moca/examples/
	cp /tmp/moca/config-to-save/* /tmp/moca/examples/
}

#restore clink.conf to clink.backup
restore()
{
	#erase moca-config partition
	mtd erase /dev/mtd-moca-config
	#reset SoC
	/etc/moca/clnkcfg --restore
	sleep 5
	/etc/moca/clnkrst -i jaws31 &
}

#echo "----- $0 $1 -----"
case "$1" in
	start)
		start
	;;
	stop)
		stop
	;;
	restart)
		restart
	;;
	show_netinfo)
		show_netinfo
	;;
	show_conf)
		show_conf
	;;
	update_conf_file)
		update_conf_file
	;;
	change_channel_by_mask)
		change_channel_by_mask
	;;
	apply_change)
		apply_change
	;;
	vlan_tag_disable_without_lan_restart)
		vlan_tag_disable_without_lan_restart
	;;
	vlan_tag_disable)
		vlan_tag_disable
	;;
	vlan_tag_enable)
		vlan_tag_enable
	;;
	save_config_file)
		save_config_file
	;;
	output_config_file)
		output_config_file
	;;
	test1)
		test1
	;;
	test2)
		test2
	;;
	test3)
		test3
	;;
	restore)
		restore
	;;
esac
