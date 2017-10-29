#include "ProducerConsumer.h"
#include <stdio.h>
#include <stdlib.h>     /* rand */
#include <semaphore.h>


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

// locks 
sem_t mutex;
sem_t empty;
sem_t full;



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
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, 1);   // Make some sort of macro something
	sem_init(&full, 0, 0);

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
	long t_id = (long) threadID;
	int sleep_time = 0;
	while(produced < total_items) {
		sleep_time = curr_time;
		usleep(producer_sleep);
		curr_time = sleep_time + producer_sleep;
		sem_wait(&empty);
		sem_wait(&mutex);

		int random = rand();
		// printf("Producer produced = %d \n", random);

		BB->append(random);
		// printf("appending completed\n");
		produced+=1;
		printf("Producer #%ld, time = %d, producing data item #%d, item value=%d\n", t_id, curr_time, produced, random);

		sem_post(&mutex);
		sem_post(&full);

	}

	// test code end
	// printf("producer done ***********************************\n");

	pthread_exit(NULL);
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	long t_id = (long) threadID;
	// printf("consumer: %ld produced\n", t_id);

	// code here ***********************
	int sleep_time = 0;
	while(consumed < total_items) {
		sleep_time = curr_time;
		usleep(consumer_sleep);
		curr_time = sleep_time + consumer_sleep;
		sem_wait(&full);
		sem_wait(&mutex);
		// printf("CONSUMERS HAVE CONSUMED %d/%d ITEMS, %d ITEMS PRODUCED, YUM YUM YUM\n", consumed, total_items-1, produced);
		int removed = BB->remove();
		consumed+=1;
		printf("Consumer #%ld, time = %d, consuming data item with value=%d\n", t_id, curr_time, removed);
		// printf("Consumer removed %d", removed);
		sem_post(&mutex);
		sem_post(&empty);
	}

	// end code here
	// printf("consumer done *********************************\n");
	pthread_exit(NULL);
}
