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
    // ①  
    p[10] = 'a';    //此句越界
    p[14] = 'D';    //此句越界
    p[15] = 'B';    //此句越界
    // ②  
    //p[10] = '\xcc'; //此句越界
    //p[10] = 0xcccccccc;
    printf("addr:%p\n", p);
    for (int i = -4; i < 16; i++) //注意，只有0-9是合理范围，其余都是越界读
        printf("%p:%02x\n", (p + i), p[i]);

    return 0;
}
