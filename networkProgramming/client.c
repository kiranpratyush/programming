/*
A simple client which will recieve hello world from the server 
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAXBUFFER 100

// take the port name and the domain name from the command line argument
// Sock addr stores generic structure need to convert into the ip4 or ip6 address
void *get_in_addr(struct sockaddr *s){
 if(s->sa_family==AF_INET){
    //first cast it to 
    return &(((struct sockaddr_in*)s)->sin_addr);
 }
 else if(s->sa_family == AF_INET6){
    // first cast it to the sockaddr_in6
   return  &(((struct sockaddr_in6*)s)->sin6_addr);
 }
}


int main(int argc,char *argv[]){
    int sockfd,num_bytes; // this is to calculate the number of bytes received
    struct addrinfo hints,*result_address,*p; // This will be used to get the address info
    char address_in_string[INET6_ADDRSTRLEN];
    int populate_addressinfo_status;
    char buffer[MAXBUFFER];
    int num_bytes_received;
    if(argc !=3){
        fprintf(stderr,"usage client hostname portnumber");
        exit(1);
    }
    // Here build the hints
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    // fill the address info
    if((populate_addressinfo_status=getaddrinfo(argv[1],argv[2],&hints,&result_address))==-1){
        fprintf(stderr,"getaddrinfo %s \n",gai_strerror(populate_addressinfo_status));
        exit(1);
    }
    // Now if the populate is successful build the socket 
    for(p=result_address;p!=NULL;p=p->ai_next){
        // build a socket 
        if((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1)
        {
            perror("client:socket failed");
            continue;
        }
        // connect to socket
        if(connect(sockfd,p->ai_addr,p->ai_addrlen)==-1){
            close(sockfd);
            perror("failed to connect");
            continue;
        }
        // break if one result is successful
        break;
    }
    if(p==NULL){
        fprintf(stderr,"client:failed to connect\n");
        exit(2);
    }
   inet_ntop(p->ai_family,get_in_addr((struct sockaddr *)p->ai_addr),address_in_string,sizeof(address_in_string));
   printf("client connecting to %s \n",address_in_string);
   freeaddrinfo(result_address);
   if((num_bytes_received=recv(sockfd,buffer,MAXBUFFER-1,0))==-1){
    perror("recv");
    exit(1);
   }
   printf("num bytes recieved %d \n",num_bytes_received);
   buffer[num_bytes_received] ='\0';
   printf("client received %s \n",buffer);
   close(sockfd);
   return 0;


}
