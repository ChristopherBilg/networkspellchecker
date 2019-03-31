#include <stdlib.h>
#include "fifo_queue.h"

// Create and return the Queue struct
struct Queue *createQueue(int max_size) {
  struct Queue *temp = (struct Queue *)malloc(sizeof(struct Queue));
  temp->front = temp->rear = NULL;
  temp->queue_size = 0;
  temp->max_size = max_size;
  return temp;
}

// Destory a Queue struct in memory
void destroyQueue(struct Queue *queue) {
  free(queue);
}

// Create and return a Node struct, used in Queue struct
struct Node *createNode(struct my_client client, char *word) {
  struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
  temp->client = client;
  temp->word = word;
  temp->next = NULL;
  return temp;
}

// Add a Node struct onto a Queue struct
void enqueue(struct Queue *queue, struct my_client client, char *word) {
  struct Node *temp = createNode(client, word);
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

// Remove a Node struct from a Queue struct
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
