/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#if (_MSC_VER)
#include <conio.h>
#endif

#include<math.h>

//������Ҫ�ɼ�������ͷ�ļ�
//�˴�Ϊʾ���������޸Ľṹ�����ƣ������޸Ľṹ���еĳ�Ա���ݣ�Ҫ��sizeof������64
struct demo {
    unsigned char d[64];
};

struct Parameter {
    const char* name;
    int begin;
    int end;
    long long min;
    long long max;
};

struct Parameter params[] = {
    { "����ǳ�", 1, 16, 0, 0 },                      // ����ǳƣ����ܷ�ΧԼ��
    { "����", 17, 18, 0, 10000 },                   // ������Χ 0-10000
    { "����", 19, 20, 0, 10000 },                   // ������Χ 0-10000
    { "����", 21, 22, 0, 8192 },                    // ���ʷ�Χ 0-8192
    { "����", 23, 24, 0, 1024 },                    // ���ɷ�Χ 0-1024
    { "��Ǯ", 25, 28, 0, 100000000 },               // ��Ǯ��Χ 0-100000000
    { "����", 29, 32, 0, 1000000 },                 // ������Χ 0-1000000
    { "����", 33, 36, 0, 1000000 },                 // ������Χ 0-1000000
    { "��Ϸ�ۼ�ʱ��", 37, 44, 0, 10000000000000000LL }, // ��Ϸ�ۼ�ʱ�䷶Χ 0-10000000000000000
    { "�ƶ��ٶ�", 45, 45, 0, 100 },                 // �ƶ��ٶȷ�Χ 0-100
    { "�����ٶ�", 46, 46, 0, 100 },                 // �����ٶȷ�Χ 0-100
    { "������Χ", 47, 47, 0, 100 },                 // ������Χ��Χ 0-100

    { "������", 49, 50, 0, 2000 },                  // ��������Χ 0-2000
    { "������", 51, 52, 0, 2000 },                  // ��������Χ 0-2000
    { "���ݶ�", 53, 53, 0, 100 },                   // ���ݶȷ�Χ 0-100
    { "����", 54, 54, 0, 100 },                     // ������Χ 0-100
    { "����", 55, 55, 0, 100 },                     // ���鷶Χ 0-100
    { "�ȼ�", 56, 56, 0, 100 },                     // �ȼ���Χ 0-100
    { "ħ��ֵ", 57, 58, 0, 10000 },                   // ħ����Χ 0-10000
    { "����ħ��ֵ", 59, 59, 0, 100 },                 // ����ħ����Χ 0-100
    { "ħ���˺���", 60, 60, 0, 100 },               // ħ���˺�����Χ 0-100
    { "ħ��������", 61, 61, 0, 100 },                   // �����ʷ�Χ 0-100
    { "ħ��������", 62, 62, 0, 100 },               // ħ����������Χ 0-100
    { "������", 63, 63, 0, 100 },                   // �����ʷ�Χ 0-100
    { "����", 64, 64, 0, 100 },                     // ������Χ 0-100
};


/* �˴�����������������������
   1�����������ĺ����������������µ� FILE* �����д�/��/д/�رյȲ���
   2����������ͬ��������main����
*/
static void usage(const char* procname)
{
    printf("usage : %s --modify | --read\n", procname);
}

long long getParameterValue(const struct demo* data, int index) 
{
    long long value = 0;
    for (int j = params[index].end - 1; j >= params[index].begin - 1; --j) { // С�����ȡ
        value = (value << 8) | data->d[j];
    }
    return value;
}

void setParameterValue(struct demo* data, int index, long long value) 
{
    for (int j = params[index].end - 1; j >= params[index].begin - 1; --j) {
        data->d[j] = value & 0xFF;
        value >>= 8;
    }
}

void r_show(const struct demo* data, char* name) {
    char displayName[32]; // ���ڴ洢����չʾ������
    long long temp = 0;
    for (size_t i = 0; i < sizeof(params) / sizeof(params[0]); ++i) {
        if (i == 0) {
            int j = 0;
            for (; j < 16 && data->d[j] != '\0'; ++j)
                name[j] = data->d[j];
            name[j] = '\0';
            printf("%18s��%s\n", "����ǳ�", name);
        }
        else {
            strncpy(displayName, params[i].name, sizeof(displayName) - 1);
            displayName[sizeof(displayName) - 1] = '\0';
            if (strstr(displayName, "ֵ") == NULL)
                strncat(displayName, "ֵ", sizeof(displayName) - strlen(displayName) - 1);
            if (strcmp(displayName, "��Ϸ�ۼ�ʱ��ֵ") == 0) {
                strncpy(displayName, "��Ϸ�ۼ�ʱ��(us)ֵ", sizeof(displayName) - 1);
                displayName[sizeof(displayName) - 1] = '\0';
            }
            temp = 0;
            if (params[i].begin > 0 && params[i].end >= params[i].begin) {
                for (size_t j = params[i].begin, j0 = j; j <= (size_t)params[i].end && j - 1 < sizeof(data->d); ++j)
                    temp += (long long)pow(256, (j - j0)) * data->d[j - 1];
            }
            printf("%18s��%lld\n", displayName, temp);
        }
    }
}

