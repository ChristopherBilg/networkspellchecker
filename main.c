#include "main.h"

#define DEFAULT_DICTIONARY "words.txt"
#define DICTIONARY_SIZE 100000
#define DEFAULT_PORT 8010
#define NUM_WORKERS 2

int main(int argc, char **argv) {
  int port;
  if (argc == 1) {
    printf("No port number given to start the server on.\n");
    printf("Starting the server on port %d.\n", DEFAULT_PORT);
    port = DEFAULT_PORT;
  }
  else {
    port = atoi(argv[1]);
  }

  if (port < 1024 || port > 65535) {
    printf("Port number must be exclusively between 1024 and 65535.\n");
    return EXIT_FAILURE;
  }

  // Setup the fifo_queues for the two C/P buffers
  struct Queue *job_buffer = createQueue();
  struct Queue *log_buffer = createQueue();

  // Setup threads for workers
  pthread_t workers[NUM_WORKERS];
  for (int i=0; i<NUM_WORKERS; i++) {
    workers[i] = pthread_create(&workers[i], NULL, &worker_thread, job_buffer);
  }

  pthread_t logger;
  // Setup thread for log file
  logger = pthread_create(&logger, NULL, &logger_thread, log_buffer);
  
  return EXIT_SUCCESS;
}

void *worker_thread(void *params) {
  return NULL;
}

void *logger_thread(void *params) {
  return NULL;
}
