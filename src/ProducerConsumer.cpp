#include "BoundedBuffer.h"
#include "ProducerConsumer.h"
#include <stdio.h>
#include <stdlib.h>     /* rand */
#include <semaphore.h>
#include <iostream>
#include <fstream>

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *BB = new BoundedBuffer(10);

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
	
// file 
ofstream output;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//TODO: How many items are in the buffer? Whats the size????
	producer_sleep = psleep;
	consumer_sleep = csleep;
  	output.open("output.txt");
	total_items = items;
	if(p < 1 || c < 1){
		printf("ERROR; number of producer or consumer threads < 1");
		exit(-1);
	}
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, 10);   
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
	int ind = 0;
	while (ind < p){
		pthread_t thread = (pthread_t)(producers[ind]);
		pthread_join(thread, NULL);
		ind+=1;
	}
	ind = 0;
	while (ind < c){
		pthread_t thread = (pthread_t)(consumers[ind]);
		pthread_join(thread, NULL);
		ind+=1;
	}
	output << "done"<<endl;
	output.close();
   
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
	long t_id = (long) threadID;
	while(produced < total_items) {
		// update the number of produced before sleep so that more producers do not enter loop
		sem_wait(&mutex);
		produced+=1;
		sem_post(&mutex);

		usleep(producer_sleep);

		sem_wait(&empty);
		sem_wait(&mutex);
		curr_time += producer_sleep;

		// generates a random integer
		int random = rand();

		BB->append(random);

		// append to output fstream 
		output << "Producer #" << t_id <<", time = " << curr_time << ", producing data item #" << produced <<", item value=" << random << "\n";

		sem_post(&mutex);
		sem_post(&full);

	}
	pthread_exit(NULL);
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
	long t_id = (long) threadID;
	while(consumed < total_items) {
		// update the number of consumed items before sleep 
		sem_wait(&mutex);
		consumed+=1;
		sem_post(&mutex);

		usleep(consumer_sleep);

		sem_wait(&full);
		sem_wait(&mutex);

		curr_time += consumer_sleep;
		
		int removed = BB->remove();

		// append to output fstream 
		output << "Consumer #" << t_id << ", time = " << curr_time << ", consuming data item with value=" << removed << "\n";
		
		sem_post(&mutex);
		sem_post(&empty);
	}

	pthread_exit(NULL);
}
