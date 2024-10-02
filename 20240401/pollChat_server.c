#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<poll.h>

#define MAX_FDS 256

char clientId[MAX_FDS][256];
int hasClientId[MAX_FDS];
char *wrong_syntax = "Ban chua co Id hay nhap lenh gui id voi cu phap clientId: clientName\n";

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
    addr.sin_port = htons(10000);

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
        int ret = poll(fds, nfds, -1);    //neu de timeout = 0 thi se luon tra ve 0, khong su kien nao duoc lang nghe
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
                if(fds[i].fd == listener){
                    //co ket noi moi -> chap nhan ket noi va cho vao tap tham do
                    int client = accept(listener, NULL, NULL);
                    if(client > MAX_FDS)
                        close(client);
                    else{
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
                        if(i < nfds) {
                            fds[i] = fds[nfds-1];
                            strcpy(clientId[fds[i].fd], clientId[fds[nfds-1].fd]);
                            hasClientId[fds[i].fd] = 0;
                            nfds--;
                            i--;
                        }
                    } else {
                        //kiem tra xem co phai client co id chua
                        if(!hasClientId[client]) {
                            //lay thong tin id
                            if(strncmp(buf, "clientId: ", 9) == 0) {
                                strncpy(clientId[client], buf+10, ret-11);
                                hasClientId[client] = 1;
                                // printf("client %d id: %s\n", client, )
                            } else {
                                send(client, wrong_syntax, strlen(wrong_syntax), 0);
                            }
                        } else {
                            //in du lieu ra man hinh
                            buf[ret] = 0;
                            printf("%s: %s\n", clientId[client], buf);

                            //chuyen tiep du lieu cho cac client con lai
                            char bufSend[2048];
                            sprintf(bufSend, "%s: %s", clientId[client], buf);
                            for(int j = 1; j < nfds; j++){
                                if(fds[j].fd != client)
                                    send(fds[j].fd, bufSend, strlen(bufSend), 0);
                            }
                        }
                        
                    }
                }

            }
        }
    }

    return 0;
}