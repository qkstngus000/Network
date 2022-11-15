#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 1234
#define MESSAGE "Message from client"

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server;
  char message[MESSAGE_BUFFER_SIZE] = {0};
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int file_desc = connect_server(&server, AF_INET, SERVER_IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  while (1)
  {
    //Send string message to server
    memset(message, 0, MESSAGE_BUFFER_SIZE);
    strcpy(message, MESSAGE);
    send_message(file_desc, message);

    //We should wait and receive acknowledgement from server
    //before we send out the next packet.

    //Send integer message to server
    memset(message, 0, MESSAGE_BUFFER_SIZE);
    sprintf(message, "%d", 12345);
    send_message(file_desc, message);

    //Receive echoed message from server
    receive_message(file_desc, receive_buffer, RECEIVE_BUFFER_SIZE);

    int result = strcmp(message, receive_buffer);

    if (result != 0) break;

    printf("Server message received: %s \n", receive_buffer);
  }
  
  close(file_desc);

  printf("Client closing...\n");

  return 0;
}
