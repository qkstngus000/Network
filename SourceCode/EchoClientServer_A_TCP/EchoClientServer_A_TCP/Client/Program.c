#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP_ADDRESS "127.0.0.1"
#define SERVER_PORT 50444
#define RECEIVE_BUFFER_SIZE 1024

const char message[] = "Hello!\n";

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server_endpoint;
  memset(&server_endpoint, 0, sizeof(struct sockaddr_in));

  int socket_desc = socket(AF_INET, SOCK_STREAM, 0);

  server_endpoint.sin_family = AF_INET;
  server_endpoint.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
  server_endpoint.sin_port = htons(SERVER_PORT);

  char *server_ip = inet_ntoa(server_endpoint.sin_addr);

  printf("Connecting to server: %s:%d\n", server_ip, ntohs(server_endpoint.sin_port));

  int result = connect(socket_desc, (struct sockaddr *)&server_endpoint, sizeof(struct sockaddr_in));

  struct sockaddr_in client_endpoint;
  memset(&client_endpoint, 0, sizeof(struct sockaddr_in));
  socklen_t endpoint_length = sizeof(client_endpoint);
  getsockname(socket_desc, (struct sockaddr *)&client_endpoint, &endpoint_length);
  char *client_ip = inet_ntoa(client_endpoint.sin_addr);
  printf("Client Endpoint: %s:%d\n", client_ip, ntohs(client_endpoint.sin_port));

  int byte_count = write(socket_desc, message, strlen(message));

  char receive_buffer[RECEIVE_BUFFER_SIZE];

  memset(receive_buffer, 0, sizeof(receive_buffer));

  byte_count = read(socket_desc, receive_buffer, sizeof(receive_buffer));

  close(socket_desc);

  printf("Server response: %s \n", receive_buffer);

  return 0;
}
