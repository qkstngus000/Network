#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define TRUE 1
#define IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 1234
#define MESSAGE "Message from client"

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server;
  struct sockaddr_in client;
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};
  char message[MESSAGE_BUFFER_SIZE] = {0};

  int serverFd = create_tcp_server_socket(&server, AF_INET, IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  printf("Listening on Network Interface: %s Network Port: %d \n", IP_ADDRESS, SERVER_PORT);
 
  while (TRUE) 
  {
    printf("Waiting for client connection...\n");
    
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int clientFd = accept(serverFd, (struct sockaddr *)&client, &sock_len);

    char *client_ip = inet_ntoa(client.sin_addr);
    printf("Accepted connection: %s:%d\n", client_ip, ntohs(client.sin_port));

    while (1)
    {
      //Receive message from client
      receive_message(clientFd, receive_buffer, RECEIVE_BUFFER_SIZE);
      printf("Client message received: %s \n", receive_buffer);

      //Receive integer value from client
      receive_message(clientFd, receive_buffer, RECEIVE_BUFFER_SIZE);
      int integer_value = atoi(receive_buffer);
      printf("Client integer value received: %d \n", integer_value);

      //Echo message back to client
      send_message(clientFd, receive_buffer);
    }

    close(clientFd);
  }

  close(serverFd);

  return 0;
}
