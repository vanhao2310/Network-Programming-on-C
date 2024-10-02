#include<stdio.h>
#include<pthread.h>

//neu can chay 2 luong khac nhau phai tao ra 2 ham khac nhau

void *thread_proc1(void*);


int main(){
    pthread_t tid1;
    int param = 6;
    pthread_create(&tid1, NULL, thread_proc1, &param);
    //neu muon truyen nhieu tham so cung kieu -> dung mang
    //truyen nhieu tham so khac nhau -> dung kieu cau truc de truyen vao
    
    printf("New thread: %lu\n", tid1);

}
void *thread_proc1(void* args) {
    int param = *(int *)args;
    printf("param = %d\n", param);
    printf("Child Thread 1 tid = %lu\n", pthread_self());
}
