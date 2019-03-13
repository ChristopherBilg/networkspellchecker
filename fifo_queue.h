#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

struct Node {
  int placeholder;
  struct Node *next;
};

struct Queue {
  struct Node *front, *rear;
  int queue_size;
};

struct Queue *createQueue();
void destoryQueue(struct Queue *queue);
struct Node *createNode(int placeholder);
void enqueue(struct Queue *queue, int placeholder);
struct Node *dequeue(struct Queue *queue);

#endif
