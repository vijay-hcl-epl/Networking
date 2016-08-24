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
    struct sockaddr_un serv_addr; 
    int len = 0;

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_UNIX, SOCK_STREAM, 0);
    memset(sendBuff, '0', sizeof(sendBuff)); 
    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, "unxskt2");

    len = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

unlink(serv_addr.sun_path);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 3); 

    printf("Waiting for client connection...\n");
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 

        close(connfd);
        sleep(1);
    }
}
