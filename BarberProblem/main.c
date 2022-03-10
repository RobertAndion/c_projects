#include <stdio.h>
#include <stdlib.h>
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
    // Create customer specific
    customer_wrapper_t *cust = malloc(sizeof(customer_wrapper_t));
    cust->value = malloc(sizeof(int));
    *cust->value = i;
    cust->rw = rw; 
    pthread_create(&customer_threads, NULL, customer, cust); // problem line
  }

  pthread_join(barber_thread, NULL);

  return 0;
}