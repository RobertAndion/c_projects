#include <stdio.h>
#include "barber.h"
#include <pthread.h>

#define CUSTOMER_AMOUNT 10

int main()
{
  customers_waiting_lock_t *rw = malloc(sizeof(customers_waiting_lock_t));
  customers_waiting_lock_init(rw);
  int i = 0;
  pthread_t barber_thread;
  pthread_create(&barber_thread, NULL, barber, rw);
  pthread_t customer_threads;
  for (; i < CUSTOMER_AMOUNT; i++)
  {
    reading_writing(7000);
    pthread_create(&customer_threads, NULL, customer, rw);
  }

  pthread_join(barber_thread, NULL);

  return 0;
}