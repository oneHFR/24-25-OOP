#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char* p;
    p = (char*)malloc(10 * sizeof(char));
    if (p == NULL)
        return -1;
    strcpy(p, "123456789");
    // ��  
    p[10] = 'a';    //�˾�Խ��
    p[14] = 'A';    //�˾�Խ��
    p[15] = 'B';    //�˾�Խ��
    // ��  
    p[10] = '\xfd'; //�˾�Խ��
    printf("addr:%p\n", p);
    for (int i = -4; i < 16; i++) //ע�⣬ֻ��0-9�Ǻ���Χ�����඼��Խ���
        printf("%p:%02x\n", (p + i), p[i]);
    // ��  
    free(p);

    return 0;
}
