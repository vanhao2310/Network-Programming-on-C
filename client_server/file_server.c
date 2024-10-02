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

    //nhan ten file
    int n;  //do dai ten file
    recv(client, &n, sizeof(n), 0);
    printf("Kich thuoc ten file: %d\n", n);
    char fileName[32];
    recv(client, fileName, n, 0);
    if(n < 32)
        fileName[n] = 0;
    printf("Ten file: %s\n", fileName);

    //kich thuoc file
    long fileSize;
    recv(client, &fileSize, sizeof(fileSize),0);
    printf("Kich thuoc file: %ld\n", fileSize);

    //nhan noi dung file
    //con tro file can ghi vao 
    FILE *recvFile = fopen("download.pdf", "ab");   /*
                                                        r: doc text
                                                        rb: cho phep doc o dang binary

                                                        w: ghi o dang nhi phan, ghi de vao noi dung cu
                                                        wb: ghi o dang nhi phan

                                                        a:ghi o dang text(ghi vao sau noi dung hien co)
                                                        ab: ghi o dang nhi nhan(ghi vao sau noi dung hien co)
                                                    */
    //dich con tro den cuoi file
    fseek(recvFile, 0, SEEK_END);

    char buff[2048];
    while(1){
        int ret = recv(client, buff, sizeof(buff), 0);
        if(ret <= 0)
            break;  
        //ghi du lieu vao file
        ret = fwrite(buff, 1, ret, recvFile);
        printf("%d bytes duoc ghi vao file download.pdf.\n", ret);
    }

    fclose(recvFile);
    close(client);
    close(listener);
}