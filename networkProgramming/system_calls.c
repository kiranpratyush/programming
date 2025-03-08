#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

/*

Flow to get the IP address of any of the domain using the getaddrinfo()

first build hint (addrinfo).
second build the addrinfo linkedlist ( This will contain all the information about the address)
build the filter ( hints with the ai_family)
ai_socktype
ai_flag ( here not necessiary)
then get the details

now iterate over the result and then get the sockaddress convert into sockaddress_in or sockaddress_in6 based on the ai_familty
convert the address to the inet_ntop


The flow of network connection:

1. Build the network addrinfo
2. Build the socket file descripter
2. Bind  the socket generally useful in the server 
3. Connect to the address
4. listen
5. accept



*/

int main() {
    struct addrinfo hint,*results,*p;
    char ip_instr[INET6_ADDRSTRLEN];
    int status;
    memset(&hint,0,sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    status = getaddrinfo("www.google.com","30",&hint,&results);
    if(status!=0){
        fprintf(stderr,"erro came");
        exit(1);
    }
    p = results;
    while(p!=NULL){
        // Here get the address
        if(p->ai_family==AF_INET){
            // This is ip4 version
            struct sockaddr_in *ip_addr = (struct sockaddr_in *)p->ai_addr;
            inet_ntop(AF_INET,&(ip_addr->sin_addr),ip_instr,INET_ADDRSTRLEN);
            printf("IP4 address is %s \n",ip_instr);
        }
        else if (p->ai_family==AF_INET6){
            struct sockaddr_in6 *ip_addr = (struct sockaddr_in6 *)p->ai_addr;
            inet_ntop(AF_INET6,&(ip_addr->sin6_addr),ip_instr,INET6_ADDRSTRLEN);
            printf("IP6 Address is %s \n",ip_instr);
        }
        p = p->ai_next;
    }
    free(results);

}
