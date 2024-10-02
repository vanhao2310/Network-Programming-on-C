/*
Bài 2.
Ứng dụng client đọc nội dung file văn bản và gửi sang server.
Ứng dụng server nhận dữ liệu từ client, in ra màn hình số lần xuất hiện xâu ký tự 
(server không cần tạo file để chứa nội dung nhận được): “0123456789”
Chú ý cần xử lý trường hợp khi xâu “0123456789” nằm giữa 2 lần truyền.
Ví dụ nội dung file văn bản:
SOICTSOICT0123456789012345678901234567890123456789SOICTSOICTSOICT012345678901234567890123456
78901234567890123456789012345678901234567890123456789SOICTSOICT
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1){
        perror("socket() failed: ");
        return -1;
    }

    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(9000);

    if(connect(client, (struct sockaddr *) &servAddr, sizeof(servAddr))){
        perror("connect to server failed: ");
        return -1;
    }
    printf("Connect to server succeeded....\n");

    FILE *f = fopen("homework.txt", "rb");
    if(f == NULL){
        printf("Khong tim thay file.\n");
        return -1;
    }
    while(1){
        
    }
}
