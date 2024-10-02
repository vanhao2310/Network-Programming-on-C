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

    //tao socket lang nghe cho server
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        perror("socket() failed: ");
        return 1;
    }

    //khai bao dia chi ip cho server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9000);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //gan dia chi voi socket
    if(bind(listener, (struct sockaddr *) &servAddr, sizeof(servAddr))){
        perror("bind() failed: ");
        return 1;
    }
    
    //Bat dau che do lang nghe cua server
    if(listen(listener, 5)){
        perror("Listen() failed: ");
        return 1;
    }

    printf("Dang doi ket noi........\n");

    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    
    //chap nhan ket noi tu client
    int client = accept(listener, (struct sockaddr *) &clientAddr, &clientAddrLen);

    // int n;
    dt sv;
    while(1){
        // int ret = recv(client, &n, sizeof(n), 0);
        int ret = recv(client, &sv, sizeof(sv), 0);
        if(ret <= 0){
            printf("Dong ket noi.\n");
            break;
        }
        printf("Toan: %d\nVan: %d\nAnh: %d\n", sv.toan, sv.van, sv.anh);
    }

    close(client);
    close(listener);

    return 0;
}