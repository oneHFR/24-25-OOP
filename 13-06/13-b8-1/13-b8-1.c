/* 2152131 ����� �ƿ� */
#include <stdio.h>

int main()
{
    /* ��������������໥λ�ÿɻ���������Ŀǰ#if���������VS2022�������Ȼ�ΪLinux��*/
#if (_MSC_VER)
    printf("VS2022\n");
#elif (_WIN32)
    printf("Dev\n");
#elif (__linux__)
    printf("Linux\n");
#endif

    return 0;
}
