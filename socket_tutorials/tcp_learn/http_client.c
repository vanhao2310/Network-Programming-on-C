#include  "socket_library.h"
//nhap dia chi server vao bang tham so dong lenh
//gui lenh get den va in ra ket qua tra ve
int main(int argc, char **argv){
    //tao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct addrinfo *res;   //dslk luu danh sach cac ten mien dc phan giai
    //phan giai ten mien server
    int ret = getaddrinfo(argv[1], "http", NULL, &res);
    if(ret == -1 || res == NULL){
        perror("domain failed: ");
        return 1;
    }else{
        struct sockaddr_in addr;
        memcpy(&addr, &res->ai_addr, sizeof(addr));
        printf("%s\n", inet_ntoa(addr.sin_addr));
    }

    if(connect(client, res->ai_addr, res->ai_addrlen)){
        perror("Connect to server failed: ");
        return 1;
    }

    //tao thong diep request
    char buff[1024] = "GET /get HTTP/1.1\r\nHost: ";
    strcat(buff, argv[1]);
    strcat(buff, "\r\nConnection: close\r\n\r\n");
    
    //gui request get toi server
    send(client, buff, strlen(buff), 0);
    //lay du lieu tu server tra ve
    ret = recv(client, buff, sizeof(buff),0);
    if(ret <= 0){
        close(client);
        return 1;
    }

    if(ret <  sizeof(buff))
        buff[ret] = 0;
    
    //in ra du lieu
    puts(buff);

    close(client);

}