/*

Take input from the commandline ( hostnmae)
get the ip address
based on the ip version write the IP address

1. Take input argc and argv argc should be equal to 2
2. getaddrinfo(hostname,portname,hint,linkedlist)
addrinfo hint,*res ( this is linked list)
INET_ADDR

*/

#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>


int main(int argc,char  *argv[]){
    struct addrinfo hints,*result,*p;
    int status;
    char ip_str[INET6_ADDRSTRLEN];
    if(argc!=2){
        fprintf(stderr,"usage: showip hostname \n");
        return 1;
    }
    // build hints here
    memset(&hints,0,sizeof(hints));
    // set the ai family,sockettype
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if((status=getaddrinfo(argv[1],NULL,&hints,&result))!=0){
        frpintf(stderr,"getaddrinfo: %s \n",gai_strerror(status));
        return 2;
    }
    for(p = result;p!=NULL;p=p->ai_next){
        void *addr;
        char *ipversion;
        if(p->ai_family==AF_INET){
            // This is ip version4 
            struct sockaddr_in *ip4 = (struct sockaddr_in * )p->ai_addr;
            inet_ntop(AF_INET,ip4->sin_addr,ipversion,sizeof)
        }
    }
}