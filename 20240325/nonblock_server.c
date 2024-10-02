//tao server hdd nonblocking
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <error.h>
int main() {
    // Tao socket cho ket noi
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed");
        return 1;
    }

    //chuyen socket sang che do bat dong bo
    unsigned long ul = 1;
    ioctl(listener, FIONBIO, &ul);

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

    int clients[64];    //mang ca client ket noi den server
    int numClients = 0;
    char buf[1024];
    while(1){
        //printf("Dang cho ket  noi .....\n");
        int client = accept(listener, NULL, NULL);
        if(client != -1){
            printf("Co client moi ket noi den: %d\n", client);
            
            //chuyen socket gan voi client do sang che don bat dong bo
            ul = 1;
            ioctl(client, FIONBIO, &ul);

            clients[numClients] = client;
            numClients++;
        }else{
            if (errno == EWOULDBLOCK) {
                // Loi do chua co ket noi
                // Khong xu ly gi them
            } else {
                break;
            }
        }

        //nhan du lieu tu cac client 
        for(int i = 0; i < numClients; i++){
            int ret = recv(clients[i], buf, sizeof(buf), 0);
            if(ret != -1){
                if(ret == 0)
                    continue;

                buf[ret] = 0;
                printf("Client %d: %s\n", i, buf);
            } else {
                //loi nhan
                if(errno == EWOULDBLOCK){
                    //loi do chua co du lieu
                    //khong lam gi ca
                }else
                    continue;
            }

        }
    }

    close(listener);

    return 1;
}

//nhuoc diem ton tai nguyen cpu
//cpu chay lien tuc de hoi 