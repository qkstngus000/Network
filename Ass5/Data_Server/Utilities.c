#include "Utilities.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int create_tcp_server_socket(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type)
{
  int socket_desc = socket(sa_family, type, 0);
  int optval = SO_REUSEPORT_OPTION_VALUE;
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));

  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);

  int result = bind(socket_desc, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));
  
  result = listen(socket_desc, 10);

  return socket_desc;
}

int connect_server(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type)
{
  int socket_desc = socket(sa_family, type, 0);

  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);

  int result = connect(socket_desc, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));

  return socket_desc;
}

int send_message(int socket_desc, char * message)
{
  int result = write(socket_desc, message, strlen(message));

  return result;
}

int receive_message(int socket_desc, char * receive_buffer, int buffer_size)
{
  memset(receive_buffer, 0, buffer_size);

  int result = read(socket_desc, receive_buffer, buffer_size);

  return result;
}

int create_udp_server_socket(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type)
{
  int socket_desc = socket(sa_family, type, 0);
  int optval = SO_REUSEPORT_OPTION_VALUE;
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));

  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);

  int result = bind(socket_desc, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in));
 
  return socket_desc;
}

int create_udp_client_socket(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port, int type)
{
  int socket_desc = socket(sa_family, type, 0);
  int optval = SO_REUSEPORT_OPTION_VALUE;
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));

  init_sock_addr_in(sock_addr, sa_family, ip_addr, port);
 
  return socket_desc;
}

int send_udp_message(int socket_desc, struct sockaddr_in* sock_addr,  char * message, int message_size)
{
  int byte_count = sendto(socket_desc, message, message_size, 0, (struct sockaddr *)sock_addr, sizeof(struct sockaddr_in)); 

  return byte_count;
}

int receive_udp_message(int socket_desc, struct sockaddr_in* sock_addr,  char * receive_buffer, int buffer_size)
{
  memset(receive_buffer, 0, buffer_size);
  socklen_t addr_len = sizeof(struct sockaddr_in);
  int byte_count = recvfrom(socket_desc, (char *)receive_buffer, RECEIVE_BUFFER_SIZE, 0, (struct sockaddr *)sock_addr, &addr_len); 

  receive_buffer[byte_count] = '\0'; 

  return byte_count;
}

void init_sock_addr_in(struct sockaddr_in* sock_addr, sa_family_t sa_family, const char * ip_addr, in_port_t port)
{
  memset(sock_addr, 0, sizeof(struct sockaddr_in));

  sock_addr->sin_family = sa_family;
  sock_addr->sin_addr.s_addr = inet_addr(ip_addr);
  sock_addr->sin_port = htons(port);
}

void read_user_input(char* prompt, char* read_buffer, int read_buffer_size)
{
  memset(read_buffer, 0, read_buffer_size);

  printf("%s", prompt);

  char* returnValue = fgets(read_buffer, read_buffer_size, stdin);

  read_buffer[strcspn(read_buffer, "\n")] = 0;
}

int write_message(char* filename, char* client_ip_addr, char* message) {
  time_t t = time(NULL);
  //printf("\n Current date and time is : %s", ctime(&t));
  FILE* fptr;
  fptr = fopen(filename, "a");
  fprintf(fptr, "%s", ctime(&t));
  fprintf(fptr, "%s\n", client_ip_addr);
  fprintf(fptr, "%s\n\n", message);
  fclose(fptr);
return 0;
}
