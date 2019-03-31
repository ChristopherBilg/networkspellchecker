#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tests.h"
#include "fifo_queue.h"
#include "client.h"

#define NUM_THREADS 3
#define QUEUE_SIZE 10

char dictionary_list[99171][256];

int main(int argc, char **argv) {
  char *test = "Test Number: ";

  printf("%s1\n", test);
  printf("The required screenshots for this testing module are found in the /screenshots directory.\n\n");

  printf("%s2\n", test);
  pthread_t threads[NUM_THREADS+1];
  for (int i=0; i<NUM_THREADS; i++)
    threads[i] = pthread_create(&threads[i], NULL, &thread_print_prompt, NULL);

  for (int i=0; i<NUM_THREADS; i++)
    pthread_join(threads[i], NULL);

  printf("%s3\n", test);
  struct Queue *queue = createQueue(QUEUE_SIZE);
  struct my_client client;
  client.client_size = 0;
  client.recv_buffer[0] = '\0';
  
  for (int i=0; i<QUEUE_SIZE+3; i++)
    enqueue(queue, client, NULL);
  printf("Enqueue of more than QUEUE_SIZE was successful.\n");

  for (int i=0; i<QUEUE_SIZE+3; i++)
    dequeue(queue);
  printf("Dequeue of more than QUEUE_SIZE was successful.\n");

  char input[100];
  printf("Please input a test word for spell checking: ");
  fgets(input, 100, stdin);

  int input_len = strlen(input);
  if (input[input_len-1] == '\n')
    input[input_len-1] = '\0';

  FILE *dictionary_fd = fopen("words.txt", "r");
  for (int i=0; i<99171; i++)
    fgets(dictionary_list[i], sizeof(dictionary_list[i]), dictionary_fd);
  fclose(dictionary_fd);

  for (int i=0; i<99171; i++) {
    int str_len = strlen(dictionary_list[i]);
    if (dictionary_list[i][str_len-1] == '\n')
      dictionary_list[i][str_len-1] = '\0';
  }

  char *valid = " WRONG\n";
  for (int i=0; i<99171; i++)
    if (strcmp(input, dictionary_list[i]) == 0)
      valid = " CORRECT\n";

  printf("%s%s", input, valid);
}

void *thread_print_prompt(void *params) {
  printf("I am a thread running.\n");
  return NULL;
}
