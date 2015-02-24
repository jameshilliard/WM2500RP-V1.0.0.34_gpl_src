#!/bin/sh

# Telenet Spec requires to get certificate revocation list (CRL) from the URL defined in Telenet
# root CA certificate.

# Section 3.2.3 ACS revocation list
# The CPE MUST include a certificate revocation list. This CRL MUST be updated with an interval of
# 24 hours (or 86400 seconds). This update functionality MUST have an aggressive retry algorithm.
# Each subsquent retry MUST be preformed in half of the time defined for the previous retry with a
# minimum of 180 seconds (3 minutes):
# --------------------------------------------------------------------------
# | Retry #     |   1   |   2   |   3   |   4   |   5  |   6  |   7  |  8  |
# |------------------------------------------------------------------------|
# | Interval(s) | 86400 | 43200 | 21600 | 10800 | 5400 | 2700 | 1350 | 675 |
# --------------------------------------------------------------------------
# The CPE MUST find the CRL URL in the root CA certificate as a CRL distribution point. The CPE
# MUST NOT contact the remote CRL for each new connection. Instead the CPE MUST have a local CRL
# which it MUST update via the remove CRL at a regular interval.
# Telenet will maintain the remove CRL. The vendor MUST provide details on the maximum allowed
# memory usage and CRL size.

# Netgear "Jean-Christophe Deperthe(jdeperthe@netgear.com)" clarify some about CRL, refer to below
# pages for detail.
# http://dniserver.dnish.net/pipermail/wndr4300/2013-July/006120.html
# http://dniserver.dnish.net/pipermail/wndr4300/2013-July/006121.html
# Key points are:
# 1) Correct URL for CRL is "http://cwmp.devicemanagement.telenet.be/crl".
# 2) CRL file use binary DER format.
# 3) If the CPE is unable to download the CRL, continue with the HTTPS connection start, and skip
#    the CRL check.
# 4) The CRL is likely to be small, less than 1KB initially.  It might grow in size later, but is
#    unlikely to exceed 4KB.

[ "x1" != "x$(/bin/config get cwmp_tr069_enable)" ] && exit

# Get URL of CRL from Telenet root CA certificate
cadir="/etc/ssl/certs/"
for f in $(ls $cadir)
do
	tnca=$(openssl x509 -noout -subject -in $cadir$f | \
		grep 'CN=Telenet CPE Root CA')
	if [ -n "$tnca" ]; then
		iscrl=0
		while read line
		do
			if [ $iscrl -eq 1 -a -n "$(echo $line | grep 'URI:')" ]; then
				CRLURL=${line#*:}
				break
			fi
			[ -n "$(echo $line | grep 'CRL Distribution Points')" ] && iscrl=1
		done <<EOF
		$(openssl x509 -noout -text -in $cadir$f)
EOF
		[ -n "$CRLURL" ] && break
	fi
done
[ -z "$CRLURL" ] && exit

maxint=86400
minint=180
interval=$maxint

tmpcrl="${CRLURL##*/}"
crldir="/etc/ssl/crl/"
telcrl="${crldir}telenet_crl.pem"

mkdir -p $crldir
mkdir -p /tmp/telenetcrl/ && cd /tmp/telenetcrl/
while [ 1 ]
do
	rm -f $tmpcrl # remove old temporary crl file
	wget -q -T 30 $CRLURL
	if [ $? -eq 0 ]; then # download success
		interval=$maxint
		openssl crl -hash -noout -in $tmpcrl
		if [ $? -eq 0 ]; then # PEM format
			cp -f $tmpcrl $telcrl
		else
			openssl crl -hash -noout -inform DER -in $tmpcrl
			if [ $? -eq 0 ]; then # DER format
				openssl crl -in $tmpcrl -inform DER -out $tmpcrl.pem
				[ $? -eq 0 ] && cp -f $tmpcrl.pem $telcrl
			fi
		fi
	else
		if [ $interval -gt $minint ]; then
			interval=$(expr $interval / 2)
			[ $interval -lt $minint ] && interval=$minint
		fi
	fi
	sleep $interval
done

