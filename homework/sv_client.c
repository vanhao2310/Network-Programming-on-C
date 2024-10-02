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

struct SV{
    char mssv[8];
    char name[256];
    char dateOfBirth[10];
    float avr;
};
typedef struct SV SV;
//argc: number of argument
//agrv[0] file name
//argv[1] IP Address
//argv[2] Port
int main(int argc, char *argv[]){

    //check number argument
    if(argc < 3){
        printf("Argument are missing.\n");
        return 1;
    }

    //tao socket cho client
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1){
        perror("socket() failed: ");
        return 1;
    }

    //khai bao dia chi ip server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));
    

    //connect den server
    if(connect(client, (struct sockaddr *) &servAddr, sizeof(servAddr))){
        perror("Connect to server failed: ");
        return 1;
    }else{
        printf("Connect to server succeeded.\n");
    }

    int mssv;
    char hoten[64];
    unsigned char ngay, thang;
    unsigned short nam;
    float diemttb;

    //gui du lieu den server
    // while(1){
    //     SV sv;

        //truyen ca cau truc gay du thua du lieu cua ten
        printf("Nhap MSSV: "); scanf("%d", &mssv);
        printf("Nhap Ho va ten: "); getchar();  fgets(hoten, sizeof(hoten), stdin); hoten[strlen(hoten) -1] = 0;
        printf("Nhap ngay sinh: "); scanf("%hhu%hhu%hu", &ngay, &thang, &nam); 
        printf("Nhap diem trung binh: "); scanf("%f", &diemttb); 
        // int ret = send(client, &sv, sizeof(sv), 0);
        
        //dong goi du lieu
        char buff[256];
        int pos = 0;

        //coppy mssv
        memcpy(buff, &mssv, sizeof(mssv));
        pos += sizeof(mssv);
        buff[pos++] = ngay; //1 byte => memory copy
        buff[pos++] = thang;

        memcpy(buff + pos, &nam, sizeof(nam));
        pos+= sizeof(nam);
        memcpy(buff+pos, &diemttb, sizeof(diemttb));
        pos+= sizeof(diemttb);

        memcpy(buff+pos, hoten, strlen(hoten));
        pos += strlen(hoten);
        //pos la do dai du lieu can gui

        //gui dang server
        int ret = send(client, buff, pos, 0);


    // }

    close(client);
}