
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Utilities.h"

#define TRUE 1
#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 20984
#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024


int connectServer() {
    //1. Server/Client Socket address initiation
    //// Server/Client would need each other's end point
    struct sockaddr_in server_socket_endpt;
    memset(&server_socket_endpt, 0, sizeof(struct sockaddr_in));
    struct sockaddr_in client_socket_endpt;
    memset(&client_socket_endpt, 0, sizeof(struct sockaddr_in));

    socklen_t client_endpt_length = sizeof(client_socket_endpt);

    // Create Socket with server socket addr
    ////AF_INET = Use IPv4     AF_INET6 = Use IPv6
    ////SOCK_STREAM = Use TCP   SOCK_DGRAM = Use UDP
    ////Protocol = 0 (Protocol Val for IP = 0)
    int client_socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    server_socket_endpt.sin_family = AF_INET;
    server_socket_endpt.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    server_socket_endpt.sin_port = htons(SERVER_PORT);
    
    char *server_ip = inet_ntoa(server_socket_endpt.sin_addr);

    printf("Connecting to server: %s:%d\n", server_ip, ntohs(server_socket_endpt.sin_port));
    if(connect(client_socket_desc, (struct sockaddr *)&server_socket_endpt, sizeof(struct sockaddr_in)) < 0) {
        perror("Connection Failure");
    }
    if (getsockname(client_socket_desc, (struct sockaddr *)&client_socket_endpt, &client_endpt_length) < 0) {
        perror("getsockname Failure");
    }
    char *client_ip = inet_ntoa(client_socket_endpt.sin_addr);
    printf("Client Endpoint: %s:%d\n", client_ip, ntohs(client_socket_endpt.sin_port));
    
    char *receive_buffer;
    char *send_buffer;
    while (TRUE) {
        //connect to server socket file descriptor
        socklen_t sock_len = sizeof(struct sockaddr_in);

        // Receieve server dialogue
        receive_buffer = (char *)malloc(RECEIVE_BUFFER_SIZE);
        if (read(client_socket_desc, receive_buffer, RECEIVE_BUFFER_SIZE) < 0) perror("Read Failure");
        else printf("%s\n", receive_buffer);
        memset(receive_buffer, 0, RECEIVE_BUFFER_SIZE);
        
        // Respond to server
        char prompt_response;
        char * message = (char *)malloc(sizeof(char *))
        scanf(message, sizeof(char));
        if (write(client_socket_desc, &prompt_response, sizeof(char)) < 0) perror("write to server failure");

        printf("write to server completed");
    }
        
    return 0;
}

int main(int argc, char const *argv[]) {
    connectServer();
    return 0;
}