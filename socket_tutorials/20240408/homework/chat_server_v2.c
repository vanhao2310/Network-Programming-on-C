//khong dung mang ma dung fdread luon de quan ly cac clien ket noi den server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<sys/select.h>

int isLogin[1024];

//xoa 
int remove_client(int client, int *client_sockets, char **client_names, int *num_clients){
    int i = 0;
    for(; i < *num_clients; i++)
        if(client_sockets[i] == client)
            break;
        
    if(i < *num_clients) {
        if(i < *num_clients - 1) {
            client_sockets[i] = client_sockets[*num_clients - 1];
            strcpy(client_names[i], client_names[*num_clients - 1]);
        }

        //giai phong bo nho
        free(client_names[*num_clients - 1]);
        //giam so phan tu di 1
        *num_clients -= 1;
    }

}

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

    char buf[1024];
    int client_sockets[FD_SETSIZE];
    char *client_name[FD_SETSIZE];
    int num_clients = 0;

    while (1) {
        fdtest = fdread;
        int ret = select(FD_SETSIZE, &fdtest, NULL, NULL, NULL);
        if(ret == -1){
            perror("select failed: ");
            break;
        }

        //kiem tra tin hieu tu cac socket
        for(int i = 0; i < FD_SETSIZE; i++){
            if(FD_ISSET(i , &fdtest)){
                if(i == listener) {
                    //co ket noi moi 
                    int client = accept(listener, NULL, NULL);

                    if(client > FD_SETSIZE)
                        close(client);
                    else {
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

                        //xoa thong tin client ra khoi mang da dang nhap
                        remove_client(client, client_sockets, client_name, &num_clients);

                        continue;
                    } else {
                        buf[ret] = 0;
                        printf("Client %d : %s\n", client, buf);
                    }


                    int j = 0;
                    //kiem tra trang thai dang nhap
                    for(; j < num_clients; j++){
                        if(client_sockets[j] == client)
                            break;
                    }
                    if(j == num_clients){
                        //chua dang nhap
                        char cmd[11], id[32], tmp[32];
                        int n = sscanf(buf, "%s %s %s", cmd, id, tmp);
                        //truyen dung 2 tu client_id: va clientName thi dung cu phap
                        //truyen nhieu hon cung sai cu phap

                        if(n == 2) {
                            if(strcmp(cmd, "client_id:") == 0){
                                char *msg = "Dung cu phap.\n";
                                send(client, msg, strlen(msg), 0);
                                int j = 0;
                                for(; j < num_clients; j++){
                                    if(strcmp(id, client_name[j]) == 0)
                                        break;
                                }
                                if(j < num_clients){
                                    //trung id 
                                    char *msg2 = "Id bi trung, hay nhap id khac!\n";
                                    send(client, msg2, strlen(msg2), 0);
                                } else {
                                    char *msg2 = "Dang nhap thanh cong\n";
                                    send(client, msg2, strlen(msg2), 0);
                                    //chuyen sang trang thai dang nhap
                                    client_sockets[num_clients] = client;
                                    client_name[num_clients] = malloc(strlen(id)+1);    //cap phat bo nho de luu ten
                                    strcpy(client_name[num_clients], id);
                                    num_clients++;
                                }
                            } else {
                                char *msg = "Sai cu phap. hay nhao lai.\n";
                                send(client, msg, strlen(msg), 0);
                            }
                        } else {
                            //so tu truyen lon hon 2
                            char *msg = "Sai cu phap. hay nhao lai.\n";
                            send(client, msg, strlen(msg), 0);
                        }
                    } else {
                        //da dang nhap

                        //lay ra id
                        char receiverId[32];
                        sscanf(buf, "%s", receiverId);

                        if(strcmp(receiverId, "all") == 0){
                            //gui cho tat ca nhom
                            //j la id cua nguoi gui
                            //chi gui cho nhung ai da dang nhap
                            char msg[2048];
                            sprintf(msg, "%s: %s", client_name[j], buf + strlen(receiverId) + 1);
                            for(int k = 0 ; k < num_clients; k++){
                                if(client_sockets[k] != client){
                                    send(client_sockets[k], msg, strlen(msg), 0);
                                }
                            }
                        } else {
                            //gui cho 1 client-
                            int k = 0;
                            for(; k < num_clients; k++){
                                if(strcmp(receiverId, client_name[k]) == 0)
                                    break;
                            }
                            if(k < num_clients) {
                                //co id trong mang
                                char msg[2048];
                                sprintf(msg, "%s: %s", client_name[j], buf + strlen(receiverId) + 1);
                                send(client_sockets[k], msg, strlen(msg), 0);
                            } else {
                                //khong co id trong mang
                                char *msg = "Khong ton tai id. hay nhap lai!\n";
                                send(client, msg, strlen(msg), 0);
                            }
                        }

                    }
                }
            }
        }
    }

    return 0;

}