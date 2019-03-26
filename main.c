#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "fifo_queue.h"
#include "main.h"

#define DEFAULT_DICTIONARY "words.txt"
#define DEFAULT_LOG_FILE "log.txt"
#define DICTIONARY_SIZE 1000000
#define BUFFER_SIZE 256
#define DEFAULT_PORT 8010
#define NUM_WORKERS 2

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
  while (fgets(line, sizeof(line), dictionary_fd)) {
    dictionary_list[index] = line;
    index++;
  }
  fclose(dictionary_fd);
  printf("%s\n", dictionary_list[0]);

  // Setup threads for workers
  pthread_t workers[NUM_WORKERS];
  for (int i=0; i<NUM_WORKERS; i++) {
    workers[i] = pthread_create(&workers[i], NULL, &worker_thread, job_buffer);
  }

  // Setup thread for log file
  pthread_t logger;
  logger = pthread_create(&logger, NULL, &logger_thread, log_buffer);

  // while(1) {
  // Setup socket
  // Accept connection
  // Put it into the job queue
  // }
  
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
