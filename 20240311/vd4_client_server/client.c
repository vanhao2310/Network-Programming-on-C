#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

/*
argv: filename server_ipaddress port_number
argv[0] - filename
argv[1] - server_ipaddress
argv[2] - port number
*/
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("argv missing\n");
        exit(1);
    }

    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
    {
        perror("socket() failed: ");
        exit(1);
    }
    printf("socket creat successfully\n");

    // get host
    struct hostent *server;
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        printf("can not get host");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect() failed: ");
        exit(1);
    }

    char buffer[255];
    while (1)
    {
        bzero(buffer, 255);

        // send to server
        fgets(buffer, 255, stdin);
        int n = write(sockfd, buffer, strlen(buffer));
        if (n < 0)
        {
            perror("write() failed: ");
            exit(1);
        }

        // reading from server
        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if (n < 0)
        {
            perror("read() failed: ");
            exit(1);
        }

        printf("Server: %s", buffer);

        int i = strncmp("stop", buffer, 4);
        if (i == 0)
            break;
    }

    close(sockfd);
    return 0;
}