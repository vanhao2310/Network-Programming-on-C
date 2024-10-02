/* thu vien sys/socket.h

    int socket(
        int domain, //giao thuc IPV4: IF_INET, IPV6: IF_INET6
        int type, //kieu socket SOCK_STREAM: TCP, SOCK_DGRAM: UDP
        INT protocol, //giao thuc IPPROTO_TCP || IPPROTO_UDP    (trong thu vien netdb.h)
    )
    => ham tra ve kieu int mo ta socket 
        thanh cong la tra ve int mo ta socket
        that bai tra ve -1 => ma loi dc luu vao bien error (can khai bao them thu vien error.h)

*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<error.h>
int main(){
    //tao socket voi IPV4 va TCP
    int tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(tcpSocket == -1)
        printf("socket() failed\n");
    printf("tcp socket: %d\n", tcpSocket);

    //tao socket udp
    int udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(udpSocket == -1)
        printf("udp socket failed\n");
    printf("udp socket: %d\n", udpSocket);
}