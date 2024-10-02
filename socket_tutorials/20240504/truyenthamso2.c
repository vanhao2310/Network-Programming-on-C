#include<stdio.h>
#include<pthread.h>

//neu can chay 2 luong khac nhau phai tao ra 2 ham khac nhau

void *thread_proc1(void*);


int main(){
    pthread_t tid1;
    int param[8];
    for(int i = 0; i < 8; i++) {
        // pthread_create(&tid1, NULL, thread_proc1, &i);
        // /*
        //     do i la bien trong ct chinh, dia chi i la co dinh
        //     luong con lay ra dia chi cua i -> cac luong deu truy nhap vao cung 1 dia chi
        // */

        // int tmp = i;
        // pthread_create(&tid1, NULL, thread_proc1, &tmp);
        // /*
        //     moi vong lap tmp dc cap phat va giai phong khi ket thuc for 
        //     cac bien cung ten se duoc cap phat dia chi giong nhau (trong chuong trinh c)
        // */

        //dung malloc voi moi lan -> can giai phong het khi ket thuc tranh ro ri bo nho

        //dung mang
        param[i] = i;
        pthread_create(&tid1, NULL, thread_proc1, &param[i]);
        printf("New thread tid = %lu, \n", tid1);
    }

    getchar();

}
void *thread_proc1(void* args) {
    int param = *(int *)args;
    printf("Child Thread 1 tid = %lu, param = %d\n", pthread_self(), param);
}
