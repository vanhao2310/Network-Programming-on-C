#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<sys/types.h>
#include <signal.h>

int main() {
    int pid = fork();
    if(pid == 0){
        printf("Child process pid = %d\n", getpid());
        while (1) {
            sleep(1);
            printf("child process\n");
        }
        exit(0);
    }

    sleep(10);
    kill(pid, SIGKILL); //Neu khong co ham nay sau khi tien trinh cha ket thuc tien trinh con van chay
    printf("parent process done\n");
    return 0;
}