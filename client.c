/*The IP address of same hosts is 127.0.0.1*/
/*Chat application*/
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <arpa/inet.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <netdb.h>
#include <time.h>


#define BUFF 100

void error(const char* ptr)
{
    perror(ptr);
    exit(1);
}

int main(int argc, char const *argv[])
{
    char buff[255];
    char IP[10] = {'\0'};
    char cmp_str[] = {"quit"};
    int sock_id=0;
    int port=0;
    int sock_len=0;
    int ret=0;
    char *retptr = NULL;
    void *ptr=calloc(BUFF,sizeof(char));
    struct sockaddr_in client_addr;
    struct hostent *server;
    
    if(argc<2)
    {
        printf("Please give port address and IP address\n");
        exit(1);
    }
    else if(argc<3)
    {
        printf("Please give IP address\n");
        exit(1);
    }

    sock_len=sizeof(client_addr);

    port=atoi(argv[1]);

    strcpy(IP,argv[2]);
    server=gethostbyname(IP);
    if(server==NULL) error("gethostname");

    /*Creating socket*/
    sock_id=socket(PF_INET,SOCK_STREAM,0);
    if(sock_id<0) error("socket");

    bzero((void *)&client_addr,sizeof(client_addr)); 
    client_addr.sin_family=AF_INET;
    client_addr.sin_port=htons(port);
    bcopy((char *)server->h_addr,(char *)&client_addr.sin_addr,server->h_length);

    /*Connecting server*/
    ret=connect(sock_id,(struct sockaddr*)&client_addr,sock_len);
    if(ret<0) error("recv");

    system("clear");

    while(1) 
    {
        /*Write*/
        bzero((void *)buff, sizeof(buff));
        retptr = fgets(buff, sizeof(buff), stdin);
        if (retptr == NULL)
            error("fgets");
        ret=write(sock_id,(void *)buff,strlen(buff));
        if(ret<0) error("write");
        
        /*Read*/
        bzero(buff, sizeof(buff));
        ret = read(sock_id, (void *)buff, sizeof(buff));
        if (ret < 0)
            perror("read");
        printf("server:- %s",buff);

        ret=strncmp("quit",buff,sizeof("quit"));
        if(ret<=0) break;

    }
    ret=close(sock_id);
    if(ret<0) error("close");
}
