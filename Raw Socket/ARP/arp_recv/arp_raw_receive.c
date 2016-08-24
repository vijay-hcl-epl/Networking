#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <netinet/in.h> 
#include <linux/ip.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

// Define a struct for ARP header
typedef struct _arp_hdr arp_hdr;
struct _arp_hdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
};

main()
{
int sock_r;
unsigned char *buffer = (unsigned char *) malloc(65536); //to receive data
struct sockaddr saddr;
struct sockaddr_in source,dest;
int buflen;
unsigned short iphdrlen;
int saddr_len = sizeof (saddr);
struct ethhdr *eth = (struct ethhdr *)(buffer);
struct iphdr *ip = (struct iphdr*)(buffer + sizeof(struct ethhdr));
arp_hdr *parphdr = (struct _arp_hdr*)(buffer + sizeof(struct ethhdr));
uint32_t srcip,dstip;

memset(buffer,0,65536);


sock_r=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
if(sock_r<0)
{
printf("error in socket\n");
return -1;
}
while(1)
{
//Receive a network packet and copy in to buffer
buflen=recvfrom(sock_r,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);
if(buflen<0)
{
printf("error in reading recvfrom function\n");
return -1;
}

memcpy(&srcip, &parphdr->sender_ip[0], 4);
//printf("Source IP is %X\n",ntohl(srcip));

memcpy(&dstip, &parphdr->target_ip[0], 4);
//printf("Dest IP is %X\n",ntohl(dstip));

memset(&source, 0, sizeof(source));
source.sin_addr.s_addr = (srcip);
memset(&dest, 0, sizeof(dest));
dest.sin_addr.s_addr = dstip;

//printf("ETH Protocol : %x\n",ntohs(eth->h_proto));
if(ntohs(eth->h_proto) == 0x806)
{
printf("\nEthernet Header\n");
printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",(eth->h_source[0]),(eth->h_source[1]),(eth->h_source[2]),(eth->h_source[3]),(eth->h_source[4]),(eth->h_source[5]));
printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
printf("\t|-Protocol : %x\n",ntohs(eth->h_proto));

printf("\n\nARP Header\n");
printf("\t|-htype : %X\n",(unsigned int)ntohs(parphdr->htype));
 
printf("\t|-ptype : %X \n",(unsigned int)ntohs(parphdr->ptype));
 
printf("\t|-hlen : %d \n",(unsigned int)(parphdr->hlen));
 
printf("\t|-plen : %d \n",(parphdr->plen));
 
printf("\t|-opcode : %d\n",ntohs(parphdr->opcode));
 
printf("\t|-sender_mac : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",parphdr->sender_mac[0],parphdr->sender_mac[1],parphdr->sender_mac[2],parphdr->sender_mac[3],parphdr->sender_mac[4],parphdr->sender_mac[5]);
 
printf("\t|-sender_ip : %s\n",inet_ntoa(source.sin_addr));
 
printf("\t|-target_mac : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",parphdr->target_mac[0],parphdr->target_mac[1],parphdr->target_mac[2],parphdr->target_mac[3],parphdr->target_mac[4],parphdr->target_mac[5]);
 
printf("\t|-target_ip : %s\n\n\n\n\n\n", inet_ntoa(dest.sin_addr));
}
}
}




