#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "fifo_queue.h"

int main(int argc, char **argv);
void *worker_thread(void *params);
void *logger_thread(void *params);

#endif
