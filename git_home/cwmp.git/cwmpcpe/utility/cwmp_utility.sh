#!/bin/sh

#################################################################################
# command usage:								#
#	$0  get <parameter name>						#
#	$0  set <parameter name> <parameter value>				#
#	$0  exist <instance name>						#
#	$0  inumber <object name> <instance id>					#
#	$0  commit								#
#################################################################################

#echo $* >&2

# tools
CONFIG=/bin/config
CAT=/bin/cat
ECHO=/bin/echo
SED=/bin/sed
AWK=/usr/bin/awk

# resource files
SOURCEFILE=/etc/cwmp/cwmp_rsc.list
ACTIONFILE=/tmp/cwmp/action

# macros of action definition
RDUT=$((1 << 0))
RWAN=$((1 << 1))
RLAN=$((1 << 2))
RWLAN=$((1 << 3))

# function instances
. /etc/cwmp/cwmp_func.list

handle_action()
{
	case $1 in
		RDUT)
		echo $(($RDUT | $($CAT $ACTIONFILE))) > $ACTIONFILE; ;;
		RWAN)
		echo $(($RWAN | $($CAT $ACTIONFILE))) > $ACTIONFILE; ;;
		RLAN)
		echo $(($RLAN | $($CAT $ACTIONFILE))) > $ACTIONFILE; ;;
		RWLAN)
		echo $(($RWLAN | $($CAT $ACTIONFILE))) > $ACTIONFILE; ;;
	esac
}

# arg1: method, arg2: parameter name, arg3: parameter value
handle_parameter()
{
	# mtd: method, pn: parameter name, pv: parameter value, ptn: pattern name
	mtd=$1
	pn=$2
	pv=$3
	#echo $#: $* >&2

	[ "x$pn" = "x" ] && return 1
	[ $mtd = set -a "x$pv" = "x" ] && return 1

	ptn=$(echo $pn | $SED -r 's/\.[0-9]+\./\.\\{i\\}\./g')
	ARGS="$(eval $AWK -F \"#\" \'/^${ptn}#/ {print \$1, \"\\\"\" \$2 \"\\\"\", \$3, \$4}\' $SOURCEFILE)"
	[ "x$ARGS" = "x" ] && return 1
	eval set $ARGS
	#echo $#: $* >&2

	# args:
	# $1: pattern name, $2: Reboot Mark, $3: get/set parameter from, $4: parameter value source
	if [ $mtd = get ]; then
		case $3 in
			D) # datalib
			$CONFIG get $4; ;;
			F) # file
			$CAT $4; ;;
			C) # command/function
			[ "x$4" != "x" ] && eval $4 get $pn; ;;
			S) # String
			$ECHO $4; ;;
		esac
	elif [ $mtd = set ]; then
		case $3 in
			D) # datalib
			$CONFIG set $4=$pv; ;;
			F) # file
			$ECHO $pv > $4; ;;
			C) # command/function
			[ "x$4" != "x" ] && eval $4 set \"$pn\" \"$pv\"; ;;
			*)
			return 1; ;;
		esac
		[ "x$2" != "x" ] && { echo 0 > $ACTIONFILE;handle_action $2; }
	fi
}

