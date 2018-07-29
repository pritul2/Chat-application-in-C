#include "socket.h" //This is my own header//
int sockfd;
void _error(const char *str)
{
    perror(str);
    sleep(1);
    close(sockfd);
    exit(1);
}

int main(int argc, char  *argv[])
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
    bzero(&servaddr, sizeof(servaddr));//set memory bytes to 0//
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);//It conver port to binary//
    
    //inet_pton converts the ASCII to numeric IP and stores in servaddr.sin_addr//
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        _error("inet_pton");
    //inet_addr() can also be used no matters//
    

    /*Connecting to the server by TCP*/
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0)//We are typecasting to SA because it is generic means we supply UDP or TCP it both accept//
        _error("connect error");

    /*receiving data from the server*/
    while (1)
    {
        n = read(sockfd, recvline, MAXLINE);
        //read MAXLINE bytes from the server and store in recvline//
        if (n < 0)
        _error("read error");

        recvline[n] = 0; /* null terminate */
        if (fputs(recvline, stdout) == EOF) //Printing data in recvline//
            _error("fputs error");
    }
    return 0;
}
