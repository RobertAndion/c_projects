#include "barber.h"

void* customer(void * wrv) {
    customer_wrapper_t *customer_info = (customer_wrapper_t *) wrv;
    customers_waiting_lock_t *rw = customer_info->rw; 
    sem_wait(&rw->lock); // aquire structure lock
    if (rw->waitingCount >= WAITING_ROOM_SIZE) { // Room is full
        sem_post(&rw->lock);
        
    } else if (rw->waitingCount == 0) {
        if (!(rw->barberWorking)) {
            rw->barberWorking = 1;
            sem_post(&rw->lock);
            sem_post(&rw->barberSleeping);
        } else {
            rw->waitingCount++;
            sem_post(&rw->lock);
            sem_wait(&rw->customersWaiting);
            printf("Customer %d being cut.\n", customer_info->value);
            sem_post(&rw->barberSleeping); // Wake barber
        }
    } else {
        rw->waitingCount++;
        sem_post(&rw->lock);
        sem_wait(&rw->customersWaiting);
        // Woke up from queue
        printf("Customer %d being cut.\n", customer_info->value);
        sem_post(&rw->barberSleeping); // Wake barber
    }
        
}

void* barber(void * rwv) {
    customers_waiting_lock_t * rw = (customers_waiting_lock_t *) rwv;
    while(1) {
        printf("In barber thread\n");
        sem_wait(&rw->barberSleeping); // Barber is sleeping
        cutHair();
        sem_wait(&rw->lock); //Grab structure
        if(rw->waitingCount > 0) {
            rw->waitingCount--;
            sem_post(&rw->lock);
            sem_post(&rw->customersWaiting); // take in a customer
        } else {
            rw->barberWorking = 0;
            sem_post(&rw->lock);
            sem_wait(&rw->barberSleeping);
        }
    }
}

void cutHair() {
    printf("Barber cutting hair.\n");
    reading_writing(10000);
    printf("Barber done cutting hair.\n");
}

long reading_writing(int waitAmount) // Function to simulate IO time or data structure access time.
{
    long x = 0;
    int T, i, j;
    T = rand() % waitAmount;
    for (i = 0; i < T; i++)
        for (j = 0; j < T; j++)
            x = i * j;
    return x;
}

void customers_waiting_lock_init(customers_waiting_lock_t *rw)
{
    rw->waitingCount = 0;
    rw->barberWorking = 0;
    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->customersWaiting, 0, 1);
    sem_init(&rw->barberSleeping, 0, 0); // Start locked
}