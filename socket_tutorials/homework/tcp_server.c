/*
2. Viết chương trình tcp_server, đợi kết nối ở cổng xác định bởi tham số dòng lệnh. 
Mỗi khi có client kết nối đến, thì gửi xâu chào được chỉ ra trong một tệp tin xác định, 
sau đó ghi toàn bộ nội dung client gửi đến vào một tệp tin khác được chỉ ra trong tham số dòng lệnh
tcp_server <cổng> <tệp tin chứa câu chào> <tệp tin lưu nội dung client gửi đến>
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


//argv[1] : port
//argv[2] : file welcome
//argv[3] : client send log file
int main(int argc, char *argv[]){
    //check number argument
    if(argc < 4){
        printf("Argument are missing.\n");
        return 1;
    }

    //tao socket lang nghe cho server
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        perror("Socket() failed: ");
        return 1;
    }

    //khai bao dia chi server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    //gan socket voi dia chi server
    if(bind(listener, (struct sockaddr *)&servAddr, sizeof(servAddr))){
        perror("bind() error: ");
        return 1;
    }

    //bat che do lang nghe cho cong
    if(listen(listener, 5)){
        perror("Listen() failed: ");
        return 1;
    }

    printf("Dang doi ket noi.........\n");

    //file chua loi chao gui den client
    FILE *wcFile = fopen(argv[2], "r");
    if(wcFile == NULL){
        printf("Khong ton tai file chua loi chao la %s\n", argv[2]);
        return 1;
    }
    //file ghi du lieu client gui den
    FILE *clientLog = fopen(argv[3], "a");

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    char welcome[256];
    fseek(wcFile, 0, SEEK_END);
    int fileSize = ftell(wcFile);
    fseek(wcFile, 0, SEEK_SET);
    fread(welcome, 1, fileSize, wcFile);
    fseek(clientLog, 0, SEEK_END);

    while(1){
        int client = accept(listener, (struct sockaddr *) &clientAddr, &clientAddrLen);
        int ret = send(client, welcome, strlen(welcome), 0);

        //nhan du lieu tu client va luu vao file
        while(1){
            char buff[256];
            ret = recv(client, buff, sizeof(buff), 0);
            if(ret <= 0){
                perror("Recv(): ");
                break;
            }
            //them ki tu ket thuc xau
            if(ret < sizeof(buff))
                buff[ret] = 0;
            printf("Client: %s\n", buff);
            fwrite(buff, 1, ret, clientLog);
            // fprintf(clientLog, "%s", buff);
        }

        close(client);
    }

    fclose(wcFile);
    fclose(clientLog);
    close(listener);
    return 0;
}