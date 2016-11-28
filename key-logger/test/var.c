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

int isValidIp4 (char *str)
{
    int segs = 0;   /* Segment count. */
    int chcnt = 0;  /* Character count within segment. */
    int accum = 0;  /* Accumulator for segment. */

    /* Catch NULL pointer. */

    if (str == NULL)
        return 0;

    /* Process every character in string. */

    while (*str != '\0') {
        /* Segment changeover. */

        if (*str == '.') {
            /* Must have some digits in segment. */

            if (chcnt == 0)
                return 0;

            /* Limit number of segments. */

            if (++segs == 4)
                return 0;

            /* Reset segment values and restart loop. */

            chcnt = accum = 0;
            str++;
            continue;
        }
 

        /* Check numeric. */

        if ((*str < '0') || (*str > '9'))
            return 0;

        /* Accumulate and check segment. */

        if ((accum = accum * 10 + *str - '0') > 255)
            return 0;

        /* Advance other segment specific stuff and continue loop. */

        chcnt++;
        str++;
    }

    /* Check enough segments and enough characters in last segment. */

    if (segs != 3)
        return 0;

    if (chcnt == 0)
        return 0;

    /* Address okay. */

    return 1;
}
int isValidport(char * port)
{

}
