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
    int len = 0,pid = 0;

    char sendBuff[1025];
    char recvBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(sendBuff, '0', sizeof(sendBuff)); 
    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
   
    printf("The val of serv_addr.sin_addr.s_addr is %s\n", inet_ntoa(serv_addr.sin_addr));

    listen(listenfd, 3); 

    printf("Waiting for client connection...\n");
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        pid = fork();
		
      if (pid < 0) {
         perror("ERROR on fork");
         exit(1);
      }
      
      if (pid == 0) {
         /* This is the child process */
         close(listenfd);
      }
      else {
         printf("I am parent. Child PID is %d\n",pid);
         close(connfd);
         continue;
      }

        while(1)
{
        recv(connfd, recvBuff, 8, 0); 
        snprintf(sendBuff, 14, "Hello %s", recvBuff);
        printf("A server serving %s\n", recvBuff);
        send(connfd, sendBuff, 14,0); 

       // close(connfd);
        sleep(1);
}
    }
}
