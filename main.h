#ifndef MAIN_H
#define MAIN_H

int main(int argc, char **argv);
void *worker_thread(void *params);
void *logger_thread(void *params);

#endif
