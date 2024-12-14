#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void *connection_handler(void *socket_desc);

int main() {
  // Disable output buffering for testing
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  int server_fd;
  socklen_t client_addr_len;
  struct sockaddr_in client_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    printf("Socket creation failed: %s...\n", strerror(errno));
    return 1;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    fprintf(stderr, "SO_REUSEADDR failed: %s \n", strerror(errno));
    return 1;
  }

  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(6379),
      .sin_addr = {htonl(INADDR_ANY)},
  };

  if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) {
    fprintf(stderr, "Bind failed: %s \n", strerror(errno));
    return 1;
  }

  int connection_backlog = 5;
  if (listen(server_fd, connection_backlog) != 0) {
    fprintf(stderr, "Listen failed: %s \n", strerror(errno));
    return 1;
  }

  printf("Waiting for a client to connect...\n");
  client_addr_len = sizeof(client_addr);
  pthread_t thread_id;

  int client_fd =
      accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
  while (client_fd) {
    if (pthread_create(&thread_id, NULL, connection_handler,
                       (void *)&client_fd) < 0) {
      perror("Could not create thread");
      return 1;
    }
    printf("connection handler thread created %lu\n", (unsigned long)thread_id);
    client_fd =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
  }
  printf("Client connected\n");

  close(server_fd);

  return 0;
}

void *connection_handler(void *socket_desc) {
  int client_fd = *(int *)socket_desc;

  while (1) {
    int req_size = 2048;
    char request[req_size];
    int bytes_received = recv(client_fd, request, req_size, 0);

    int bytes_to_remove = 8;
    if (bytes_received > 0) {
      char *req = request + bytes_to_remove;
      req[bytes_received] = '\0';
      printf("Received message: `%s`\n", req);

      char message[7] = "+PONG\r\n";
      int sent = send(client_fd, message, 7, 0);
      if (sent < 0) {
        fprintf(stderr, "Could not send response: %s\n", strerror(errno));
      } else {
        printf("bytes sent %d\n", sent);
      }
    } else if (bytes_received == 0) {
      printf("Client did not send any data");
      break;
    } else {
      perror("Could not read from connection");
    }
  }

  return NULL;
}
