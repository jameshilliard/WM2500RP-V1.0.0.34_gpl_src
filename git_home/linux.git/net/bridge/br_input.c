/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include "br_private.h"

#ifdef CONFIG_DNI_MCAST_TO_UNICAST
#include <linux/ip.h>
#include <linux/igmp.h>
#include "br_igmp.h"

static inline void 
add_mac_cache(struct sk_buff *skb)
{
      unsigned char i, num = 0xff;
      unsigned char *src, check = 1;
      struct iphdr *iph;
      struct ethhdr *ethernet=(struct ethhdr *)skb->mac_header;

      iph = (struct iphdr *)skb->network_header;
      src = ethernet->h_source;

      for (i = 0; i < MCAST_ENTRY_SIZE; i++)
      {
              if (mac_cache[i].valid)
                      if ((++mac_cache[i].count) == MAX_CLEAN_COUNT)
                              mac_cache[i].valid = 0;
      }

      for (i = 0; i < MCAST_ENTRY_SIZE; i++)
      {
              if (mac_cache[i].valid)
              {
                      if (mac_cache[i].sip==iph->saddr)
                      {
                              num = i;
                              break;
                      }
              }
              else if (check)
              {
                      num=i;
                      check = 0;
              }
      }

      if (num < MCAST_ENTRY_SIZE)
      {
              mac_cache[num].valid = mac_cache[num].count = 1;
              memcpy(mac_cache[num].mac, src, 6);
              mac_cache[num].sip = iph->saddr;
              mac_cache[num].dev = skb->dev;
      }
}

static void detect_mgroup(const struct sk_buff *skb)
{
	struct iphdr *igmp_iph;
	unsigned char *igmp_type;
	unsigned short igmpv3_numsrc;
	unsigned short igmpv3_numgrps;
	unsigned char igmpv3_type;
	int i;
	uint32_t gip;
	unsigned long sip;
	igmpr_t *igmpv12_report;
	igmp_report_t *igmpv3_report;
	igmp_iph = (struct iphdr *)skb->network_header;

	sip = igmp_iph->saddr;

	if(igmp_iph->protocol == IPPROTO_IGMP && igmp_iph->daddr != 0XEFFFFFFA)
	{
		igmp_type = (unsigned char *)igmp_iph + (igmp_iph->ihl << 2);
		//printk("%s+%d: --- Dst:%d.%d.%d.%d ---\n", __FILE__, __LINE__, (igmp_iph->daddr>>24)&0xff,(igmp_iph->daddr>>16)&0xff,(igmp_iph->daddr>>8)&0xff,(igmp_iph->daddr)&0xff);
		switch(*igmp_type)
		{
			case IGMP_V1_MEMBERSHIP_REPORT:
			case IGMP_V2_MEMBERSHIP_REPORT:
				igmpv12_report = (igmpr_t *)igmp_type;
				//gip = (uint32_t)igmpv12_report->igmpr_group; //error: aggregate value used where an integer was expected
				gip = *((uint32_t *)&igmpv12_report->igmpr_group);
				//printk("%s+%d: --- IGMP_V2: %s Join %d.%d.%d.%d ---\n",__FILE__,__LINE__,skb->dev->name,(gip>>24)&0xff,(gip>>16)&0xff,(gip>>8)&0xff,(gip)&0xff);
				/*
				if(!iptv_port_find_mgroup(skb->dev->name, gip, 1))
				{
					iptv_port_add_mgroup(skb->dev->name, gip);
				}
				*/
				proc_mcast_entry('a', sip, gip);
				break;
			case IGMP_V2_MEMBERSHIP_LEAVE:
				igmpv12_report = (igmpr_t *)igmp_type;
				gip = *((uint32_t *)&igmpv12_report->igmpr_group);
				//printk("%s+%d: --- IGMP_V2: %s Leave %d.%d.%d.%d ---\n",__FILE__,__LINE__,skb->dev->name,(gip>>24)&0xff,(gip>>16)&0xff,(gip>>8)&0xff,(gip)&0xff);
				/*
				if(iptv_port_find_mgroup(skb->dev->name, gip, 0))
				{
					iptv_port_del_mgroup(skb->dev->name, gip);
				}
				*/
				proc_mcast_entry('d', sip, gip);
				break;
			case IGMP_V3_MEMBERSHIP_REPORT:
				igmpv3_report = (igmp_report_t *)igmp_type;
				igmpv3_numgrps = ntohs(igmpv3_report->igmpr_numgrps);
				for(i=0; i<igmpv3_numgrps; i++)
				{
					igmpv3_type   = (unsigned char)igmpv3_report->igmpr_group[i].igmpg_type;
					igmpv3_numsrc = ntohs(igmpv3_report->igmpr_group[i].igmpg_numsrc);
					switch(igmpv3_type)
					{
						case 1:
						case 2:
						case 4:
						case 5:
						case 6:
							gip = *((uint32_t *)&igmpv3_report->igmpr_group[i].igmpg_group);
							//printk("%s+%d: --- IGMP_V3: %s Join %d.%d.%d.%d ---\n",__FILE__,__LINE__,skb->dev->name,(gip>>24)&0xff,(gip>>16)&0xff,(gip>>8)&0xff,(gip)&0xff);
							/*
							if(!iptv_port_find_mgroup(skb->dev->name, gip, 1))
							{
								iptv_port_add_mgroup(skb->dev->name, gip);
							}
							*/
							proc_mcast_entry('a', sip, gip);
							break;
						case 3:
							if(igmpv3_numsrc == 0)
							{
								gip = *((uint32_t *)&igmpv3_report->igmpr_group[i].igmpg_group);
								//printk("%s+%d: --- IGMP_V3: %s Leave %d.%d.%d.%d ---\n",__FILE__,__LINE__,skb->dev->name,(gip>>24)&0xff,(gip>>16)&0xff,(gip>>8)&0xff,(gip)&0xff);
								/*
								if(iptv_port_find_mgroup(skb->dev->name, gip, 0))
								{
									iptv_port_del_mgroup(skb->dev->name, gip);
								}
								*/
								proc_mcast_entry('d', sip, gip);
							}else{
								gip = *((uint32_t *)&igmpv3_report->igmpr_group[i].igmpg_group);
								//printk("%s+%d: --- IGMP_V3: %s Join %d.%d.%d.%d ---\n",__FILE__,__LINE__,skb->dev->name,(gip>>24)&0xff,(gip>>16)&0xff,(gip>>8)&0xff,(gip)&0xff);
								/*
								if(!iptv_port_find_mgroup(skb->dev->name, gip, 1))
								{
									iptv_port_add_mgroup(skb->dev->name, gip);
								}
								*/
								proc_mcast_entry('a', sip, gip);
							}
							break;
						default:
							break;
					}
				}
				break;
			default:
				break;
		}
	}
}
#endif
/* Bridge group multicast address 802.1d (pg 51). */
const u8 br_group_address[ETH_ALEN] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };

