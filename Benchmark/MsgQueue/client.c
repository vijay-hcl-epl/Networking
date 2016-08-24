#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>

#define MAX_SIZE (1024 * 1024)

int main(int argc, char **argv)
{
    mqd_t mq;
    char buffer[MAX_SIZE];
    struct timeval tv;
    unsigned long time_in_micros;
    int ret = 0;


    /* open the mail queue */
    mq = mq_open("/msgq", O_WRONLY);

    memset(buffer, 0, MAX_SIZE);
   gettimeofday(&tv,NULL);

   time_in_micros = (1000000 * tv.tv_sec) + tv.tv_usec;
   printf("The time is %lu\n",time_in_micros);
   mq_send(mq, buffer, MAX_SIZE, 0);

    return 0;
}
