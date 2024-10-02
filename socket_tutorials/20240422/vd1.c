#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main() {
    printf("Before fork pid = %d\n", getpid());
    //tao mot ban sao
    if(fork() == 0){
        sleep(10);
        printf("CHild process pid = %d\n", getpid());
        
    }
    printf("After fork pid = %d\n", getpid()); 
    sleep(5);
    return 0;
}