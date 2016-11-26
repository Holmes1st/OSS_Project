/*

@ echo server
@ MADE by Holmes1st
@ First modify 2016.11.24 (yyyy.mm.dd)

GOAL
    read /dev/klg every 3 seconds, and send it to peer

UPDATES
    2016-11-26
        trying socket communication (send_only) 

    2016-11-25
        success to read "updated" message
        split read_log.c -> read_log.c (for main)
                            var.c      (for diff, cpstring)
    2016-11-24
        first create read_log.c
        success to read whole /dev/klg
        TODO : read only updated message
*/


#include <unistd.h> // sleep, read, close
#include <fcntl.h>  // open, O_RDONLY
#include <string.h> // memset, memcpy
#include <stdio.h>
#include <errno.h>  // perror

#include "var.h"    // cpstring, diff, struct buf_addr

int main(int argc, char *argv[])
{
    /* log file variables */
    int fd = -1;
    char buf1[BUFSIZE], buf2[BUFSIZE];
    char send_data[BUFSIZE];
    ssize_t sendsize;
    buf_addr addr = {NULL,NULL};


    /* socket variables */
    int sockfd;
    

    /* init */
    memset(buf1, 0x00, BUFSIZE);
    memset(buf2, 0x00, BUFSIZE);
    memset(send_data, 0x00, BUFSIZE);
    puts("[INIT] Complete");

    while(1)
    {
        if ((fd = open("/dev/klg", O_RDONLY)) == -1)
        {
            perror("[DEV] ");
            return 0;
        }

        if (read(fd, buf2, BUFSIZE - 1) == -1)
        {
            perror("[READ] ");
            close(fd);
            return 0;
        }
        // printf("%X\n", buf2[0]);
        addr = diff(buf1, buf2);
        // printf("start: 0x%p\tend: 0x%p\n", addr.start,addr.end);

        if (addr.start != NULL)
        {
            printf("[READ] %dBytes\n", (int)(addr.end - addr.start));

            if((sendsize = cpstring(addr, send_data)) < 1)
            {
                printf("[COPY] String copy error\n");
                close(fd);
                return 0;
            }
            // printf("%d\n", send_data[0]);
            puts(send_data);
            /* TODO 
               - send send_data to server
            */
        }
        memcpy(buf1, buf2, BUFSIZE-1);
        
        // sendsize = addr.start - addr.end;

        // printf("%s\n",send_data);
        close(fd);
        sleep(5);
    }

}
