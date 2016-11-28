#include <sys/types.h>
#define BUFSIZE     1025

typedef struct buf_addr
{
    char *start;
    char *end;
}buf_addr;

buf_addr diff(char *buf1, char *buf2);
ssize_t cpstring(buf_addr addr, char *dst);
int isValidIp4 (char *str);
