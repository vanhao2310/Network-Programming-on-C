#include<stdio.h>
#include<pthread.h>

//neu can chay 2 luong khac nhau phai tao ra 2 ham khac nhau

void *thread_proc1(void*);
void *thread_proc2(void*);

int main(){
    pthread_t tid1;
    pthread_create(&tid1, NULL, thread_proc1, NULL);
    printf("New thread: %lu\n", tid1);

    pthread_t tid2;
    pthread_create(&tid2, NULL, thread_proc2, NULL);
    printf("New thread: %lu\n", tid2);
    getchar();
}

void *thread_proc1(void* args) {
    printf("Child Thread 1 tid = %lu\n", pthread_self());
}
void *thread_proc2(void* args) {
    printf("Child Thread 2 tip = %lu\n", pthread_self());
}