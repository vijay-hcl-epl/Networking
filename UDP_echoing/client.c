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

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char ReqBuff[1024];
    struct sockaddr_in serv_addr; 
    int nBytes = 0, ret = 0;

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
    
    while(1)
    {
    
    printf("Give input\n");
    
    fgets(ReqBuff, sizeof(ReqBuff), stdin);
    
    nBytes = strlen(ReqBuff) + 1;

    ret = sendto(sockfd,ReqBuff,nBytes,0,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(ret < 0)
    {
        perror("sendto failed");
    }
    
    nBytes = recvfrom(sockfd,ReqBuff,1024,0,NULL, NULL);
    
    ReqBuff[nBytes] = '\0';
    
    printf("Response received from server is - %s\n\n\n", ReqBuff);
    }
    
    return 0;
}




