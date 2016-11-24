#include <sys/time.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <errno.h>

#define BUFSIZE     1050
#define READSIZE    1

typedef struct buf_addr
{
    char *start;
    char *end;
}buf_addr;

buf_addr diff(char *buf1, char *buf2);
void cpstring(buf_addr addr, char *dst);

int main(int argc, char *argv[])
{
    int fd = -1;
    char buf1[BUFSIZE], buf2[BUFSIZE];
    char send_data[BUFSIZE];
    ssize_t readbytes;
    ssize_t sendsize;
    buf_addr addr = {NULL,NULL};

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

        if ((readbytes = read(fd, buf2, BUFSIZE - 1)) == -1)
        {
            perror("[READ] ");
            close(fd);
            return 0;
        }

        printf("[READ] %ldBytes\n", readbytes);
        // puts(buf2);

        addr = diff(buf1, buf2);
        printf("[READ] %dBytes\n", (int)(addr.end - addr.start));
        memcpy(buf1, buf2, BUFSIZE-1);
        cpstring(addr, send_data);
        printf("%s\n", send_data);
        // sendsize = addr.start - addr.end;

        // printf("%s\n",send_data);
        close(fd);
        sleep(5);
    }

}

buf_addr diff(char *buf1, char *buf2)
{
    buf_addr ret = { NULL, NULL };

    for(int i=0; i < BUFSIZE-1; i++)
    {
        if (buf1[i] != buf2[i] && !ret.start)
        {
            ret.start = buf1 + i;
        }
        
        if (buf1[i] == buf2[i] && ret.start)
        {
            ret.end = buf1 + i;
            break;
        }
    }

    return ret;
}

void cpstring(buf_addr addr, char *dst)
{
    char *temp;
    memset(dst, 0x00, BUFSIZE);

    for (temp = addr.start; temp < addr.end; temp++)
    {
        *dst = *temp;
        dst++;
    }
    *dst = '\0';
}