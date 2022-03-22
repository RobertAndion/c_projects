#include "producer_consumer.h"
#include <stdlib.h>
#include <stdio.h>

void *producer(void *args_void)
{
  myargs_t *args = (myargs_t *)args_void;
  while (1)
  {
    sem_wait(&args->buffer_reference->lock);
    if (((args->buffer_reference->end + 1) % args->buffer_reference->size) == args->buffer_reference->start)
    { // Something here is failing.
      sem_post(&args->buffer_reference->lock);
      continue;
    }
    reading_writing();                                                               // Simulate work being done by producer
    args->buffer_reference->buffer[args->buffer_reference->end] = *args->identifier; // Added data to buffer
    args->buffer_reference->end = (args->buffer_reference->end + 1) % args->buffer_reference->size;
    sem_post(&args->buffer_reference->lock);
    break;
  }
  printf("Producer: %d is complete.\n", *args->identifier);

  return NULL;
}

void *consumer(void *args_void)
{
  myargs_t *args = (myargs_t *)args_void;
  while (1)
  {
    sem_wait(&args->buffer_reference->lock);
    if (args->buffer_reference->start == args->buffer_reference->end)
    {
      sem_post(&args->buffer_reference->lock);
      continue; // Wait if empty
    }
    reading_writing(); // Simulate work being done by consumer
    printf("Consumer: %d is complete. Data %d\n", *args->identifier, args->buffer_reference->buffer[args->buffer_reference->start]);
    args->buffer_reference->start = (args->buffer_reference->start + 1) % args->buffer_reference->size;
    sem_post(&args->buffer_reference->lock);
    break;
  }

  return NULL;
}

void initialize_bounded_buffer(bounded_buffer_t *buff)
{
  int i;
  sem_init(&buff->lock, 0, 1);
  buff->size = BUFFER_SIZE;
  buff->start = 0;
  buff->end = 0;
  buff->buffer = malloc(sizeof(int) * BUFFER_SIZE);
  for (i = 0; i < BUFFER_SIZE; ++i)
  {
    buff->buffer[i] = 0; // no data representation
  }
}

long reading_writing() // Function to simulate IO time or data structure access time.
{
  long x = 0;
  int T, i, j;
  T = rand() % 10000;
  for (i = 0; i < T; i++)
    for (j = 0; j < T; j++)
      x = i * j;
  return x;
}
