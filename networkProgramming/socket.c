/*
Goals to build a client and server to connect and get the data

byte orders : host byte orders and the network byte ordering 
before sending the bytes convert into the network ordering 
before putting the data in the host convert into the host ordering 



Socket Descriptor: int (data type)
struct addrinfo used to prep socket address structures for subsequent uses
host name lookups
service name lookups (I don't understand this)

There are three interfaces:

struct addrinfo
struct sockaddr ai_addr
This sockaddr can be casted back from sockaddr_in ( This is for internet specific)
sockaddr_in6 which is for IPv6 
Then there is another struct which is sockaddr_storage which can store both the ipv4 and ipv6 address
check the ss_family if AF_INET or AF_INET6 then based on that cast it to something 


Network address translation 

*/

#include<arpa/inet.h>
#include<limits.h>
#include<stddef.h>
#include <sys/socket.h>
#include <unistd.h>
#include<stdio.h>


int main(){
    struct sockaddr_in sa; // This is for IP4 address
    struct sockaddr_in6 sa6; // This is for IP6 address
    // Now how to add the address in the . notation to the struct
    if( !inet_pton(AF_INET,"10.12.110.57",&(sa.sin_addr))){
        fprintf(stderr,"Conversion failed");
    };
    // Add this to the IPv6 address
    if(!inet_pton(AF_INET6,"2001:db8:63b8:1::3490",&(sa6.sin6_addr))){
        fprintf(stderr,"Conversion failed");
    }
    // now convert the sa into the printable format
    char ip4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET,&(sa.sin_addr),ip4,INET_ADDRSTRLEN);
    printf("The IP4 address is : %s \n",ip4);
    char ip6[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6,&(sa6.sin6_addr),ip6,INET6_ADDRSTRLEN);
    printf("The IP6 address is : %s \n",ip6);
    short x  = htons(10);
    x = ntohs(x);
    printf("%d",x);
    
}