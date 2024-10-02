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

    fd_set fdread;

    //mang socket client
    int clients[FD_SETSIZE];    //1024
    int numClients = 0;
    char buf[1024];
    int maxdp;  //socket lon nhat 

    while (1) {
        //khoi tao lai tap fdread
        FD_ZERO(&fdread);

        //gan cac socket vao fdread
        FD_SET(listener, &fdread);
        maxdp = listener;

        for(int i = 0; i < numClients; i++){
            FD_SET(clients[i], &fdread);
            if(clients[i] > maxdp)
                maxdp = clients[i];
        }

        //goi ham select cho su kien
        int ret = select(maxdp+1, &fdread, NULL, NULL, NULL);
        if(ret == -1){
            perror("select failed: ");
            break;
        }
        
        //kiem tra su kien cua socket cho ket noi
        if(FD_ISSET(listener, &fdread)){
            //co ket noi den server
            int client = accept(listener, NULL, NULL);
            if(client >= FD_SETSIZE) {
                //socket lon hon so luong FD_SETSIZE = 1024
                close(client);
            } else {
                clients[numClients] = client;
                numClients++;
                //thong bao co ket noi moi
                printf("CO client moi ket noi den: %d\n", client);
            }
        }

        //kiem tra tung ket noi xem co yeu cau khong
        for(int i = 0; i < numClients; i++){
            if(FD_ISSET(clients[i], &fdread)){
                ret = recv(clients[i], buf, sizeof(buf), 0);
                if(ret <= 0){
                    //ket noi bi ngat
                    //can xoa khoi mang de vong lap sau khong bi loi
                    printf("socket closeed %d\n");
                    
                    //xoa socket tai vi tri i
                    if(i < numClients - 1){
                        clients[i] = clients[numClients - 1]; 
                    }
                    numClients--;
                    i--;
                    continue;
                } else {
                    //in du lieu ra man hinh
                    buf[ret] = 0;
                    printf("client %d send : %s\n", clients[i], buf);
                }
            }
        }
    }
    return 0;
}