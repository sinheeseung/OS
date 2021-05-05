#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define Q_SIZE 10
#define P_CNT 6
#define C_CNT 15

pthread_cond_t buffers = PTHREAD_COND_INITIALIZER;
pthread_cond_t items = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

int CQ[Q_SIZE];
int front = 0;
int rear = 0;
int CQ_cnt = 0;

int pc= 1;
int cc= 1;

int getQ(){
    int output = 99;
    if (CQ_cnt == 0){
	printf("Queue is empty.\n");
	return -1;
    }else{
	output = CQ[rear];
	rear = (rear + 1) % 10;
	CQ_cnt--;
	return output;
	}
}

int addQ(int input){
    if(CQ_cnt == 10){
	printf("Queue is full.\n");
	return -1;
    } else{
	CQ[front] = input;
	front = (front+1)%10;
	CQ_cnt++;
	return 1;
    }
}

void *producer(void *arg) {
  int i;
  int id;
  int input;

  id = pc++;
  for (i = 0; i < P_CNT; i++) {
    input = random() % 100;
    usleep(input);
    printf("producer %d add %d\n",id, input);

    pthread_mutex_lock(&buffer_lock);
    while(CQ_cnt == Q_SIZE){
        pthread_cond_wait(&buffers, &buffer_lock);
    }
    addQ(input);
    pthread_cond_signal(&items);
    pthread_mutex_unlock(&buffer_lock);


  }
}

void *consumer(void *arg) {
  int i;
  int id;
  int output;

  id = cc++;
  for (i = 0; i < C_CNT; i++) {
    usleep(random()%100);
    
    pthread_mutex_lock(&buffer_lock);
    while(CQ_cnt == 0){
	    pthread_cond_wait(&items,&buffer_lock);
    }
    output = getQ();
    pthread_cond_signal(&buffers);
    pthread_mutex_unlock(&buffer_lock);
    if (output != -1) 
        printf("consumer %d get Q %d\n", id, output);

  }
}

int main(){
	pthread_t producer_thread[5];
	int producer_id[5];
	pthread_t consumer_thread[2];
	int consumer_id[2];
	
	int status;
	int i;
	
	srand(time(NULL));
	
	for(int i=0;i<5;i++){
		if((producer_id[i] = pthread_create(&producer_thread[i], NULL, producer, NULL)) < 0)
		exit(0);
	}
	
	for(int i=0;i<2;i++){
		if((consumer_id[i] = pthread_create(&consumer_thread[i], NULL, consumer, NULL)) < 0)
		exit(0);
	}
	
	pthread_join(producer_thread[0], (void **)&status);	
	pthread_join(producer_thread[1], (void **)&status);
	pthread_join(producer_thread[2], (void **)&status);
	pthread_join(producer_thread[3], (void **)&status);
	pthread_join(producer_thread[4], (void **)&status);
	pthread_join(consumer_thread[0], (void **)&status);
	pthread_join(consumer_thread[1], (void **)&status);	
	return 0;

}
