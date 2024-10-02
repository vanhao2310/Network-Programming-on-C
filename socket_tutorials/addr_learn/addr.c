//struct sockaddr : mo ta dia chi chung
//struct sockaddr_in: mo ta dia chi ipv4
//struct sockaddr_in6:              ipv6
#include <stdio.h>
#include <stdlib.h>
#include<arpa/inet.h>   //dung 3 thu vien tren de dung sockaddr_in
int main(){
    //khai bao dia chi cho ung dung server
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET; //AF_INET ho dia chi ipv4
    saddr.sin_addr.s_addr = INADDR_ANY; //  INADDR_ANY la bat cu dia chi nao cung duoc
        /*
            sin_addr la kieu struct in_addr: kieu du lieu luu dia chi ip 32 bit
        */
    saddr.sin_port = htons(9000);  //cho o cong 9000
            //sin_port kieu unsigned short int : 16 bit
            //kieu du lieu duoc luu theo kieu dau nho nhung cong luu theo kieu du lieu dau to -> can chuyen doi va dung ham htons
        //vd neu cong la 8000 luu kieu dau nho la 1F40
        //neu khong dung ham htons cong se luu theo kieu dau to la 1F40 va doc thanh 401F = 16415
    //khai bao dia chi ung dung client 

    struct sockaddr_in caddr;
    caddr.sin_family = AF_INET;
    caddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        //inet_addr(<xau dia chi>) : la ham chuyen doi dia chi dang xau sang so nguyen 32bit
        //      => tra ve kieu in_addr : luu dia chi dang int 32bit
    caddr.sin_port = htons(9000);

    /*
        thu vien chuyen doi dia chi <arpa/inet.h>
            chuyen doi xau dia chi sang dang int 32bit: in_addr_t inet_addr( char *cp )
            chuyen doi xau dia chi sang cau truc in_addr int inet_aton( char *cp, struct in_addr *inp) => tra ve 1 neu thanh cong tra ve 0 neu loi
            chuyen doi tu in_addr sang xau ip4: char *inet_ntoa( struct in_addr in )

    */

   /*
        chuyen doi big endian <=> little endian

        htons : 2byte tu little -> big
        htonl : 4byte tu little -> big

        ntohs : 2byte tu big -> little
        ntohl : 4byte tu big -> little
   */


  //neu dia chi cho duoi dang ten mien can phan giai ten mien de lay dia chi ip
  /*   slide 94
    int getaddrinfo(
        const char *nodename,       //ten mien can phan giai
        const char *servname,       //dich vu hoac cong
        const struct addrinfo* hints,   //cau truc goi y
        const addrinfo** res        //ket qua tra ve duoi dang danh sach lien ket e cac ten mien
    )
        =>thanh cong: tra ve 0
        that bai: tra ve ma loi (dung ham gai_strerror() in ra thong bao loi)
  */
}