#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

int main(){
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9000);

    int ret = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        perror("connect() failed");
        return 1;
    }

    fd_set fdread;
    FD_ZERO(&fdread);

    //gan mo ta client vao tap fdread
    FD_SET(client, &fdread);
    //gan mo ta vao ra chuan stdin vao fread
    FD_SET(STDIN_FILENO, &fdread);


    fd_set fdtest;  //goi ham select su dung fd nay
    struct timeval tv;

    char buf[256];
    while (1) {
        fdtest = fdread;

        //thiet lap thoi gian cho la 5s
        tv.tv_sec = 5;
        tv.tv_usec = 0; //usec: microsecond

        /*
            int select(
                int nfds, //socket can lang nghe lon nhat + 1 (kiem tra tu 0->client)
                fd_set *readfds, //tap cac socket cho su kien doc
                fd_set *writefds,    //tap cac socket cho su kien ghi
                fd_set *exceptfds,    //tap cac socket cho su kien ngoai le hoac loi
                struct timeval *timeout // thoi gian cho cac su kien
            )
        */
        int ret = select(client+1, &fdtest, NULL, NULL, &tv);
        if (ret == -1) {
            perror("select() failed");
            break;
        }
        if (ret == 0) {
            //het thoi gian cho
            printf("Timed out...\n");
            continue;
        }

        // Kiem tra kieu su kien 
        if (FD_ISSET(STDIN_FILENO, &fdtest)) {
            // Doc du lieu tu ban phim, gui sang server
            fgets(buf, sizeof(buf), stdin);
            
            ret = send(client, buf, strlen(buf), 0);
            printf("%d bytes sent\n", ret);
            if (strncmp(buf, "exit", 4) == 0)
                break;
        }

        if (FD_ISSET(client, &fdtest)) {
            // Nhan du lieu tu server, in ra man hinh
            ret = recv(client, buf, sizeof(buf), 0);
            if (ret <= 0)
                break;
            buf[ret] = 0;
            printf("Received: %s\n", buf);
        }
    }

    close(client);

    return 0;
}