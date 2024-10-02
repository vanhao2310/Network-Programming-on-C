#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) 
{
    char buf[2048];
    int pos = 0;

    char path[256];
    getcwd(path, sizeof(path));
    strcpy(buf, path);
    pos += strlen(path) + 1;
    
    DIR *d = opendir(path);
    struct dirent *dir;
    struct stat st;

    if (d != NULL)
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                stat(dir->d_name, &st);
                // printf("1");
                printf("%s - %ld bytes\n", dir->d_name, st.st_size);

                strcpy(buf + pos, dir->d_name);
                pos += strlen(dir->d_name) + 1;
                memcpy(buf + pos, &st.st_size, sizeof(st.st_size));
                pos += sizeof(st.st_size);
            }
        }
}