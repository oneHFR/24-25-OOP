#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char p[10];
    strcpy(p, "123456789");
    //int p[10]; //char* p;
    //for (int i = 0; i < 10; ++i)
    //    p[i] = i;
    // ��  
    p[10] = 'a';    //�˾�Խ��
    p[14] = 'D';    //�˾�Խ��
    p[15] = 'B';    //�˾�Խ��
    // ��  
    //p[10] = '\xcc'; //�˾�Խ��
    //p[10] = 0xcccccccc;
    printf("addr:%p\n", p);
    for (int i = -4; i < 16; i++) //ע�⣬ֻ��0-9�Ǻ���Χ�����඼��Խ���
        printf("%p:%02x\n", (p + i), p[i]);

    return 0;
}
