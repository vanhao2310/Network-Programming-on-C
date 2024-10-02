#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <malloc.h>

int main(int n, char *args[]) {
    SSL_library_init();				// Khởi tạo thư viện OpenSSL
    const SSL_METHOD *meth = TLS_client_method();	// Khai báo phương thức mã hóa TLS
    SSL_CTX *ctx = SSL_CTX_new(meth);	// Tạo context mới
    SSL *ssl = SSL_new(ctx);		// Tạo con trỏ ssl
    if (!ssl) {
        printf("Error creating SSL.\n");
        return -1;
    }

    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //lay thong tin dia chi
    char *url = args[1];
    int protocol;   //1: https, 0: http
    char proto[9];
    char domain_name[100], path[100];
    if (strncmp(url, "https://", 8) == 0){
        protocol = 1;
        sprintf(proto, "https");
    } else if (strncmp(url, "http://", 7) == 0) {
        protocol = 0;
        sprintf(proto, "http");
    }
    
    char *p1 = strstr(url, "://") + 3;
    char *p2 = strchr(p1, '/');

    memcpy(domain_name, p1, p2 - p1);
    memcpy(path, p2, strlen(p2));

    struct  addrinfo *info;
    
    // https://lebavui.github.io/videos/ecard.mp4

    if (getaddrinfo(domain_name, "https", NULL, &info)) {
        puts("getaddrinfo() failed");
        return -1;
    }

    if (connect(client, info->ai_addr, info->ai_addrlen)) {
        perror("connect() failed");
        return -1;
    }

    if (protocol == 1) {
        //https
        SSL_set_fd(ssl, client);		// Gắn con trỏ ssl với socket
        int err = SSL_connect(ssl);	// Tạo kết nối ssl
        if (err <= 0) {
            printf("Error creating SSL connection.  err=%x\n", err);
            return -1;
        }

        char req[1024];
        sprintf(req, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, domain_name);
        SSL_write(ssl, req, strlen(req));

        char buf[2048];
        // Nhan noi dung phan header
        char *header = NULL;
        int len = 0;

        while (1) {
            int ret = SSL_read(ssl, buf, sizeof(buf));
            if (ret <= 0) {
                close(client);
                printf("disconnected\n");
                return 1;
            }

            header = realloc(header, len + ret);
            memcpy(header + len, buf, ret);
            len += ret;

            if (memmem(header, len, "\r\n\r\n", 4) != NULL) {
                break;
            }
        }

        // In header
        puts(header);

        char *pos = memmem(header, len, "\r\n\r\n", 4);

        FILE *f = fopen("download.mp4", "wb");

        // Nhan noi dung file
        int header_length = pos - header;
        if (header_length + 4 < len)
            fwrite(pos + 4, 1, len - header_length - 4, f);

        while (1) {
            int ret = SSL_read(ssl, buf, sizeof(buf));
            if (ret <= 0)
                break;
            fwrite(buf, 1, ret, f);
        }

        free(header);

        fclose(f);
        close(client);
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    } else {
        //http
        char req[1024];
        sprintf(req, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, domain_name);
        send(client, req, strlen(req), 0);

        char buf[2048];
        // Nhan noi dung phan header
        char *header = NULL;
        int len = 0;

        while (1) {
            int ret = recv(client, buf, sizeof(buf), 0);
            if (ret <= 0) {
                close(client);
                printf("disconnected\n");
                return 1;
            }

            header = realloc(header, len + ret);
            memcpy(header + len, buf, ret);
            len += ret;

            if (memmem(header, len, "\r\n\r\n", 4) != NULL) {
                break;
            }
        }

        // In header
        puts(header);

        char *pos = memmem(header, len, "\r\n\r\n", 4);

        FILE *f = fopen("download.mp4", "wb");

        // Nhan noi dung file
        int header_length = pos - header;
        if (header_length + 4 < len)
            fwrite(pos + 4, 1, len - header_length - 4, f);
        
        while (1) {
            int ret = recv(client, buf, sizeof(buf), 0);
            if (ret <= 0)
                break;
            fwrite(buf, 1, ret, f);
        }

        free(header);
        fclose(f);
        close(client);

        printf("Done.");
    }

    
}