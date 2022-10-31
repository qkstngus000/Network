#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "Utilities.h"

#define TRUE 1
#define IP_ADDRESS "0.0.0.0"
#define SERVER_PORT 1234

int starting_prompt(int fd, int counter) {
  if (counter == 0) {
    char message[] = "Would you like to play (y/n)? ";
    send_message(fd, message);
  }
  else {
    char message[] = "Would you like to play again (y/n)? ";
    send_message(fd, message);
  }
  return 0;
}

int status_check(char *receive_buffer) {
  if (strcmp(receive_buffer, "y") == 0) {
    printf("Client response to play the game: %s\n", receive_buffer);
    return 1;
  }
  else if(strcmp(receive_buffer, "n") == 0) {
    printf("Client response to play the game: %s\n", receive_buffer);
    return 0;
  }
  
}

int math_question(int fd) {
  int size = 2;
  int rand_num[size]; 
  
  for (int i = 0; i < size; i++) {
    rand_num[i] = (rand() % 10);
    // printf("%d", rand_num[i]);
  }
  char *message = (char *)malloc(1024 * sizeof(char*));
  sprintf(message, "What is %d times %d: ", rand_num[0], rand_num[1]);
  printf("Question sent to client: %s", message);
  send_message(fd, message);

  int ans = rand_num[0] * rand_num[1];
  free(message);
  return ans;

}

int check_answer(int fd, int ans, int client_ans) {
  if (ans == client_ans) {
    char message[] = "Correct!\n";
    send_message(fd, message);
    return 1;
  }
  else {
    char message[] = "Wrong!\n";
    send_message(fd, message);
    return 0;
  }
}

int send_total_correctiness(int fd, int counter, int ans_count) {
  char *message = (char *)malloc(1024 * sizeof(char*));
  sprintf(message, "Correct: %d, Wrong: %d", ans_count, counter - ans_count);
  printf("Results sent to client: %s\n\n", message);
  send_message(fd, message);
  free(message);
}

int main(int argc, char const *argv[]) 
{
  struct sockaddr_in server;
  struct sockaddr_in client;
  char receive_buffer[RECEIVE_BUFFER_SIZE] = {0};

  int serverFd = create_server_socket(&server, AF_INET, IP_ADDRESS, SERVER_PORT, SOCK_STREAM);

  printf("Listening on Network Interface: %s Network Port: %d \n", IP_ADDRESS, SERVER_PORT);
 
  while (TRUE) 
  {
    printf("Waiting for client connection...\n");
    
    socklen_t sock_len = sizeof(struct sockaddr_in);
    int clientFd = accept(serverFd, (struct sockaddr *)&client, &sock_len);

    char *client_ip = inet_ntoa(client.sin_addr);

    printf("Accepted connection: %s:%d\n", client_ip, ntohs(client.sin_port));

    int quit = -1;
    int counter = 0;
    int ans_count = 0;
    int continued = 1;
    while (continued != 0) {
      // sends starting prompt to client
      starting_prompt(clientFd, counter);

      //Receive message from client
      receive(clientFd, receive_buffer, RECEIVE_BUFFER_SIZE);
      
      // Check if received message is to play game or not
      continued = status_check(receive_buffer);
      
      if (continued != 0) {
        // send math question
        int ans = math_question(clientFd);
        
        // Read client sent answer
        receive(clientFd, receive_buffer, RECEIVE_BUFFER_SIZE);
        printf("%s\n", receive_buffer);
        int client_ans = atoi(receive_buffer);

        // Check for answer if it is right
        ans_count += check_answer(clientFd, ans, client_ans);

        // Check acknowledgement
        receive(clientFd, receive_buffer, RECEIVE_BUFFER_SIZE);
        // printf("%s\n", receive_buffer);  //Log for acknowledgement

        counter++;
      }
      else {
        // Send total right and wrong to client
        send_total_correctiness(clientFd, counter, ans_count);
      }
    }
    

    close(clientFd);
  }

  close(serverFd);

  return 0;
}
