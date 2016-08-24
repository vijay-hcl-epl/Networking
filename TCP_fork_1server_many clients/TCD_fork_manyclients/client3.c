#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1024];
    struct sockaddr_in serv_addr; 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.17.2");
    serv_addr.sin_port = htons(5000);
    
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       perror("connect failed");
       return 1;
    } 
    
    strncpy(sendBuff, "Client3", 7);
    sendBuff[7] = '\0';
    
    while(1)
    {
    send(sockfd, sendBuff, 8, 0);
    
    n = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
    
    recvBuff[n] = '\0';

    printf("Client3 : Server sends - %s\n", recvBuff);
    }
    
    return 0;
}
