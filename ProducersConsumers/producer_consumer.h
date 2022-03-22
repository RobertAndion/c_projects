#ifndef __PRODUCER__CONSUMER__H__
#define __PRODUCER__CONSUMER__H__

#include <semaphore.h>
#include <time.h>

#define PRODUCER_COUNT 50
#define CONSUMER_COUNT 45
#define BUFFER_SIZE 10

typedef struct
{
  unsigned int size;
  int *buffer;
  unsigned int start;
  unsigned int end;
  sem_t lock;
} bounded_buffer_t;

typedef struct
{
  int *identifier;
  bounded_buffer_t *buffer_reference;
} myargs_t;

void *producer(void *);
void *consumer(void *);
void initialize_bounded_buffer(bounded_buffer_t *);
long reading_writing();

#endif
