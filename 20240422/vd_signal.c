#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>

//khai bao ham callback xu lyy su kien khi tien trinh con ket thuc
void signalHanlder(int signo) {
    int pid = wait(NULL);
    //thong bao chuong trinh con ket thuc
    printf("Child process terminated, pid = %d\n", pid);
}

int main() {
    //dang ki xu ly khi su kien tien trinh con ket thuc
    signal(SIGCHLD, signalHanlder);
    //xu ly tinh huong tien trinh con ket thuc khi tien trinh cha van hoat dong -> trang thai zombie
    if(fork() == 0){
        printf("Child process pid\n");
        sleep(5);
        exit(0);
    }

     // main process
    getchar();
    return 0;
}