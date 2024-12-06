/* 2152131 吴洪蕊 计科 */
#include "16-b6.h"
#include <string>
#include <cstring>

bool isUninitialized(const week& w) {
    int val = int(w);
    return static_cast<unsigned int>(val) == 0xcccccccc;
}

ostream& operator<<(ostream& out, const week& w) {
    static bool firstCall = true;
    if (firstCall) {
        srand(static_cast<unsigned int>(time(0)));
        firstCall = false;
    }

    if (isUninitialized(w)) {
        int randVal = rand() % 7;
        week tempW = week(randVal);
        switch (tempW) {
            case week::sun: out << "星期日"; break;
            case week::mon: out << "星期一"; break;
            case week::tue: out << "星期二"; break;
            case week::wed: out << "星期三"; break;
            case week::thu: out << "星期四"; break;
            case week::fri: out << "星期五"; break;
            case week::sat: out << "星期六"; break;
        }
    }

    else {
        int val = int(w);
        if (val >= 0 && val <= 6) {
            switch (w) {
                case week::sun: out << "星期日"; break;
                case week::mon: out << "星期一"; break;
                case week::tue: out << "星期二"; break;
                case week::wed: out << "星期三"; break;
                case week::thu: out << "星期四"; break;
                case week::fri: out << "星期五"; break;
                case week::sat: out << "星期六"; break;
            }
        }
        else {
            out << "错误";
        }
    }
    return out;
}

istream& operator>>(istream& in, week& w) {
    char str[80];
    in >> str;

    char tmp[80];
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        tmp[i] = tolower(str[i]);
    }
    tmp[i] = '\0';

    if (strcmp(tmp, "sun") == 0)
        w = week::sun;
    else if (strcmp(tmp, "mon") == 0)
        w = week::mon;
    else if (strcmp(tmp, "tue") == 0)
        w = week::tue;
    else if (strcmp(tmp, "wed") == 0)
        w = week::wed;
    else if (strcmp(tmp, "thu") == 0)
        w = week::thu;
    else if (strcmp(tmp, "fri") == 0)
        w = week::fri;
    else if (strcmp(tmp, "sat") == 0)
        w = week::sat;
    else
        w = week(-1);
    return in;
}


// 前缀++
week& operator++(week& w) 
{
    int val = int(w);
    if (val < 0 || val > 6) {
        val = 0;
    }
    val = (val + 1) % 7;
    w = week(val);
    return w;
}

// 后缀++
week operator++(week& w, int) 
{
    week temp = w;
    ++w;
    return temp;
}

// 前缀--
week& operator--(week& w)
{
    int val = int(w);
    if (val < 0 || val > 6) {
        val = 0;
    }
    val = (val - 1 + 7) % 7;
    w = week(val);
    return w;
}

// 后缀--
week operator--(week& w, int) 
{
    week temp = w;
    --w;
    return temp;
}





week operator+(const week& w, int n) 
{
    int val = int(w);
    if (val < 0 || val > 6) {
        val = 0;
    }
    val = ((val + n) % 7 + 7) % 7;
    return week(val);
}

week operator-(const week& w, int n) 
{
    return w + (-n);
}

week& operator+=(week& w, int n) 
{
    w = w + n;
    return w;
}

week& operator-=(week& w, int n) 
{
    w = w - n;
    return w;
}



