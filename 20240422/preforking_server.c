#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


/*
Mỗi lần có client kết nối đến mới tạo tt mới -> tốn thời gian 
tạo truóc một số lượng nhất định tiến trình để gửi nhận dữ liệu với các client kết nối đến
 tạo sẵn một số giới hạn các process, mỗi
 process sẽ thực hiện lặp lại các công việc: chờ kết
 nối, xử lý yêu cầu và trả kết quả cho client.
 
*/

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

    for(int i = 0; i < 10; i++) {
        if (fork() == 0) {
            //tien trinh con chay nhu 1 server
            while (1) {
                int client = accept(listener, NULL, NULL);
                printf("New client connect: %d\n", client);

                //nhan du lieu tu client 
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0) {
                    continue;
                }
                buf[ret] = 0;
                printf("Received from %d: %s\n", client, buf);
                send(client, buf, strlen(buf), 0);

                close(client);
            }

            exit(0);
        }
    }

    getchar();
    killpg(0, SIGKILL);

    return 0;

}