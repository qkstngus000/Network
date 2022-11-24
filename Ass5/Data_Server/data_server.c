#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define TRUE 1
#define IP_ADDRESS "0.0.0.0"
#define DATA_PORT 1235
#define file "Data.txt"

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in data_endpoint;
  struct sockaddr_in relay_endpoint;
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int data_sock_desc = create_tcp_server_socket(&data_endpoint, AF_INET, IP_ADDRESS, DATA_PORT, SOCK_STREAM);

  printf("Listening on Network Interface: %s Network Port: %d \n\n", IP_ADDRESS, DATA_PORT);
 
  while (TRUE) 
  {
    printf("Waiting for relay server connection...\n");
    
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int relay_sock_desc = accept(data_sock_desc, (struct sockaddr *)&relay_endpoint, &sock_len);

    char *relay_ip = inet_ntoa(relay_endpoint.sin_addr);

    printf("Accepted connection: %s:%d\n", relay_ip, ntohs(relay_endpoint.sin_port));

    // int quit = -1;

    // while (quit != 0)
    // {
      //Receive message from relay server
      int byte_count = receive_message(relay_sock_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
      printf("Message forwarded from relay server: %s \n\n", receive_buffer);

      // Store data to data.txt
      if (write_message(file, relay_ip, receive_buffer) == 0) {
        send_message(relay_sock_desc, "STATUS_MESSAGE_SUCCESS");
      }
      else {
        send_message(relay_sock_desc, "STATUS_MESSAGE_FAIL");
      }

    close(relay_sock_desc);
  }

  close(data_sock_desc); //Will never reach this point.

  return 0;
}
