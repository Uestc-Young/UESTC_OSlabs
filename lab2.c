#include<stdio.h> 
#include<stdlib.h> 
#include<pthread.h> 
#include<semaphore.h>
#include <unistd.h>

pthread_mutex_t mutex;
sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buff[10];
int fileindex = 0;
FILE *fp = NULL;
int producer_id[3]={0,1,2};
int consumer_id[4]={0,1,2,3};

void *producer(void *arg){
    int i = *(int *)arg;
    while (1){
        sleep(1);
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        in++;
        in=in%10;
        fseek(fp, fileindex , SEEK_SET);
        buff[in]=fgetc(fp);
        if(buff[in] ==EOF){
            printf("END OF FILE\n");
            exit(0);
        }
        fileindex++;
        printf("[%d] Producer put an %c in buff %d\n", i , buff[in], in);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

void *consumer(void *arg){
    int i = *(int *)arg;
    while (1){
        sleep(1);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        out++;
        out=out%10;
        printf("[%d] Consumer consume %c in buff %d\n", i , buff[out], out);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(){
    fp = fopen("123.txt","r");
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, 10);
    sem_init(&full, 0, 0);
    pthread_t producerid[3];
    pthread_t consumerid[4];
    for (int i = 0; i < 3; i++){
        pthread_create(&producerid[i], NULL, producer, &producer_id[i]);
    }
    for (int i = 0; i < 4; i++){
        pthread_create(&consumerid[i], NULL, consumer, &consumer_id[i]);
    }
    for (int i = 0; i < 3; i++){
        pthread_join(producerid[i], NULL);
    }
    for (int i = 0; i < 4; i++){
        pthread_join(consumerid[i], NULL);
    }
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    fclose(fp);  
}
