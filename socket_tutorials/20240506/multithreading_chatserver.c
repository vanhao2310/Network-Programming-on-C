#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>


//can kiem tra trang thai dang nhap
//-> su dung mang luu tru cac client da dang nhap thanh cong
int client_sockets[1024];   //mang luu cac socket cua client da dang nhap thanh cong
char *client_names[1024];    //mang luu id cua cac socket da dang nhap
int num_clients = 0;        //so luong client da dang nhap thanh cong


void remove_client(int);
void *thread_proc(void *);


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

    //luong chinh cho ket noi tu client
    while (1) {
        // printf("Waiting for new client connect......\n");
        int client = accept(listener, NULL, NULL);
        printf("New client connect: %d\n", client);

        //tao luong moi de giao tiep voi client
        pthread_t tid;
        pthread_create(&tid, NULL, thread_proc, &client);
        //khai bao luong o trang thai tach roi 
        pthread_detach(tid);    //-> luong se tu giai phong tai nguyen khi ket thuc
    }

    close(listener);
    return 0;
}

//luong con nhan du lieu tu client va in ra man hinh
void *thread_proc(void *arg) {
    //lay thong tin socket client ket noi den
    int client = *(int *)arg;

    char buf[256];
    while (1) {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) {
            remove_client(client);
            break;
        }

        buf[ret] = 0;
        printf("Received from client %d: %s\n", client, buf);

        //kiem tra trang thai dang nhap
        int i = 0;
        for (; i < num_clients ; i++) {
            if(client == client_sockets[i])
                break;
        }
        if (i == num_clients) {
            //chua dang nhap
            char cmd[16], id[32], tmp[32];
            int ret1 = sscanf(buf, "%s %s %s", cmd, id, tmp);
            if (ret1 == 2) {
                //dung so luong tham so
                if (strcmp(cmd, "client_id:") == 0) {
                    char *msg = "Dung cu phap\n";
                    send(client, msg, strlen(msg), 0);

                    //kiem tra trung id hay khong 
                    int j = 0;
                    for (; j < num_clients; j++) {
                        if (strcmp(id, client_names[j]) == 0)
                            break;
                    }
                    if (j < num_clients) {
                        //trung id
                        char *msg2 = "Id da ton tai. Hay nhap id khac!\n";
                        send(client, msg2, strlen(msg2), 0);
                    } else {
                        //khong trung
                        client_sockets[num_clients] = client;
                        client_names[num_clients] = malloc(strlen(id) + 1);
                        strcpy(client_names[num_clients], id);
                        char *msg2 = "Dang nhap thanh cong!\n";
                        send(client, msg2, strlen(msg2), 0);
                        num_clients++;
                    }
                } else {
                    char *msg = "Sai cu phap. Hay nhap lai\n";
                    send(client, msg, strlen(msg), 0);
                }
            } else {
                char *msg = "Sai cu phap. Hay nhap lai\n";
                send(client, msg, strlen(msg), 0);
            }
        } else {
            //da dang nhap
            //i la chi so cua client gui
            
            //lay ra id nguoi nhan
            char receiver_id[32];
            sscanf(buf, "%s", receiver_id);

            if (strcmp(receiver_id, "all") == 0) {
                //chuyen cho toan bo
                char msg[2048];
                sprintf(msg, "%s: %s", client_names[i], buf + strlen(receiver_id) + 1);
                for (int j = 0; j < num_clients; j++) {
                    if(client_sockets[j] != client) 
                        send(client_sockets[j], msg, strlen(msg), 0);
                }
            } else {
                //chuyen cho mot nguoi 
                //kiem tra xem nguoi nhan co trong mang da luu hay khong
                int j = 0;
                for (; j < num_clients; j++) {
                    if (strcmp(client_names[j], receiver_id) == 0) 
                        break;
                }
                if (j < num_clients) {
                    //nguoi nhan co ton tai
                    char msg[2048];
                    sprintf(msg, "%s: %s", client_names[i], buf + strlen(receiver_id) + 1);
                    send(client_sockets[j], msg, strlen(msg), 0);
                } else {
                    //nguoi nhan khong ton tai
                    char *msg = "Nguoi nhan khong ton tai. hay nhap lai!\n";
                    send(client, msg, strlen(msg), 0);
                }
            }
        }
    } 


    //dong socket client
    close(client);
}

void remove_client(int client) {
    int i = 0;
    for (; i < num_clients; i++) {
        if (client = client_sockets[i])
            break;
    }

    if (i < num_clients) {
        if (i < num_clients - 1) {
            client_sockets[i] = client_sockets[num_clients - 1];
            strcpy(client_names[i], client_names[num_clients - 1]);
        }

        //giai phong bo nho 
        free(client_names[num_clients + 1]);
        num_clients--;
    }
}