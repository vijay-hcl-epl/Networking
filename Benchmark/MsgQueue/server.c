#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

#define MAX_SIZE (1024 * 1024)
#define NAME "/msgq1"

int main(int argc, char **argv)
{
    mqd_t mq;
    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
        ssize_t bytes_read;
    struct timeval tv;
    unsigned long time_in_micros;
    int ret = 0;

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mq_unlink(NAME);

    /* create the message queue */
    mq = mq_open(NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if(mq < 0)
       perror("mq_open failed");

        /* receive the message */
   bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
   if(bytes_read < 0)
       perror("mq_recv failed");
   gettimeofday(&tv,NULL);

   time_in_micros = (1000000 * tv.tv_sec) + tv.tv_usec;
   printf("The time is %lu\n",time_in_micros);

        buffer[bytes_read] = '\0';
    mq_close(mq);
    mq_unlink(NAME);

    return 0;
}
