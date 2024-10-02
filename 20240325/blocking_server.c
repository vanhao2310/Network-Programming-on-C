/*
server nhan du lieu tu nhieu ket noi
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

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

    int clients[64];
    int numClients = 0;
    char buf[1024];
    while(1){
        printf("Dang cho ket  noi .....\n");
        //phai cho den bao h co client ket noi thi moi tiep tuc
        int client = accept(listener, NULL, NULL);
        clients[numClients] = client;
        numClients++;


        for(int i = 0; i < numClients; i++){
            int ret = recv(clients[i], buf, sizeof(buf), 0);
            if(ret <= 0)
                continue;

            buf[ret] = 0;
            printf("Client %d: %s\n", i, buf);
        }
    }

    close(listener);

    return 1;
}