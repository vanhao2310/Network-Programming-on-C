//chuyen sang bat dong bo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <error.h>
#include <errno.h>

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

    //chuyen socket sang che do non blocking
    unsigned long ul = 1;
    ioctl(client, FIONBIO, &ul);
    
    //chuyen luong nhap du lieu (STDIN) sang che do bat dong bo
    ioctl(STDIN_FILENO, FIONBIO, &ul);


    char buf[256];
    while (1) {

        //neu khong co du lieu tra ve null
       
        //printf("Nhap chuoi ky tu: ");
         //xu ly loi khong co du lieu nhap        
        if(fgets(buf, sizeof(buf), stdin) != NULL){
            send(client, buf, strlen(buf), 0);
            if (strncmp(buf, "exit", 4) == 0)
            break;
        }

        //nhan du lieu tu server va in ra man hinh
        //printf("Dang cho du lieu tu server:\n");
        int ret = recv(client, buf, sizeof(buf), 0);
        //kiem tra nhan du lieu tu server
        if(ret != -1){
            if(ret == 0)
                break;  //ket noi bi dong
            if(ret < sizeof(buf))
            buf[ret] = 0;
            printf("Received: %s", buf);
        } else {
            if(errno == EWOULDBLOCK){
                //loi do khong co du lieu tu server gui den
                //khong lm gi
            }else{
                //khong phai loi ve bat dong bo
                break;  //thoat
            }
        }
        
    
    }

    close(client);

    return 0;
}