
# Recognize tag packet from CPU
ethreg -i eth0 0x620=0x000004f0
ethreg -i eth0 0x660=0x0014017e
ethreg -i eth0 0x66c=0x0014017d
ethreg -i eth0 0x678=0x0014017b
ethreg -i eth0 0x684=0x00140177
ethreg -i eth0 0x690=0x0014016f
ethreg -i eth0 0x69c=0x0014015f
ethreg -i eth0 0x6a8=0x0014013f

# Insert PVID 1 to CPU and LAN ports
ethreg -i eth0 0x420=0x00010001 
ethreg -i eth0 0x428=0x00010001
ethreg -i eth0 0x430=0x00010001
ethreg -i eth0 0x438=0x00010001
ethreg -i eth0 0x440=0x00010001
ethreg -i eth0 0x448=0x00010001

# Insert PVID 2 to Target port
ethreg -i eth0 0x450=0x00020002


# Egress tag packet to CPU and untagged packet to all LAN and Target ports
ethreg -i eth0 0x424=0x00002040

ethreg -i eth0 0x42c=0x00001040
ethreg -i eth0 0x434=0x00001040
ethreg -i eth0 0x43c=0x00001040
ethreg -i eth0 0x444=0x00001040
ethreg -i eth0 0x44c=0x00001040
ethreg -i eth0 0x454=0x00001040

# Group port - 0,1,2,3,4,5 to VID 1 
ethreg -i eth0 0x610=0x001b5560
# BUSY is changed to bit[31],need to modify register write driver
 ethreg -i eth0 0x614=0x80010002


# Group port - 0 , 6  to VID 2
ethreg -i eth0 0x610=0x0019ffe0
# BUSY is changed to bit[31],need to modify register write driver
ethreg -i eth0 0x614=0x80020002
# ethreg -i eth0 -p 0x18 0x0=0x3
# ethreg -i eth0 -p 0x10 0xa=0x0002
# ethreg -i eth0 -p 0x10 0xb=0x8002

#vconfig add eth0 1
#vconfig add eth0 2
#ifconfig eth0.1 up
#ifconfig eth0.2 up

#ifconfig eth0.2 192.168.2.1

#brctl addif br0 eth0.1
#brctl addif br0 eth0.2
#ifconfig br0 192.168.1.2

#cfg -a AP_VLAN_MODE=1

# Set mtu for eth0
#ifconfig eth0 mtu 2000
#ifconfig eth0.1 mtu 2000

echo 1 > /proc/sys/net/ipv4/ip_forward
