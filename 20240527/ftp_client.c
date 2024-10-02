#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

int main() 
{
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.0.5");
    addr.sin_port = htons(21);

    int ret = connect(client, (struct sockaddr *) &addr, sizeof(addr));
    if (ret == -1) {
        perror("connect() failed");
        return 1;
    }

    char buf[2048];

    // Nhan xau chao
    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
        close(client);
        return 1;
    }

    buf[ret] = 0;
    puts(buf);

    // Dang nhap
    char user[32], pass[32];
    printf("Nhap username: ");
    fgets(user, sizeof(user), stdin);
    printf("Nhap password: ");
    fgets(pass, sizeof(pass), stdin);

    // Xoa ky tu xuong trong
    user[strlen(user) - 1] = 0;
    pass[strlen(pass) - 1] = 0;

    // Gui lenh USER
    sprintf(buf, "USER %s\r\n", user);
    send(client, buf, strlen(buf), 0);

    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
        close(client);
        return 1;
    }

    buf[ret] = 0;
    puts(buf);

    // Gui lenh PASS
    sprintf(buf, "PASS %s\r\n", pass);
    send(client, buf, strlen(buf), 0);

    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
        close(client);
        return 1;
    }

    buf[ret] = 0;
    puts(buf);

    // Kiem tra dang nhap thanh cong
    if (strncmp(buf, "230 ", 4) == 0) {
        puts("Dang nhap thanh cong.");
    } else {
        puts("Dang nhap that bai.");
        close(client);
        return 1;
    }

    // Dang nhap thanh cong. Lay ve danh sach thu muc va tap tin.
    // Gui lenh PASV
    send(client, "PASV\r\n", 6, 0);
    
    //nhan ve dia chi ip cua ftp_server
    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
        close(client);
        return 1;
    }

    buf[ret] = 0;
    puts(buf);
    // 227 Ebtering passive mode (10,13,32,202,201,21)
    // Xu ly ket qua lenh PASV
    // 1. Lay ra dia chi IP
    // char resp[] = "227 Entering Passive Mode (213,229,112,130,216,4)";
    char *pos = strchr(buf, '(');
    char *p1 = strtok(pos, "(),");
    char *p2 = strtok(NULL, "(),");
    char *p3 = strtok(NULL, "(),");
    char *p4 = strtok(NULL, "(),");
    char ip[64];
    sprintf(ip, "%s.%s.%s.%s", p1, p2, p3, p4);
    
    // 2. Lay ra gia tri cong
    char *i1 = strtok(NULL, "(),");
    char *i2 = strtok(NULL, "(),");
    int port = atoi(i1) * 256 + atoi(i2);
    
    printf("IP: %s, port: %d\n", ip, port);

    // 3. Mo ket noi den dia chi tren server
    int client_data = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr_data;
    addr_data.sin_family = AF_INET;
    addr_data.sin_addr.s_addr = inet_addr(ip);
    addr_data.sin_port = htons(port);

    
    ret = connect(client_data, (struct sockaddr *) &addr_data, sizeof(addr_data));
    if (ret == -1) {
        perror("connect() failed");
        return 1;
    }

    //gui lenh LIST len server
    send(client, "LIST\r\n", 6, 0);

    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
        close(client);
        return 1;
    }

    buf[ret] = 0;
    puts(buf);

    //nhan du lieu tu kenh truyen du lieu
    while (1) {
        ret = recv(client_data, buf, sizeof(buf), 0);
        if (ret <= 0) {
            close(client_data);
            break;
        }
        buf[ret] = 0;
        puts(buf);
    }

    //nhan thong bao tu server
    ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
        close(client);
        return 1;
    }

    buf[ret] = 0;
    puts(buf);

    close(client);
    close(client_data);
}