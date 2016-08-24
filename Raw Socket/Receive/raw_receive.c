#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <netinet/in.h> 
#include <linux/ip.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
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

memset(buffer,0,65536);


sock_r=socket(AF_PACKET,SOCK_RAW,htons(ETH_P_IP));
if(sock_r<0)
{
printf("error in socket\n");
return -1;
}

//Receive a network packet and copy in to buffer
buflen=recvfrom(sock_r,buffer,65536,0,&saddr,(socklen_t *)&saddr_len);
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

}




