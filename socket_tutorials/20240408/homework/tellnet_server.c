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

    //mang chua thong tin cac client da dang nhap
    int client_sockets[FD_SETSIZE];
    int num_clients = 0;

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

                    if(client >= FD_SETSIZE)
                        close(client);
                    else {
                        //gan socket vao tap fdread
                        FD_SET(client, &fdread);
                        //thong bao
                        printf("New client connected: %d\n", client);
                    }
                }else {
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
                    } 

                    buf[ret] = 0;
                    printf("REceived  from  %d: %s\n", client, buf);

                    //kiem tra xem da login chua
                    int j = 0;
                    for(; j < num_clients; j++){
                        if(client_sockets[j] == client)
                            break;
                    }

                    if(j == num_clients) {
                        //chua dang nhap
                        char user[32], pass[32], tmp[65], line[65];
                        int n = sscanf(buf, "%s %s %s", user, pass, tmp);
                        //kiem tra cu phap gui du lieu
                        if(n != 2) {
                            //gui thua hoac thieu du lieu
                            char *msg = "Sai cu phap. Hay nhap lai.\n";
                            send(client, msg, strlen(msg), 0);
                        } else {
                            //gui dung
                            //kiem tra user va pass
                            //coppy user va pass vao 1 dong de kiem tra voi 1 dong trong file text
                            sprintf(tmp, "%s %s\n", user, pass);
                            FILE *f = fopen("account.txt", "rb");
                            int found = 0;
                            while (fgets(line, sizeof(line), f) != NULL) {
                                if (strcmp(tmp, line) == 0) {
                                    found = 1;
                                    break;
                                }
                            }
                            if (found) {
                                //user pass dung
                                char *msg = "Dang nhap thanh cong\n";
                                send(client, msg, strlen(msg), 0);

                                //luu thong tin da dang nhap
                                client_sockets[num_clients] = client;
                                num_clients++;
                            } else {
                                char *msg = "Dang nhap that bai. Hay nhap lai.\n";
                                send(client, msg, strlen(msg), 0);
                            }
                            //dong file
                            fclose(f);
                        }
                    } else {
                        //da dang nhap
                        char cmd[300];

                        //xoa ki tu xuong dong o cuoi
                        sprintf(cmd, "%s > out.txt", buf);
                        system(cmd);

                        //tra lai  ket qua cho client
                        FILE *f = fopen("out.txt", "rb");
                        while (1) {
                            int n = fread(buf, 1, sizeof(buf), f);  //1 la moi phan tu duoc doc kich thuoc 1 byte -> doc tu file f sizeof(buf) byte
                            if(n <= 0)  //het du lieu
                                break;
                                //gui du lieu cho client
                            send(client, buf, n, 0);
                        }
                        //dong file
                        fclose(f); 
                    }
                }
            }
        }
    }
    return 0;
}