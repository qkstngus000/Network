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
    //Receive starting prompt from server
    receive(file_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
    printf("%s", receive_buffer);

    //Input user's answer to play or not
    read_user_input("", message, MESSAGE_BUFFER_SIZE);
    //Send user's message to server
    send_message(file_desc, message);
    if (strcmp(message, "y") == 0) {


      //Receive echoed message from server about question
      receive(file_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
      printf("%s", receive_buffer);

      // Receive user answer for question and send to server
      read_user_input("", message, MESSAGE_BUFFER_SIZE);
      send_message(file_desc, message);

      // Receive correctiness of answer
      receive(file_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
      printf("%s", receive_buffer);

      // Send acknowledgement
      char ack[] = "Acknowledged answer";
      send_message(file_desc, ack);
    }
    else {
      receive(file_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
      printf("%s\n", receive_buffer);
      quit = 0;
    }

    
  }
  
  close(file_desc);

  return 0;
}
