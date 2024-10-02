#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void *client_proc(void *);

int main() {
    // Tao socket cho ket noi
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Khai bao dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);

    // Gan socket voi cau truc dia chi
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind() failed");
        return 1;
    }

    // Chuyen socket sang trang thai cho ket noi
    if (listen(listener, 10)) {
        perror("listen() failed");
        return 1;
    }

    while (1) {
        printf("Waiting for new client\n");
        int client = accept(listener, NULL, NULL);
        printf("New client accepted, client = %d\n", client);
        
        pthread_t tid;
        pthread_create(&tid, NULL, client_proc, &client);
        pthread_detach(tid);
    }

    return 0;
}

void *client_proc(void *args) {
    int client = *(int *)args;
    char buf[2048];

    // Nhan du lieu tu client
    int ret = recv(client, buf, sizeof(buf), 0);
    if (ret <= 0) {
        close(client);
        pthread_exit(NULL);
    }

    buf[ret] = 0;
    printf("Received from %d: %s\n", client, buf);
    
    // GET /?a=1&b=2&cmd=add
    
    double a, b;
    char cmd[5];

    char *p = strchr(buf, '?');
    char *p1 = strchr(p, '&');
    char *p2 = strchr(p1 + 1, '&');

    a = atof(p+3);
    b = atof(p1+3);
    strncpy(cmd, p2+5, 3);

    printf("\na=%f, b=%f, cmd=%s\n", a, b, cmd);

    if (strncmp(cmd, "add", 3) == 0)
        sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><h1>%f + %f = %f</h1></html>", a, b, a + b);
    else if (strncmp(cmd, "sub", 3) == 0)
        sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><h1>%f - %f = %f</h1></html>", a, b, a - b);
    else if (strncmp(cmd, "mul", 3) == 0)
        sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><h1>%f x %f = %f</h1></html>", a, b, a * b);
    else if (strncmp(cmd, "div", 3) == 0)
    {
        if (b == 0)
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><h1>Can not devide to 0</h1></html>");
        else
            sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><h1>%f : %f = %f</h1></html>", a, b, a / b);
    }
    else
        sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><h1>Syntax Error</h1></html>");

    send(client, buf, strlen(buf), 0);
    close(client);
    pthread_exit(NULL);
}