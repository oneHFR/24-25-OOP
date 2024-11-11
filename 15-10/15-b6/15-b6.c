/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 256

int usage(const char* procname)
{
    printf("Usage: %s --check �ļ��� | --convert { wtol|ltow } Դ�ļ��� Ŀ���ļ���\n", procname);
    printf("               %s --check a.txt\n", procname);
    printf("               %s --convert wtol a.win.txt a.linux.txt\n", procname);
    printf("               %s --convert ltow a.linux.txt a.win.txt\n", procname);
    return 0;
}

int check(FILE* in, char** argv, int index)
{
    in = fopen(argv[index], "rb");
    if (in == NULL) {
        printf("�ļ���ʧ��\n");
        return -2;
    }

    char curr_ch = 0, prev_ch = 0;
    int win = 0, lin = 0;

    while ((curr_ch = fgetc(in)) != EOF) {
        if (curr_ch == 10 && prev_ch == 13) // (CRLF)
            win = 1;
        else if (curr_ch == 10 && prev_ch != 13) // (LF)
            lin = 1;
        prev_ch = curr_ch;
    }

    fclose(in);

    if ((win && lin) || (!win && !lin))
        return -1;
    else if (win)
        return 0;
    else if (lin)
        return 1;
    else
        return -1024;
}

void wtol(FILE* in, FILE* out, char** argv)
{
    int remove_num = 0;
    in = fopen(argv[3], "rb");
    out = fopen(argv[4], "wb");

    if (in == NULL || out == NULL) {
        printf("�ļ���ʧ��\n");
        return;
    }

    char curr_ch = 0;
    while ((curr_ch = fgetc(in)) != EOF) {
        if (curr_ch == 13) {  // �Ƴ� '\r' �ַ�
            remove_num++;
            continue;
        }
        else {
            fputc(curr_ch, out);
        }
    }
    printf("ת����ɣ�ȥ�� %d �� 0x0D\n", remove_num);

    fclose(in);
    fclose(out);
}

void ltow(FILE* in, FILE* out, char** argv)
{
    int add_num = 0;
    in = fopen(argv[3], "rb");
    out = fopen(argv[4], "wb");

    if (in == NULL || out == NULL) {
        printf("�ļ���ʧ��\n");
        return;
    }

    char curr_ch = 0;
    while ((curr_ch = fgetc(in)) != EOF) {
        if (curr_ch == 10) {
            add_num++;
            fputc(13, out);  // ��� '\r' �ַ�
        }
        fputc(curr_ch, out);
    }
    printf("ת����ɣ����� %d �� 0x0D\n", add_num);

    fclose(in);
    fclose(out);
}

int main(int argc, char* argv[])
{
    if (argc != 3 && argc != 5) {
        usage(argv[0]);
        return 0;
    }

    FILE* in = NULL;
    FILE* out = NULL;

    if (argc == 3) {
        if (strcmp(argv[1], "--check") != 0) {
            usage(argv[0]);
            return 0;
        }
        else {
            int format = check(in, argv, 2);
            if (format == -2) {
                return 0;
            }
            else if (format == -1) {
                printf("�ļ���ʽ�޷�ʶ��\n");
            }
            else if (format == 0) {
                printf("Windows��ʽ\n");
            }
            else if (format == 1) {
                printf("Linux��ʽ\n");
            }
        }
    }
    else if (argc == 5) {
        if (strcmp(argv[1], "--convert") != 0) {
            usage(argv[0]);
            return 0;
        }
        if (strcmp(argv[2], "wtol") != 0 && strcmp(argv[2], "ltow") != 0) {
            usage(argv[0]);
            return 0;
        }

        int format = check(in, argv, 3);
        if (format == -2) {
            return 0;
        }
        else if (format == -1) {
            printf("�ļ���ʽ�޷�ʶ��\n");
        }
        else if (format == 0) {
            wtol(in, out, argv);
        }
        else if (format == 1) {
            ltow(in, out, argv);
        }
    }

    return 0;
}
