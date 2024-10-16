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
    // ①  
    //p[10] = 'a';    //此句越界
    p[14] = 'A';    //此句越界
    p[15] = 'B';    //此句越界
    // ②  
    //p[10] = '\xcc'; //此句越界
    //p[10] = 0xcccccccc;
    cout << "addr:" << hex << (void*)(p) << endl;
    for (int i = -4; i < 16; i++) //注意，只有0-9是合理范围，其余都是越界读
        cout << hex << (void*)(p + i) << ":" << int(p[i]) << endl;

    return 0;
}
