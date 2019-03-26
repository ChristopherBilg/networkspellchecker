#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

#include "client.h"

struct Node {
  struct my_client client;
  char *word;
  struct Node *next;
};

struct Queue {
  struct Node *front, *rear;
  int queue_size;
};

struct Queue *createQueue();
void destroyQueue(struct Queue *queue);
struct Node *createNode(struct my_client client, char *word);
void enqueue(struct Queue *queue, struct my_client client, char *word);
struct Node *dequeue(struct Queue *queue);

#endif
