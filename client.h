#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#define BUFFER_SIZE 256

struct my_client {
  struct sockaddr_in client;
  int client_size;
  int connection_socket;
  int client_socket;
  int bytes_returned;
  char recv_buffer[BUFFER_SIZE];
};

#endif
