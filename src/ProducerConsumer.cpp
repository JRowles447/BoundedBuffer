#include "ProducerConsumer.h"
#include <stdio.h>
#include <stdlib.h>     /* rand */


//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *BB = new BoundedBuffer(1);

// time in ms since the producers and consumers have started
int curr_time = 0;

// number of items produced
int produced = 0;
int total_items = 0;
int consumed = 0;

//sleep times
int producer_sleep = 0; 
int consumer_sleep = 0;

pthread_mutex_t produced_lock = PTHREAD_MUTEX_INITIALIZER; //lock for access to buffer to provide mutual exclusion
// pthread_mutex_t producer_lock = PTHREAD_MUTEX_INITIALIZER; //lock for access to buffer to provide mutual exclusion


// use usleep to put the threads to sleep (will wake after the interval? Then wait maybe) 
// have set start time as sys time in ms. add the wait time and subtract for nice numbers 
// that are desired in the output, 
// But bounded buffer also has waiting built in so its probs fine just to deal with the waiting

// two threads:
// one waits and the other waits, when one finishes update the time with the difference of the wait time and the current_time (since nothing was running?)
// do this out on paper to test with one and more threads

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//TODO: How many items are in the buffer? Whats the size????
	producer_sleep = psleep;
	consumer_sleep = csleep;
	total_items = items;
	if(p < 1 || c < 1){
		printf("ERROR; number of producer or consumer threads < 1");
		exit(-1);
	}

	// array of pthreads for producers
	pthread_t producers[p];
	// array of pthreads for consumers
	pthread_t consumers[c];

	long index = 0; 
	int rc;
	// spawn the producers
	while(index < p){
		rc = pthread_create(&producers[index], NULL, producer, (void *)index);
		if(rc) {
			printf("ERROR; return code from pthread_create() for producer is %d\n", rc);
			exit(-1);
		}
		index++;
	}

	index = 0; 
	// spawn the consumers
	while(index < c){
		rc = pthread_create(&consumers[index], NULL, consumer, (void *)index);
		if(rc) {
			printf("ERROR; return code from pthread_create() for consumer is %d\n", rc);
			exit(-1);
		}
		index++;
	}

    pthread_exit(NULL);

}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
	printf("sleep interval is: %d\n",producer_sleep);
	long t_id = (long) threadID;
	printf("producer: %ld produced\n", t_id);

	// code here
	int sleep_time = 0;
	while(produced < total_items) {
		// pthread_mutex_lock(&producer_lock);
		sleep_time = curr_time;

		printf("Producer sleeps at time: %d\n", sleep_time);

		usleep(producer_sleep); // sleep for specified time
		int random = rand();

		pthread_mutex_lock(&produced_lock);
		curr_time = sleep_time + producer_sleep;
		printf("Producer wakes at time: %d with produced = %d \n", curr_time, produced);
		BB->append(random);
		produced+=1;
		printf("Now produced = %d\n", produced);
		pthread_mutex_unlock(&produced_lock);

		printf("Produced item %d\n", random);
		// pthread_mutex_unlock(&producer_lock);
	}
	// test code end
	printf("producer done \n");

	pthread_exit(NULL);
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	long t_id = (long) threadID;
	printf("consumer: %ld produced\n", t_id);

	// code here ***********************

	int sleep_time = 0;
	// while there are still items being produced an in the queue
	while(produced < total_items || consumed != total_items){
		printf("CONSUMERS HAVE CONSUMED %d/%d ITEMS, %d ITEMS PRODUCED, YUM YUM YUM\n", consumed, total_items, produced);
		sleep_time = curr_time;
		printf("Consumer sleeps at time: %d\n", sleep_time);
		usleep(consumer_sleep);
		pthread_mutex_lock(&produced_lock);
		curr_time = sleep_time + consumer_sleep;
		printf("Consumer wakes at time %d\n", curr_time);
		pthread_mutex_unlock(&produced_lock);
		int removed = BB->remove();
		pthread_mutex_lock(&produced_lock);
		consumed+=1;
		pthread_mutex_unlock(&produced_lock);

		printf("Consumed item %d\n", removed);
	}

	// end code here
	printf("consumer done\n");
	pthread_exit(NULL);
}
