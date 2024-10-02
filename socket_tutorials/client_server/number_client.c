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


struct dt{
    int toan;
    int van;
    int anh;
};
typedef struct dt dt;

int main(){

    //tao socket cho client
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1){
        perror("socket() failed: ");
        return 1;
    }

    //khai bao dia chi server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(9000);

    //connect den server
    if(connect(client, (struct sockaddr *) &servAddr, sizeof(servAddr))){
        perror("Connect to server failed: ");
        return 1;
    }else{
        printf("connect to server succeeded.\n");
    }

    // int n;
    dt sv;
    while(1){
        // printf("Nhap so muon gui den server: ");
        // scanf("%d", &n);
        // int ret = send(client, &n, sizeof(n), 0);
        printf("Nhap diem toan: "); scanf("%d", &sv.toan);
        printf("Nhap diem van: "); scanf("%d", &sv.van);
        printf("Nhap diem and: "); scanf("%d", &sv.anh);
        
        int ret = send(client, &sv, sizeof(sv), 0);
        if(ret <= 0){
            perror("Ket noi da bi dong: ");
            break;
        }
        printf("Gui diem thi toi server thanh cong.\n");
    }

    close(client);

    return 0;
}