#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define TRUE 1
#define IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 1234
#define DATA_PORT 1235
#define prompt "Would you like to send a message (y/n)? "

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server_endpoint;
  struct sockaddr_in client_endpoint;
  struct sockaddr_in data_endpoint;

  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int server_sock_desc = create_tcp_server_socket(&server_endpoint, AF_INET, IP_ADDRESS, SERVER_PORT, SOCK_STREAM);
  
  // Connect to data server
  int data_sock_desc = connect_server(&data_endpoint, AF_INET, IP_ADDRESS, DATA_PORT, SOCK_STREAM);

  
  printf("Listening on Network Interface: %s Network Port: %d \n", IP_ADDRESS, SERVER_PORT);
 
  while (TRUE) 
  {
    printf("Waiting for client connection...\n");
    
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int client_sock_desc = accept(server_sock_desc, (struct sockaddr *)&client_endpoint, &sock_len);

    char *client_ip = inet_ntoa(client_endpoint.sin_addr);

    printf("Accepted connection: %s:%d\n", client_ip, ntohs(client_endpoint.sin_port));

    int quit = -1;

    while (quit != 0)
    {
      // Send the starting prompt
      if (send_message(client_sock_desc, prompt) < 0) perror("Prompt Send Failure");
      //Receive message from client
      if (receive_message(client_sock_desc, receive_buffer, RECEIVE_BUFFER_SIZE) < 0) perror("Prompt Response Receving Failure");
      
      // If user respond not to send message b
      if (strcmp(receive_buffer, "n") != 0) {
        printf("Client message received: %s", receive_buffer);
      }

      //Echo message back to client
      send_message(data_sock_desc, receive_buffer);
      
      quit = strcmp(receive_buffer, "n");
    }

    close(client_sock_desc);
  }

  close(server_sock_desc); //Will never reach this point.

  return 0;
}
