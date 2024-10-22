/* 2152131 吴洪蕊 计科 */
#include <stdio.h>

int main()
{
    /* 三个条件编译的相互位置可互换（例：目前#if满足是输出VS2022，允许先换为Linux）*/
#if (_MSC_VER)
    printf("VS2022\n");
#elif (_WIN32)
    printf("Dev\n");
#elif (__linux__)
    printf("Linux\n");
#endif

    return 0;
}