void printGameData(const struct demo* data, char* name) {
    printf("--------------------------------------\n");
    printf("  ��Ϸ�浵�ļ��޸Ĺ���\n");
    printf("--------------------------------------\n");

    for (size_t i = 0; i < sizeof(params) / sizeof(params[0]); ++i) {
        printf("  %c.%-12s", (char)('a' + i), params[i].name);
        if (i == 0) {
            if (name[0] == '\0') {
                int j = 0;
                for (; j < 16 && data->d[j] != '\0'; ++j) {
                    name[j] = data->d[j];
                }
                name[j] = '\0';
            }
            printf("(%s)\n", name);
        }
        else
            printf("(%lld)\n", getParameterValue(data, i));
    }

    printf("--------------------------------------\n");
    printf("  0.�����޸�\n");
    printf("  1.�����˳�\n");
    printf("--------------------------------------\n");
}


int modifyGameData(struct demo* data, char* name) {
    while (1) {
        printGameData(data, name);
        printf("��ѡ��[a..y, 0..1] ");
        #if (_MSC_VER)
            char choice = getch();
        #elif (__linux__)
            char choice = getchar();
        #endif
        if (choice == '0') {
            printf("\n");
            return 0;
        }
        else if (choice == '1') {
            printf("\n");
            return 1;
        }
        else if (choice >= 'a' && choice <= 'y') {
            size_t index = choice - 'a'; // �� index ��Ϊ size_t ���ͣ�������Ų�ƥ��
            if (index >= 0 && index < sizeof(params) / sizeof(params[0])) {
                if (index == 0) {
                    char currentName[16] = { 0 };
                    strncpy(currentName, name, sizeof(currentName) - 1);
                    printf("\n\n����ǳƣ���ǰֵ=%s�������� : ", currentName);
                    scanf("%s", name);
                    while (getchar() != '\n');
                }
                else {
                    long long value;
                    printf("\n\n%s����ǰֵ=%lld����Χ[%lld..%lld]�������� : ",
                        params[index].name,
                        getParameterValue(data, index),
                        params[index].min, params[index].max);
                    scanf("%lld", &value);
                    if (value >= params[index].min && value <= params[index].max) {
                        setParameterValue(data, index, value);
                    }
                }
            }
        }
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ��������������ֻ�������һ��fopen��һ��fread����Ϊ����������������fclose��
***************************************************************************/
int read(struct demo* data)
{
    /* ��������ֻ������һ�� FILE* */
    FILE* fp;

    /* �ļ��򿪣�����Ҫ��Ϊ��
       1��Ҫ���Զ���ʽ�򿪣��򿪷�ʽ***����ָ��
       2��������fopen�⣬�����������ط��������ٳ���fopen/freopen  */
    fp = fopen("game.dat", "rb");

    /* ���к����������������������ļ�����ʾ����Ϸ���ֵ���ر��ļ��ȣ�������ú���
       ���У�ֻ������һ���Զ�ȡ64�ֽڵķ�����game.dat�����ݶ���***�����������ơ��ṹ����������ָ����
                 fread(***, 1, sizeof(demo), fp);
    */
    if (!fp) {
        printf("�޷����ļ�game.dat\n");
        return -1;
    }
    fread(data->d, sizeof(data->d), 1, fp);
    fclose(fp);
    return 1;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ��������������ֻ�������һ��open��һ��read��һ��write����Ϊ����������������fclose��
***************************************************************************/
int modify(struct demo* data)
{
    /* ��������ֻ������һ�� FILE* */
    FILE* fp;
    //�޷����ļ�game.dat
    /* �ļ��򿪣�����Ҫ��Ϊ��
       1��Ҫ���Զ�д��ʽ�򿪣��򿪷�ʽ***����ָ��
       2��������fopen�⣬�����������ط��������ٳ���fopen/freopen  */
    fp = fopen("game.dat", "wb");

    /* ���к����������������������ļ�����ʾ����Ϸ���ֵ���ر��ļ��ȣ�������ú���
       ���У�ֻ������һ���Զ�ȡ64�ֽڵķ�����game.dat�����ݶ���***�����������ơ��ṹ����������ָ����
                 fread(***, 1, sizeof(demo), fp);
             ֻ������һ����д��64�ֽڵķ�����***������д��game.dat�У����������ơ��ṹ����������ָ����
                 fwrite(***, 1, sizeof(demo), fp);
    */
    if (!fp) {
        printf("�޷����ļ�game.dat\n");
        return -1;
    }
    fwrite(data->d, sizeof(data->d), 1, fp);
    fclose(fp);
    return 1;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main�������������������������ļ���д
***************************************************************************/
int main(int argc, char** argv)
{
    struct demo gameData = { 0 };
    char name[16] = { 0 };
    if (argc != 2) {
        usage(argv[0]);
        return -1;
    }
    if (strcmp(argv[1], "--read") == 0) {
        if (read(&gameData))
            r_show(&gameData, name);
        else
            return -1;
    }
    else if (strcmp(argv[1], "--modify") == 0) {
        if (read(&gameData)) {
            if (modifyGameData(&gameData, name)) {
                modify(&gameData);
            }
            else
                return -1;
        }
    }
    return 0;
} 

