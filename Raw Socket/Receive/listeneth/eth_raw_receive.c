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

main()
{
int sock_r;
unsigned char *buffer = (unsigned char *) malloc(65536); //to receive data
struct ifreq ifreq_i;
struct ifreq ifreq_c;
struct ifreq ifreq_ip;
struct sockaddr saddr;
struct sockaddr_in source,dest;
int buflen;
int i = 0;
unsigned short iphdrlen;
int saddr_len = sizeof (saddr);
struct ethhdr *eth = (struct ethhdr *)(buffer);
struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
struct udphdr *udp= NULL;
unsigned char * data = (buffer + iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));
int remaining_data = 0;


memset(buffer,0,65536);


sock_r=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP));
if(sock_r<0)
{
printf("error in socket\n");
return -1;
}

memset(&ifreq_i,0,sizeof(ifreq_i));
strncpy(ifreq_i.ifr_name,"eth0",IFNAMSIZ-1); //giving name of Interface

if((ioctl(sock_r,SIOCGIFINDEX,&ifreq_i))<0)
printf("error in index ioctl reading");//getting Index Name
 
printf("index=%d\n",ifreq_i.ifr_ifindex);

memset(&ifreq_c,0,sizeof(ifreq_c));
strncpy(ifreq_c.ifr_name,"eth0",IFNAMSIZ-1);//giving name of Interface

if((ioctl(sock_r,SIOCGIFHWADDR,&ifreq_c))<0) //getting MAC Address
printf("error in SIOCGIFHWADDR ioctl reading");

memset(&ifreq_ip,0,sizeof(ifreq_ip));
strncpy(ifreq_ip.ifr_name,"eth0",IFNAMSIZ-1);//giving name of Interface
if(ioctl(sock_r,SIOCGIFADDR,&ifreq_ip)<0) //getting IP Address
{
printf("error in SIOCGIFADDR \n");
perror("SIOCGIFADDR");
}

//Receive a network packet and copy in to buffer
buflen=recvfrom(sock_r,buffer,65535,0,&saddr,(socklen_t *)&saddr_len);
if(buflen<0)
{
printf("error in reading recvfrom function\n");
return -1;
}

memset(&source, 0, sizeof(source));
source.sin_addr.s_addr = ip->saddr;
memset(&dest, 0, sizeof(dest));
dest.sin_addr.s_addr = ip->daddr;

printf("\nEthernet Header\n");
printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
printf("\t|-Protocol : %x\n",eth->h_proto);

printf("\n\nIP Header\n");
printf("\t|-Version : %d\n",(unsigned int)ip->version);
 
printf("\t|-Internet Header Length : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
 
printf("\t|-Type Of Service : %d\n",(unsigned int)ip->tos);
 
printf("\t|-Total Length : %d Bytes\n",ntohs(ip->tot_len));
 
printf("\t|-Identification : %d\n",ntohs(ip->id));
 
printf("\t|-Time To Live : %d\n",(unsigned int)ip->ttl);
 
printf("\t|-Protocol : %d\n",(unsigned int)ip->protocol);
 
printf("\t|-Header Checksum : %d\n",ntohs(ip->check));
 
printf("\t|-Source IP : %s\n", inet_ntoa(source.sin_addr));
 
printf("\t|-Destination IP : %s\n",inet_ntoa(dest.sin_addr));


/* getting actual size of IP header*/
iphdrlen = ip->ihl*4;

udp=(struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

printf("\n\nUDP Header\n");
printf("\t|-Source Port : %d\n" , ntohs(udp->source));
printf("\t|-Destination Port : %d\n" , ntohs(udp->dest));
printf("\t|-UDP Length : %d\n" , ntohs(udp->len));
printf("\t|-UDP Checksum : %d\n" , ntohs(udp->check));

remaining_data = buflen - (iphdrlen + sizeof(struct ethhdr) + sizeof(struct udphdr));

printf("\n\nDATA\n");
for(i=0;i<remaining_data;i++)
{
if(i!=0 && i%16==0)
printf("\n");
printf(" %.2X ",data[i]);
}
}




