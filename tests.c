#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "tests.h"

#define NUM_THREADS 10

int main(int argc, char **argv) {
  char *test = "Test Number: ";

  printf("%s1\n", test);
  printf("The required screenshots for this testing module are found in the /screenshots directory.\n\n");

  printf("%s2\n", test);
  pthread_t threads[NUM_THREADS];
  for (int i=0; i<NUM_THREADS; i++)
    threads[i] = pthread_create(&threads[i], NULL, &thread_print_prompt, NULL);

  for (int i=0; i<NUM_THREADS; i++)
    pthread_join(threads[i], NULL);
}

void *thread_print_prompt(void *params) {
  printf("I am a thread running.\n");
  return NULL;
}
