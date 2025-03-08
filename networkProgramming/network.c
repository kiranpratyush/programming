/*
Experiment on creating and exposing the socket
*/
#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

#define PORT "8080"    // Browser-friendly port
#define BACKLOG 10     // Max pending connections
#define BUFFER_SIZE 5000 // Bigger buffer for browser requests
#define DATA_SIZE (10 * 1024 * 1024)  

void send_all(int sockfd, const char *data, size_t length) {
    size_t total_sent = 0;
    while (total_sent < length) {
        ssize_t sent = send(sockfd, data + total_sent, length - total_sent, 0);
        if (sent == -1) {
            perror("send failed");
            return;
        }
        total_sent += sent;
    }
}

void sample_server_connection(){
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd;
    char buffer[BUFFER_SIZE];

    // 1. Setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // ✅ Use TCP
    hints.ai_flags = AI_PASSIVE;      // Use wildcard address

    // 2. Get address info
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // 3. Create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 4. Bind the socket
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);  // Free address info

    // 5. Listen for incoming connections
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("🚀 Web Server started! Open your browser and visit: http://localhost:%s/\n", PORT);

    // 6. Keep accepting connections
    while (1) {
        addr_size = sizeof(their_addr);
        new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &addr_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        printf("🌍 Received a browser request!\n");

        // 7. Receive the HTTP request
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(new_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate request
            printf("📩 Received:\n%s\n", buffer);

            // 8. Prepare HTTP Headers
            char headers[BUFFER_SIZE];
            snprintf(headers, sizeof(headers),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/plain\r\n"
                     "Content-Length: %d\r\n"
                     "Connection: close\r\n"
                     "\r\n", DATA_SIZE);

            // 9. Send Headers First
            send_all(new_fd, headers, strlen(headers));

            // 10. Allocate and Send Large Data
            char *large_data = malloc(DATA_SIZE);
            if (!large_data) {
                perror("Memory allocation failed");
                close(new_fd);
                continue;
            }
            memset(large_data, 'A', DATA_SIZE);  // Fill with 'A'

            send_all(new_fd, large_data, DATA_SIZE);
            free(large_data);
        } else {
            perror("recv");
        }

        // 11. Close client socket
        close(new_fd);
    }

    close(sockfd);

}




int main(){
    
    sample_server_connection();

}