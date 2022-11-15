#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TRUE 1
#define SERVER_IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 50444
#define RECEIVE_BUFFER_SIZE 1024

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server_endpoint;
  struct sockaddr_in client_endpoint;
  char receive_buffer[RECEIVE_BUFFER_SIZE];

  int server_sock_desc = socket(AF_INET, SOCK_STREAM, 0);

  server_endpoint.sin_family = AF_INET;
  server_endpoint.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
  server_endpoint.sin_port = htons(SERVER_PORT);

  bind(server_sock_desc, (struct sockaddr *)&server_endpoint, sizeof(struct sockaddr_in));
  listen(server_sock_desc, 10);
 
  while (TRUE) 
  {
    printf("Waiting for client connection...\n");
    
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int client_sock_desc = accept(server_sock_desc, (struct sockaddr *)&client_endpoint, &sock_len);

    char *client_ip = inet_ntoa(client_endpoint.sin_addr);

    printf("Accepted connection: %s:%d\n", client_ip, ntohs(client_endpoint.sin_port));

    memset(receive_buffer, 0, sizeof(receive_buffer));

    int size = read(client_sock_desc, receive_buffer, sizeof(receive_buffer));

    printf("Client message: %s \n", receive_buffer);

    write(client_sock_desc, receive_buffer, size);

    close(client_sock_desc);
  }

  close(server_sock_desc);

  return 0;
}
