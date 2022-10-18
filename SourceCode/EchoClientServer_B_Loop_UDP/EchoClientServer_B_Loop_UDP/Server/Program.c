#include <stdio.h>      //perror, printf
#include <stdlib.h>     // exit, atoi
#include <unistd.h>     // read, write, close
#include <arpa/inet.h>  // sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY
#include <string.h>     // memset

#include "Utilities.h"

#define TRUE 1
#define SERVER_IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 1234

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server;
  struct sockaddr_in client;
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int file_desc = create_udp_server_socket(&server, AF_INET, SERVER_IP_ADDRESS, SERVER_PORT, SOCK_DGRAM);

  printf("Listening on Network Interface: %s Network Port: %d \n", SERVER_IP_ADDRESS, SERVER_PORT);
 
  while (TRUE) 
  {
    printf("Waiting for client connection...\n");

    //Receive message from client
    memset(&client, 0, sizeof(client)); 
    int byte_count = receive_udp_message(file_desc, &client, receive_buffer, RECEIVE_BUFFER_SIZE);

    char *client_ip = inet_ntoa(client.sin_addr);
    printf("Accepted connection: %s:%d\n", client_ip, ntohs(client.sin_port));

    printf("Client message received: %s \n", receive_buffer);

    //Echo message back to client
    byte_count = send_udp_message(file_desc, &client, receive_buffer, byte_count);
  }

  close(file_desc);

  return 0;
}
