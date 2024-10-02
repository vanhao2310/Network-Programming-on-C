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


//argc : number of argument
//argv[0]: file name
//argv[1]: server ip address
//argv[2]: port number
int main(int argc, char *argv[]){
    if(argc < 3){
        printf("argument missing\n");
        return 1;
    }
    //khoi tao socket client
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client == -1){
        perror("socket() failed: ");
        return 1;
    }

    //khai bao dia chi server
    struct sockaddr_in  servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    servAddr.sin_port = htons(atoi(argv[2]));

    //ket noi den server
    if(connect(client, (struct sockaddr *)&servAddr, sizeof(servAddr))){
        perror("Connect to server failed.\n");
        return 1;
    }else{
        printf("Connect to server succeeded.\n");
    }


    
    int ret1, ret2;
    while(1){
        char recvBuff[256];
        char sendBuff[256];
        ret1 = recv(client, recvBuff, sizeof(recvBuff), 0);
        if(ret1 < 0){
            perror("recv() failed: ");
            break;
        }
        if(strcmp(recvBuff, "CLOSE!") == 0){
            //tra loi tu server neu gui yeu cau dong ket noi
            printf("DOng ket noi. \n");
            break;
        }

        //them ki tu ket thuc xau vao cuoi neu chuoi nhan dc ngan hon kich thuoc buffer
        if(ret1 < sizeof(recvBuff))
            recvBuff[ret1] = 0;
        printf("Server: %s", recvBuff);

        printf("Client: ");
        fflush(stdin);
        fgets(sendBuff, sizeof(sendBuff), stdin);
        if(sendBuff[0] == '\n'){
            //neu khong gui ki tu nao ma chi bam enter thi gui tin hieu dong ket noi den server
            send(client, "exit", 4, 0);
            break;
        }
        ret2 = send(client, sendBuff, strlen(sendBuff), 0);
        if(ret2 < 0){   //loi gui di 
            perror("Send() failed: ");
            break;
        }

    }

    close(client);

    return 0;
}