handle_object()
{
	# mtd: method, obj: object/instance name, id: instance id
	mtd=$1
	obj=$2
	id=$3
	#echo $#: $* >&2

	[ "x$obj" = "x" ] && return 1
	[ $mtd = inumber -a "x$id" = "x" ] && return 1

	flag=$($ECHO $obj |$AWK -F "." '{print $NF}')
	if [ "$flag" ];then
		pobj=${obj}"."
		obj=$pobj
	fi

	pobj=$($ECHO $obj | $SED -r 's/\.[0-9]+\./\.\\{i\\}\./g')
	if [ $mtd = inumber ]; then
		pobj=${pobj%\\\{i\\\}*}
		ARGS="$(eval $AWK -F \"#\" \'/^${pobj}\\{i\\}\.#/ {print \$1, \"\\\"\" \$2 \"\\\"\", \$4, \$5, \$6, \$7}\' $SOURCEFILE)"
	elif [ $mtd = exist ]; then
		pobj=${pobj%\\\{i\\\}*}
		ARGS="$(eval $AWK -F \"#\" \'/^${pobj}\\{i\\}\.#/ {print \$1, \"\\\"\" \$2 \"\\\"\", \$4, \$5, \$6, \$7}\' $SOURCEFILE)"
	else
		ARGS="$(eval $AWK -F \"#\" \'/^${pobj}#/ {print \$1, \"\\\"\" \$2 \"\\\"\", \$4, \$5, \$6, \$7}\' $SOURCEFILE)"
	fi
	[ "x$ARGS" = "x" ] && return 1
	eval set $ARGS
	#echo $#: $* >&2

	# args:
	# $1: pattern name, $2: Reboot mark, $3: exist handle, $4: inumber handle, $5: add handle, $6: delete handle
	if [ "$mtd" = "exist" ]; then
		eval $3 $obj
	elif [ "$mtd" = "inumber" ]; then
		eval $4 $obj $id
	elif [ "$mtd" = "add" ]; then
		[ "x$(eval $3 $obj)" != "x0" ] \
			&& { eval $5 $obj; [ "x$2" != "x" ] && handle_action $2; }
	elif [ "$mtd" = "del" ]; then
		[ "x$(eval $3 $obj)" = "x0" ] \
			&& { eval $6 $obj; [ "x$2" != "x" ] && handle_action $2; }
	fi
}

handle_commit()
{
	$CONFIG commit
	[ -f $ACTIONFILE ] && ACTION="$($CAT $ACTIONFILE)" || ACTION=0

	test 0 -ne $(($ACTION & $RDUT)) && \
		{ echo $(($ACTION & ~$RDUT)) > $ACTIONFILE; /sbin/reboot; exit 0; }
	test 0 -ne $(($ACTION & $RWAN)) && \
		{ echo $(($ACTION & ~$RWAN)) > $ACTIONFILE; /etc/init.d/net-wan restart; }
	test 0 -ne $(($ACTION & $RLAN)) && \
		{ echo $(($ACTION & ~$RLAN)) > $ACTIONFILE; /etc/init.d/net-lan restart; }
	test 0 -ne $(($ACTION & $RWLAN)) && \
		{ echo $(($ACTION & ~$RWLAN)) > $ACTIONFILE; wlan down; wlan up ; }
}

handle_usage()
{
	$ECHO -e "Usages:\n" \
		"$0  get <parameter name>\n" \
		"	Example: $0 get \"InternetGatewayDevice.ManagementServer.EnableCWMP\"\n" \
		"$0  set <parameter name> <parameter value>\n" \
		"	Example: $0 set \"InternetGatewayDevice.ManagementServer.EnableCWMP\" \"1\"\n" \
		"$0  exist <instance name>\n" \
		"	Example: $0 exist \"InternetGatewayDevice.LANDevice.1.\"\n" \
		"$0  inumber <object name> <instance id>\n" \
		"	Example: $0 inumber \"InternetGatewayDevice.LANDevice.\" \"1\"\n" \
		"$0  commit"
	exit 1
}

case $1 in
	get | set)
		# args: $@
		#	case 1: $1 - get, $2 - <parameter name>
		#		Example: get "InternetGatewayDevice.ManagementServer.EnableCWMP"
		#	case 2: $1 - set, $2 - <parameter name>, $3 - <parameter value>
		#		Example: set "InternetGatewayDevice.ManagementServer.EnableCWMP" "1"
		handle_parameter $@
		;;
	exist | inumber)
		# args: $@
		#	case 1: $1 - exist, $2 - <instance name>
		#		Example: exist "InternetGatewayDevice.LANDevice.1."
		#	case 2: $1 - inumber, $2 - <parental object name>, $3 - <instance id>
		#		Example: inumber "InternetGatewayDevice.LANDevice." "1"
		#TODO	case 3: $1 - add, $2 - <instance name>
		#TODO	case 4: $1 - del, $2 - <instance name>
		handle_object $@
		;;
	commit)
		handle_commit
		;;
	*)
		handle_usage
		;;
esac

