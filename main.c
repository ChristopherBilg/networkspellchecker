#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "fifo_queue.h"
#include "main.h"
#include "open_listenfd.h"
#include "client.h"

#define DEFAULT_DICTIONARY "words.txt"
#define DEFAULT_LOG_FILE "log.txt"
#define DICTIONARY_SIZE 100000
#define BUFFER_SIZE 256
#define DEFAULT_PORT 8010
#define NUM_WORKERS 2
#define NUM_LOGGERS 1

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dataNotProduced = PTHREAD_COND_INITIALIZER;
pthread_cond_t dataNotConsumed = PTHREAD_COND_INITIALIZER;

int main(int argc, char **argv) {
  int port;
  char *dictionary;
  FILE *dictionary_fd;

  if(argc == 3) {
    port = atoi(argv[1]);
    dictionary = argv[2];
  }
  else if (argc == 2) {
    printf("No dictionary file given. Using default dictionary.\n");
    port = atoi(argv[1]);
    dictionary = DEFAULT_DICTIONARY;
  }
  else if (argc == 1) {
    printf("No port number given to start the server on. Using default port.\n");
    printf("No dictionary file given. Using default dictionary.\n");
    port = DEFAULT_PORT;
    dictionary = DEFAULT_DICTIONARY;
  }

  if (port < 1024 || port > 65535) {
    printf("Port number must be exclusively between 1024 and 65535.\n");
    return EXIT_FAILURE;
  }
  else {
    printf("Starting the server on port %d.\n", port);
  }

  dictionary_fd = fopen(dictionary, "r");
  if (dictionary_fd == NULL) {
    printf("Dictionary given not found. Please give a valid dictionary file name.\n");
    return EXIT_FAILURE;
  }

  // Setup the fifo_queues for the two C/P buffers
  struct Queue *job_buffer = createQueue();
  struct Queue *log_buffer = createQueue();

  // Setup the dictionary array
  char *dictionary_list[DICTIONARY_SIZE];
  char line[BUFFER_SIZE];
  int index = 0;
  while (fgets(line, sizeof(line), dictionary_fd) && index < DICTIONARY_SIZE) {
    if (feof(dictionary_fd))
      break;

    int str_len = strlen(line);
    if (line[str_len-1] == '\n')
      line[str_len-1] = '\0';

    dictionary_list[index] = line;
    index++;
  }
  fclose(dictionary_fd);

  // Setup threads for workers
  pthread_t workers[NUM_WORKERS];
  for (int i=0; i<NUM_WORKERS; i++)
    workers[i] = pthread_create(&workers[i], NULL, &worker_thread, job_buffer);

  // Setup thread for log file
  pthread_t loggers[NUM_LOGGERS];
  for (int i=0; i<NUM_LOGGERS; i++)
    loggers[i] = pthread_create(&loggers[i], NULL, &logger_thread, log_buffer);

  char *conn_success = "Connected to server. Please wait for further instructions.\n";
  while(1) {
    // Setup socket
    struct my_client client;
    client.client_size = sizeof(struct sockaddr_in);
    client.recv_buffer[0] = '\0';
    client.connection_socket = open_listenfd(port);
    if (client.connection_socket == -1)
      continue;

    // Accept connection
    // TODO: THIS MIGHT NOT WORK BECAUSE OF THE WHILE LOOP: TRY NO WHILE LOOP
    client.client_socket = accept(client.connection_socket,
                                  (struct sockaddr *)&client.client,
                                  &client.client_size);
    printf("Connected to a new client!\n");
    send(client.client_socket, conn_success, strlen(conn_success), 0);

    // Put it into the job queue
    enqueue(job_buffer, client, NULL);
  }

  printf("Server on port %d exited successfully.\n", port);
  return EXIT_SUCCESS;
}

void *worker_thread(void *params) {
  while (1) {
    // dequeue connection file descriptor from job queue
    // communicate with client through recv()/send()
    // do the above with the spell check
    // enqueue the result for each spell check into the log queue
    break;
  }
  
  return NULL;
}

void *logger_thread(void *params) {
  while(1) {
    // dequeue item from log buffer (critical section)
    // open log.txt file and write item to file
    // close log.txt file
    break;
  }
  
  return NULL;
}
