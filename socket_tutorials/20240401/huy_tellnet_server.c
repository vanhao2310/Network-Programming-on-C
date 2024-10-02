#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/select.h>
//port 9000
int authenticate(char buf[]){
    FILE *f = fopen("log.txt","rb");
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

    fd_set fdread,fdtest,fd_authentication;
    FD_ZERO(&fdread);
    FD_ZERO(&fdtest);
    FD_ZERO(&fd_authentication);

    FD_SET(listener,&fdread);

        int count = 0;
    while(1){
        fdtest = fdread;
        int ret = select(FD_SETSIZE,&fdtest,NULL,NULL,NULL);
        if(ret ==-1){
            perror("Failed to select()!");
            break;
        }
        count++;
        for(int i=0;i<FD_SETSIZE;i++){
            if(FD_ISSET(i,&fdtest)){
                if(i==listener){
                    int client = accept(listener,NULL,NULL);
                    if(client<1024){
                        FD_SET(client,&fdread);
                        printf("New Client!\n");
                        char msg[] = "Input your username and password?<username>_<password>\n";
                        write(client,msg,strlen(msg));
                    }else {
                        char msg[] = "Too much client!"; 
                        write(client,msg,strlen(msg));
                        close(client);
                    }
                } else {
                    char buf[2048];
                    int ret = read(i,buf,sizeof(buf));
                    if(ret==0){
                        FD_CLR(i,&fd_authentication);
                        FD_CLR(i,&fdread);
                        close(i);
                    }
                    buf[ret] = 0;
                    if(strncmp(buf,"logout",6)==0){
                        FD_CLR(i, &fd_authentication);
                        continue;
                    }
                    if(FD_ISSET(i, &fd_authentication)){
                            //thuc hien lenh
                            system(buf);
                            puts(buf);
                    }else{
                        char msg[2048];
                        int authen = authenticate(buf);
                        if(authen==1){
                            strcpy(msg,"Correct!\n---------\n");
                            FD_SET(i,&fd_authentication);
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