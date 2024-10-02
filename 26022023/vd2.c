/*
•VD2: Lệnh gửi từ client là chuỗi ký tự có dạng “CMD X Y” trong đó CMD là các lệnh ADD,SUB,MUL,DIV, X và Y là 2 số thực. 
Viết đoạn chương trình kiểm tra một chuỗi ký tự có theo cú pháp trên không, xác định các giá trị của CMD, X và Y.
*/
#include <stdio.h>
#include <string.h>

void main() {
    char data[32];
    char cmd[16], tmp[16];
    float x, y;

    printf("Nhap lenh can kiem tra: ");
    fgets(data, 32, stdin);

    int n = sscanf(data, "%s%f%f%s", cmd, &x, &y, tmp);
    printf("n = %d\n", n);
    printf("cmd = %s\n", cmd);
    printf("x = %f\n", x);
    printf("y = %f\n", y);
    if (n == 3) {
        if (strcmp(cmd, "ADD") == 0 || strcmp(cmd, "SUB") == 0
            || strcmp(cmd, "MUL") == 0 || strcmp(cmd, "DIV") == 0) {
                printf("Dung cu phap\n");
            }
        else 
            printf("Lenh khong duoc ho tro\n");
    } else {
        printf("Sai cu phap\n");
    }
}