#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

void send_packets(int packet_quantity, int sock_desc, char* send_buffer, int send_buffer_size);
void send_char(int sock_desc, char* send_buffer, char c, int send_buffer_size);

#define TRUE 1
#define IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 1234
#define BUFFER_SIZE 512

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

    //Send packets to client
    send_packets(5, client_sock_desc, send_buffer, BUFFER_SIZE);

    close(client_sock_desc);
  }

  close(server_sock_desc);

  return 0;
}

void send_packets(int packet_quantity, int sock_desc, char* send_buffer, int send_buffer_size)
{
  char c = 'A';

  for (int packet_count = 0; packet_count < packet_quantity; packet_count++)
  {
    send_char(sock_desc, send_buffer, 'A' + packet_count , BUFFER_SIZE);
  }
}

void send_char(int sock_desc, char* send_buffer, char c, int send_buffer_size)
{
  memset(send_buffer, 0, send_buffer_size);
  
  send_buffer[0] = c;

  send_message(sock_desc, send_buffer);
}