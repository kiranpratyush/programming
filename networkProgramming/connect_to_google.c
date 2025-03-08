#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT "80"  // Use "443" for HTTPS (but needs OpenSSL)
#define BUFFER_SIZE 4096

int main() {
    struct addrinfo hints, *res, *p;
    int sockfd;
    char buffer[BUFFER_SIZE];

    // Prepare hints structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    // Get address info for google.com
    if (getaddrinfo("google.com", PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }

    // Try each returned address until we successfully connect
    for (p = res; p != NULL; p = p->ai_next) {
        // Create socket
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1) continue; // Try next address if socket fails

        // Connect to Google
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) break; // Success

        close(sockfd); // Otherwise, close and try next
    }
    freeaddrinfo(res); // Free allocated memory

    if (p == NULL) {
        fprintf(stderr, "Failed to connect to google.com\n");
        return 2;
    }

    // Send HTTP GET request
    char request[] = "GET / HTTP/1.1\r\nHost: google.com\r\nConnection: close\r\n\r\n";
    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        close(sockfd);
        return 3;
    }

    // Receive response
    printf("Response from Google:\n");
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("%s", buffer);
    }

    if (bytes_received == -1) {
        perror("recv");
    }

    // Close socket
    close(sockfd);
    return 0;
}
