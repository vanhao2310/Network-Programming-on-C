#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<poll.h>
#include<stdbool.h>

#define MAX_FDS 256

bool authentication[MAX_FDS];

//port 9000
int authenticate(char buf[]){
    FILE *f = fopen("pass.txt","rb");
    char line[2048];
    char _username[2048];
    char _password[2048];
    sscanf(buf,"%s %s",_username,_password);
    while(fgets(line,2048,f)!=NULL){
        char username[2048];
        char password[2048];
        sscanf(line,"%s %s",username,password);
        if(strcmp(username,_username)==0){
            if(strcmp(password,_password)==0){
                return 1;
            }
            return 2;//2 is pass word false
        }
    }
    fclose(f);
    return 0;
}
int main(){
        // Tao socket cho ket noi
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1) {
        perror("socket() failed");
        return 1;
    }

    // Khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(10000);

    // Gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed");
        return 1;
    }

    // Chuyen socket sang trang thai cho ket noi
    if (listen(listener, 5)) {
        perror("listen() failed");
        return 1;
    }

    struct pollfd fds[MAX_FDS];
    int nfds = 0;

    fds[0].fd = listener;
    fds[0].events = POLLIN;
    nfds++;

    while(1){
        int ret = poll(fds, nfds, 5000);
        if(ret ==-1){
            perror("Socket failed: ");
            break;
        }
        if(ret ==  0) {
            //timeout
            printf("Timeout..\n");
        }

        for(int i=0; i < nfds; i++){
            if(fds[i].revents & POLLIN){
                if(fds[i].fd == listener){
                    //co ket noi moi -> chap nhan ket noi va cho vao tap tham do
                    int client = accept(listener, NULL, NULL);
                    if(client > MAX_FDS)
                        close(client);
                    else{
                        fds[nfds].fd = client;
                        fds[nfds].events = POLLIN;
                        nfds++;
                        //thong bao co ket noi moi
                        printf("Co client moi ket noi den: %d\n", client);
                    }
                } else {

                    int client = fds[i].fd;
                    char buf[2048];
                    int ret = read(i,buf,sizeof(buf));
                    if(ret <= 0){
                        //socket dong

                    }
                    buf[ret] = 0;
                    if(strncmp(buf,"logout",6)==0){
                        authentication[client] = false;
                        continue;
                    }
                    if(authentication[client]){
                            //thuc hien lenh
                            system(buf);
                            puts(buf);
                    }else{
                        char msg[2048];
                        int authen = authenticate(buf);
                        if(authen==1){
                            strcpy(msg,"Correct!\n---------\n");
                            authentication[client] = true;
                            write(i,msg,strlen(msg));
                        }else if(authen==2){
                            strcpy(msg,"Password Incorrect!\n----------------\nPlease input your username and password again!\n");
                            write(i,msg,strlen(msg));
                        }else {
                            strcpy(msg,"No account found!\n\nPlease input again!\n");
                            write(i,msg,strlen(msg));
                        }
                    }
                }
            }
        }
    }
    close(listener);
    
}