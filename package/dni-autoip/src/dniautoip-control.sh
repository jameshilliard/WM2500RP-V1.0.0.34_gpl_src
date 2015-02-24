#!/bin/sh

CONFIG="/bin/config"
BR_IF=br0
LAN_IF=eth1

output_conf_file(){
    cat <<EOF
pidfile /var/run/udhcpd.pid
start 192.168.1.100
end 192.168.1.249
interface br0
remaining yes
auto_time 5
lease_file /tmp/udhcpd.leases
option subnet 255.255.255.0
option router 192.168.1.250
option dns 192.168.1.250
option lease 60
EOF
}

get_dynamic_ip()
{
	$CONFIG set dniautoip_get_dynamic_ip=1
}

not_get_dynamic_ip()
{
	$CONFIG set dniautoip_get_dynamic_ip=0
}

#This function is only called by udhcpc
#system("[ $(ps|grep -c [u]dhcpd) -eq 0 ] && ifconfig br0 192.168.1.250 && udhcpd /tmp/udhcpd.conf");
setup_local_system()
{
	#If this function runs, it means it didn't get dynamic IP address from Outside DHCP Server
	not_get_dynamic_ip

	#set system with fixed ip (192.168.1.250) whenever device cannot get an dynamic ip.
	set_system_with_fixed_private_ip

	#dniautoip is only running when DHCP server mode is "AUTO"
	#"/tmp/master_log" is created (or deleted) when dniautoip gets running (or exit)
	if [ ! -f /tmp/master_log ]; then
		exit
	fi

	#Notice other peers(Master or Slaves) to update their IP address
	send_sigusr2

	#If I am slave...
	[ $(grep -c "[M]aster Or Not(1: Master): 1" /tmp/master_log) -eq 0 ] && {
		exit
	}

	#If I am master...
	#echo "------------set_master--------------"
	#Exit if I was set as master before, which means I have had Local DHCP Server.
	if [ $(ps|grep -v /bin/sh|grep -c "[u]dhcpd_master.conf") -gt 0 ]; then
		exit
	else
		enable_local_dhcp_server
	fi
}

#Configure system with fixed IP addr: 192.168.1.250
set_system_with_fixed_private_ip()
{
	lan_ip=$($CONFIG get lan_ipaddr)
	lan_mask=$($CONFIG get lan_netmask)
	if [ "x$lan_ip" == "x0.0.0.0" -o "x$lan_mask" == "x0.0.0.0" ]; then
		lan_ip=
		lan_mask=
	fi
	#ifconfig $BR_IF ${lan_ip:-192.168.1.250} netmask ${lan_mask:-255.255.255.0}
	cur_lan_ip=$(ifconfig $BR_IF|grep "[i]net addr"|awk -F ":" '{print $2}'| awk '{print $1}')
	#this should work under DHCP mode, not static IP mode.
	lan_dhcp=$($CONFIG get lan_dhcp)
	[ "x$cur_lan_ip" != "x192.168.1.250" -a "x$lan_dhcp" == "x1" ] && {
		ifconfig $BR_IF 192.168.1.250 netmask 255.255.255.0
		ifconfig $LAN_IF down && sleep 3 && ifconfig $LAN_IF up
		$CONFIG set lan_ipaddr=192.168.1.250
		$CONFIG set lan_netmask=255.255.255.0
		$CONFIG set lan_gateway=192.168.1.250
		$CONFIG set lan_ether_dns1=192.168.1.250
		$CONFIG commit
		echo "nameserver 192.168.1.250" > /tmp/resolv.conf
	}
}

#Start/Restart Local DHCP Server ...
enable_local_dhcp_server()
{
	#[ $(ps|grep -c [u]dhcpd) -eq 0 ] && udhcpd /tmp/udhcpd.conf
	[ $(ps|grep -v /bin/sh|grep -c "[u]dhcpd.conf") -gt 0 ] && killall -9 udhcpd
	[ $(ps|grep -v /bin/sh|grep -c "[u]dhcpd_master.conf") -eq 0 ] && {
		[ ! -f /tmp/udhcpd_master.conf ] && output_conf_file > /tmp/udhcpd_master.conf
		udhcpd /tmp/udhcpd_master.conf
	}
}

#system("[ $(ps|grep -c [u]dhcpc) -gt 0 ] && kill -SIGUSR1 $(ps|grep [u]dhcpc|awk '{print $1}')");
renew_udhcpc()
{
	[ $(ps|grep -v /bin/sh|grep -c [u]dhcpc) -gt 0 ] && kill -SIGUSR1 $(ps|grep -v /bin/sh|grep [u]dhcpc|awk '{print $1}')
	#renew 6dhcpc
	/usr/sbin/6dhcpc renew
}

#system("[ $(ps|grep -c [u]dhcpd) -gt 0 ] && killall -9 udhcpd");
remove_udhcpd()
{
	if [ "x$($CONFIG get enable_dhcpd)" == "x2" ]; then
		[ $(ps|grep -v /bin/sh|grep -c [u]dhcpd) -gt 0 ] && killall -9 udhcpd
	fi
}

remove_udhcpd_renew_udhcpc()
{
	remove_udhcpd
	renew_udhcpc
}

#This function is called by moca-util
send_sigusr1()
{
	#[ $(ps|grep -v /bin/sh|grep -c [d]niautoip) -gt 0 ] && kill -SIGUSR1 $(ps|grep -v /bin/sh|grep [d]niautoip|head -n 1|awk '{print $1}')
	if [ -f /tmp/master_log ]; then
		renew_udhcpc
	fi
}

send_sigusr2()
{
	[ $(ps|grep -v /bin/sh|grep -c [d]niautoip) -gt 0 ] && kill -SIGUSR2 $(ps|grep -v /bin/sh|grep [d]niautoip|head -n 1|awk '{print $1}')
}

start()
{
	[ $(ps|grep -v /bin/sh|grep -c [d]niautoip) -eq 0 ] && {
		dniautoip&
	}
}

stop()
{
	killall dniautoip
	rm /tmp/master_log
}

restart()
{
	stop
	start
}

#echo "----- $0 $1 -----"
case "$1" in
	setup_local_system)
		setup_local_system
	;;
	renew_udhcpc)
		renew_udhcpc
	;;
	remove_udhcpd)
		remove_udhcpd
	;;
	remove_udhcpd_renew_udhcpc)
		remove_udhcpd_renew_udhcpc
	;;
	send_sigusr1)
		send_sigusr1
	;;
	send_sigusr2)
		send_sigusr2
	;;
	start)
		start
	;;
	stop)
		stop
	;;
	restart)
		restart
	;;
	not_get_dynamic_ip)
		not_get_dynamic_ip
	;;
	get_dynamic_ip)
		get_dynamic_ip
	;;
esac
