#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Utilities.h"

#define TRUE 1
#define SERVER_IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 20984
#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024


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
    if (bind(server_socket_desc, (struct sockaddr *)&server_socket_endpt, sizeof(struct sockaddr_in)) < 0) {
        perror("Bind Failure");
    }
    
    // listen: mark socket that will be used to accpet incoming connection request
    if (listen(server_socket_desc, 10) < 0) {
        perror("Listen Failure");
    }

    char *receive_buffer;
    char *send_buffer;
    // Wait for client response
    while (TRUE) {
        printf("Waiting for client connection...\n");
        socklen_t sock_len = sizeof(struct sockaddr_in);
        int client_socket_desc = accept(server_socket_desc, (struct sockaddr *)&client_socket_endpt, &sock_len);
        
        char *client_ip = inet_ntoa(client_socket_endpt.sin_addr);
        printf("Accepted connection: %s:%d\n\n", client_ip, ntohs(client_socket_endpt.sin_port));

        const char starting_prompt[] = "Would you like to play (y/n)?\0";
        if (write(client_socket_desc, starting_prompt, strlen(starting_prompt)) < 0) perror("Write Failure");
        

        if (read(client_socket_desc, receive_buffer, sizeof(char))< 0) perror("Read Failure");
        else printf("Receieved from client: %s", receive_buffer);

    }

}

int main(int argc, char const *argv[]) {
    connectClient();
    return 0;
}