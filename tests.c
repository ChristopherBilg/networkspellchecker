#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "tests.h"
#include "fifo_queue.h"
#include "client.h"

#define NUM_THREADS 3
#define QUEUE_SIZE 10

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
}

void *thread_print_prompt(void *params) {
  printf("I am a thread running.\n");
  return NULL;
}
