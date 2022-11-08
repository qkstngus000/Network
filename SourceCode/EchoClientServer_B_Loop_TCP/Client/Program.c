#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 1234

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server;
  char message[MESSAGE_BUFFER_SIZE] = {0};
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int file_desc = connect_server(&server, AF_INET, SERVER_IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  int quit = -1;

  while (quit != 0)
  {
    //Input user's message
    read_user_input("Enter message or quit to exit: ", message, MESSAGE_BUFFER_SIZE);
  
    //Send user's message to server
    send_message(file_desc, message);

    //Receive echoed message from server
    receive(file_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
    printf("Server message received: %s \n", receive_buffer);

    quit = strcmp(message, "quit");
  }
  
  close(file_desc);

  return 0;
}
