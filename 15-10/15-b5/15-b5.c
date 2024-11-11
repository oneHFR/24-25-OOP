/* 2152131 吴洪蕊 计科 */
/* 2352363 杨霁然 2351138 邱子峰 2352976 吉镓熠 2352018 刘彦 2351114 朱俊泽 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 256


int usage(const char* procname)
{
    printf("Usage: %s --infile hex格式文件 --outfile bin格式文件\n", procname);
    printf("       %s --infile a.hex --outfile a.bin\n", procname);

    return 0;
} 

void print_out(FILE* in, FILE* out)
{ 
    int curr_ch = 0, prev_ch = 0, last_ch = 0, last2_ch = 0, hex_value;
    while ((curr_ch = fgetc(in)) != EOF) {
        if (last2_ch == 32 && curr_ch == 32) {
            if (prev_ch <= 'f' && prev_ch >= 'a') {
                if (last_ch <= 'f' && last_ch >= 'a') {
                    hex_value = (last_ch - 'a' + 10) * 16 + (prev_ch - 'a' + 10);
                    fputc((char)hex_value, out);
                }
                else if (last_ch <= '9' && last_ch >= '0') {
                    hex_value = (last_ch - '0') * 16 + (prev_ch - 'a' + 10);
                    fputc((char)hex_value, out);
                }
            }
            else if (prev_ch >= '0' && prev_ch <= '9') {
                if (last_ch <= 'f' && last_ch >= 'a') {
                    hex_value = (last_ch - 'a' + 10) * 16 + (prev_ch - '0');
                    fputc((char)hex_value, out);
                }
                else if (last_ch <= '9' && last_ch >= '0') {
                    hex_value = (last_ch - '0') * 16 + (prev_ch - '0');
                    fputc((char)hex_value, out);
                }
            }
        }
        last2_ch = last_ch; 
        last_ch = prev_ch;
        prev_ch = curr_ch;
    }
}

int h2b(const char* infile_name, const char* outfile_name)
{
    FILE* in = fopen(infile_name, "rb");
    if (in == NULL) {
        printf("输入文件 %s 打开失败!\n", infile_name);
        return -1;
    }

    FILE* out = fopen(outfile_name, "wb");
    if (out == NULL) {
        printf("输出文件 %s 打开失败!\n", outfile_name);
        fclose(in);
        return -1;
    }

    print_out(in, out);

    fclose(out);
    fclose(in);
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc != 5) {
        usage(argv[0]);
        return 0;
    }

    const char* infile_name = NULL;
    const char* outfile_name = NULL;

    if (strcmp(argv[1], "--infile") == 0 && strcmp(argv[3], "--outfile") == 0) {
        infile_name = argv[2];
        outfile_name = argv[4];
    }
    else if (strcmp(argv[3], "--infile") == 0 && strcmp(argv[1], "--outfile") == 0) {
        infile_name = argv[4];
        outfile_name = argv[2];
    }
    else {
        usage(argv[0]);
        return 0;
    }

    h2b(infile_name, outfile_name);

    return 0;
}
