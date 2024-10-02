/*
vd: phan giai ten mien nhap vao tu tham so dong lenh
*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<netdb.h>
#include<arpa/inet.h>
int main(int argc, char *argv[]){
    struct addrinfo *res; //con tro tro den danh sach ket qua
    int ret = getaddrinfo(argv[1], "http", NULL, &res);
        //argv[1]: ten mien can phan truyen boi tham so dong lenh
        //"http" dich vu (cong 80)
        //NULL : cau truc goi y( mac dinh NULL)
        //&res dia chi con tro luu danh sach ket qua
            //=> thanh cong tra ve 0
    
    if(ret != 0){ //loi => tra ve ma loi
        printf("khong phan giai duoc ten mien\n");
        return 1;
    }

    struct addrinfo *p; //con tro duyet danh sach ket qua
    p = res;
    while(p != NULL){
        /*
            cau truc cua addrinfo
            struct addrinfo{
                int ai_flags;   //thuong la AI_CANONNAME
                int ai_family;  //AF_INET: IPV4 | AF_INET6: IPV6 | AF_UNSPEC: khong xac dinh
                int ai_socktype; //SOCK_STREAM: TCP | SOCK_DGRAM: UDP | 0: Khong xac dinh
                int ai_protocol; // IPPROTO_TCP: TCP | IPPROTO_UDP: UDP | 0: Khong xac dinh
                socklen_t ai_addrlen; //chieu dai cua ai_addr
                struct sockaddr* ai_addr; //dia chi socket da duoc phan giai (dia chi chung muon lay phai 
                                                    ep kieu sang sockaddr_in(IPV4) hoac sockaddr_in6(IPV6) )
                struct addrinfo *ai_next; //con tro tro toi addrinfo tiep theo
            }
        */
        if(p->ai_family = AF_INET){ //neu la ho dia chi ipv4
            printf("IPV4\t");
            if(p->ai_socktype == SOCK_STREAM)
                printf("TCP\n");
            else
                printf("UDP\n");
            struct sockaddr_in addr;
            memcpy(&addr, p->ai_addr, p->ai_addrlen);   //coppy dia chi tu p->ai_addr vao addr
            printf("IP: %s\n", inet_ntoa(addr.sin_addr));   //inet_ntoa: ham chuyen tu dia chi ip in_addr_t sang xau

        }else{
            printf("IPV6\n");
        }
        p = p->ai_next;
    }

    freeaddrinfo(res);
}