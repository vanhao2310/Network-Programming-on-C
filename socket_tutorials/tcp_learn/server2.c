#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<error.h>
#include <arpa/inet.h>  //thu vien de dung ham chuyen doi dang dia chi ip qua lai
#include<unistd.h>   
int main(){
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        perror("socket() failed: ");
        return 1;
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8800);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // gan socket voi dia chi nghe 
    if(bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr))){
        perror("bind() failed: ");
        return 1;
    }

    //chuyen sang trang thai listen
    if(listen(listener, 5)){
        perror("listen() failed: ");
        return 1;
    }

    printf("Dang cho ket noi tu client...........\n");

    //dia chi client
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    while(1){
        int client = accept(listener, (struct sockaddr *) &clientAddr, &clientAddrLen);
        printf("Client moi da ket noi IP: %s Port: %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        char *message = "Hello Client\n";
        int ret = send(client, message, strlen(message), 0);
        if(ret <= 0){
            printf("send(message) falied\n");
            break;
        }else{
            printf("%dbyte are sent to Client\n", ret);
        }

        
        char buffer[256];
        ret = recv(client, buffer, sizeof(buffer), 0);
        if(ret <= 0){
            close(client);
            continue;
        }else{
            buffer[ret] = 0;
            printf("Client: %s\n", buffer);
        }

        char ms[256];
        fflush(stdin);
        fgets(ms, sizeof(ms), stdin);
        ret = send(client, ms, strlen(ms), 0);
        if(ret <= 0){
            printf("send() failed\n");
        }else{
            printf("%d byte are sent to client\n", ret);
        }
        close(client);
    }

    close(listener);
    return 0;
}