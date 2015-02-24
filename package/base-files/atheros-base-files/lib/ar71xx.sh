#!/bin/sh
#
# Copyright (C) 2009 OpenWrt.org
#

ar71xx_board_name() {
	local machine
	local name

	machine=$(awk 'BEGIN{FS="[ \t]+:[ \t]"} /machine/ {print $2}' /proc/cpuinfo)

	case "$machine" in
	*AP81)
		name="ap81"
		;;
	*AP83)
		name="ap83"
		;;
	*AW-NR580)
		name="aw-nr580"
		;;
	*LS-SR71)
		name="ls-sr71"
		;;
	*MZK-W04NU)
		name="mzk-w04nu"
		;;
	*MZK-W300NH)
		name="mzk-w300nh"
		;;
	*PB42)
		name="pb42"
		;;
	*PB44)
		name="pb44"
		;;
	*R6000D)
		name="r6000d"
		;;
	*RB-411)
		name="rb-411"
		;;
	*RB-433)
		name="rb-433"
		;;
	*RB-450)
		name="rb-450"
		;;
	*RB-493)
		name="rb-493"
		;;
	*RouterStation)
		name="routerstation"
		;;
	*TEW-632BRP)
		name="tew-632brp"
		;;
	*TL-WR741ND)
		name="tl-wr741nd"
		;;
	*TL-WR941ND)
		name="tl-wr941nd"
		;;
	*WP543)
		name="wp543"
		;;
	*WNDR3400v3)
		name="wndr3400v3"
		;;
	*WNDR3700)
		name="wndr3700"
		;;
	*WNR2000)
		name="wnr2000"
		;;
	*WRT160NL)
		name="wrt160nl"
		;;
	*WRT400N)
		name="wrt400n"
		;;
	*WM2500RP)
		name="wm2500rp"
		;;
	*)
		name="generic"
		;;
	esac

	echo $name
}
