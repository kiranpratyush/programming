#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include<signal.h>
#include<sys/wait.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>

#define PORT "8080"
#define BACKLOG 10

/*
What is a client server architecture:
There is a server which will handle multiple clients using fork()
The basic routine is : server will wait for the connection, accept() it and fork()
a child process to handle it.
*/

// This is unclear to me once I have finished the CS:APP it will be bit clearer to me 

void sigchld_handler(int s){
    int saved_errorno = errno;
    while(waitpid(-1,NULL,WNOHANG)>0);
    errno = saved_errorno;
}

// This just casts different sockaddr to the ip specific structure
void *get_in_addr(struct sockaddr *addr){
    if(addr->sa_family==AF_INET){
        return &(((struct sockaddr_in *)addr)->sin_addr);
    }
    else if (addr->sa_family==AF_INET6){
        return &(((struct sockaddr_in6 *)addr)->sin6_addr);
    }
}


int main(){
    int sockfd,new_fd ; // listen on sockfd, accept on new_fd
    struct addrinfo hints,*servinfo,*p; // hints will be used to filter out the data , servinfo will be a linked list of addrinfo which will be filled by the getaddrinfo p will be later used to iterate over the servinfo
    struct sockaddr_storage their_addr; // This will be used to store the client address
    socklen_t sin_size;
    struct sigaction sa;
    int yes =1;
    char s[INET6_ADDRSTRLEN]; // max length of the ip6 address length for the char representaion
    int rv; // This is used to store the status code when trying to fill in the address info

    // fill in the address
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_UNSPEC; // can accept both the ip4 and the ip6
    hints.ai_socktype = SOCK_STREAM; // Tells that this should be a tcp connection
    hints.ai_flags = AI_PASSIVE; // use my ip

    // Now fill in the result 
    if((rv=getaddrinfo(NULL,PORT,&hints,&servinfo))!=0){
        // it means the address fill up is not successful so print the address
        fprintf(stderr,"get address info failed with : %s\n",gai_strerror(rv));
    }
    // Now need to go through the linked list of servinfo and bind to the first available addrinfo
    for(p=servinfo;p!=NULL;p=p->ai_next){
        // create a socket 
        if((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol))==-1){
            perror("server:socket");
            continue; // continue exploring the next addrinfo
        }
        // Here set the socketoption to reuse the address
        if((setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)))==-1){
            perror("setsocketopt");
            exit(1);
        }

        // bind the socket
        if(bind(sockfd,p->ai_addr,p->ai_addrlen)==-1){
            // bind failed here so close the existing socket and continue
            close(sockfd);
            perror("server:bind");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);
    // Now after the loop if p is NULL that means we are unable to bind to anysocket simply exit 
    if(p==NULL){
        fprintf(stderr,"Server failed to bind");
        exit(1);
    }
    // Now the bind is successfull we need to listen to socket with some backlog
    if(listen(sockfd,BACKLOG)==-1){
        perror("listen");
        exit(1);
    }
    // reap the zombie processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if(sigaction(SIGCHLD,&sa,NULL)==-1){
        perror("sigaction");
        exit(1);
    }
    printf("server is waiting for connection.... \n");

    while(1){
        sin_size = sizeof(their_addr);
        new_fd = accept(sockfd,(struct sockaddr *)&their_addr,&sin_size);
        if(new_fd==-1){
            perror("accept");
            continue;
        }
        // now just print out from whom you got the connection from
        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s,sizeof(s));
        printf("Before fork: Parent PID = %d\n", getpid());
        if(!fork()){
            printf("After fork: Child PID = %d (Parent PID = %d)\n", getpid(), getppid());
            printf("Server got connection form %s \n",s);
            // This is a child process
            close(sockfd); // we don't need the original sockfd
            if(send(new_fd,"Hello world!",13,0)==-1){
                perror("send");
            }
            close(new_fd);
            exit(0);
        }
        // Here in the parent process we don't need the new_fd
        close(new_fd);
    }

    return 0;
}