#ifndef __BARBER__H__
#define __BARBER__H__

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define WAITING_ROOM_SIZE 5

typedef struct {
    sem_t lock; // Lock the structure
    sem_t customersWaiting;
    sem_t barberSleeping;
    int waitingCount;
    int barberWorking;
} customers_waiting_lock_t;

long reading_writing(int);
void *barber(void *);
void *customer(void *);
void customers_waiting_lock_init(customers_waiting_lock_t *);
void cutHair();

#endif