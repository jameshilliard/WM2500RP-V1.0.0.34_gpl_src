#!/bin/sh

###
# Read certs (CAs and private cert) from cwmp patition
# Write certs (CAs and private cert) to cwmp partition

maxlen=524288 # 512 kbytes
mtdname=$(grep cwmp /proc/mtd | cut -d: -f1) # mtd=/dev/mtd12 ?
[ -z $mtdname ] && exit

get_certs()
{
	# type:
	#	TRCA - CA, PCRT - Private cert, DTBS - Database of cwmpcpe
	local type=
	local length=
	local typesize=4
	local lengthsize=4
	local canum=0
	local offset=0

	[ -d /etc/ssl/private ] || mkdir -p /etc/ssl/private
	[ -d /etc/ssl/CA ] || mkdir -p /etc/ssl/CA/
	[ -d /etc/ssl/certs ] || mkdir -p /etc/ssl/certs

	while [ $offset -lt $maxlen ]
	do
		type=$(hexdump -n $typesize -s $offset -e "1/$typesize \"%s\"" /dev/$mtdname)
		type=$(expr substr $type 1 4)
		length=$(hexdump -n $lengthsize -s $(($offset + $typesize)) -e "1/$lengthsize \"%u\"" /dev/$mtdname)
		offset=$((offset + typesize + lengthsize))
		[ $((offset + length)) -gt $maxlen ] && break

		case $type in
		"PCRT")
			dd if=/dev/$mtdname of=/etc/ssl/private/telenet_cpe.pem count=$length skip=$offset bs=1
			offset=$((offset + length))
			;;
		"TRCA")
			dd if=/dev/$mtdname of=/etc/ssl/CA/telenet_ca$canum.crt count=$length skip=$offset bs=1
			ln -sf /etc/ssl/CA/telenet_ca$canum.crt /etc/ssl/certs/$(openssl x509 -hash -noout -in /etc/ssl/CA/telenet_ca$canum.crt).0
			canum=$((canum + 1))
			offset=$((offset + length))
			;;
		*)
			echo "data in cwmp partition is wrong!"
			break
			;;
		esac
	done
}

set_certs()
{
	local hexlen
	local cwmpimg=/tmp/cwmp.img
	rm -f $cwmpimg

	if [ -f /etc/ssl/private/telenet_cpe.pem ]; then
		length=$(wc -c /etc/ssl/private/telenet_cpe.pem | awk '{print $1}')
		echo -ne "PCRT" >> $cwmpimg
		hexlen=$(printf %08x $length)
		echo -ne "\x$(expr substr $hexlen 1 2)\x$(expr substr $hexlen 3 2)\x$(expr substr $hexlen 5 2)\x$(expr substr $hexlen 7 2)" >> $cwmpimg
		cat /etc/ssl/private/telenet_cpe.pem >> $cwmpimg
	fi

	ls /etc/ssl/CA/telenet_ca* | while read name
	do
		length=$(wc -c $name | awk '{print $1}')
		echo -ne "TRCA" >> $cwmpimg
		hexlen=$(printf %08x $length)
		echo -ne "\x$(expr substr $hexlen 1 2)\x$(expr substr $hexlen 3 2)\x$(expr substr $hexlen 5 2)\x$(expr substr $hexlen 7 2)" >> $cwmpimg
		cat $name >> $cwmpimg
	done
	
	[ ! -f $cwmpimg ] && return
	
	local FF="\xff"
	while [ ${#FF} -lt $maxlen ]
	do
		FF=${FF}${FF}
	done
	rm -f ${cwmpimg}.pad
	echo -ne $FF$FF$FF$FF >> ${cwmpimg}.pad
	unset FF

	cat $cwmpimg ${cwmpimg}.pad > ${cwmpimg}.new
	head -c $maxlen ${cwmpimg}.new > $cwmpimg
	rm -f ${cwmpimg}.pad ${cwmpimg}.new

	mtd erase $mtdname && mtd write $cwmpimg $mtdname
	rm -f $cwmpimg
}

case $1 in
	getcerts)
		get_certs
		;;
	setcerts)
		set_certs
		;;
esac
