/*Chat application*/
#include<stdio.h>
#include<sys/types.h>
#include <sys/socket.h>
#include<errno.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include <string.h>
#include <signal.h>

#define BACKLOG 3

void error(const char* ptr);
/*Global Variables*/
int sockfd = 0, newsockfd = 0;
int ret=0;
/*Once communication is completed we are closing the socket*/
void _close()
{
    printf("closing the connection");
    ret = close(newsockfd);
    if (ret < 0)
        error("close");
    ret = close(sockfd);
    if (ret < 0)
        error("close");
    exit(0);
}

void handler(int _catch)
{
    printf("received signal of %d\n",_catch);
    _close();
}

int main(int argc, char* argv[])
{
    int port=0;
    int socklen=0;
    char buff[255];
    char cmp_str[]={"quit"};
    char* retptr=NULL;
    struct sockaddr_in server_addr,client_addr;

    if(argc<2)
    {
        printf("Please give the port address\n");
        exit(1);
    }
    socklen=sizeof(client_addr);

    /*Taking port address*/
    port=atoi(argv[1]);

    /**1) Creating a socket**/
    sockfd=socket(PF_INET,SOCK_STREAM,0);
    if(sockfd<0)
        error("socket");

    /** sockaddr**/
    server_addr.sin_family=AF_INET;  /* Internet address family for IPv4*/
    server_addr.sin_addr.s_addr=(htonl)INADDR_ANY;/*Any incoming interface*/
    server_addr.sin_port=htons(port);/*Local port*/

    /**2) Bind the socket**/
    ret=bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    if(ret<0) error("bind");

    /**listen**/
    ret=listen(sockfd,BACKLOG);
    if(ret<0) error("listen");

    /**accept function**/
    newsockfd=accept(sockfd,(struct sockaddr *)&client_addr,&socklen);
    //newsockfd contains the reference of client request//
    if(newsockfd<0) error("accept");
    
    /*Signal Handling*/
    signal(SIGINT,handler);
    
    /*Communication*/

    system("clear");
    while(1)
    {
        /*read*/
        bzero(buff,sizeof(buff));//clearing the buffer so that the garbage value doesnot comes//

        ret=read(newsockfd,(void *)buff,sizeof(buff));//reading the request of client through newsockfd//
        if(ret<0) perror("read");
        
        printf("Client :- %s",buff);//prints client message on terminal//

        /*write*/
        bzero((void *)buff, sizeof(buff));//clearing the buffer to write the message//
        retptr = fgets(buff, sizeof(buff), stdin);//taking keyboard input//
        if (retptr == NULL)
            error("fgets");
        ret = write(newsockfd, (void *)buff, strlen(buff));//writing the message on buffer through newsockfd//
        if (ret < 0)
            error("write");
        ret = strncmp("quit", buff, sizeof("quit"));//check whether client wants to terminate connection//
        if(ret<=0) break;//return value is 0 if client wants to quit then ending the loop//
    }
    _close();
}
void error(const char* ptr)
{
    perror(ptr);
    exit(1);
}
