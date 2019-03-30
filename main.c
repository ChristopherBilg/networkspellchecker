#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "fifo_queue.h"
#include "main.h"
#include "open_listenfd.h"
#include "client.h"

#define DEFAULT_DICTIONARY "words.txt"
#define DEFAULT_LOG_FILE "log.txt"
#define DEFAULT_DELIMITER " \n"
#define DICTIONARY_SIZE 99171
#define BUFFER_SIZE 256
#define DEFAULT_PORT 8010
#define NUM_WORKERS 2
#define NUM_LOGGERS 1

pthread_mutex_t job_buffer_lock;
pthread_mutex_t log_buffer_lock;
pthread_mutex_t log_file_lock;

struct Queue *job_buffer;
struct Queue *log_buffer;

char dictionary_list[DICTIONARY_SIZE][BUFFER_SIZE];

int main(int argc, char **argv) {
  int port;
  char *dictionary;
  FILE *dictionary_fd;

  if(argc >= 3) {
    port = atoi(argv[1]);
    dictionary = argv[2];
  }
  else if (argc == 2) {
    printf("No dictionary file given. Using default dictionary.\n");
    port = atoi(argv[1]);
    dictionary = DEFAULT_DICTIONARY;
  }
  else {
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
  
  // Initialize the fifo_queues for the two C/P buffers
  job_buffer = createQueue();
  log_buffer = createQueue();

  // Initialize the mutex locks
  pthread_mutex_init(&job_buffer_lock, NULL);
  pthread_mutex_init(&log_buffer_lock, NULL);
  pthread_mutex_init(&log_file_lock, NULL);
  
  // Setup the dictionary array
  for (int i=0; i<99171; i++) {
    fgets(dictionary_list[i], sizeof(dictionary_list[i]), dictionary_fd);
  }
  fclose(dictionary_fd);

  for (int i=0; i<99171; i++) {
    int str_len = strlen(dictionary_list[i]);
    if (dictionary_list[i][str_len-1] == '\n')
      dictionary_list[i][str_len-1] = '\0';
  }

  // Setup threads for workers
  pthread_t workers[NUM_WORKERS];
  for (int i=0; i<NUM_WORKERS; i++)
    workers[i] = pthread_create(&workers[i], NULL, &worker_thread, NULL);

  // Setup thread for log file
  pthread_t loggers[NUM_LOGGERS];
  for (int i=0; i<NUM_LOGGERS; i++)
    loggers[i] = pthread_create(&loggers[i], NULL, &logger_thread, NULL);

  // Setup socket
  struct my_client client;
  client.client_size = sizeof(struct sockaddr_in);
  client.recv_buffer[0] = '\0';
  client.connection_socket = open_listenfd(port);
  
  char *conn_success = "Connected to server. Please wait for further instructions.\n";
  while(1) {
    // Accept connection
    client.client_socket = accept(client.connection_socket, (struct sockaddr *)&client.client, &client.client_size);
    if (client.client_socket == -1)
      continue;
    
    printf("Connected to a new client! ");
    printf("Client: %d\n", client.client_socket);
    send(client.client_socket, conn_success, strlen(conn_success), 0);
    
    // Put it into the job queue
    pthread_mutex_lock(&job_buffer_lock);
    enqueue(job_buffer, client, NULL);
    pthread_mutex_unlock(&job_buffer_lock);
  }

  printf("Server on port %d exited successfully.\n", port);

  pthread_mutex_destroy(&job_buffer_lock);
  pthread_mutex_destroy(&log_buffer_lock);
  return EXIT_SUCCESS;
}

void *worker_thread(void *params) {
  char *prompt_msg = "Words to be spell checked (separate with a space): ";
  char *close_msg = "You closed the connection with the server.\n";
  char *error_msg = "The message didn't come through correctly. Please send it again.\n";

  while (1) {
    // Safely dequeue connection socket information from job queue
    pthread_mutex_lock(&job_buffer_lock);
    if (job_buffer->queue_size <= 0) {
      pthread_mutex_unlock(&job_buffer_lock);
      continue;
    }
    
    struct Node *job = dequeue(job_buffer);
    pthread_mutex_unlock(&job_buffer_lock);

    // Get the client struct
    struct my_client client = job->client;
    
    // communicate with client through recv()/send()
    while (1) {
      char recv_buffer[BUFFER_SIZE] = "";
      send(client.client_socket, prompt_msg, strlen(prompt_msg), 0);
      client.bytes_returned = recv(client.client_socket, recv_buffer, BUFFER_SIZE, 0);
      
      // Handle the message (error, exit, etc.)
      if (client.bytes_returned <= -1) {
        send(client.client_socket, error_msg, strlen(error_msg), 0);
        continue;
      }
      else if (recv_buffer[0] == 27) {
        send(client.client_socket, close_msg, strlen(close_msg), 0);
        close(client.client_socket);
        break;
      }
      else {
        char *recvd = recv_buffer;
        int bytes = client.bytes_returned;
        recvd[strlen(recvd)-1] = '\0';

        recvd[bytes-2] = '\0';

        char *valid = " WRONG\n";
        for (int i=0; i<DICTIONARY_SIZE; i++)
          if (strcmp(recvd, dictionary_list[i]) == 0)
            valid = " CORRECT\n";

        char *ptr = recvd;
        strcat(ptr, valid);
        printf("%s", ptr);
        
        // send()
        send(client.client_socket, ptr, strlen(ptr), 0);

        // Log buffer enqueue
        pthread_mutex_lock(&log_buffer_lock);
        enqueue(log_buffer, client, ptr);
        pthread_mutex_unlock(&log_buffer_lock);
      }
    }
  }

  return NULL;
}

void *logger_thread(void *params) {
  while(1) {
    // Safely dequeue from the log buffer
    pthread_mutex_lock(&log_buffer_lock);
    if (log_buffer->queue_size <= 0) {
      pthread_mutex_unlock(&log_buffer_lock);
      continue;
    }
    
    char *word = dequeue(log_buffer)->word;
    pthread_mutex_unlock(&log_buffer_lock);

    if (word == NULL)
      continue;

    // Aquire log file lock
    pthread_mutex_lock(&log_file_lock);

    // Safely write to the log file
    FILE *log_file = fopen(DEFAULT_LOG_FILE, "a");
    fwrite(word, sizeof(word[0]), sizeof(word)/sizeof(word[0]), log_file);
    fclose(log_file);

    // Release log file lock
    pthread_mutex_unlock(&log_file_lock);
  }

  return NULL;
}
