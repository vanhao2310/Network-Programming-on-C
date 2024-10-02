#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void *client_proc(void *);

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
    addr.sin_port = htons(8000);

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
        printf("Waiting for new client connect......\n");
        int client = accept(listener, NULL, NULL);
        printf("New client connect: %d\n", client);

        //tao luong moi de giao tiep voi client
        pthread_t tid;
        pthread_create(&tid, NULL, client_proc, &client);
        //khai bao luong o trang thai tach roi 
        pthread_detach(tid);    //-> luong se tu giai phong tai nguyen khi ket thuc
    }

    close(listener);
    return 0;
}

//luong con nhan du lieu tu client va in ra man hinh
void *client_proc(void *arg) {
    //lay thong tin socket client ket noi den
    int client = *(int *)arg;

    char buf[256];
    while (1) {
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) {
            break;
        }

        buf[ret] = 0;
        printf("Received from client %d: %s\n", client, buf);

        //phan tich cu phap
        char cmd[32], format[32], tmp[32];
        int n = sscanf(buf, "%s%s%s", cmd, format, tmp);
        if (n != 2) {
            char *msg = "Sai cu phap, Hay nhap lai.\n";
            send(client, msg, strlen(msg), 0);
        } else {
            if (strcmp(cmd, "GET_TIME") !=  0) {
                char *msg = "Sai cu phap, Hay nhap lai.\n";
                send(client, msg, strlen(msg), 0);
            } else {
                time_t t = time(NULL);
                struct tm *tinfo = localtime(&t);

                if (strcmp(format, "dd/mm/yyyy") == 0) {
                    sprintf(buf, "%02d/%02d/%4d\n", 
                        tinfo->tm_mday, tinfo->tm_mon+1, tinfo->tm_year + 1900);
                    send(client, buf, strlen(buf), 0);
                } else if (strcmp(format, "dd/mm/yy") == 0) {
                    sprintf(buf, "%02d/%02d/%02d\n", 
                        tinfo->tm_mday, tinfo->tm_mon+1, tinfo->tm_year - 100);
                    send(client, buf, strlen(buf), 0);
                } else if (strcmp(format, "mm/dd/yy") == 0) {
                    sprintf(buf, "%02d/%02d/%02d\n", 
                        tinfo->tm_mon+1, tinfo->tm_mday, tinfo->tm_year - 100);
                    send(client, buf, strlen(buf), 0);
                } else if (strcmp(format, "mm/dd/yyyy") == 0) {
                    sprintf(buf, "%02d/%02d/%4d\n", 
                        tinfo->tm_mday, tinfo->tm_mon+1, tinfo->tm_year + 1900);
                    send(client, buf, strlen(buf), 0);
                } else {
                    char *msg = "Format khong duoc ho tro, Hay nhap lai.\n";
                    send(client, msg, strlen(msg), 0);
                    continue;
                }
            }
        }
    } 
    //dong socket client
    close(client);
}