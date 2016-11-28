/*
@ echo server
@ MADE by Holmes1st
@ First modify 2016.11.24 (yyyy.mm.dd)

HOW TO USE
    ./echo_server client_ip server_port

GOAL
    read /dev/klg every N seconds, and send it to peer

UPDATES
    2016-11-28
        success to accept only special client

    2016-11-26
        change name read_log.c -> echo_server.c
        success socket communication (send_only)
        TODO : accept client only argv[1]
            ex) accept only 127.0.0.1

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
#include <string.h> // memset, memcpy, strcpy
#include <stdio.h>
#include <stdlib.h> // atoi
#include <errno.h>  // perror

#include <sys/socket.h>
#include <arpa/inet.h>

#include "var.h"    // cpstring, diff, struct buf_addr

#define default_port    ("31337")
#define default_client  ("127.0.0.1")

int main(int argc, char *argv[])
{
    // argv[1] == client ip
    // argv[2] == port number
    char client_ip[16] = "";
    char server_port[6] = "";
    if (!isValidIp4(argv[1]))   strcpy(client_ip, default_client);
    else                        strcpy(client_ip, argv[1]);

    if (atoi(argv[2]) < 1 || atoi(argv[2]) > 65535) strcpy(server_port, default_port);
    else                                            strcpy(server_port, argv[2]);


    /* log file variables */
    int fd = -1;
    char buf1[BUFSIZE], buf2[BUFSIZE];
    char send_data[BUFSIZE];
    ssize_t sendsize;
    buf_addr addr = {NULL,NULL};


    /* socket variables */
    int server_sock, client_sock;
    socklen_t addr_size;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    

    /* init */
    memset(buf1, 0x00, BUFSIZE);
    memset(buf2, 0x00, BUFSIZE);
    memset(send_data, 0x00, BUFSIZE);
    puts("[INIT] Complete");

    /* setting socket from here*/
    if ((server_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[SOCK ERROR] ");
        return 0;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(server_sock, (struct sockaddr* )&server_addr, sizeof(server_addr)) == -1)
    {
        perror("[BIND ERROR] ");
        return 0;
    }

LISTEN:
    if(listen(server_sock, 5) == -1)
    {
        perror("[LISTEN ERROR] ");
        return 0;
    }

    addr_size = sizeof(client_addr);

    if ((client_sock = accept(server_sock, (struct sockaddr* )&client_addr, &addr_size)) == -1)
    {
        perror("[ACCEPT ERROR] ");
        return 0;
    }
    // socket setting end

    printf("%s\n",inet_ntoa(client_addr.sin_addr));
    if(strcmp(inet_ntoa(client_addr.sin_addr), client_ip) != 0){
        write( client_sock, "[DECLINE] You are not allowed ip.\n", 35);
        close( client_sock );
        goto LISTEN;
    }


    while(1)
    {
        if ((fd = open("/dev/klg", O_RDONLY)) == -1)
        {
            perror("[DEV] ");
            close( server_sock );
            close( client_sock );
            return 0;
        }

        if (read(fd, buf2, BUFSIZE - 1) == -1)
        {
            perror("[READ] ");
            close(fd);
            close( server_sock );
            close( client_sock );
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
                close( server_sock );
                close( client_sock );
                return 0;
            }
            // printf("%d\n", send_data[0]);
            puts(send_data);
            
            // send log to client
            write(client_sock, send_data, sendsize);
        }
        memcpy(buf1, buf2, BUFSIZE-1);
        
        // sendsize = addr.start - addr.end;

        // printf("%s\n",send_data);
        close(fd);
        sleep(3);
    }

}
