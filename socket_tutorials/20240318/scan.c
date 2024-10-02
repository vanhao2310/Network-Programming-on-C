/*
lay ten thu muc hien tai, danh sach cac file va kich thuoc file.
Dong goi lai va chuyen sang info_server
Bài 1.
Ứng dụng info_client lấy tên thư mục hiện tại, danh sách các tập tin và kích thước (trên máy client).
Các dữ liệu này sau đó được đóng gói và chuyển sang info_server.
Ứng dụng info_server nhận dữ liệu từ info_client, tách các dữ liệu và in ra màn hình.
Yêu cầu kích thước dữ liệu được gửi là nhỏ nhất.
Ví dụ:
/mnt/c/Users/admin/source/baitap
bai1.c – 12871 bytes
bai2.c – 1437 bytes
bai3.c – 1461 bytes*/

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

int main(int argc, char **argv)
{
  int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(client == -1){
    perror("Socket() failed: ");
    return -1;
  }

  struct sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(9000);

  if(connect(client, (struct sockaddr *)&servAddr, sizeof(servAddr))){
    perror("Connect() failed: ");
    return -1;
  }

  //get list file in folder
  char path[256];
  getcwd(path, sizeof(path));
  puts(path);
  DIR *d = opendir(path);
  struct dirent *dir;
  struct stat st;

  if (d != NULL)
    while ((dir = readdir(d)) != NULL)
    {
      if (dir->d_type == DT_REG)  //DT_REG : file , DT_DIR : thu muc
      {
        stat(dir->d_name, &st);
        printf("%s - %ld bytes\n", dir->d_name, st.st_size);
        char *fileInfo = dir->d_name;
        memcpy(fileInfo + strlen(dir->d_name), &(st.st_size), sizeof(st.st_size));
        send(client, fileInfo, strlen(dir->d_name) + sizeof(st.st_size), 0);
      }
    }
  
}