#include <string.h>

#include "var.h"

buf_addr diff(char *buf1, char *buf2)
{
    buf_addr ret = { NULL, NULL };

    for(int i=0; i < BUFSIZE-1; i++)
    {
        if (buf1[i] != buf2[i] && !ret.start)
        {
            ret.start = buf2 + i;
        }
        
        if (ret.start && buf1[i] == buf2[i])
        {
            ret.end = buf2 + i;
            break;
        }
    }

    return ret;
}

ssize_t cpstring(buf_addr addr, char *dst)
{
    char *temp, *dst_temp = dst;
    // memset(dst, 0x00, BUFSIZE);

    for (temp = addr.start; temp < addr.end; temp++)
    {
        *dst = *temp;
        // printf("dst: %d\ttemp: %d",*dst,*temp);
        dst++;
    }
    *dst = '\0';

    return (ssize_t)(dst - dst_temp);
}