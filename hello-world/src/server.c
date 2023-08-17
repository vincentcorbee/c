#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 1234
#define MAX_LINE 4096

int guard(int num, char *msg)
{
  if (num < 0)
  {
    perror(msg);

    exit(EXIT_FAILURE);
  }

  return num;
}

int main()
{
  int server_socket;

  struct sockaddr_in server_address;

  socklen_t server_length = sizeof(server_address);

  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket < 0)
  {
    printf("Could not create socket");

    return EXIT_FAILURE;
  }

  fcntl(server_socket, F_SETFL, O_NONBLOCK);

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(PORT);

  if (bind(server_socket, (struct sockaddr *)&server_address, server_length) < 0)
  {
    printf("Could not bind to port: %d\n", PORT);

    return EXIT_FAILURE;
  }

  if (listen(server_socket, 1) < 0)
  {
    printf("Error while listening\n");

    return EXIT_FAILURE;
  }

  printf("Listening for connections\n\n");

  while (1)
  {
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    char client_addr[MAX_LINE];

    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);

    if (client_socket == -1)
    {
      if (errno == EWOULDBLOCK || errno == EAGAIN)
      {
        usleep(10000);

        continue;
      }

      perror("Could not accept connection");

      exit(EXIT_FAILURE);
    }

    fcntl(client_socket, F_SETFL, O_NONBLOCK);

    inet_ntop(AF_INET, &client_address, client_addr, MAX_LINE);

    printf("Client address: %s\n\n", client_addr);

    printf("Connection accepted\n\n");

    uint8_t request_buffer[MAX_LINE + 1];

    ssize_t bytes_read = recv(client_socket, request_buffer, MAX_LINE, MSG_DONTWAIT);

    if (bytes_read > 0)
    {
      uint8_t response_buffer[MAX_LINE + 1];

      printf("Read %lu bytes\n\n", bytes_read);

      printf("%s\n------\n", request_buffer);

      FILE *file = fopen("./index.html", "r");

      fseek(file, 0L, SEEK_END);

      long size = ftell(file);

      char response[size + 1];

      rewind(file);

      fread(response, size, 1, file);

      response[size] = '\0';

      fclose(file);

      snprintf((char *)response_buffer, sizeof(response_buffer),
               "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: %lu\r\n"
               "\r\n"
               "%s",
               strlen(response), response);

      printf("%s\n", response);

      send(client_socket, (char *)response_buffer, strlen((char *)response_buffer), 0);
    }

    fflush(stdout);

    close(client_socket);
  }
}