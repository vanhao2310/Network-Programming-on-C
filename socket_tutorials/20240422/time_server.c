/*
Bài 2: Lập trình ứng dụng time_server thực hiện chức năng sau:
+ Chấp nhận nhiều kết nối từ các client.
+ Cient gửi lệnh GET_TIME [format] để nhận thời gian từ server.
+ format là định dạng thời gian server cần trả về client. Các format cần hỗ trợ gồm:
    dd/mm/yyyy – vd: 30/01/2023
    dd/mm/yy – vd: 30/01/23
    mm/dd/yyyy – vd: 01/30/2023
    mm/dd/yy – vd: 01/30/23
+ Cần kiểm tra tính đúng đắn của lệnh client gửi lên server.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>


void signalHandler(int signo) {
    pid_t pid = wait(NULL);
    printf("CHild process terminated, pid = %d\n", pid);
}

int main() {
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket failed: ");
        return 1;
    }
    
    //khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);


    //gan socket voi dia chi
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind failed: ");
        return 1;
    }

    //chuyen sang che do cho ket noi
    if (listen(listener, 5)) {
        perror("listen failed: ");
        return 1;
    }

    //tao server de lang nghe
    char buf[256];
    //khai bao xu li su kien, signalHandler dc goi moi khi co mt client dong ket noi
    signal(SIGCHLD, signalHandler);

    while (1) {
        printf("Cho client moi ket noi den...\n");
        int client = accept(listener, NULL, NULL);
        printf("Client moi ket noi den: %d\n", client);

        if (fork() == 0) {
            //tien trinh con xu ly yeu cau tu client

            //dong socket listener, ban sao nay duoc tao ra nhung chuong trinh con khong su dung den
            close(listener);

            //nhan lenh va gui ket qua cho client
            while (1) {
                int ret = recv(client, buf, sizeof(buf), 0);
                if(ret<=0){
                    //dong ket noi
                    break;
                }
                buf[ret] = 0;
                printf("Received from %d: %s\n", client, buf);

                //xu ly lenh gui tu client
                char comm[10], format[12], tmp[32];
                int a = sscanf(buf, "%s %s %s", comm, format, tmp);
                char repply_msg[32];    //ket qua tra ve cho client
                if (a == 2) {
                    //dung so tham so can gui
                    time_t rawtime;
                    struct tm *timeinfo;
                    time(&rawtime);
                    timeinfo = localtime(&rawtime); 
                    //kiem tra lenh
                    if (strcmp(format, "dd/mm/yyyy") == 0) {
                        strftime(repply_msg, sizeof(repply_msg), "%d/%m/%Y\n", timeinfo);
                    } else if (strcmp(format, "dd/mm/yy") == 0) {
                        strftime(repply_msg, sizeof(repply_msg), "%d/%m/%y\n", timeinfo);
                    } else if (strcmp(format, "mm/dd/yyyy") == 0) {
                        strftime(repply_msg, sizeof(repply_msg), "%m/%d/%Y\n", timeinfo);
                    } else if (strcmp(format, "mm/dd/yy") == 0) {
                        strftime(repply_msg, sizeof(repply_msg), "%m/%d/%y\n", timeinfo);
                    } else {
                        //sai lenh
                        char* msg = "Sai lenh, hay nhap lai!\n";
                        send(client, msg, strlen(msg), 0);
                        continue;
                    }
                } else {
                    char* msg = "Sai cu phap, hay nhap lai!\n";
                    send(client, msg, strlen(msg), 0);
                    continue;
                }
                // Trả lại kết quả cho client
                send(client, repply_msg, strlen(repply_msg), 0);
            }

            close(client);
            exit(0);
        }
        close(client);
    }

    return 0;
}