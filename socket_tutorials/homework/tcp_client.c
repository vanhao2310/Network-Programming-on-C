/*
1. Viết chương trình tcp_client, kết nối đến một máy chủ xác định bởi địa chỉ IP và cổng. 
Sau đó nhận dữ liệu từ bàn phím và gửi đến server. Tham số được truyền vào từ dòng lệnh có dạng
tcp_client <địa chỉ IP> <cổng>
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<error.h>
#include<arpa/inet.h> 
#include<unistd.h>   
#include<netdb.h>


//argc: number of argument
//agrv[0] file name
//argv[1] IP Address
//argv[2] Port
int main(int argc, char *argv[]){

    //check number argument
    if(argc < 3){
        printf("Argument are missing.\n");
        return 1;
    }

    //tao socket cho client
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1){
        perror("socket() failed: ");
        return 1;
    }

    //khai bao dia chi ip server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    

    //connect den server
    if(connect(client, (struct sockaddr *) &servAddr, sizeof(servAddr))){
        perror("Connect to server failed: ");
        return 1;
    }else{
        printf("Connect to server succeeded.\n");
    }

    //nhan xau chao tu server
    char buff[2048];
    int ret = recv(client, buff, sizeof(buff), 0);
    if(ret <= 0){
        printf("Received welcome message failed.\n");
    }
    if(ret < sizeof(buff))
        buff[ret] = 0;
    printf("Server: %s\n", buff);


    //gui du lieu den server
    while(1){
        printf("Nhap du lieu muon gui den server: ");
        char sendBuff[2048];
        fflush(stdin);
        fgets(sendBuff, sizeof(sendBuff), stdin);
        if(sendBuff[0] == '\n')
            break;
        ret = send(client, sendBuff, strlen(sendBuff), 0);
        if(ret <= 0){
            printf("Send() failed: ");
            break;
        }
    }

    close(client);
    return 0;
}