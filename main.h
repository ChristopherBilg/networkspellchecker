#ifndef MAIN_H
#define MAIN_H

// Function definitions for main, worker, and logger threads
int main(int argc, char **argv);
void *worker_thread(void *params);
void *logger_thread(void *params);

#endif
