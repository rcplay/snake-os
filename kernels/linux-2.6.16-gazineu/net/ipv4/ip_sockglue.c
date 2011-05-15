/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  INET is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		The IP to API glue.
 *		
 * Version:	$Id: ip_sockglue.c,v 1.62 2002/02/01 22:01:04 davem Exp $
 *
 * Authors:	see ip.c
 *
 * Fixes:
 *		Many		:	Split from ip.c , see ip.c for history.
 *		Martin Mares	:	TOS setting fixed.
 *		Alan Cox	:	Fixed a couple of oopses in Martin's 
 *					TOS tweaks.
 *		Mike McLagan	:	Routing by source
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/icmp.h>
#include <net/tcp_states.h>
#include <linux/udp.h>
#include <linux/igmp.h>
#include <linux/netfilter.h>
#include <linux/route.h>
#include <linux/mroute.h>
#include <net/route.h>
#include <net/xfrm.h>
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
#include <net/transp_v6.h>
#endif

#include <linux/errqueue.h>
#include <asm/uaccess.h>

#define IP_CMSG_PKTINFO		1
#define IP_CMSG_TTL		2
#define IP_CMSG_TOS		4
#define IP_CMSG_RECVOPTS	8
#define IP_CMSG_RETOPTS		16

/*
 *	SOL_IP control messages.
 */

static void ip_cmsg_recv_pktinfo(struct msghdr *msg, struct sk_buff *skb)
{
	struct in_pktinfo info;
	struct rtable *rt = (struct rtable *)skb->dst;

	info.ipi_addr.s_addr = skb->nh.iph->daddr;
	if (rt) {
		info.ipi_ifindex = rt->rt_iif;
		info.ipi_spec_dst.s_addr = rt->rt_spec_dst;
	} else {
		info.ipi_ifindex = 0;
		info.ipi_spec_dst.s_addr = 0;
	}

	put_cmsg(msg, SOL_IP, IP_PKTINFO, sizeof(info), &info);
}

static void ip_cmsg_recv_ttl(struct msghdr *msg, struct sk_buff *skb)
{
	int ttl = skb->nh.iph->ttl;
	put_cmsg(msg, SOL_IP, IP_TTL, sizeof(int), &ttl);
}

static void ip_cmsg_recv_tos(struct msghdr *msg, struct sk_buff *skb)
{
	put_cmsg(msg, SOL_IP, IP_TOS, 1, &skb->nh.iph->tos);
}

static void ip_cmsg_recv_opts(struct msghdr *msg, struct sk_buff *skb)
{
	if (IPCB(skb)->opt.optlen == 0)
		return;

	put_cmsg(msg, SOL_IP, IP_RECVOPTS, IPCB(skb)->opt.optlen, skb->nh.iph+1);
}


static void ip_cmsg_recv_retopts(struct msghdr *msg, struct sk_buff *skb)
{
	unsigned char optbuf[sizeof(struct ip_options) + 40];
	struct ip_options * opt = (struct ip_options*)optbuf;

	if (IPCB(skb)->opt.optlen == 0)
		return;

	if (ip_options_echo(opt, skb)) {
		msg->msg_flags |= MSG_CTRUNC;
		return;
	}
	ip_options_undo(opt);

	put_cmsg(msg, SOL_IP, IP_RETOPTS, opt->optlen, opt->__data);
}


