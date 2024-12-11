/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <iomanip>
#include "17-b2-time.h"
using namespace std;

/* --- 给出Time类的成员函数的体外实现(含友元及其它必要的公共函数)  --- */ 


/* 构造函数实现 */
Time::Time()
{
    hour = 0;
    minute = 0;
    second = 0;
}

Time::Time(int h, int m, int s)
{
    if (h < 0 || h > 23) {
        hour = 0;
        minute = 0;
        second = 0;
        return;
    }
    if (m < 0 || m > 59) {
        hour = 0;
        minute = 0;
        second = 0;
        return;
    }
    if (s < 0 || s > 59) {
        hour = 0;
        minute = 0;
        second = 0;
        return;
    }
    hour = h;
    minute = m;
    second = s;
}

Time::Time(int sum)
{
    sum = ((sum % total) + total) % total;
    second = sum % 60;
    sum /= 60;
    minute = sum % 60;
    hour = sum / 60;
}

Time::operator int() const
{
    return hour * 3600 + minute * 60 + second;
}

void Time::set(int h, int m, int s)
{
    if (h < 0 || h > 23) {
        hour = 0;
        minute = 0;
        second = 0;
        return;
    }
    if (m < 0 || m > 59) {
        hour = 0;
        minute = 0;
        second = 0;
        return;
    }
    if (s < 0 || s > 59) {
        hour = 0;
        minute = 0;
        second = 0;
        return;
    }
    hour = h;
    minute = m;
    second = s;
}

void Time::set(int h)
{
    (*this).set(h, 0, 0);
}

void Time::set(int h, int m)
{
    (*this).set(h, m, 0);
}

void Time::set()
{
    hour = 0;
    minute = 0;
    second = 0;
}

void Time::show() const
{
    cout << std::right;
    cout << setfill('0') << setw(2) << hour << ":"
        << setfill('0') << setw(2) << minute << ":"
        << setw(2) << second << setfill(' ') << endl;
}

void Time::get(int& y, int& m, int& d)
{
    y = hour;
    m = minute;
    d = second;
}

/* 运算符重载 */
Time& Time::operator++()
{
    int s = int(*this);
    s++;
    s = (s % total + total) % total;
    *this = Time(s);
    return *this;
}

Time Time::operator++(int)
{
    Time t(*this);
    int s = int(*this);
    s++;
    s = (s % total + total) % total;
    *this = Time(s);
    return t;
}

Time& Time::operator--()
{
    int s = int(*this);
    s--;
    s = (s % total + total) % total;
    *this = Time(s);
    return *this;
}

Time Time::operator--(int)
{
    Time t(*this);
    int s = int(*this);
    s--;
    s = (s % total + total) % total;
    *this = Time(s);
    return t;
}

bool Time::operator>(const Time& t)
{
    return int(*this) > int(t);
}

bool Time::operator>=(const Time& t)
{
    return int(*this) >= int(t);
}

bool Time::operator<(const Time& t)
{
    return int(*this) < int(t);
}

bool Time::operator<=(const Time& t)
{
    return int(*this) <= int(t);
}

bool Time::operator==(const Time& t)
{
    return int(*this) == int(t);
}

bool Time::operator!=(const Time& t)
{
    return int(*this) != int(t);
}

/* 友元函数实现 */
const Time operator+(const Time& time, int s)
{
    int t1 = time;
    s = (s % total + total) % total;
    t1 += s;
    t1 = (t1 % total + total) % total;
    return Time(t1);
}

const Time operator+(int s, const Time& time)
{
    return time + s;
}

const Time operator-(const Time& time, int s)
{
    int t1 = time;
    s = (s % total + total) % total;
    t1 -= s;
    t1 = (t1 % total + total) % total;
    return Time(t1);
}

const int operator-(const Time& t1, const Time& t2)
{
    return int(t1) - int(t2);
}

ostream& operator<<(ostream& out, const Time& t)
{
    out << std::right;
    out << setfill('0') << setw(2) << t.hour << ":"
        << setw(2) << t.minute << ":"
        << setw(2) << t.second << setfill(' ');
    return out;
}

istream& operator>>(istream& in, Time& t)
{
    int h = 0, m = 0, s = 0;
    in >> h >> m >> s;
    t.set(h, m, s);
    return in;
}