#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TRUE 1
#define SERVER_IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 20984
#define RECEIVE_BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    connectClient();
    return 0;
}

int connectClient() {
    // Server would need both of client and server end points
    printf("Hello World\n");
    struct sockaddr_in server_socket_endpt;
    memset(&server_socket_endpt, 0, sizeof(struct sockaddr_in));
    struct sockaddr_in client_socket_endpt;
    memset(&client_socket_endpt, 0, sizeof(struct sockaddr_in));

    int server_socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    server_socket_endpt.sin_family = AF_INET;
    server_socket_endpt.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    server_socket_endpt.sin_port = htons(SERVER_PORT);

    // bind: Assign name of socket
    // USAGE: bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    if (bind(server_socket_desc, &server_socket_endpt, sizeof(struct sockaddr_in)) < 0) {
        perror("Bind Failure");
    }
    
    // listen: mark socket that will be used to accpet incoming connection request
    if (listen(server_socket_desc, 10) < 0) {
        perror("Listen Failure");
    } 
}