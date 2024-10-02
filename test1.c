#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

void encrypt(char *msg) {
    char path[256];
  getcwd(path, sizeof(path));
  puts(path);
}

int a;

int main(int n, char* args[]) {
  //lay thong tin dia chi
    char *url = args[1];
    int protocol;   //1: https, 0: http
    char proto[9];
    if (strncmp(url, "https://", 8) == 0){
        protocol = 1;
        url += 8;
        sprintf(proto, "https");
    } else if (strncmp(url, "http://", 7) == 0) {
        protocol = 0;
        url += 7;
        sprintf(proto, "http");
    }
    printf("protocol: %s\n", proto);

    char *p1 = strchr(url, '/');
    int domain_length = p1 - url;

    char *domain_name = malloc(domain_length+1);
    //lay ra dia chi 
    memcpy(domain_name, url, domain_length);
    url += domain_length;
    printf("domain: %s\n", domain_name);

    //lay ra duong dan den file can tai
    char *path = malloc(strlen(url));
    memcpy(path, url, strlen(url));
    printf("path: %s\n", path);
}