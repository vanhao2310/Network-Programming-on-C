#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == - 1){
        perror("socket failed: ");
        return -1;
    }

    struct sock
}