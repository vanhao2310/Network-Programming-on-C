#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//khai bao ct con su li su kien khi ct con ket thuc
void signalHandler(int signo) {
    int status;
    pid_t pid = wait(&status);
    printf("CHild process terminated, pid = %d\nStatus: %d\n", pid, status);
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

    //khai bao xu li su kien, signalHandler dc goi moi khi co mot client dong ket noi
    signal(SIGCHLD, signalHandler);

    while (1) {
        printf("Cho client moi ket noi den...\n");
        int client = accept(listener, NULL, NULL);
        printf("Client moi ket noi den: %d\n", client);

        //tao mot tien trinh moi phuc vu cho client
        if (fork() == 0) {
            //tien trinh con xu ly nhan du lieu tu client va in ra man hinh

            //dong socket listener, ban sao nay duoc tao ra nhung chuong trinh con khong su dung den
            close(listener);

            //nhan du lieu va in ra man hinh
            char buf[256];
            while (1) {
                int ret = recv(client, buf, sizeof(buf), 0);
                if(ret<=0){
                    //dong ket noi
                    break;
                }
                buf[ret] = 0;
                printf("client: %s\n", buf);

            }
            close(client);
            exit(0);
        } 

        //dong socket tu tien trinh cha
        close(client);  
        //-> cac client sau do se cung socket la 4, chỉ là mô tả giống nhau
    }
}