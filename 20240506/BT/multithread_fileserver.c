/*
Sử dụng kỹ thuật đa tiến trình hoặc đa luồng, lập trình ứng dụng file server đơn giản như sau:
Khi client mới được chấp nhận, server sẽ gửi danh sách các file cho client. Các file này trong thư mục được thiết lập 
    trên server. Dòng đầu danh sách là chuỗi “OK N\r\n” nếu có N file trong thư mục. Các tên file phân cách nhau bởi ký 
    tự \r\n, kết thúc danh sách bởi ký tự \r\n\r\n.
Nếu không có file nào trong thư mục thì server gửi chuỗi “ERROR No files to download \r\n” rồi đóng kết nối.
Client gửi tên file để tải về, server kiểm tra nếu file tồn tại thì gửi chuỗi “OK N\r\n” trong đó N là kích thước file, 
    sau đó là nội dung file cho client và đóng kết nối, nếu file không tồn tại thì gửi thông báo lỗi và yêu cầu client 
    gửi lại tên file.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>


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

    //get list file in folder
    DIR *d = opendir(".");
    struct dirent *dir;
    struct stat st;
    char buf[2048];
    int pos = 0;
    int number_file = 0;
    //dem so file

        buf[0] = '\0';
        //lay ten cac file
        if (d != NULL)
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                strcat(buf, dir->d_name);
                strcat(buf, "\r\n");
                number_file++;
            }
        }

        puts(buf);

    //luong chinh cho ket noi tu client
    while (1) {
        // printf("Waiting for new client connect......\n");
        int client = accept(listener, NULL, NULL);
        printf("New client connect: %d\n", client);

        if (number_file == 0) {
            char *msg = "ERROR No files to download \r\n";
            send(client, msg, strlen(msg), 0);
            close(client);
        } else {
            char msg[32];
            sprintf(msg, "OK %d file\r\n", number_file);
            send(client, msg, strlen(msg), 0);
            send(client, buf, strlen(buf), 0);

            //tao luong moi de giao tiep voi client
            pthread_t tid;
            pthread_create(&tid, NULL, client_proc, &client);
            //khai bao luong o trang thai tach roi 
            pthread_detach(tid);    //-> luong se tu giai phong tai nguyen khi ket thuc
        }
    }

    close(listener);
    return 0;
}

void *client_proc(void *arg) {
    //lay thong tin socket client ket noi den
    int client = *(int *)arg;

    char buf[256];
    while (1) {
        //nhan ten file tu client
        int ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0) {
            printf("Client %d has been terminated\n", client);
            break;
        }

        buf[ret] = 0;
        printf("Received from client %d: %s\n", client, buf);
        //xoa ki tu xuong dong cuoi ten file
        if (buf[ret - 1] == '\n') {
            buf[ret - 1] = 0;
        }

        FILE *f = fopen(buf, "rb");
        if(f == NULL) {
            char *msg = "file khong ton tai, hay gui lai ten file\n";
            send(client, msg, strlen(msg), 0);
        } else {
            //lay kich thuoc file
            fseek(f, 0, SEEK_END);
            long file_size = ftell(f);
            fseek(f, 0, SEEK_SET);
            char tmp[32];
            sprintf(tmp, "OK %ld bytes\r\n", file_size);
            send(client, tmp, strlen(tmp), 0);

            char send_buf[1024];
            while (!feof(f)) {
                int ret = fread(buf, 1, sizeof(buf), f);
                if (ret <= 0)
                    break;
                send(client, buf, ret, 0);
            }
            
        }
    } 
    //dong socket client
    close(client);
}

