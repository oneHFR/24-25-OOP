#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    FILE* fp;
    char buf[80];

    fp = fopen("in.txt", "r");
    int ret = fread(buf, 26, 1, fp);
    printf("ret=%d\n", ret);
    fclose(fp);

    return 0;
}
