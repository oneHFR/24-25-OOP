#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

int main()
{
    char* p;
    p = new(nothrow) char[10];
    if (p == NULL)
        return -1;
    strcpy(p, "123456789");
    // ��  
    p[10] = 'a';    //�˾�Խ��
    p[14] = 'A';    //�˾�Խ��
    p[15] = 'B';    //�˾�Խ��
    // ��  
    p[10] = '\xfd'; //�˾�Խ��
    cout << "addr:" << hex << (void*)(p) << endl;
    for (int i = -4; i < 16; i++) //ע�⣬ֻ��0-9�Ǻ���Χ�����඼��Խ���
        cout << hex << (void*)(p + i) << ":" << int(p[i]) << endl;
    // ��  
    delete[]p;

    return 0;
}
