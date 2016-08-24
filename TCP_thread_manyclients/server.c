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
#include<pthread.h>

    char sendBuff1[1025];
    char recvBuff1[1025];

    char sendBuff3[1025];
    char sendBuff2[1025];
    char recvBuff2[1025];
    char recvBuff3[1025];


void * fn1(void *arg)
{
int *ptr = (int *)arg;
int connfd = *ptr;

while(1)
{
        printf("In thread 1..\n");
        recv(connfd, recvBuff1, 8, 0);
        snprintf(sendBuff1, 14, "Hello %s", recvBuff1);
        printf("Thread 1 serving %s\n", recvBuff1);
        send(connfd, sendBuff1, 14,0);

       // close(connfd);
        sleep(1);
}
while(1);
return NULL;
 
}


void * fn2(void *arg)
{
int *ptr = (int *)arg;
int connfd = *ptr;

while(1)
{
        printf("In thread 2..\n");
        recv(connfd, recvBuff2, 8, 0);
        snprintf(sendBuff2, 14, "Hello %s", recvBuff2);
        printf("Thread 2 serving %s\n", recvBuff2);
        send(connfd, sendBuff2, 14,0);

       // close(connfd);
        sleep(1);
}
while(1);
return NULL; 
}


void * fn3(void *arg)
{
int *ptr = (int *)arg;
int connfd = *ptr;

while(1)
{
        printf("In thread 3..\n");
        recv(connfd, recvBuff3, 8, 0);
        snprintf(sendBuff3, 14, "Hello %s", recvBuff3);
        printf("Thread 3 serving %s\n", recvBuff3);
        send(connfd, sendBuff3, 14,0);

       // close(connfd);
        sleep(1);
}
while(1);
return NULL; 
}
int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int len = 0,pid = 0;
    pthread_t th[3];
    void * (*ptr[3])(void *);
    int i = 0;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
   
    printf("The val of serv_addr.sin_addr.s_addr is %s\n", inet_ntoa(serv_addr.sin_addr));

    listen(listenfd, 3); 

    ptr[0] = fn1;
    ptr[1] = fn2;
    ptr[2] = fn3;


    printf("Waiting for client connection...\n");
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
        
        pthread_create(&th[i],NULL,ptr[i],(void *)&connfd);
        i++;
    }

        pthread_join(th[0],NULL);
        pthread_join(th[1],NULL);
        pthread_join(th[2],NULL);
while(1);
}
