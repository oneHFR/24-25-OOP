#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

int main()
{
    char p[10];
    strcpy(p, "123456789");
    //int p[10]; 
    //for (int i = 0; i < 10; ++i)
    //    p[i] = i;
    // ��  
    //p[10] = 'a';    //�˾�Խ��
    p[14] = 'A';    //�˾�Խ��
    p[15] = 'B';    //�˾�Խ��
    // ��  
    //p[10] = '\xcc'; //�˾�Խ��
    //p[10] = 0xcccccccc;
    cout << "addr:" << hex << (void*)(p) << endl;
    for (int i = -4; i < 16; i++) //ע�⣬ֻ��0-9�Ǻ���Χ�����඼��Խ���
        cout << hex << (void*)(p + i) << ":" << int(p[i]) << endl;

    return 0;
}
