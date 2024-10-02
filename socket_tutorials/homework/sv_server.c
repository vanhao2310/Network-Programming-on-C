/*4. Viết chương trình sv_server, nhận dữ liệu từ sv_client, in ra màn hình và đồng thời ghi vào file sv_log.txt.  
Dữ liệu được ghi trên một dòng với mỗi client, kèm theo địa chỉ IP và thời gian client đã gửi. 
Tham số cổng và tên file log được nhập từ tham số dòng lệnh.
Ví dụ dữ liệu trong file log:
127.0.0.1 2023-04-10 09:00:00 20201234 Nguyen Van A 2002-04-10 3.99
127.0.0.1 2023-04-10 09:00:10 20205678 Tran Van B 2002-08-18 3.50
*/
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
#include<time.h>

struct SV{
    char mssv[8];
    char name[256];
    char dateOfBirth[10];
    float avr;
};
typedef struct SV SV;

//argc: number of argument
//agrv[0] file name
//argv[1] port
//argv[2] file name
int main(int argc, char *argv[]){
    //check number argument
    if(argc < 3){
        printf("Argument are missing.\n");
        return 1;
    }

    //file ghi du lieu client gui den
    FILE *clientLog = fopen(argv[2], "a");


    //tao socket lang nghe cho server
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        perror("Socket() failed: ");
        return 1;
    }

    //khai bao dia chi server
    struct sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(atoi(argv[1]));

    //gan socket voi dia chi server
    if(bind(listener, (struct sockaddr *)&servAddr, sizeof(servAddr))){
        perror("bind() error: ");
        return 1;
    }

    //bat che do lang nghe cho cong
    if(listen(listener, 5)){
        perror("Listen() failed: ");
        return 1;
    }

    printf("Dang doi ket noi.........\n");


    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *) &clientAddr, &clientAddrLen);
    while(1){
        int mssv;
        char hoten[64];
        unsigned char ngay, thang;
        unsigned short nam;
        float diemttb;
        // SV sv;
        char buf[256];
        int ret = recv(client, buf, sizeof(buf), 0);
        if(ret <= 0)
            break;
        time_t rawtime;
        struct tm * timeinfo;
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        char *clientIP = inet_ntoa(clientAddr.sin_addr);
        int pos = 0;
        memcpy(&mssv, buf, sizeof(mssv));
        pos+= sizeof(mssv);
        ngay = buf[pos++]; //1 byte => memory copy
        thang = buf[pos++];

        memcpy(&nam,buf + pos, sizeof(nam));
        pos+= sizeof(nam);
        memcpy(&diemttb, buf+pos, sizeof(diemttb));
        pos+= sizeof(diemttb);

        memcpy(hoten, buf+pos, strlen(hoten));//thieu can sua them
        pos += strlen(hoten);
        // printf("%s %d-%d-%d %.2d:%.2d:%.2d %s %s %s %.2f\n", clientIP, 
        //                                             (timeinfo->tm_yday) + 1900, timeinfo->tm_mon, timeinfo->tm_mday, 
        //                                             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, 
        //                                             sv.mssv, sv.name, sv.dateOfBirth, sv.avr);
        // fprintf(clientLog, "%s %d-%d-%d %.2d:%.2d:%.2d %s %s %s %.2f\n", clientIP, 
        //                                             timeinfo->tm_yday + 1900, timeinfo->tm_mon, timeinfo->tm_mday, 
        //                                             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, 
        //                                             sv.mssv, sv.name, sv.dateOfBirth, sv.avr);
    }
    close(client);
    close(listener);
    return 0;

}