#ifndef FIFO_QUEUE_H
#define FIFO_QUEUE_H

struct Node {
  int fd;
  char *word; // only used if fd value is -1
  struct Node *next;
};

struct Queue {
  struct Node *front, *rear;
  int queue_size;
};

struct Queue *createQueue();
void destoryQueue(struct Queue *queue);
struct Node *createNode(int fd, char *word);
void enqueue(struct Queue *queue, int fd, char *word);
struct Node *dequeue(struct Queue *queue);

#endif
