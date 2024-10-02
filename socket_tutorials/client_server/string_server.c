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
//agrv[0] : file name
//argv[1]: port number
int main(int argc, char *argv[]){
    //kiem tra so tham so nhap vao
    if(argc < 2){
        printf("argument is missing\n");
        return 1;
    }

    int port = atoi(argv[1]);
    
    //tao socket lang nghe
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        printf("socket() failed\n");
        return 1;
    }

    //khai bao dia chi Ip cua server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //gan dia chi ip voi socket
    if(bind(listener, (struct sockaddr *) &servAddr, sizeof(servAddr))){
        perror("bind() failed: ");
        return 1;
    }

    //bat dau lang nghe tren cong voi hang doi la 5
    if(listen(listener, 5)){
        perror("listen() failed: ");
        return 1;
    }

    int ret;
    struct sockaddr_in clientAddr;  //dia chi client
    int clientAddrLen;  //do dai dia chi client
    while(1){   //vong lap de nhan ket noi nhieu lan
        printf("Dang cho ket noi........\n");
        int client = accept(listener, (struct sockaddr *) &clientAddr, &clientAddrLen);
        printf("Mot client da ket noi den IP: %s Port: %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        char helloMsg[] = "Hello Client\n";
        ret = send(client, helloMsg, strlen(helloMsg), 0);

        while(1){
            char recvBuff[256];
            char sendBuff[256];
            //nhan tin nhan tu client
            int ret1 = recv(client, recvBuff, sizeof(recvBuff), 0);
            //loi  khong nhan duoc
            if(ret < 0){
                perror("recv() failed: ");
                break;
            }
            if(strncmp(recvBuff, "exit", 4) == 0){
                send(client, "CLOSE!", 6, 0);
                printf("Dong ket noi.\n");
                break;
            }

            //them ki  tu  ket thuc xau neu xau co do dai nho hon kich thuoc buffer
            if(ret1 < sizeof(recvBuff))
                recvBuff[ret1] = 0;
            printf("Client: %s", recvBuff);

            printf("Server: ");
            fflush(stdin);
            fgets(sendBuff, sizeof(sendBuff), stdin);
            int ret2 = send(client, sendBuff, strlen(sendBuff), 0);
            //loi gui 
            if(ret2 < 0){
                perror("send() failed: ");
                break;
            }

        }

        close(client);
    }

}