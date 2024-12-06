/* 2152131 吴洪蕊 计科 */
#include "16-b7.h"
#include <cstring>

using namespace std;

week& operator++(week& w) 
{
    return w = week((int(w) + 1) % 7);
}

week operator++(week& w, int) 
{
    week tmp = w;
    ++w;
    return tmp;
}

week& operator--(week& w) 
{
    return w = week((int(w) - 1 + 7) % 7);
}

week operator--(week& w, int) 
{
    week tmp = w;
    --w;
    return tmp;
}

week operator+(week w, int n) 
{
    return week((int(w) + n) % 7);
}

week operator-(week w, int n) 
{
    int val = int(w) - n % 7;
    if (val < 0)
        val += 7;
    return week(val);
}

int operator-(week w1, week w2) 
{
    return int(w1) - int(w2);
}

week& operator+=(week& w, int n) 
{
    return w = w + n;
}

week& operator-=(week& w, int n) 
{
    return w = w - n;
}

ostream& operator<<(ostream& os, const week& w)
{
    const char* name[] = { "星期日","星期一","星期二","星期三","星期四","星期五","星期六" };
    int val = int(w);
    if (val >= 0 && val <= 6)
        os << name[val];
    else
        os << "错误";
    return os;
}

istream& operator>>(istream& is, week& w) 
{
    char str[80];
    is >> str;
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = tolower(str[i]);

    int val = -1;
    if (strcmp(str, "sun") == 0) 
        val = 0;
    else if (strcmp(str, "mon") == 0) 
        val = 1;
    else if (strcmp(str, "tue") == 0) 
        val = 2;
    else if (strcmp(str, "wed") == 0) 
        val = 3;
    else if (strcmp(str, "thu") == 0) 
        val = 4;
    else if (strcmp(str, "fri") == 0) 
        val = 5;
    else if (strcmp(str, "sat") == 0) 
        val = 6;

    w = week(val);
    return is;
}