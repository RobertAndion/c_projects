producer_consumer: main.o producer_consumer.o
	gcc -o producer_consumer main.o producer_consumer.o -lpthread

main.o: main.c producer_consumer.c producer_consumer.h
	gcc -c main.c -lpthread

producer_consumer.o: producer_consumer.c producer_consumer.h
	gcc -c producer_consumer.c
