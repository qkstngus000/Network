
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 20984
#define RECEIVE_BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    //1. Server Socket address initiation
    //// Clients would need server end point
    struct sockaddr_in server_endpt;
    memset(&server_endpt, 0, sizeof(struct sockaddr_in));

    // Create Socket with server socket addr
    ////AF_INET = Use IPv4     AF_INET6 = Use IPv6
    ////SOCK_STREAM = Use TCP   SOCK_DGRAM = Use UDP
    ////Protocol = 0 (Protocol Val for IP = 0)
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    server_endpt.sin_family = AF_INET;
    server_endpt.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    server_endpt.sin_port = htons(SERVER_PORT);
    
    char *server_ip = inet_ntoa(server_endpt.sin_addr);

    printf("Connecting to server: %s:%d\n", server_ip, ntohs(server_endpt.sin_port));
    



    

}