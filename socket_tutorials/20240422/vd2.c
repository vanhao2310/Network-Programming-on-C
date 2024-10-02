#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
int main() {
//     if(fork() == 0) {
//         printf("TT con \n");
//         sleep(10);  //-> TT con van chay trong khi tt cha ket thuc
//         printf("TT con ket thuc\n");
//         exit(0);
//     }

//     sleep(2);
//     printf("TT cha ket thuc\n");

    if(fork() == 0) {
        printf("TT con, pid = %d \n", getpid());
        sleep(10);  //-> TT con van chay trong khi tt cha ket thuc
        printf("TT con ket thuc\n");
        exit(0);
    }

    if(fork() == 0) {
        printf("TT con, pid = %d \n", getpid());
        sleep(15);  //-> TT con van chay trong khi tt cha ket thuc
        printf("TT con ket thuc\n");
        exit(0);
    }

    printf("CHo tien trinh con ket thuc\n");
    // pid_t pid = wait(NULL); //dung lai doi cho den khi 1 trong cac tt con ket thuc -> tra ve id cua tien trinh vua ket thuc
    //-> chi cho 1 tt con ket thuc
    //=> muon cho tat ca cac tien trinh con ket thuc can wait nhieu lan
    while(wait(NULL) != -1);

    printf("TT con ket thuc Het \n");
    printf("TT cha ket thuc\n");
}