#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

#define SERVER "www.google.com"  // Google's main web server
#define PORT 80               // HTTPS port
#define BUFFER_SIZE 65536         // Large buffer for high-speed reception

// Function to get current time in seconds
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (tv.tv_usec / 1e6);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *host;
    char *buffer = malloc(BUFFER_SIZE);

    // 1. Resolve hostname to IP address
    if ((host = gethostbyname(SERVER)) == NULL) {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    // 2. Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 3. Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr, host->h_addr, host->h_length);

    // 4. Connect to Google's server
    printf("Connecting to %s (%s) on port %d...\n", SERVER, inet_ntoa(*(struct in_addr*)host->h_addr), PORT);
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected! Starting speed test...\n");

    // 5. Send an HTTP GET request
    char request[] = "GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 6. Receive data and measure speed
    double start_time = get_time();
    size_t bytes_received = 0;
    ssize_t received;

    while ((received = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        bytes_received += received;
    }

    double end_time = get_time();
    double duration = end_time - start_time;
    double speed_mbps = (bytes_received * 8) / (duration * 1e6);  // Convert to Mbps

    printf("Received %lu KB in %.2f seconds\n", bytes_received / 1024, duration);
    printf("TCP Speed from Google: %.2f Mbps\n", speed_mbps);

    // 7. Cleanup
    close(sockfd);
    free(buffer);
    return 0;
}
