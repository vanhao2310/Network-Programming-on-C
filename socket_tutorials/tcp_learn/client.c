/*
    socket() : tao socket
        dien thong tin ve server vao sockaddr_in (neu biet dia chi ip cua server)
                        neu khong biet dia chi ip cua server thi can phan giai ten mien domain.c
    connect() : ket noi den server
    send() : gui du lieu den server
    recv() : nhan du lieu tu server
    close() : dong socket khi truyen nhan ket thuc
*/
#include "socket_library.h"
int main(){
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1){
        printf("socket() failed\n");
        return 1;
    }

    //khai bao dia chi cua server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(9090);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect() : ket noi den server
    /*
        int connect(
            int sockfd,     //socket da dc khoi tao
            const struct sockaddr *addr, // con tro dia chi cua server (can ep kieu tu sockaddr_in sang sockaddr)
            socklen_t addrlen       //do dai cau truc dia chi
        )
        => ham tra ve 0 neu thanh cong 
                        -1 neu gap loi
    */
   if(connect(client, (struct sockaddr *) &servAddr, sizeof(servAddr))){
    perror("connect to server failed: ");
    return 1;
   }else{
    printf("Connect to server succedded\n");
   }

    //gui du lieu sang server
    char buff[256];
    int ret;
    while(1){
        printf("Ban muon gui gi den server?\n");
        fflush(stdin);
        fgets(buff, sizeof(buff), stdin);
        ret = send(client, buff, strlen(buff), 0);
        if( (ret <= 0) || (buff[0] == '\n') || (strcmp(buff, "exit\n") == 0) ){
            printf("Ket thuc gui du lieu.\n");
            close(client);
            break;
        }
        printf("%d byte are sent to server\n", ret);
    }

    close(client);
}