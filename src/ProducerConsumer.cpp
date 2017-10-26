#include "ProducerConsumer.h"
#include <stdio.h>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *BB = new BoundedBuffer(5);
// time in ms since the producers and consumers have started
int curr_time = 0;
// number of items produced
int produced=0;
int producer_sleep = 0; 
int consumer_sleep = 0;
pthread_mutex_t producer_lock = PTHREAD_MUTEX_INITIALIZER; //lock for access to buffer to provide mutual exclusion


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
	while(produced < items) {
		pthread_mutex_lock(&producer_lock);
		start_time = time;
		usleep(producer_sleep); // sleep for specified time
		pthread_mutex_unlock(&producer_lock);

	}
	usleep(producer_sleep);

	// test code end
	printf("producer done \n");

	pthread_exit(NULL);
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	long t_id = (long) threadID;
	printf("consumer: %ld produced\n", t_id);

	// code here
	usleep(consumer_sleep);

	// end code here
	printf("consumer done\n");
	pthread_exit(NULL);
}
