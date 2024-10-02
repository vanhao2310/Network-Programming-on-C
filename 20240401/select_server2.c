//khong dung mang ma dung fdread luon de quan ly cac clien ket noi den server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<sys/select.h>

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

    fd_set fdread, fdtest;
    FD_ZERO(&fdread);

    //gan socket lang nghe vao tap fdset
    FD_SET(listener, &fdread);

    char buf[256];

    while (1) {
        fdtest = fdread;
        int ret = select(FD_SETSIZE, &fdtest, NULL, NULL, NULL);
        if(ret == -1){
            perror("select failed: ");
            break;
        }
        
        for(int i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i , &fdtest)){
                if(i == listener) {
                    //co ket noi moi 
                    int client = accept(listener, NULL, NULL);

                    if(client > FD_SETSIZE)
                        close(client);
                    else{
                        //gan socket vao tap fdread
                        FD_SET(client, &fdread);
                        //thong bao
                        printf("New client connected: %d\n", client);
                    }
                } else {
                    //co du lieu truyen den
                    int client = i;
                    ret = recv(client, buf, sizeof(buf), 0);
                    if(ret <= 0) {
                        //dong ket noi
                        printf("client %d closed.\n", client);
                        close(client);
                        //set bit ung voi client ve 0
                        FD_CLR(client, &fdread);
                        continue;
                    } else {
                        buf[ret] = 0;
                        printf("Client %d : %s\n", client, buf);
                    }
                }
            }
        }
    }

}