#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<poll.h>

#define MAX_FDS 256

int main() {
    // Tao socket cho ket noi
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed");
        return 1;
    }

    // Khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    // Gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed");
        return 1;
    }

    // Chuyen socket sang trang thai cho ket noi
    if (listen(listener, 5)) {
        perror("listen() failed");
        return 1;
    }

    //khai bao mang pollfd quan ly cac socket
    struct pollfd fds[MAX_FDS];
    int nfds = 0;


    //gan socket listener vao tap tham do
    fds[0].fd = listener;
    fds[0].events = POLLIN | POLLOUT;
    nfds++;

    char buf[1024];
    while (1) {
        int ret = poll(fds, nfds, 5000);    //neu de timeout = 0 thi se luon tra ve 0, khong su kien nao duoc lang nghe
        if(ret == -1)
            //loi
            break;
        if(ret == 0){
            //time out
            printf("Timeout!\n");
            continue;
        }
        for(int i = 0; i < nfds; i++){
            if(fds[i].revents & POLLIN){
                if(fds[i].fd == listener) {
                    //co ket noi moi -> chap nhan ket noi va cho vao tap tham do
                    int client = accept(listener, NULL, NULL);
                    if(client > MAX_FDS)
                        close(client);
                    else {
                        fds[nfds].fd = client;
                        fds[nfds].events = POLLIN;
                        nfds++;
                        //thong bao co ket noi moi
                        printf("Co client moi ket noi den: %d\n", client);
                    }
                } else {    //khong phai socket listener -> co du lieu tu client gui den
                    //nhan du lieu va in ra man hinh
                    int client = fds[i].fd;
                    ret = recv(client, buf, sizeof(buf), 0);

                    if (ret <= 0) {
                        //ket noi bi dong khong bi bao loi nhu select
                        //xoa ra khoi mang
                    } else {
                        buf[ret] = 0;
                        printf("Client %d : %s\n", client, buf);
                    }
                }

            }
        }
    }

    return 0;
}