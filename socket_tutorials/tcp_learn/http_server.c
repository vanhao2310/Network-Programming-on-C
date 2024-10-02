#include "socket_library.h"
int main(){
    //tao socket de server lang nghe
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        printf("socket() failed\n");
        return 1;
    }

    //khai bao dia chi IP cua server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(9000);

    //gan socket voi dia chi ip 
    if(bind(listener, (struct sockaddr *) &servAddr, sizeof(servAddr))){
        perror("bind() failed: ");
        return 1;
    }

    //bat che do lang nghe 
    if(listen(listener, 5)){
        perror("bind() failed: ");
        return 1;
    }

    printf("Dang cho ket noi.......\n");

    int ret;
    struct sockaddr_in clientAddr;  //dia chi cua client 
    int clientAddrLen;      //do dai dia chi cua client
    while(1){
        //chap nhan ket noi cua client
        int client = accept(listener, (struct sockaddr *) &clientAddr, &clientAddrLen);
        printf("Co client ket noi\n IP: %s Port: %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        char msg[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello World</h1></body></html>";
        //gui reponse msg lai cho client
        ret = send(client, msg, strlen(msg), 0);
        if(ret <= 0){
            perror("send msg failed: ");
            close(client);
            break;
        }
        printf("%d byte are sent to client succeeded\n", ret);
        close(client);
    }

    close(listener);
    return 0;
}