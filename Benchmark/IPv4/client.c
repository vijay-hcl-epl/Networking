#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define MAXSIZE 1024

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char ReqBuff[MAXSIZE];
    struct sockaddr_in serv_addr; 
    int nBytes = 0, ret = 0;
    struct timeval tv;
    unsigned long time_in_micros;

    memset(ReqBuff, '0',sizeof(ReqBuff));
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons(5000); 
    serv_addr.sin_addr.s_addr   = inet_addr("192.168.17.2");
    
    strcpy(ReqBuff, "Pls send me the time");
    
    nBytes = strlen(ReqBuff) + 1;

   gettimeofday(&tv,NULL);

   time_in_micros = (1000000 * tv.tv_sec) + tv.tv_usec;
   printf("The time is %lu\n",time_in_micros);
    ret = sendto(sockfd,ReqBuff,MAXSIZE,0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    
    printf("Ret value of sendto is %d\n", ret);
    if(ret < 0)
    {
        perror("sendto failed");
    }
    printf("Request Sent to server..\n");
    
    return 0;
}




