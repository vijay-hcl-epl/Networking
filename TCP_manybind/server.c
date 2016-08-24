#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h> 

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int len = 0;

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(sendBuff, '0', sizeof(sendBuff)); 
    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.17.2");
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 3); 

    printf("Waiting for client connection...\n");
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        strncpy(sendBuff, "Hello Client", 12);
        sendBuff[12] = '\0';
        printf("Accepted Client..\n");
        //snprintf(sendBuff, sizeof(sendBuff), "%s", "Hello Client");
        send(connfd, sendBuff, 12,0); 

       // close(connfd);
        sleep(1);
    }
}
