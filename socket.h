#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <strings.h>

#define SA struct sockaddr
int sockfd;
#define MAXLINE 4096

void _error(char *str)
{
    perror(str);
    sleep(1);
    close(sockfd);
    exit(1);
}