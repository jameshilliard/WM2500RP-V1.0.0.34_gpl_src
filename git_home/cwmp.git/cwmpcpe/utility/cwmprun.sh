#!/bin/sh

CONFIG=/bin/config
CWMPCPE=/usr/bin/cwmpcpe
CWMPDIR=/tmp/cwmp
CWMPRUN_LOCK=${CWMPDIR}/cwmprun.lock

[ ! -d $CWMPDIR ] && mkdir -p $CWMPDIR

trap "rm -f $CWMPRUN_LOCK; exit 1" SIGTERM
trap "rm -f $CWMPRUN_LOCK; exit 1" SIGINT
[ -f $CWMPRUN_LOCK ] \
	&& { echo "cwmpcpe_run.sh is running!"; exit 0; } \
	|| touch $CWMPRUN_LOCK

cwmp_endis=$($CONFIG get cwmp_tr069_enable)
wan_type=
wansta="down"
wanif_eth="br0"

check_wan_conn()
{
	wansta="down"
	local eth1_value=$(ifconfig $wanif_eth | grep "inet addr:")
	[ "x$eth1_value" != "x" ] && wansta="up"
	wan_type="ip"
}

cwmpcpe_start()
{
	#delete the file which is used to indicate that the port is not available
	/bin/config unset NATDetected
	/bin/config unset UDPConnectionAddr
	rm -fr /tmp/cwmp/bind_error
	[ "x$cwmp_endis" != "x1" ] && return

	while true
	do
		check_wan_conn
		[ "$wansta" != "up" ] && { sleep 10; continue; }
		$CWMPCPE -w $wan_type &
		break
	done
}

cwmpcpe_stop()
{
	/usr/bin/killall -9 cwmpcpe
}

case $1 in
	start)
	cwmpcpe_stop
	cwmpcpe_start
	;;
	restart)
	cwmpcpe_stop
	cwmpcpe_start
	;;
	stop)
	cwmpcpe_stop
	;;
	*)
	echo "Usage: $0 start|stop|restart"
	;;
esac

rm -f $CWMPRUN_LOCK
