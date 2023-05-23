#include<stdio.h>
#include<stdlib.h> 
#include<pthread.h> 
#include<semaphore.h> 
#include <unistd.h>

pthread_mutex_t chopsticks[5]; 
int philosopherid[5]={0,1,2,3,4};

void *philosopher(void* arg) {
	int i = *(int *)arg;
	int left = i;
	int right = (i + 1) % 5;
	while (1) {
		printf("[%d] is thinking ...\n", i);
		sleep(3); 
		if (i % 2 == 0) {
			pthread_mutex_lock(&chopsticks[right]);
			pthread_mutex_lock(&chopsticks[left]);
			printf("[%d] is Eating...\n", i);
			sleep(2);
			pthread_mutex_unlock(&chopsticks[left]);			
			pthread_mutex_unlock(&chopsticks[right]);

		} else {
			pthread_mutex_lock(&chopsticks[left]);
			pthread_mutex_lock(&chopsticks[right]);
			printf("[%d] is Eating...\n", i);
			sleep(2);
			pthread_mutex_unlock(&chopsticks[right]);			
			pthread_mutex_unlock(&chopsticks[left]);

		}
	}
}
 
int main() {
 
	pthread_t id[5];

 
	for (int i = 0; i< 5; i++)
 
		pthread_mutex_init(&chopsticks[i], NULL);
 
	for (int i = 0; i< 5; i++)
		pthread_create(&id[i], NULL, philosopher, &philosopherid[i]);
 
	for (int i = 0; i< 5; i++)
 
		pthread_join(id[i], NULL);
 
}
