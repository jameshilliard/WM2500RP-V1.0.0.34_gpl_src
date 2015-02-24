/*
 * dni_netfilter_sysctl.c: DNI netfilter sysctl interface to net subsystem.
 *
 * Copyright (C) 2010 Delta Networks Inc.
 *
 */
#ifdef CONFIG_SYSCTL
#include <linux/sysctl.h>
#include <net/netfilter/nf_conntrack.h>

extern int sysctl_conntrack_refresh_support;
#if defined(CONFIG_NF_CONNTRACK_IPV6) || defined(CONFIG_NF_CONNTRACK_IPV6_MODULE)
int ipv6_ip6frag_not_check_icmp = 0;
EXPORT_SYMBOL(ipv6_ip6frag_not_check_icmp);
#endif
int sysctl_conntrack_refresh_outbound_only = 0;
int sysctl_do_flush_conntrack = 0;

EXPORT_SYMBOL(sysctl_conntrack_refresh_outbound_only);

static int proc_flush_conntrack(ctl_table * ctl,
				int write, struct file *filp,
				void __user * buffer, size_t * lenp,
				loff_t * ppos)
{
	proc_dointvec(ctl, write, filp, buffer, lenp, ppos);
	if (write && (sysctl_do_flush_conntrack & 1))
		do_flush_conntrack_table();
	return 0;
}

ctl_table dni_table[] = {
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "conntrack_refresh_support",
		.data		= &sysctl_conntrack_refresh_support,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
		.strategy	= sysctl_intvec,
	},
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "conntrack_refresh_outbound_only",
		.data		= &sysctl_conntrack_refresh_outbound_only,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
		.strategy	= sysctl_intvec,
	},
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "flush_conntrack_table",
		.data		= &sysctl_do_flush_conntrack,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_flush_conntrack,
		.strategy	= sysctl_intvec,
	},
#if defined(CONFIG_NF_CONNTRACK_IPV6) || defined(CONFIG_NF_CONNTRACK_IPV6_MODULE)
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "ipv6_ip6frag_not_check_icmp",
		.data		= &ipv6_ip6frag_not_check_icmp,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &proc_dointvec,
        },
#endif
	{0}
};

ctl_table dni_netfilter_sysctl_table[2] = {
	{
		.ctl_name	= CTL_UNNUMBERED,
		.procname	= "dni_netfilter",
		.mode		= 0555,
		.child		= dni_table
	},
	{0}
};
EXPORT_SYMBOL(dni_netfilter_sysctl_table);
#endif
