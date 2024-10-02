#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
//neu can chay 2 luong khac nhau phai tao ra 2 ham khac nhau

void *thread_proc1(void*);

int sum;

int main(){
    pthread_t tid1;

    pthread_create(&tid1, NULL, thread_proc1, NULL);
    printf("New thread created.\n");
    //doi luong con hoan thanh 
    pthread_join(tid1, NULL);
    printf("sum = %d\n", sum);

}
void *thread_proc1(void* args) {
    sum = 0;
    printf("Start computing.\n");
    for(int i = 1; i <= 10; i++){
        sleep(1);
        sum += i*i;
    }
    printf("Done!\n");
}
