#include<stdio.h>
#include<time.h>
#include<pthread.h>

#define NUM_THREAD 8    //number of thread

double local_sum[NUM_THREAD];   //sum of each thread
int num_step = 1000000000;
double step;

void *thread_proc(void*);

int main() {
    step = 1.0/num_step;

    //mang tid de quan ly
    pthread_t tids[NUM_THREAD];
    //mang tham so truyen vao luong con 
    int param[NUM_THREAD];

    //tao luong con
    for (int i = 0; i < NUM_THREAD; i++) {
        param[i] = i;
        pthread_create(&tids[i], NULL, thread_proc, &param[i]);
    }

    //cho doi cac luong con thuc hien xong
    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(tids[i], NULL);
    }
    
    double sum = 0;
    for(int i = 0; i < NUM_THREAD; i++)
        sum += local_sum[i];

    double pi = sum *step;
    printf("PI = %.10f\n", pi);
    return 0;
}

void *thread_proc(void* args) {
    int index = *(int*) args;
    double csum = 0;
    double x;
    for (int i = index; i < num_step; i+= NUM_THREAD) {
        x = (i + 0.5) * step;
        csum += 4.0 / (1.0 + x*x);
    }
    local_sum[index] = csum;
}
