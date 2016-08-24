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

#define MAXSIZE 1024*1024

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[MAXSIZE];
    struct sockaddr_un serv_addr; 
struct timeval tv;
 unsigned long time_in_micros;

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, "unxskt2");

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       perror("connect failed");
       return 1;
    } 

     read(sockfd, recvBuff, sizeof(recvBuff));

       gettimeofday(&tv,NULL);

   time_in_micros = (1000000 * tv.tv_sec) + tv.tv_usec;
   printf("The time is %lu\n",time_in_micros);


    return 0;
}
