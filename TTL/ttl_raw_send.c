#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <netinet/in.h> 
#include <netinet/udp.h>
#include <linux/ip.h>
#include <linux/if.h>
#include <linux/sockios.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

unsigned short checksum(unsigned short* buff, int _16bitword)
{
unsigned long sum;
for(sum=0;_16bitword>0;_16bitword--)
sum+=htons(*(buff)++);
sum = ((sum >> 16) + (sum & 0xFFFF));
sum += (sum>>16);
return (unsigned short)(~sum);
}

main()
{
int sock_raw;
struct ifreq ifreq_i;
struct ifreq ifreq_c;
struct ifreq ifreq_ip;
char sendbuff[65535];
int total_len = 0;
int send_len;
struct ethhdr *eth = (struct ethhdr *)(sendbuff);
struct iphdr *iph = (struct iphdr*)(sendbuff + sizeof(struct ethhdr));
struct udphdr *uh = (struct udphdr *)(sendbuff + sizeof(struct iphdr) + sizeof(struct ethhdr));
struct sockaddr_ll sadr_ll;

sock_raw=socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
if(sock_raw == -1)
printf("error in socket");

memset(&ifreq_i,0,sizeof(ifreq_i));
strncpy(ifreq_i.ifr_name,"eth2",IFNAMSIZ-1); //giving name of Interface

if((ioctl(sock_raw,SIOCGIFINDEX,&ifreq_i))<0)
printf("error in index ioctl reading");//getting Index Name
 
printf("index=%d\n",ifreq_i.ifr_ifindex);

memset(&ifreq_c,0,sizeof(ifreq_c));
strncpy(ifreq_c.ifr_name,"eth2",IFNAMSIZ-1);//giving name of Interface

if((ioctl(sock_raw,SIOCGIFHWADDR,&ifreq_c))<0) //getting MAC Address
printf("error in SIOCGIFHWADDR ioctl reading");

memset(&ifreq_ip,0,sizeof(ifreq_ip));
strncpy(ifreq_ip.ifr_name,"eth2",IFNAMSIZ-1);//giving name of Interface
if(ioctl(sock_raw,SIOCGIFADDR,&ifreq_ip)<0) //getting IP Address
{
printf("error in SIOCGIFADDR \n");
perror("SIOCGIFADDR");
}

//ETHERNET HEADER
eth->h_source[0] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
eth->h_source[1] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
eth->h_source[2] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
eth->h_source[3] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
eth->h_source[4] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
eth->h_source[5] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);

/* filling destination mac. DESTMAC0 to DESTMAC5 are macro having octets of mac address. */
eth->h_dest[0] = 0x00;//DESTMAC0;
eth->h_dest[1] = 0xe0;//DESTMAC1;
eth->h_dest[2] = 0x4c;//DESTMAC2;
eth->h_dest[3] = 0x19;//DESTMAC3;
eth->h_dest[4] = 0x37;//DESTMAC4;
eth->h_dest[5] = 0x46;//DESTMAC5;
 
eth->h_proto = htons(ETH_P_IP); //means next header will be IP header

/* end of ethernet header */
total_len+=sizeof(struct ethhdr);

// IP HEADER
iph->ihl = 5;
iph->version = 4;
iph->tos = 16;
iph->id = htons(10201);
iph->ttl = 2;//64;
iph->protocol = 17;
iph->saddr = inet_addr(inet_ntoa((((struct sockaddr_in *)&(ifreq_ip.ifr_addr))->sin_addr)));
iph->daddr = inet_addr("216.58.196.196"); // put destination IP address
 
total_len += sizeof(struct iphdr);


//UDP HEADER
uh->source = htons(23451);
uh->dest = htons(23452);
uh->check = 0;
 
total_len+= sizeof(struct udphdr);

// DATA
sendbuff[total_len++] = 0xAA;
sendbuff[total_len++] = 0x11;
sendbuff[total_len++] = 0xCC;
sendbuff[total_len++] = 0x22;
sendbuff[total_len++] = 0xEE;

uh->len = htons((total_len - sizeof(struct iphdr) - sizeof(struct ethhdr)));
//UDP length field
iph->tot_len = htons(total_len - sizeof(struct ethhdr));
//IP length field

iph->check = checksum((unsigned short*)(sendbuff + sizeof(struct ethhdr)), (sizeof(struct iphdr)/2));


sadr_ll.sll_ifindex = ifreq_i.ifr_ifindex; // index of interface
sadr_ll.sll_halen = ETH_ALEN; // length of destination mac address
sadr_ll.sll_addr[0] = 0x00;//DESTMAC0;
sadr_ll.sll_addr[1] = 0xe0;//DESTMAC1;
sadr_ll.sll_addr[2] = 0x4c;//DESTMAC2;
sadr_ll.sll_addr[3] = 0x19;//DESTMAC3;
sadr_ll.sll_addr[4] = 0x37;//DESTMAC4;
sadr_ll.sll_addr[5] = 0x46;//DESTMAC5;

send_len = sendto(sock_raw,sendbuff,64,0,(const struct sockaddr*)&sadr_ll,sizeof(struct sockaddr_ll));
if(send_len<0)
{
printf("error in sending....sendlen=%d....errno=%d\n",send_len,errno);
return -1;
} else
{ 
printf("Send success..\n");
}

}
