#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 1234
#define BUFFER_SIZE 512
#define ACKNOWLEDGEMENT_MESSAGE "MESSAGE_RECEIVED"

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server_endpoint;
  char message[BUFFER_SIZE] = {0};
  char receive_buffer[BUFFER_SIZE] = {0};

  int sock_desc = connect_server(&server_endpoint, AF_INET, SERVER_IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

 //Receive message from server
  int bytes_read = receive_message(sock_desc, receive_buffer, BUFFER_SIZE);
  
  while (bytes_read != 0)
  {
    //https://embeddedartistry.com/blog/2017/07/05/printf-a-limited-number-of-characters-from-a-string/
    printf("Server message received: %.*s \n", BUFFER_SIZE, receive_buffer);
    bytes_read = receive_message(sock_desc, receive_buffer, BUFFER_SIZE);
  }

  close(sock_desc);

  return 0;
}
