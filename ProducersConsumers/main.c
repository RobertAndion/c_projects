#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "producer_consumer.h"

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
