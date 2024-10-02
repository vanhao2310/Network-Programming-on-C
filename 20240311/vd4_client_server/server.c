#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>

// argc = 2 - number of parameters
// argv[] - filename and port number
int main(int argc, char *argv[])
{
    // check parameter
    if (argc < 2)
    {
        printf("port is not provided\n");
        exit(0);
    }

    // create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1)
    {
        perror("socket() failed: ");
        exit(1);
    }
    printf("create socket successfully\n");

    // address struct
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // binding
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
    {
        perror("bind() failed: ");
        exit(1);
    }
    printf("binding socket successfully\n");

    // ready
    if (listen(sockfd, 5) == -1)
    {
        perror("listen() failed: ");
        exit(1);
    }
    printf("waiting for connection...\n");

    // create client address
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);

    // socket to send/receive data
    int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd == -1)
    {
        perror("accept() failed: ");
        exit(1);
    }
    printf("accept successfully\n");

    char buffer[255];
    while (1)
    {
        bzero(buffer, 255);
        // receive message from client
        int n = read(newsockfd, buffer, 255);
        if (n == -1)
        {
            perror("read() failed: ");
            exit(1);
        }
        printf("Client: %s", buffer);
        bzero(buffer, 255);

        // send message to client
        fgets(buffer, 255, stdin);
        n = write(newsockfd, buffer, strlen(buffer));
        if (n == -1)
        {
            perror("write() failed: ");
            exit(1);
        }

        int i = strncmp("stop", buffer, 4);
        if (i == 0)
            break;
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}   