static void br_pass_frame_up(struct net_bridge *br, struct sk_buff *skb)
{
	struct net_device *indev, *brdev = br->dev;

#ifdef CONFIG_DNI_MCAST_TO_UNICAST
	unsigned char *dest;
	struct iphdr *iph;
	unsigned char proto=0;
	struct ethhdr *ethernet=(struct ethhdr *)skb->mac_header;

	// if skb come from wireless interface, ex. ath0, ath1, ath2...
	if (skb->dev->name[0] == 'a')
	{
		iph = (struct iphdr *)skb->network_header;
		proto =  iph->protocol;
		dest = ethernet->h_dest;
		if ( igmp_snoop_enable && MULTICAST_MAC(dest) 
				&& (ethernet->h_proto == ETH_P_IP))
		{
			if (proto == IPPROTO_IGMP)
			{
				add_mac_cache(skb);
				detect_mgroup(skb);
			}

		}
	}
#endif
	brdev->stats.rx_packets++;
	brdev->stats.rx_bytes += skb->len;

	indev = skb->dev;
	skb->dev = brdev;

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		netif_receive_skb);
}

/* note: already called with rcu_read_lock (preempt_disabled) */
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	struct sk_buff *skb2;

	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;

#ifdef CONFIG_ATH_WRAP
	/*Skip updating fdb on MPVAP, if dest is mcast*/
	if(unlikely(PTYPE_IS_WRAP(p->type))){
		int type = p->type & PTYPE_MASK;
		if(!WRAP_PTYPE_HAS_ISO(p->type)){
			if((type != WRAP_PTYPE_MPVAP) 
		  		||(!is_multicast_ether_addr(dest))){
				br_fdb_update(br, p, eth_hdr(skb)->h_source);
			}
		}	
	}else
#else	
	{	
		br_fdb_update(br, p, eth_hdr(skb)->h_source);
	}
#endif	

	if (p->state == BR_STATE_LEARNING)
		goto drop;

	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	dst = NULL;

	if (is_multicast_ether_addr(dest) 
#ifdef CONFIG_DNI_IPV6_PASSTHROUGH
		|| (skb->protocol == __constant_htons(ETH_P_IPV6))
#endif
	) {
		br->dev->stats.multicast++;
		skb2 = skb;
	} else if ((dst = __br_fdb_get(br, dest)) && dst->is_local) {
		skb2 = skb;
		/* Do not forward the packet since it's local. */
		skb = NULL;
	}

	if (skb2 == skb)
		skb2 = skb_clone(skb, GFP_ATOMIC);

	if (skb2)
		br_pass_frame_up(br, skb2);

	if (skb) {
		if (dst)
			br_forward(dst->dst, skb);
		else
			br_flood_forward(br, skb);
	}

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

/* note: already called with rcu_read_lock (preempt_disabled) */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = rcu_dereference(skb->dev->br_port);

	if (p)
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source);
	return 0;	 /* process further */
}

/* Does address match the link local multicast address.
 * 01:80:c2:00:00:0X
 */
