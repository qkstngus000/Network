#include <stdio.h>      // perror, printf
#include <stdlib.h>     // exit, atoi
#include <unistd.h>     // write, read, close
#include <arpa/inet.h>  // sockaddr_in, AF_INET, SOCK_STREAM, INADDR_ANY
#include <string.h>     // strlen, memset

//https://tf.nist.gov/tf-cgi/servers.cgi

#define SERVER_IP_ADDRESS "132.163.96.5"
#define SERVER_PORT 13
#define RECEIVE_BUFFER_SIZE 100

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in sock_endpoint;
  char receive_buffer[RECEIVE_BUFFER_SIZE];
  
  int sock_desc = socket(AF_INET, SOCK_STREAM, 0);

  sock_endpoint.sin_family = AF_INET;
  sock_endpoint.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
  sock_endpoint.sin_port = htons(SERVER_PORT);

  int result = connect(sock_desc, (struct sockaddr *)&sock_endpoint, sizeof(struct sockaddr_in));

  memset(receive_buffer, 0, RECEIVE_BUFFER_SIZE);
  int byte_count = read(sock_desc, receive_buffer, RECEIVE_BUFFER_SIZE);

  close(sock_desc);

  receive_buffer[byte_count] = '\0'; 

  printf("Day/Time: %s", receive_buffer);
  
  int c = getchar();

  return 0;
}
