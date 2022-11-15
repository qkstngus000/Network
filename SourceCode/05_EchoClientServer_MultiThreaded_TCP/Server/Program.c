#include <stdio.h>      //perror, printf
#include <stdlib.h>     //exit, atoi
#include <unistd.h>     //read, write, close
#include <arpa/inet.h>  //sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY
#include <string.h>     //memset
#include<pthread.h>     //gcc option: -pthread

#include "Utilities.h"

void *connection_handler(void *socket_desc);

#define TRUE 1
#define IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 1234

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server_endpoint;
  struct sockaddr_in client_endpoint;
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int server_sock_desc = create_tcp_server_socket(&server_endpoint, AF_INET, IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  printf("Listening on Network Interface: %s Network Port: %d \n", IP_ADDRESS, SERVER_PORT);
 
  while (TRUE) 
  {
    printf("Waiting for client connection...\n");
    
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int client_sock_desc = accept(server_sock_desc, (struct sockaddr *)&client_endpoint, &sock_len);

    char *client_ip = inet_ntoa(client_endpoint.sin_addr);

    printf("Accepted connection: %s:%d\n", client_ip, ntohs(client_endpoint.sin_port));

    pthread_t thread_id;

    if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock_desc) < 0)
    {
        perror("could not create thread");
        return 1;
    }
  }

  close(server_sock_desc);

  return 0;
}

/*
 * Handle client connection
 */
void *connection_handler(void *socket_desc)
{
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  //Get the file descriptor
  int* client_sock_desc_ptr = (int*)socket_desc;
  int client_sock_desc = *client_sock_desc_ptr;
  
  int quit = -1;

  while (quit != 0)
  {
    //Receive message from client
    receive_message(client_sock_desc, receive_buffer, RECEIVE_BUFFER_SIZE);
    printf("Client message received: %s \n", receive_buffer);

    //Echo message back to client
    send_message(client_sock_desc, receive_buffer);

    quit = strcmp(receive_buffer, "quit");
  }

  close(client_sock_desc);
         
  return 0;
} 
