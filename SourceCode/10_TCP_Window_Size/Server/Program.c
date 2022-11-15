#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

void populate_buffer(char* send_buffer, char* message, int send_buffer_size);
void populate_buffer_2(char* send_buffer, char* message, int send_buffer_size);
void populate_buffer_3(char* send_buffer, int send_buffer_size);

#define TRUE 1
#define IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 1234
#define BUFFER_SIZE 512
#define MESSAGE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server_endpoint;
  struct sockaddr_in client_endpoint;
  char send_buffer[BUFFER_SIZE] = {0};
  char receive_buffer[BUFFER_SIZE] = {0};

  int server_sock_desc = create_tcp_server_socket(&server_endpoint, AF_INET, IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  printf("Listening on Network Interface: %s Network Port: %d \n", IP_ADDRESS, SERVER_PORT);
 
  while (TRUE) 
  {
    printf("Waiting for client connection...\n");
    
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int client_sock_desc = accept(server_sock_desc, (struct sockaddr *)&client_endpoint, &sock_len);

    char *client_ip = inet_ntoa(client_endpoint.sin_addr);

    printf("Accepted connection: %s:%d\n", client_ip, ntohs(client_endpoint.sin_port));

    populate_buffer_3(send_buffer, BUFFER_SIZE);

      //Send message to client
    for (int message_count = 0; message_count < 10000; message_count++)
    {
      send_message(client_sock_desc, send_buffer);
    }

    close(client_sock_desc);
  }

  close(server_sock_desc); //Will never reach this point.

  return 0;
}

void populate_buffer(char* send_buffer, char* message, int send_buffer_size)
{
  int message_size = strlen(message);

  memset(send_buffer, 0, send_buffer_size);
  
  strncat(send_buffer, message, message_size);
}

void populate_buffer_2(char* send_buffer, char* message, int send_buffer_size)
{
  int message_size = strlen(message);
  int copy_count = send_buffer_size / message_size;

  memset(send_buffer, 0, send_buffer_size);
  
  for (int count = 0; count < copy_count; count++)
  {
    strncat(send_buffer, message, message_size);
  }

  send_buffer[send_buffer_size - 1] = '\0';
}

void populate_buffer_3(char* send_buffer, int send_buffer_size)
{
  const int LETTERS_IN_ALPHABET = 26;

  memset(send_buffer, 0, send_buffer_size);
  
  char c = 'A';
  int offset = 0;
  
  for (int count = 0; count < send_buffer_size; count++)
  {
    send_buffer[count] = c + offset;

    offset++;
    
    if (offset == LETTERS_IN_ALPHABET) offset = 0;
  }
}