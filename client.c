#include "socket.h" //This is my own header//


int main(int argc, char const *argv[])
{
    int n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    if (argc != 2)
    {
        printf("provide IP");
        sleep(1);
        exit(1);
    }

    /*Creating Internet stream socket*/
    if (sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0)
        _error("socket");
    
    /*Initializing TCP/IP,port number , IP address of server*/
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        _error("inet_pton");
    
    /*Connecting to the server*/
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)
        _error("connect error");

    /*receiving data from the server*/
    while (1)
    {
        n = read(sockfd, recvline, MAXLINE);
        if (n < 0)
        _error("read error");

        recvline[n] = 0; /* null terminate */
        if (fputs(recvline, stdout) == EOF)
            _error("fputs error");
    }
    return 0;
}
