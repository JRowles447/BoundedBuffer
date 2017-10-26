#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int N){
	//TODO: constructor to initiliaze all the varibales declared in BoundedBuffer.h
	buffer = new int[N];
	buffer_size = N;
	buffer_cnt = 0;
	buffer_last = 0;

	pthread_mutex_init(&buffer_lock, NULL);
	pthread_cond_init(&buffer_full, NULL);
	pthread_cond_init(&buffer_empty, NULL);
}


//TODO: DEAL WITH SIGNALLING EMPTY/ FULL BUFFER TO GET THE CONDITIONS
void BoundedBuffer::append(int data){
	//TODO: append a data item to the circular buffer
	pthread_mutex_lock(&buffer_lock);
	// the buffer is full, wait for the buffer to not be full
	while(buffer_cnt==buffer_size){ 
		pthread_cond_wait(&buffer_full, &buffer_lock);
	}
	// nothing is in the buffer so the tail is the head
	int new_tail = 0;
	if(buffer_cnt == 0){
		new_tail = buffer_last;
	}
	// buffer has space now, circular (wrap around)
	else {
		new_tail = (buffer_last + 1) % buffer_size;
		while(new_tail < 0){
			new_tail += buffer_size;
		}
	} 
	buffer[new_tail] = data; 
	buffer_last = new_tail; 
	buffer_cnt++;

	cout << "after appending: " << data << endl;
	return_info();

	pthread_cond_signal(&buffer_empty);
	pthread_mutex_unlock(&buffer_lock);

}

int BoundedBuffer::remove(){
	//TODO: remove and return a data item from the circular buffer
	pthread_mutex_lock(&buffer_lock);
	// wait until the buffer has something to remove
	while(buffer_cnt == 0){
		pthread_cond_wait(&buffer_empty, &buffer_lock);
	}
	
	int head = ((buffer_last - buffer_cnt + 1) % buffer_size);
	// for cases where the result is negative since c++ % op not mathematical mod
	while(head < 0) {
		head += buffer_size;
	}
	
	int ret = buffer[head];
	buffer_cnt--;
	pthread_cond_signal(&buffer_full);
	pthread_mutex_unlock(&buffer_lock);
	cout << "after removing: " << ret << " at location head = " << head << endl;
	return_info();
	return ret;
}

bool BoundedBuffer::isEmpty(){
	//TODO: check is the buffer is empty
	return (buffer_cnt == 0);
}


void BoundedBuffer::return_info(){
	cout << "buffer: " << buffer << endl;
	cout << "buffer_size: " << buffer_size << endl;
	cout << "buffer_cnt: " << buffer_cnt << endl;
	cout << "buffer_last: " << buffer_last << endl << endl;

}
