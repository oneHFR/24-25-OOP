#include <iostream>
using namespace std;

int main()
{
    char (*p)[6];
    p = new char[2][6]{ {"Hello"}, {"China"} };

    return 0;
}
