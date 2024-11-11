/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 256

int usage(const char* procname) {
    printf("Usage: %s --infile 原始文件[--outfile hex格式文件]\n", procname);
    printf("       %s --infile a.docx\n", procname);
    printf("       %s --infile a.docx --outfile a.hex\n", procname);

    return 0;
}

void print_out(FILE* in, FILE* out) {
    int byteValue, offset = 0;
    int asciiValues[16];
    int isValid[16] = { 0 };
    int div_flag = 0;
    while ((byteValue = fgetc(in)) != EOF) {
        if (offset % 16 == 0)
            fprintf(out, "%08x  ", offset);
        if (div_flag == 1) {
            div_flag = -1;
            fprintf(out, "- ");
        }

        fprintf(out, "%02x ", byteValue);

        asciiValues[offset % 16] = byteValue;
        isValid[offset % 16] = 1;
        offset++;

        if (offset % 8 == 0 && offset % 16)
            div_flag = 1;

        if (offset % 16 == 0) {
            fprintf(out, "    ");
            for (int i = 0; i < 16; i++) {
                if (isValid[i] && asciiValues[i] >= 33 && asciiValues[i] <= 126)
                    fprintf(out, "%c", asciiValues[i]);
                else
                    fprintf(out, ".");
                isValid[i] = 0;
            }
            fprintf(out, "\n");
            div_flag = 0;
        }
    }
    if (div_flag != -1 && !(offset % 16 == 0)) {
        fprintf(out, "  ");
    }
    //fprintf(out, "  ");
    if (offset % 16 != 0) {
        int remainingBytes = offset % 16;
        for (int i = remainingBytes; i < 16; i++)
            fprintf(out, "   ");
        fprintf(out, "    ");
        for (int i = 0; i < remainingBytes; i++) {
            if (isValid[i] && asciiValues[i] >= 33 && asciiValues[i] <= 126)
                fprintf(out, "%c", asciiValues[i]);
            else
                fprintf(out, ".");
        }
        fprintf(out, "\n");
    }
}

int d2h(const char* infile_name, const char* outfile_name) {
    FILE* in = fopen(infile_name, "rb");
    if (in == NULL) {
        printf("输入文件%s打开失败!\n", infile_name);
        return -1;
    }

    FILE* out = stdout;  // 默认输出到屏幕
    if (outfile_name != NULL) {
        out = fopen(outfile_name, "w");
        if (out == NULL) {
            printf("文件%s打开失败!\n", outfile_name);
            fclose(in);
            return -1;
        }
    }

    print_out(in, out);

    if (out != stdout) {  // 只有在文件输出时才需要关闭文件
        fclose(out);
    }
    fclose(in);
    return 0;
}

int main(int argc, char* argv[]) {
    if ((argc != 3) && (argc != 5)) {
        usage(argv[0]);
        return 0;
    }

    const char* infile_name = NULL;
    const char* outfile_name = NULL;

    if (argc == 3) {
        if (strcmp(argv[1], "--infile") != 0) {
            usage(argv[0]);
            return 0;
        }
        infile_name = argv[2];
    }
    else if (argc == 5) {
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
    }

    d2h(infile_name, outfile_name);

    return 0;
}
