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
  struct sockaddr_in server_endpoint;
  char message[MESSAGE_BUFFER_SIZE] = {0};
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int sock_desc = connect_server(&server_endpoint, AF_INET, SERVER_IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  int quit = -1;

  while (quit != 0)
  {
    // Receive Client prompt message
    if (receive_message(sock_desc, receive_buffer, RECEIVE_BUFFER_SIZE) < 0) perror("Receive Prompt Failure");
    printf("%s", receive_buffer);

    //Input user's response to prompt
    read_user_input("", message, MESSAGE_BUFFER_SIZE);

    if (strcmp(message, "n") == 0) {
      if (send_message(sock_desc, message) < 0) perror("Prompt Response Send Failure");
    }
    else {
      //Send user's message to server
      read_user_input("Enter message: ", message, MESSAGE_BUFFER_SIZE);
      if (send_message(sock_desc, message) < 0) perror("Message Send Failure");

      //Receive echoed message from server
      receive_message(sock_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
      printf("Server message received: %s \n", receive_buffer);
    }
    
    quit = strcmp(message, "n");
  }
  
  close(sock_desc);

  return 0;
}
