#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "data_structures.h"

void *producer(void *);
void *consumer(void *);
void initialize_bounded_buffer(bounded_buffer_t *);
long reading_writing();

int main()
{
  time_t t;
  srand((unsigned)time(&t));
  int i;
  int T;
  int producers_created = 0, consumers_created = 0;
  bounded_buffer_t *buffer_structure = malloc(sizeof(bounded_buffer_t));
  initialize_bounded_buffer(buffer_structure);
  pthread_t pthread_collection[CONSUMER_COUNT + PRODUCER_COUNT];
  myargs_t *args;
  for (i = 0; i < CONSUMER_COUNT + PRODUCER_COUNT; ++i)
  {
    T = rand() % 100;
    if (producers_created == PRODUCER_COUNT)
    { // Only create consumers
      consumers_created++;
      args = malloc(sizeof(myargs_t));
      args->identifier = malloc(sizeof(int));
      *args->identifier = consumers_created;
      args->buffer_reference = buffer_structure;

      pthread_create(&pthread_collection[i], NULL, consumer, (void *)args);
    }
    else if (consumers_created == CONSUMER_COUNT)
    { // Only create producers
      producers_created++;
      args = malloc(sizeof(myargs_t));
      args->identifier = malloc(sizeof(int));
      *args->identifier = producers_created;
      args->buffer_reference = buffer_structure;

      pthread_create(&pthread_collection[i], NULL, producer, (void *)args);
    }
    else if (T % 2 == 0)
    { // Leave it up to chance, Producer
      producers_created++;
      args = malloc(sizeof(myargs_t));
      args->identifier = malloc(sizeof(int));
      *args->identifier = producers_created;
      args->buffer_reference = buffer_structure;

      pthread_create(&pthread_collection[i], NULL, producer, (void *)args);
    }
    else
    {
      consumers_created++;
      args = malloc(sizeof(myargs_t));
      args->identifier = malloc(sizeof(int));
      *args->identifier = consumers_created;
      args->buffer_reference = buffer_structure;

      pthread_create(&pthread_collection[i], NULL, consumer, (void *)args);
    }
  }
  for (i = 0; i < CONSUMER_COUNT + PRODUCER_COUNT; ++i)
  { // Join on all
    pthread_join(pthread_collection[i], NULL);
  }
  return 0;
}

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
