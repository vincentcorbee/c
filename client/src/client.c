#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BUFF_SIZE 1024
#define PORT 1234
#define HOST "127.0.0.1"

int main(int argc, char *argv[])
{
  char *message = argv[1];

  int clientSocket;

  struct sockaddr_in clientAddress;

  clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (clientSocket == -1)
  {
    perror("Could not create socket\n");

    return EXIT_FAILURE;
  }

  char *host = "127.0.0.1";

  clientAddress.sin_family = AF_INET;
  clientAddress.sin_port = htons(PORT);

  inet_pton(AF_INET, host, &clientAddress.sin_addr.s_addr);

  if (connect(clientSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) == -1)
  {
    perror("Could not connect to socket\n");

    return EXIT_FAILURE;
  }

  char *request;

  request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";

  ssize_t bytesSend = send(clientSocket, request, strlen(request), 0);

  printf("%lu\n", bytesSend);

  char buffer[BUFF_SIZE];

  ssize_t bytesRecieved = recv(clientSocket, buffer, BUFF_SIZE, 0);

  printf("%lu\n", bytesRecieved);

  buffer[bytesRecieved] = '\0';

  printf("%s\n", buffer);

  return 0;
}