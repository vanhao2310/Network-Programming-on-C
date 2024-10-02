/*
vd client vừa nhận dữ liệu từ server in ra màn hình và nhận dữ liệu từ bàn phím để gửi cho server
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() 
{
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect() failed");
        return 1;
    }


    char buf[256];
    while (1) {

        //khong the gui hai lan lien tiep ma phai doi 
        printf("Nhap chuoi ky tu: ");
        fgets(buf, sizeof(buf), stdin);

        send(client, buf, strlen(buf), 0);

        if (strncmp(buf, "exit", 4) == 0)
            break;
    
        //nhan du lieu tu server
        printf("Dang cho du lieu tu server:\n");
        int ret = recv(client, buf, sizeof(buf), 0);
        
        if (ret <= 0)
            break;

        if(ret < sizeof(buf))
            buf[ret] = 0;
        printf("Received: %s", buf);
    
    }

    close(client);

    return 0;
}