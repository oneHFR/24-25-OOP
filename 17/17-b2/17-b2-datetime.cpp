/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <iomanip>
#include "17-b2-datetime.h"
using namespace std;



/* --- 给出DateTime类的成员函数的体外实现(含友元及其它必要的公共函数)  --- */ 

/* 构造函数实现 */
DateTime::DateTime() : Date(), Time()
{

}

DateTime::DateTime(int y, int m, int d, int h, int min, int s)
{
    int m_day[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    int temp = m == 2 ? m_day[m] + is_leapyear(y) : m_day[m];
    if (y < 1900 || y > 2099 || m < 1 || m > 12 || d < 1 || d > temp
        || h < 0 || h > 23 || min < 0 || min > 59 || s < 0 || s > 59) {
        year = 1900;
        month = 1;
        day = 1;
        hour = 0;
        minute = 0;
        second = 0;
    }
    else {
        year = y;
        month = m;
        day = d;
        hour = h;
        minute = min;
        second = s;
    }
}

DateTime::DateTime(long long s)
    : Date(static_cast<int>(((s% sum + sum) % sum) / total)),
    Time(static_cast<int>(((s% sum + sum) % sum) % total))
{

}


DateTime::operator long long() const
{
    long long s = 0;
    Date d(year, month, day);
    Time t(hour, minute, second);
    s = d * (long long)total + (int)t;
    return s;
}

void DateTime::set()
{
    Date::set();
    Time::set();
}

void DateTime::set(int y)
{
    (*this).set(y, 1, 1, 0, 0, 0);
}

void DateTime::set(int y, int m)
{
    (*this).set(y, m, 1, 0, 0, 0);
}

void DateTime::set(int y, int m, int d)
{
    (*this).set(y, m, d, 0, 0, 0);
}

void DateTime::set(int y, int m, int d, int h)
{
    (*this).set(y, m, d, h, 0, 0);
}

void DateTime::set(int y, int m, int d, int h, int min)
{
    (*this).set(y, m, d, h, min, 0);
}

void DateTime::set(int y, int m, int d, int h, int min, int s)
{
    int m_day[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    int temp = m == 2 ? m_day[m] + is_leapyear(y) : m_day[m];
    if (y < 1900 || y > 2099 || m < 1 || m > 12 || d < 1 || d > temp
        || h < 0 || h > 23 || min < 0 || min > 59 || s < 0 || s > 59) {
        year = 1900;
        month = 1;
        day = 1;
        hour = 0;
        minute = 0;
        second = 0;
    }
    else {
        year = y;
        month = m;
        day = d;
        hour = h;
        minute = min;
        second = s;
    }
}

void DateTime::show() const
{
    cout << std::right;
    cout << year << "-" << setfill('0') << setw(2) << month << "-" << setw(2) << day << " "
        << setw(2) << hour << ":" << setw(2) << minute << ":" << setw(2) << second
        << setfill(' ') << endl;
}

void DateTime::get(int& y, int& mon, int& d, int& h, int& m, int& s)
{
    y = year;
    mon = month;
    d = day;
    h = hour;
    m = minute;
    s = second;
}

/* 运算符重载实现 */
const DateTime operator+(const DateTime& d, long long s)
{
    long long day = (long long)(d);
    long long res = (((long long)s + day) % sum + sum) % sum;
    return res;
}

const DateTime operator+(long long s, const DateTime& d)
{
    return (d + s);
}

const DateTime operator+(const DateTime& d, int s)
{
    return (d + (long long)s);
}

const DateTime operator+(int s, const DateTime& d)
{
    return (d + (long long)s);
}

const DateTime operator+(const DateTime& d, long s)
{
    return (d + (long long)s);
}

const DateTime operator+(long s, const DateTime& d)
{
    return (d + (long long)s);
}

const long long operator-(DateTime& t1, DateTime& t2)
{
    return (long long)t1 - (long long)t2;
}

const DateTime operator-(const DateTime& d, long long t)
{
    long long sum = (long long)d - (long long)t;
    sum = ((sum % ::sum + ::sum) % ::sum);
    return sum;
}

const DateTime operator-(const DateTime& d, long t)
{
    return (d - (long long)t);
}

const DateTime operator-(const DateTime& d, int t)
{
    return (d - (long long)t);
}

DateTime& DateTime::operator++()
{
    long long s = (long long)(*this);
    s++;
    s = (s % sum + sum) % sum;
    *this = DateTime(s);
    return *this;
}

DateTime DateTime::operator++(int)
{
    DateTime t(*this);
    long long s = (long long)(*this);
    s++;
    s = (s % sum + sum) % sum;
    *this = DateTime(s);
    return t;
}

DateTime& DateTime::operator--()
{
    long long s = (long long)(*this);
    s--;
    s = (s % sum + sum) % sum;
    *this = DateTime(s);
    return *this;
}

DateTime DateTime::operator--(int)
{
    DateTime t(*this);
    long long s = (long long)(*this);
    s--;
    s = (s % sum + sum) % sum;
    *this = DateTime(s);
    return t;
}

bool DateTime::operator>(const DateTime& t)
{
    return (long long)(*this) > (long long)t;
}

bool DateTime::operator>=(const DateTime& t)
{
    return (long long)(*this) >= (long long)t;
}

bool DateTime::operator<(const DateTime& t)
{
    return (long long)(*this) < (long long)t;
}

bool DateTime::operator<=(const DateTime& t)
{
    return (long long)(*this) <= (long long)t;
}

bool DateTime::operator==(const DateTime& t)
{
    return (long long)(*this) == (long long)t;
}

bool DateTime::operator!=(const DateTime& t)
{
    return (long long)(*this) != (long long)t;
}

ostream& operator<<(ostream& out, const DateTime& t)
{
    out << std::right;
    out << t.year << setfill('0') << '-' << setw(2) << t.month << '-'
        << setw(2) << t.day << ' ' << setw(2) << t.hour << ':'
        << setw(2) << t.minute << ':' << setw(2) << t.second;
    out << setfill(' ');
    return out;
}

istream& operator>>(istream& in, DateTime& dt)
{
    int y, mon, d, h, m, s;
    in >> y >> mon >> d >> h >> m >> s;
    dt.set(y, mon, d, h, m, s);
    return in;
}