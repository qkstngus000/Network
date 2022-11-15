#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 1234
#define BUFFER_SIZE 27
#define ACKNOWLEDGEMENT_MESSAGE "MESSAGE_RECEIVED"

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server_endpoint;
  char message[BUFFER_SIZE] = {0};
  char receive_buffer[BUFFER_SIZE] = {0};

  int sock_desc = connect_server(&server_endpoint, AF_INET, SERVER_IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  //Receive message from server
  int byte_count = receive_message(sock_desc, receive_buffer, BUFFER_SIZE - 1);
  while (byte_count != 0)
  {
    printf("Server message received: %s \n", receive_buffer);

    byte_count = receive_message(sock_desc, receive_buffer, BUFFER_SIZE - 1);
  }

  close(sock_desc);

  return 0;
}
