/* �ƿ� 2152131 ����� */
#include <iostream>
using namespace std;

int main()
{
    const char* c[] = { "John learn C++ language",
                            "Be well!", "You", "Not very"};
    const char** p[] = { c + 3, c + 2, c + 1, c };
    const char*** pp = p;
    cout << (**++pp);
    cout << (*-- * ++pp + 4);
    cout << (*pp[-2] + 3);
    cout << (pp[-1][-1] + 2);
    cout << endl;
    return 0;
}

