#!/bin/sh 

conf_file="/tmp/etc/snmpd.conf"
pid_file="/tmp/snmpd.pid"
SNMP=`config get snmp_enable`

create_config_file() {
	rocomm="$(/bin/config get snmp_read_community)"
	rwcomm="$(/bin/config get snmp_write_community)"

	echo -n "" > $conf_file

	[ x$rocomm != x ] && echo "rocommunity $rocomm" >> $conf_file
	[ x$rwcomm != x ] && echo "rwcommunity $rwcomm" >> $conf_file
	
	echo "authtrapenable  1" >> $conf_file
	echo "rwuser root" >> $conf_file
}

if [ "$1" = "start" ]; then
	if [ $SNMP -eq 1 ]; then
		echo -n "Starting snmpd:"
		create_config_file
		/usr/sbin/snmpd -c $conf_file -P $pid_file 2>/dev/null
		echo "."
	fi
elif [ "$1" = "stop" ]; then
	echo -n "Stopping snmpd:"
	killall snmpd
	echo "."
else
	echo -n "invalid argument\n"
fi
