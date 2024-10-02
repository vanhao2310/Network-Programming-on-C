#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
int main() {
    for (int i = 0; i < 5; i++) {
        if (fork() == 0) { // child process
            int pid = getpid();
            printf("Child process %d started\n", pid);
            int n = 10;
            while (1) {
                sleep(1);
                printf("Child process %d running\n", pid);
            }
        }
    }
    printf("Parent process\n");
    sleep(20);
    printf("Parent done\n");
    killpg(0, SIGKILL);
    return 0;
}