
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

    char fileName[] = "test.pdf";
    FILE *f = fopen(fileName, "rb");

    //truyen ten file
    int n = strlen(fileName);   //do dai ten file
    send(client, &n, sizeof(n), 0);
    send(client, fileName, n, 0);

    //kich thuoc file
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    send(client, &fileSize, sizeof(fileSize), 0);

    //truyen noi dung file
    fseek(f, 0, SEEK_SET);
    char buff[2048];
    while(1){
        int ret = fread(buff, 1, sizeof(buff), f);
        if(ret <= 0){
            break;
        }
        send(client, buff, ret, 0);
    }

    fclose(f);
    close(client);
}   