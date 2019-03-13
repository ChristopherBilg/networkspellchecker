#include <stdlib.h>
#include "fifo_queue.h"

struct Queue *createQueue() {
  struct Queue *temp = (struct Queue *)malloc(sizeof(struct Queue));
  temp->front = temp->rear = NULL;
  temp->queue_size = 0;
  return temp;
}

void destroyQueue(struct Queue *queue) {
  free(queue);
}

struct Node *createNode(int placeholder) {
  struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
  temp->placeholder = placeholder;
  temp->next = NULL;
  return temp;
}

void enqueue(struct Queue *queue, int placeholder) {
  struct Node *temp = createNode(placeholder);
  queue->queue_size++;

  // If queue is empty, then new node is front and rear both
  if (queue->rear == NULL)
  {
    queue->front = queue->rear = temp;
    queue->queue_size = 1;
  }
  else {
    // Add the new node at the end of queue and change rear
    queue->rear->next = temp;
    queue->rear = temp;
  }
  
  return;
}

struct Node *dequeue(struct Queue *queue) {
  // If queue is empty, return NULL.
  if (queue->front == NULL) {
    queue->queue_size = 0;
    return NULL;
  }

  // Store previous front and move front one node ahead
  struct Node *temp = queue->front;
  queue->front = queue->front->next;
  queue->queue_size--;

  // If front becomes NULL, then change rear also as NULL
  if (queue->front == NULL)
    queue->rear = NULL;
  
  return temp;
}
