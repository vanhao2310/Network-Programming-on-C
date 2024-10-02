/*
    socket() : tao socket
    bind() : gan socket vao mot giao dien mang
    listen() : chuyen sang trang thai doi ket noi
            //mac dinh la trang thai blocking: cho bao h co ket noi moi chay tiep
    accept() : chap nhan ket noi
    send()/write() : gui du lieu toi client
    recv()/read() : nhan du lieu tu client
    close() : dong socket 
    shutdown dong ket noi 
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<error.h>
#include <arpa/inet.h>  //thu vien de dung ham chuyen doi dang dia chi ip qua lai
#include<unistd.h>      //thu vien de dung ham write(), read() va close()
int main(){
    //khoi tao socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener == -1){
        printf("listener socket() failed\n");
        return 1;
    }

    //khai bao cau truc dia chi cua server
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(9090);


    //ham bind()
    /*
        int bind(
            int sockfd,                     //mo ta cua socket
            const struct sockaddr *addr,    //con tro cau truc dia chi (can ep kieu tu sockaddr_in 
                                                        hoac sockaddr_in6 sang sockaddr)
            socklen_t addrlen               //do dai cau truc dia chi
        )
        => ham tra ve 0 neu thanh cong 
                        -1 neu bi loi
    */
   //gan dia chi voi socket
   if(bind(listener, (struct sockaddr*) &serv_addr, sizeof(serv_addr))){  //that bai tra ve -1 != 0
        perror("bind() failed: ");
        return 1;
   }

   //ham listen()
   /*
        int listen(
            int fd, //mo ta cua socket
            int n   //chieu dai hang doi cho ket noi
        )
        => tra ve 0 neu thanh cong 
                    -1 neu loi
   */
  //listen
  if(listen(listener, 5)){
    perror("listen() failed: ");
    return 1;
  }
  printf("Dang cho ket noi ........\n");

  //ham accept()
  /*
    int accept(
        int sockfd,  //socket cho ket noi dc dc khoi tao
        struct sockaddr *addr, // con tro tro dia chi client can ep kieu sockaddr_in|sockaddr_in6 -> sockaddr
        socklen_t *addrlen //con tro do dai dia chi client
    )
            //hai tham so cuoi la NULL -> khong lay dia chi cua client
    => ham tra ve mo ta cua socket neu thanh cong 
                    -1 neu loi
    socket tra ve dung lm ket noi truyen nhan du lieu cua client va server

  */
    //accept
    struct sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
        //socket client tao ra ket noi de truyen nhan du lieu
    printf("client moi ket noi: %d - IP: %s Port: %d\n",
        client, inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

    
    //ham send() truyen du lieu tren socket
    /*
        ssize_t send(
            int sockfd,     //socket o trang thai da ket noi (dc tao ra bang lenh accept())
            const void *buf, //buffer chua du lieu can gui
            size_t len,      //so byte can gui
            int flags       //co quy dinh cach truyen (mac dinh la 0)
        )
        => tra ve so byte da gui neu thanh cong
                        -1 neu that bai
    */
    //vd gui di 1 chuoi ki tu 
    char* str = "hello client\n";
    int retSend = send(client, str, strlen(str), 0);
    if(retSend <= 0){
        perror("send() string failed");
        return 1;
    }else{
        printf("%d byte are sent\n", retSend);
    }

    //gui di mang du lieu
    char buf[256];
    int i = 0;
    for(; i<10 ; i++){
        buf[i] = i + 97;    //neu gui cho netcat se chuyen sang ky tu ascii sau do hien thi len man hinh                       
                             //neu de buf[i] = i; chuyen sang ki tu ascii tu 0->10 la cac ki tu dieu khien khong the hien 
                                                    //thi ra man hinh
    }
    buf[i] = '\n';

    retSend = send(client, buf, i+1, 0);  //truyen di i byte
    if(retSend <= 0){
        perror("send() array failed");
        return 1;
    }else{
        printf("%d byte are sent\n", retSend);
    }

    //gui di mot bien kieu double
    double d = 3.141;       //khi gui sang netcat bi chuyen thanh ma ascii -> khong doc duoc so. De doc duoc can tao ung dung clien nhan ve du lieu va sao chep vao bien double de hien thi
    retSend = send(client, &d, sizeof(d), 0);
    if(retSend <= 0){
        perror("send() double failed");
        return 1;
    }else{
        printf("%d byte are sent\n", retSend);
    }


    //ham write()  : tuong tu send() nhung khong co tham so flag
        //can include thu vien unistd.h
    /*
        ssize_t write(
            int fd,     //socket o trang thai da ket noi 
            const void *buf, // buffer chua du lieu can gui
            ssize_t n       //so byte can gui
        )
        => ham tra ve so byte neu gui thanh cong 
                    -1 neu gap loi
    */
    //vd gui di 1 chuoi ki tu voi ham write()
    char *str2 = "\nHello Network Programming\n";
    int retWrite = write(client, str2, strlen(str2));
    if(retWrite <= 0)
        perror("write() string failed: ");
    else    
        printf("%d byte are sent\n", retWrite);


    //Ham recv(): nhan du lieu tu socket
    /*
        ssize_t recv(
            int sockfd,         //socket o trang thai da ket noi (tao boi ham accept)
            void *buf,      //buffer chua du lieu se nhan duoc
            size_t n,       //so byte muon nhan (do dai cua buffer)
            int flags       //co quy  dinh (mac dinh la 0)
        )
        => ham tra ve so byte da nhan neu thanh cong
                        0 neu ket noi bi dong
                        -1 neu gap loi
    */
    //vd 
    char recvBuff[256];
    while(1){
        printf("Dang cho du lieu ......\n");
        int recvRet = recv(client, recvBuff, sizeof(recvBuff), 0);
        if(recvRet <= 0 || recvBuff[0] == '\n'){
            printf("ket noi bi dong hoac loi truyen du lieu\n");
            break;
        }

        //them ky tu ket thuc xau
        if(recvRet < sizeof(recvBuff))      //neu so byte nhan duoc nho hon ki thuoc buffer can tren ki tu ket thuc xau 
            recvBuff[recvRet] = 0;                  //vao cuoi xau de in ra man hinh dung so byte duoc nhan
        
        //in ra du lieu nhan duoc
        printf("so byte nhan duoc: %d\n", recvRet);
        //printf("du lieu nhan duoc: %s\n", recvBuff);
        
        // int n = atoi(recvBuff);
        // printf("So nguyen nhan duoc la: %d\n", n);

        double dd = atof(recvBuff);
        printf("So thuc nhan duoc la: %.10lf\n", dd);
    }


    //ham close(): dong ket noi 
    /*
        int close( 
            int sockfd  //socket can dong ket noi
        )
        => tra e 0 neu thanh cong
                -1 neu loi

        //co the dong bâng ham shutdown()
        int shutdown(
            int sockfd, //socket can dong ket noi
            int how     //cach thuc dong ket noi   
                            | SHUT_RD : khong nhan them du lieu
                            | SHUT_WR : khong truyen them du lieu
                            | SHUT_RDWR : khong truyen va nhan them du lieu (tuong duong voi close(fd) )
        )
        => tra ve 0 neu thanh cong 
                    -1 neu that bai
    */
    close(client);
    close(listener);
}