void ip_cmsg_recv(struct msghdr *msg, struct sk_buff *skb)
{
	struct inet_sock *inet = inet_sk(skb->sk);
	unsigned flags = inet->cmsg_flags;

	/* Ordered by supposed usage frequency */
	if (flags & 1)
		ip_cmsg_recv_pktinfo(msg, skb);
	if ((flags>>=1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_ttl(msg, skb);
	if ((flags>>=1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_tos(msg, skb);
	if ((flags>>=1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_opts(msg, skb);
	if ((flags>>=1) == 0)
		return;

	if (flags & 1)
		ip_cmsg_recv_retopts(msg, skb);
}

int ip_cmsg_send(struct msghdr *msg, struct ipcm_cookie *ipc)
{
	int err;
	struct cmsghdr *cmsg;

	for (cmsg = CMSG_FIRSTHDR(msg); cmsg; cmsg = CMSG_NXTHDR(msg, cmsg)) {
		if (!CMSG_OK(msg, cmsg))
			return -EINVAL;
		if (cmsg->cmsg_level != SOL_IP)
			continue;
		switch (cmsg->cmsg_type) {
		case IP_RETOPTS:
			err = cmsg->cmsg_len - CMSG_ALIGN(sizeof(struct cmsghdr));
			err = ip_options_get(&ipc->opt, CMSG_DATA(cmsg), err < 40 ? err : 40);
			if (err)
				return err;
			break;
		case IP_PKTINFO:
		{
			struct in_pktinfo *info;
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(struct in_pktinfo)))
				return -EINVAL;
			info = (struct in_pktinfo *)CMSG_DATA(cmsg);
			ipc->oif = info->ipi_ifindex;
			ipc->addr = info->ipi_spec_dst.s_addr;
			break;
		}
		default:
			return -EINVAL;
		}
	}
	return 0;
}


/* Special input handler for packets caught by router alert option.
   They are selected only by protocol field, and then processed likely
   local ones; but only if someone wants them! Otherwise, router
   not running rsvpd will kill RSVP.

   It is user level problem, what it will make with them.
   I have no idea, how it will masquearde or NAT them (it is joke, joke :-)),
   but receiver should be enough clever f.e. to forward mtrace requests,
   sent to multicast group to reach destination designated router.
 */
struct ip_ra_chain *ip_ra_chain;
DEFINE_RWLOCK(ip_ra_lock);

int ip_ra_control(struct sock *sk, unsigned char on, void (*destructor)(struct sock *))
{
	struct ip_ra_chain *ra, *new_ra, **rap;

	if (sk->sk_type != SOCK_RAW || inet_sk(sk)->num == IPPROTO_RAW)
		return -EINVAL;

	new_ra = on ? kmalloc(sizeof(*new_ra), GFP_KERNEL) : NULL;

	write_lock_bh(&ip_ra_lock);
	for (rap = &ip_ra_chain; (ra=*rap) != NULL; rap = &ra->next) {
		if (ra->sk == sk) {
			if (on) {
				write_unlock_bh(&ip_ra_lock);
				kfree(new_ra);
				return -EADDRINUSE;
			}
			*rap = ra->next;
			write_unlock_bh(&ip_ra_lock);

			if (ra->destructor)
				ra->destructor(sk);
			sock_put(sk);
			kfree(ra);
			return 0;
		}
	}
	if (new_ra == NULL) {
		write_unlock_bh(&ip_ra_lock);
		return -ENOBUFS;
	}
	new_ra->sk = sk;
	new_ra->destructor = destructor;

	new_ra->next = ra;
	*rap = new_ra;
	sock_hold(sk);
	write_unlock_bh(&ip_ra_lock);

	return 0;
}

void ip_icmp_error(struct sock *sk, struct sk_buff *skb, int err, 
		   u16 port, u32 info, u8 *payload)
{
	struct inet_sock *inet = inet_sk(sk);
	struct sock_exterr_skb *serr;

	if (!inet->recverr)
		return;

	skb = skb_clone(skb, GFP_ATOMIC);
	if (!skb)
		return;

	serr = SKB_EXT_ERR(skb);  
	serr->ee.ee_errno = err;
	serr->ee.ee_origin = SO_EE_ORIGIN_ICMP;
	serr->ee.ee_type = skb->h.icmph->type; 
	serr->ee.ee_code = skb->h.icmph->code;
	serr->ee.ee_pad = 0;
	serr->ee.ee_info = info;
	serr->ee.ee_data = 0;
	serr->addr_offset = (u8*)&(((struct iphdr*)(skb->h.icmph+1))->daddr) - skb->nh.raw;
	serr->port = port;

	skb->h.raw = payload;
	if (!skb_pull(skb, payload - skb->data) ||
	    sock_queue_err_skb(sk, skb))
		kfree_skb(skb);
}

void ip_local_error(struct sock *sk, int err, u32 daddr, u16 port, u32 info)
{
	struct inet_sock *inet = inet_sk(sk);
	struct sock_exterr_skb *serr;
	struct iphdr *iph;
	struct sk_buff *skb;

	if (!inet->recverr)
		return;

	skb = alloc_skb(sizeof(struct iphdr), GFP_ATOMIC);
	if (!skb)
		return;

	iph = (struct iphdr*)skb_put(skb, sizeof(struct iphdr));
	skb->nh.iph = iph;
	iph->daddr = daddr;

	serr = SKB_EXT_ERR(skb);  
	serr->ee.ee_errno = err;
	serr->ee.ee_origin = SO_EE_ORIGIN_LOCAL;
	serr->ee.ee_type = 0; 
	serr->ee.ee_code = 0;
	serr->ee.ee_pad = 0;
	serr->ee.ee_info = info;
	serr->ee.ee_data = 0;
	serr->addr_offset = (u8*)&iph->daddr - skb->nh.raw;
	serr->port = port;

	skb->h.raw = skb->tail;
	__skb_pull(skb, skb->tail - skb->data);

	if (sock_queue_err_skb(sk, skb))
		kfree_skb(skb);
}

/* 
 *	Handle MSG_ERRQUEUE
 */
int ip_recv_error(struct sock *sk, struct msghdr *msg, int len)
{
	struct sock_exterr_skb *serr;
	struct sk_buff *skb, *skb2;
	struct sockaddr_in *sin;
	struct {
		struct sock_extended_err ee;
		struct sockaddr_in	 offender;
	} errhdr;
	int err;
	int copied;

	err = -EAGAIN;
	skb = skb_dequeue(&sk->sk_error_queue);
	if (skb == NULL)
		goto out;

	copied = skb->len;
	if (copied > len) {
		msg->msg_flags |= MSG_TRUNC;
		copied = len;
	}
	err = skb_copy_datagram_iovec(skb, 0, msg->msg_iov, copied);
	if (err)
		goto out_free_skb;

	sock_recv_timestamp(msg, sk, skb);

	serr = SKB_EXT_ERR(skb);

	sin = (struct sockaddr_in *)msg->msg_name;
	if (sin) {
		sin->sin_family = AF_INET;
		sin->sin_addr.s_addr = *(u32*)(skb->nh.raw + serr->addr_offset);
		sin->sin_port = serr->port;
		memset(&sin->sin_zero, 0, sizeof(sin->sin_zero));
	}

	memcpy(&errhdr.ee, &serr->ee, sizeof(struct sock_extended_err));
	sin = &errhdr.offender;
	sin->sin_family = AF_UNSPEC;
	if (serr->ee.ee_origin == SO_EE_ORIGIN_ICMP) {
		struct inet_sock *inet = inet_sk(sk);

		sin->sin_family = AF_INET;
		sin->sin_addr.s_addr = skb->nh.iph->saddr;
		sin->sin_port = 0;
		memset(&sin->sin_zero, 0, sizeof(sin->sin_zero));
		if (inet->cmsg_flags)
			ip_cmsg_recv(msg, skb);
	}

	put_cmsg(msg, SOL_IP, IP_RECVERR, sizeof(errhdr), &errhdr);

	/* Now we could try to dump offended packet options */

	msg->msg_flags |= MSG_ERRQUEUE;
	err = copied;

	/* Reset and regenerate socket error */
	spin_lock_bh(&sk->sk_error_queue.lock);
	sk->sk_err = 0;
	if ((skb2 = skb_peek(&sk->sk_error_queue)) != NULL) {
		sk->sk_err = SKB_EXT_ERR(skb2)->ee.ee_errno;
		spin_unlock_bh(&sk->sk_error_queue.lock);
		sk->sk_error_report(sk);
	} else
		spin_unlock_bh(&sk->sk_error_queue.lock);

out_free_skb:	
	kfree_skb(skb);
out:
	return err;
}


/*
 *	Socket option code for IP. This is the end of the line after any TCP,UDP etc options on
 *	an IP socket.
 */

int ip_setsockopt(struct sock *sk, int level, int optname, char __user *optval, int optlen)
{
	struct inet_sock *inet = inet_sk(sk);
	int val=0,err;

	if (level != SOL_IP)
		return -ENOPROTOOPT;

	if (((1<<optname) & ((1<<IP_PKTINFO) | (1<<IP_RECVTTL) | 
			    (1<<IP_RECVOPTS) | (1<<IP_RECVTOS) | 
			    (1<<IP_RETOPTS) | (1<<IP_TOS) | 
			    (1<<IP_TTL) | (1<<IP_HDRINCL) | 
			    (1<<IP_MTU_DISCOVER) | (1<<IP_RECVERR) | 
			    (1<<IP_ROUTER_ALERT) | (1<<IP_FREEBIND))) || 
				optname == IP_MULTICAST_TTL || 
				optname == IP_MULTICAST_LOOP) { 
		if (optlen >= sizeof(int)) {
			if (get_user(val, (int __user *) optval))
				return -EFAULT;
		} else if (optlen >= sizeof(char)) {
			unsigned char ucval;

			if (get_user(ucval, (unsigned char __user *) optval))
				return -EFAULT;
			val = (int) ucval;
		}
	}

	/* If optlen==0, it is equivalent to val == 0 */

#ifdef CONFIG_IP_MROUTE
	if (optname >= MRT_BASE && optname <= (MRT_BASE + 10))
		return ip_mroute_setsockopt(sk,optname,optval,optlen);
#endif

	err = 0;
	lock_sock(sk);

	switch (optname) {
		case IP_OPTIONS:
		{
			struct ip_options * opt = NULL;
			if (optlen > 40 || optlen < 0)
				goto e_inval;
			err = ip_options_get_from_user(&opt, optval, optlen);
			if (err)
				break;
			if (inet->is_icsk) {
				struct inet_connection_sock *icsk = inet_csk(sk);
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
				if (sk->sk_family == PF_INET ||
				    (!((1 << sk->sk_state) &
				       (TCPF_LISTEN | TCPF_CLOSE)) &&
				     inet->daddr != LOOPBACK4_IPV6)) {
#endif
					if (inet->opt)
						icsk->icsk_ext_hdr_len -= inet->opt->optlen;
					if (opt)
						icsk->icsk_ext_hdr_len += opt->optlen;
					icsk->icsk_sync_mss(sk, icsk->icsk_pmtu_cookie);
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
				}
#endif
			}
			opt = xchg(&inet->opt, opt);
			kfree(opt);
			break;
		}
		case IP_PKTINFO:
			if (val)
				inet->cmsg_flags |= IP_CMSG_PKTINFO;
			else
				inet->cmsg_flags &= ~IP_CMSG_PKTINFO;
			break;
		case IP_RECVTTL:
			if (val)
				inet->cmsg_flags |=  IP_CMSG_TTL;
			else
				inet->cmsg_flags &= ~IP_CMSG_TTL;
			break;
		case IP_RECVTOS:
			if (val)
				inet->cmsg_flags |=  IP_CMSG_TOS;
			else
				inet->cmsg_flags &= ~IP_CMSG_TOS;
			break;
		case IP_RECVOPTS:
			if (val)
				inet->cmsg_flags |=  IP_CMSG_RECVOPTS;
			else
				inet->cmsg_flags &= ~IP_CMSG_RECVOPTS;
			break;
		case IP_RETOPTS:
			if (val)
				inet->cmsg_flags |= IP_CMSG_RETOPTS;
			else
				inet->cmsg_flags &= ~IP_CMSG_RETOPTS;
			break;
		case IP_TOS:	/* This sets both TOS and Precedence */
			if (sk->sk_type == SOCK_STREAM) {
				val &= ~3;
				val |= inet->tos & 3;
			}
			if (IPTOS_PREC(val) >= IPTOS_PREC_CRITIC_ECP && 
			    !capable(CAP_NET_ADMIN)) {
				err = -EPERM;
				break;
			}
			if (inet->tos != val) {
				inet->tos = val;
				sk->sk_priority = rt_tos2priority(val);
				sk_dst_reset(sk); 
			}
			break;
		case IP_TTL:
			if (optlen<1)
				goto e_inval;
			if (val != -1 && (val < 1 || val>255))
				goto e_inval;
			inet->uc_ttl = val;
			break;
		case IP_HDRINCL:
			if (sk->sk_type != SOCK_RAW) {
				err = -ENOPROTOOPT;
				break;
			}
			inet->hdrincl = val ? 1 : 0;
			break;
		case IP_MTU_DISCOVER:
			if (val<0 || val>2)
				goto e_inval;
			inet->pmtudisc = val;
			break;
		case IP_RECVERR:
			inet->recverr = !!val;
			if (!val)
				skb_queue_purge(&sk->sk_error_queue);
			break;
		case IP_MULTICAST_TTL:
			if (sk->sk_type == SOCK_STREAM)
				goto e_inval;
			if (optlen<1)
				goto e_inval;
			if (val==-1)
				val = 1;
			if (val < 0 || val > 255)
				goto e_inval;
			inet->mc_ttl = val;
	                break;
		case IP_MULTICAST_LOOP: 
			if (optlen<1)
				goto e_inval;
			inet->mc_loop = !!val;
	                break;
		case IP_MULTICAST_IF: 
		{
			struct ip_mreqn mreq;
			struct net_device *dev = NULL;

			if (sk->sk_type == SOCK_STREAM)
				goto e_inval;
			/*
			 *	Check the arguments are allowable
			 */

			err = -EFAULT;
			if (optlen >= sizeof(struct ip_mreqn)) {
				if (copy_from_user(&mreq,optval,sizeof(mreq)))
					break;
			} else {
				memset(&mreq, 0, sizeof(mreq));
				if (optlen >= sizeof(struct in_addr) &&
				    copy_from_user(&mreq.imr_address,optval,sizeof(struct in_addr)))
					break;
			}

			if (!mreq.imr_ifindex) {
				if (mreq.imr_address.s_addr == INADDR_ANY) {
					inet->mc_index = 0;
					inet->mc_addr  = 0;
					err = 0;
					break;
				}
				dev = ip_dev_find(mreq.imr_address.s_addr);
				if (dev) {
					mreq.imr_ifindex = dev->ifindex;
					dev_put(dev);
				}
			} else
				dev = __dev_get_by_index(mreq.imr_ifindex);


			err = -EADDRNOTAVAIL;
			if (!dev)
				break;

			err = -EINVAL;
			if (sk->sk_bound_dev_if &&
			    mreq.imr_ifindex != sk->sk_bound_dev_if)
				break;

			inet->mc_index = mreq.imr_ifindex;
			inet->mc_addr  = mreq.imr_address.s_addr;
			err = 0;
			break;
		}

		case IP_ADD_MEMBERSHIP:
		case IP_DROP_MEMBERSHIP: 
		{
			struct ip_mreqn mreq;

			if (optlen < sizeof(struct ip_mreq))
				goto e_inval;
			err = -EFAULT;
			if (optlen >= sizeof(struct ip_mreqn)) {
				if(copy_from_user(&mreq,optval,sizeof(mreq)))
					break;
			} else {
				memset(&mreq, 0, sizeof(mreq));
				if (copy_from_user(&mreq,optval,sizeof(struct ip_mreq)))
					break; 
			}

			if (optname == IP_ADD_MEMBERSHIP)
				err = ip_mc_join_group(sk, &mreq);
			else
				err = ip_mc_leave_group(sk, &mreq);
			break;
		}
		case IP_MSFILTER:
		{
			extern int sysctl_igmp_max_msf;
			struct ip_msfilter *msf;

			if (optlen < IP_MSFILTER_SIZE(0))
				goto e_inval;
			if (optlen > sysctl_optmem_max) {
				err = -ENOBUFS;
				break;
			}
			msf = kmalloc(optlen, GFP_KERNEL);
			if (msf == 0) {
				err = -ENOBUFS;
				break;
			}
			err = -EFAULT;
			if (copy_from_user(msf, optval, optlen)) {
				kfree(msf);
				break;
			}
			/* numsrc >= (1G-4) overflow in 32 bits */
			if (msf->imsf_numsrc >= 0x3ffffffcU ||
			    msf->imsf_numsrc > sysctl_igmp_max_msf) {
				kfree(msf);
				err = -ENOBUFS;
				break;
			}
			if (IP_MSFILTER_SIZE(msf->imsf_numsrc) > optlen) {
				kfree(msf);
				err = -EINVAL;
				break;
			}
			err = ip_mc_msfilter(sk, msf, 0);
			kfree(msf);
			break;
		}
		case IP_BLOCK_SOURCE:
		case IP_UNBLOCK_SOURCE:
		case IP_ADD_SOURCE_MEMBERSHIP:
		case IP_DROP_SOURCE_MEMBERSHIP:
		{
			struct ip_mreq_source mreqs;
			int omode, add;

			if (optlen != sizeof(struct ip_mreq_source))
				goto e_inval;
			if (copy_from_user(&mreqs, optval, sizeof(mreqs))) {
				err = -EFAULT;
				break;
			}
			if (optname == IP_BLOCK_SOURCE) {
				omode = MCAST_EXCLUDE;
				add = 1;
			} else if (optname == IP_UNBLOCK_SOURCE) {
				omode = MCAST_EXCLUDE;
				add = 0;
			} else if (optname == IP_ADD_SOURCE_MEMBERSHIP) {
				struct ip_mreqn mreq;

				mreq.imr_multiaddr.s_addr = mreqs.imr_multiaddr;
				mreq.imr_address.s_addr = mreqs.imr_interface;
				mreq.imr_ifindex = 0;
				err = ip_mc_join_group(sk, &mreq);
				if (err && err != -EADDRINUSE)
					break;
				omode = MCAST_INCLUDE;
				add = 1;
			} else /* IP_DROP_SOURCE_MEMBERSHIP */ {
				omode = MCAST_INCLUDE;
				add = 0;
			}
			err = ip_mc_source(add, omode, sk, &mreqs, 0);
			break;
		}
		case MCAST_JOIN_GROUP:
		case MCAST_LEAVE_GROUP: 
		{
			struct group_req greq;
			struct sockaddr_in *psin;
			struct ip_mreqn mreq;

			if (optlen < sizeof(struct group_req))
				goto e_inval;
			err = -EFAULT;
			if(copy_from_user(&greq, optval, sizeof(greq)))
				break;
			psin = (struct sockaddr_in *)&greq.gr_group;
			if (psin->sin_family != AF_INET)
				goto e_inval;
			memset(&mreq, 0, sizeof(mreq));
			mreq.imr_multiaddr = psin->sin_addr;
			mreq.imr_ifindex = greq.gr_interface;

			if (optname == MCAST_JOIN_GROUP)
				err = ip_mc_join_group(sk, &mreq);
			else
				err = ip_mc_leave_group(sk, &mreq);
			break;
		}
		case MCAST_JOIN_SOURCE_GROUP:
		case MCAST_LEAVE_SOURCE_GROUP:
		case MCAST_BLOCK_SOURCE:
		case MCAST_UNBLOCK_SOURCE:
		{
			struct group_source_req greqs;
			struct ip_mreq_source mreqs;
			struct sockaddr_in *psin;
			int omode, add;

			if (optlen != sizeof(struct group_source_req))
				goto e_inval;
			if (copy_from_user(&greqs, optval, sizeof(greqs))) {
				err = -EFAULT;
				break;
			}
			if (greqs.gsr_group.ss_family != AF_INET ||
			    greqs.gsr_source.ss_family != AF_INET) {
				err = -EADDRNOTAVAIL;
				break;
			}
			psin = (struct sockaddr_in *)&greqs.gsr_group;
			mreqs.imr_multiaddr = psin->sin_addr.s_addr;
			psin = (struct sockaddr_in *)&greqs.gsr_source;
			mreqs.imr_sourceaddr = psin->sin_addr.s_addr;
			mreqs.imr_interface = 0; /* use index for mc_source */

			if (optname == MCAST_BLOCK_SOURCE) {
				omode = MCAST_EXCLUDE;
				add = 1;
			} else if (optname == MCAST_UNBLOCK_SOURCE) {
				omode = MCAST_EXCLUDE;
				add = 0;
			} else if (optname == MCAST_JOIN_SOURCE_GROUP) {
				struct ip_mreqn mreq;

				psin = (struct sockaddr_in *)&greqs.gsr_group;
				mreq.imr_multiaddr = psin->sin_addr;
				mreq.imr_address.s_addr = 0;
				mreq.imr_ifindex = greqs.gsr_interface;
				err = ip_mc_join_group(sk, &mreq);
				if (err && err != -EADDRINUSE)
					break;
				greqs.gsr_interface = mreq.imr_ifindex;
				omode = MCAST_INCLUDE;
				add = 1;
			} else /* MCAST_LEAVE_SOURCE_GROUP */ {
				omode = MCAST_INCLUDE;
				add = 0;
			}
			err = ip_mc_source(add, omode, sk, &mreqs,
				greqs.gsr_interface);
			break;
		}
		case MCAST_MSFILTER:
		{
			extern int sysctl_igmp_max_msf;
			struct sockaddr_in *psin;
			struct ip_msfilter *msf = NULL;
			struct group_filter *gsf = NULL;
			int msize, i, ifindex;

			if (optlen < GROUP_FILTER_SIZE(0))
				goto e_inval;
			if (optlen > sysctl_optmem_max) {
				err = -ENOBUFS;
				break;
			}
			gsf = kmalloc(optlen,GFP_KERNEL);
			if (gsf == 0) {
				err = -ENOBUFS;
				break;
			}
			err = -EFAULT;
			if (copy_from_user(gsf, optval, optlen)) {
				goto mc_msf_out;
			}
			/* numsrc >= (4G-140)/128 overflow in 32 bits */
			if (gsf->gf_numsrc >= 0x1ffffff ||
			    gsf->gf_numsrc > sysctl_igmp_max_msf) {
				err = -ENOBUFS;
				goto mc_msf_out;
			}
			if (GROUP_FILTER_SIZE(gsf->gf_numsrc) > optlen) {
				err = -EINVAL;
				goto mc_msf_out;
			}
			msize = IP_MSFILTER_SIZE(gsf->gf_numsrc);
			msf = kmalloc(msize,GFP_KERNEL);
			if (msf == 0) {
				err = -ENOBUFS;
				goto mc_msf_out;
			}
			ifindex = gsf->gf_interface;
			psin = (struct sockaddr_in *)&gsf->gf_group;
			if (psin->sin_family != AF_INET) {
				err = -EADDRNOTAVAIL;
				goto mc_msf_out;
			}
			msf->imsf_multiaddr = psin->sin_addr.s_addr;
			msf->imsf_interface = 0;
			msf->imsf_fmode = gsf->gf_fmode;
			msf->imsf_numsrc = gsf->gf_numsrc;
			err = -EADDRNOTAVAIL;
			for (i=0; i<gsf->gf_numsrc; ++i) {
				psin = (struct sockaddr_in *)&gsf->gf_slist[i];

				if (psin->sin_family != AF_INET)
					goto mc_msf_out;
				msf->imsf_slist[i] = psin->sin_addr.s_addr;
			}
			kfree(gsf);
			gsf = NULL;

			err = ip_mc_msfilter(sk, msf, ifindex);
mc_msf_out:
			kfree(msf);
			kfree(gsf);
			break;
		}
		case IP_ROUTER_ALERT:	
			err = ip_ra_control(sk, val ? 1 : 0, NULL);
			break;

		case IP_FREEBIND:
			if (optlen<1)
				goto e_inval;
			inet->freebind = !!val; 
	                break;			
 
		case IP_IPSEC_POLICY:
		case IP_XFRM_POLICY:
			err = -EPERM;
			if (!capable(CAP_NET_ADMIN))
				break;
			err = xfrm_user_policy(sk, optname, optval, optlen);
			break;

		default:
#ifdef CONFIG_NETFILTER
			err = nf_setsockopt(sk, PF_INET, optname, optval, 
					    optlen);
#else
			err = -ENOPROTOOPT;
#endif
			break;
	}
	release_sock(sk);
	return err;

e_inval:
	release_sock(sk);
	return -EINVAL;
}

/*
 *	Get the options. Note for future reference. The GET of IP options gets the
 *	_received_ ones. The set sets the _sent_ ones.
 */

int ip_getsockopt(struct sock *sk, int level, int optname, char __user *optval, int __user *optlen)
{
	struct inet_sock *inet = inet_sk(sk);
	int val;
	int len;
	
	if(level!=SOL_IP)
		return -EOPNOTSUPP;

#ifdef CONFIG_IP_MROUTE
	if(optname>=MRT_BASE && optname <=MRT_BASE+10)
	{
		return ip_mroute_getsockopt(sk,optname,optval,optlen);
	}
#endif

	if(get_user(len,optlen))
		return -EFAULT;
	if(len < 0)
		return -EINVAL;
		
	lock_sock(sk);

	switch(optname)	{
		case IP_OPTIONS:
			{
				unsigned char optbuf[sizeof(struct ip_options)+40];
				struct ip_options * opt = (struct ip_options*)optbuf;
				opt->optlen = 0;
				if (inet->opt)
					memcpy(optbuf, inet->opt,
					       sizeof(struct ip_options)+
					       inet->opt->optlen);
				release_sock(sk);

				if (opt->optlen == 0) 
					return put_user(0, optlen);

				ip_options_undo(opt);

				len = min_t(unsigned int, len, opt->optlen);
				if(put_user(len, optlen))
					return -EFAULT;
				if(copy_to_user(optval, opt->__data, len))
					return -EFAULT;
				return 0;
			}
		case IP_PKTINFO:
			val = (inet->cmsg_flags & IP_CMSG_PKTINFO) != 0;
			break;
		case IP_RECVTTL:
			val = (inet->cmsg_flags & IP_CMSG_TTL) != 0;
			break;
		case IP_RECVTOS:
			val = (inet->cmsg_flags & IP_CMSG_TOS) != 0;
			break;
		case IP_RECVOPTS:
			val = (inet->cmsg_flags & IP_CMSG_RECVOPTS) != 0;
			break;
		case IP_RETOPTS:
			val = (inet->cmsg_flags & IP_CMSG_RETOPTS) != 0;
			break;
		case IP_TOS:
			val = inet->tos;
			break;
		case IP_TTL:
			val = (inet->uc_ttl == -1 ?
			       sysctl_ip_default_ttl :
			       inet->uc_ttl);
			break;
		case IP_HDRINCL:
			val = inet->hdrincl;
			break;
		case IP_MTU_DISCOVER:
			val = inet->pmtudisc;
			break;
		case IP_MTU:
		{
			struct dst_entry *dst;
			val = 0;
			dst = sk_dst_get(sk);
			if (dst) {
				val = dst_mtu(dst);
				dst_release(dst);
			}
			if (!val) {
				release_sock(sk);
				return -ENOTCONN;
			}
			break;
		}
		case IP_RECVERR:
			val = inet->recverr;
			break;
		case IP_MULTICAST_TTL:
			val = inet->mc_ttl;
			break;
		case IP_MULTICAST_LOOP:
			val = inet->mc_loop;
			break;
		case IP_MULTICAST_IF:
		{
			struct in_addr addr;
			len = min_t(unsigned int, len, sizeof(struct in_addr));
			addr.s_addr = inet->mc_addr;
			release_sock(sk);

  			if(put_user(len, optlen))
  				return -EFAULT;
			if(copy_to_user(optval, &addr, len))
				return -EFAULT;
			return 0;
		}
		case IP_MSFILTER:
		{
			struct ip_msfilter msf;
			int err;

			if (len < IP_MSFILTER_SIZE(0)) {
				release_sock(sk);
				return -EINVAL;
			}
			if (copy_from_user(&msf, optval, IP_MSFILTER_SIZE(0))) {
				release_sock(sk);
				return -EFAULT;
			}
			err = ip_mc_msfget(sk, &msf,
				(struct ip_msfilter __user *)optval, optlen);
			release_sock(sk);
			return err;
		}
		case MCAST_MSFILTER:
		{
			struct group_filter gsf;
			int err;

			if (len < GROUP_FILTER_SIZE(0)) {
				release_sock(sk);
				return -EINVAL;
			}
			if (copy_from_user(&gsf, optval, GROUP_FILTER_SIZE(0))) {
				release_sock(sk);
				return -EFAULT;
			}
			err = ip_mc_gsfget(sk, &gsf,
				(struct group_filter __user *)optval, optlen);
			release_sock(sk);
			return err;
		}
		case IP_PKTOPTIONS:		
		{
			struct msghdr msg;

			release_sock(sk);

			if (sk->sk_type != SOCK_STREAM)
				return -ENOPROTOOPT;

			msg.msg_control = optval;
			msg.msg_controllen = len;
			msg.msg_flags = 0;

			if (inet->cmsg_flags & IP_CMSG_PKTINFO) {
				struct in_pktinfo info;

				info.ipi_addr.s_addr = inet->rcv_saddr;
				info.ipi_spec_dst.s_addr = inet->rcv_saddr;
				info.ipi_ifindex = inet->mc_index;
				put_cmsg(&msg, SOL_IP, IP_PKTINFO, sizeof(info), &info);
			}
			if (inet->cmsg_flags & IP_CMSG_TTL) {
				int hlim = inet->mc_ttl;
				put_cmsg(&msg, SOL_IP, IP_TTL, sizeof(hlim), &hlim);
			}
			len -= msg.msg_controllen;
			return put_user(len, optlen);
		}
		case IP_FREEBIND: 
			val = inet->freebind; 
			break; 
		default:
#ifdef CONFIG_NETFILTER
			val = nf_getsockopt(sk, PF_INET, optname, optval, 
					    &len);
			release_sock(sk);
			if (val >= 0)
				val = put_user(len, optlen);
			return val;
#else
			release_sock(sk);
			return -ENOPROTOOPT;
#endif
	}
	release_sock(sk);
	
	if (len < sizeof(int) && len > 0 && val>=0 && val<255) {
		unsigned char ucval = (unsigned char)val;
		len = 1;
		if(put_user(len, optlen))
			return -EFAULT;
		if(copy_to_user(optval,&ucval,1))
			return -EFAULT;
	} else {
		len = min_t(unsigned int, sizeof(int), len);
		if(put_user(len, optlen))
			return -EFAULT;
		if(copy_to_user(optval,&val,len))
			return -EFAULT;
	}
	return 0;
}

EXPORT_SYMBOL(ip_cmsg_recv);

EXPORT_SYMBOL(ip_getsockopt);
EXPORT_SYMBOL(ip_setsockopt);