static inline int is_link_local(const unsigned char *dest)
{
	__be16 *a = (__be16 *)dest;
	static const __be16 *b = (const __be16 *)br_group_address;
	static const __be16 m = cpu_to_be16(0xfff0);

	return ((a[0] ^ b[0]) | (a[1] ^ b[1]) | ((a[2] ^ b[2]) & m)) == 0;
}

/* /proc/dns_hijack */
int sysctl_dns_hijack;

struct dns_nl_packet_msg {
	size_t data_len;
	char saddr[6];
	unsigned char data[0]
};
typedef struct dns_nl_packet_msg dns_nl_packet_msg_t;

static int findstring(char * query_start, char * dest,int query_len)
{
	if(query_start == NULL || dest == NULL )
		return NULL;
	char *s1 = query_start, *d1 = dest;
	int i = 0;

	/* query = "routerlogin.com" + 4 = 16 + 4 */
	while ( i < (query_len - 16 - 4) )
	{
		if ( memcmp(s1,d1,16) == 0)
			return 1;
		s1++;
		i++;
	}
	return 0;
}

int br_dns_netgear(struct sk_buff *skb)
{
	struct ethhdr                   *eth = (struct ethhdr *)skb->mac_header;
	struct iphdr                    *iph;
	struct udphdr                   *udph;
	if (eth->h_proto != htons(ETH_P_IP))
		return 1;
	/* Check for possible (maliciously) malformed IP frame (thanks Dave) */
	iph = (struct iphdr *) skb->data;
	if(iph->protocol != 17)
		return 1;
	udph = (struct udphdr *)(((unsigned char *)iph) + (iph->ihl<<2));

	if( udph->dest == 53)
	{
		static char domain[20], domain2[20];
		memset(domain, '\0', 20);
		memset(domain2, '\0', 20);
		domain[0] = 0x0b;
		domain2[0] = 0x0b;
		memcpy(domain+1, "routerlogin.com", 15);
		memcpy(domain2+1, "routerlogin.net", 15);
		domain[12] = 0x03;
		domain2[12] = 0x03;
		domain[16]= '\0';
		domain2[16]= '\0';
		/* 20 = 8(UDP HEADER) + 12(DNS QUERY HEADER) */
		if( sysctl_dns_hijack || findstring((unsigned char *)((unsigned char *)udph + 20),domain,udph->len - 20)
				|| findstring((unsigned char *)((unsigned char *)udph + 20),domain2,udph->len - 20) )
		{
			dns_nl_packet_msg_t *pm;
			struct nlmsghdr *nlh;
			size_t size, copy_len;
			copy_len = skb->len;
			size = NLMSG_SPACE(sizeof(*pm)+copy_len);
			struct sk_buff *skbnew =  alloc_skb(4096, GFP_ATOMIC);
			if(!skbnew)
			{
				printk(KERN_ERR "boxlogin: can't alloc whole buffer " "of size %ub!\n", 4096);
				skbnew = alloc_skb(size, GFP_ATOMIC);
				if(!skbnew)
				{
					printk(KERN_ERR "boxlogin: can't alloc buffer " "of size %ub!\n", size);
					return 1;
				}
			}
			nlh = NLMSG_PUT(skbnew, 0, 0, 0, (size - NLMSG_ALIGN(sizeof(*nlh))));
			nlh->nlmsg_len = sizeof(*pm)+skb->len;
			pm = NLMSG_DATA(nlh);
			pm->data_len = skb->len;
			memcpy(pm->saddr,eth->h_source,6);
			skb_copy_bits(skb, 0, pm->data, skb->len);
			NETLINK_CB(skbnew).dst_group = 5;
			netlink_broadcast(dnsnl, skbnew, 0, 5, GFP_ATOMIC);
			return 0;
		}
	}
	return 1;
nlmsg_failure:
	printk(KERN_CRIT "dns net link: error during NLMSG_PUT. This should "
			"not happen, please report to author.\n");
}

/*
 * Called via br_handle_frame_hook.
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock (preempt_disabled)
 */
struct sk_buff *br_handle_frame(struct net_bridge_port *p, struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	int (*rhook)(struct sk_buff *skb);

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		return NULL;

	if (unlikely(is_link_local(dest))) {
		/* Pause frames shouldn't be passed up by driver anyway */
		if (skb->protocol == htons(ETH_P_PAUSE))
			goto drop;

		/* If STP is turned off, then forward */
		if (p->br->stp_enabled == BR_NO_STP && dest[5] == 0)
			goto forward;

		if (NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
			    NULL, br_handle_local_finish))
			return NULL;	/* frame consumed by filter */
		else
			return skb;	/* continue processing */
	}

forward:
	switch (p->state) {
	case BR_STATE_FORWARDING:
		/*2007-6-25 stoneliu hijack routerlogin dns packet*/
		if(br_dns_netgear(skb) == 0)
		{
			kfree_skb(skb);
			return NULL;
		}

		rhook = rcu_dereference(br_should_route_hook);
		if (rhook != NULL) {
			if (rhook(skb))
				return skb;
			dest = eth_hdr(skb)->h_dest;
		}
		/* fall through */
	case BR_STATE_LEARNING:
		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

		NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return NULL;
}
