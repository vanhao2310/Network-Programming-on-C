#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

void *thread_proc(void *);

int main() {
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect() failed");
        return 1;
    }

    //luong con doc du lieu tu ban phim va gui cho server
    pthread_t tid;
    pthread_create(&tid, NULL, thread_proc, &client);


    //luong chihh nhan du lieu tu server
    char buf[256];
    while (1) {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) {
            break;
        }
        buf[ret] = 0;
        puts(buf);
    }
    close(client);
    return 0;
}

//luong con doc tu ban phim gui cho server
void *thread_proc(void *arg) {
    char buf[256];
    int client = *(int *)arg;
    while (1) {
        fgets(buf, sizeof(buf), stdin);
        send(client, buf, strlen(buf), 0);
        if (strncmp(buf, "exit", 4) == 0) {
            break;
        }
    }
    close(client);
}

/*
    để luồng con nhận từ bàn phím
    -> khi nhận được exit => đóng socket và kết thúc luồng con nhưng luồng chính vẫn chạy nhưng k sử dụng đươc socket 

    để luồng con nhận từ server rồi in ra màn hình sẽ hợp lý hơn
    Khi luồng chính nhận từ bàn phím exit sẽ đóng socket và kết thúc ->> luồng con cũng kết thúc

*/

