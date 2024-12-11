/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <iomanip>
#include "17-b2-date.h"

using namespace std;

/* --- 给出Date类的成员函数的体外实现(含友元及其它必要的公共函数)  --- */ 



/* 全局函数实现 */
bool is_leapyear(const int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int check_year_legal(const int year)
{
    return (year >= 1900 && year <= 2099);
}

int check_month_legal(const int month)
{
    return (month >= 1 && month <= 12);
}

int check_day_legal(const int year, const int month, const int day)
{
    int month_day[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    if (is_leapyear(year))
        ++month_day[2];
    if (day < 1 || day > month_day[month])
        return 0;
    else
        return 1;
}


/* 构造函数实现 */
Date::Date()
{
    year = 1900;
    month = 1;
    day = 1;
}

Date::Date(const int y, const int m, const int d)
{
    if (!check_year_legal(y) || !check_month_legal(m) || !check_day_legal(y, m, d)) {
        year = 1900;
        month = 1;
        day = 1;
    }
    else {
        year = y;
        month = m;
        day = d;
    }
}

Date::Date(int totalday)
{
    int month_day[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    while (totalday < 0)
        totalday += MAXDATENUM;
    totalday %= MAXDATENUM;
    totalday++;
    set(1900, 1, 1);
    while (totalday > 365){
        if (is_leapyear(year)){
            if (totalday > 366)
                totalday -= 366;
            else
                break;
        }
        else
            totalday -= 365;
        ++year;
    }

    if (is_leapyear(year))
        ++month_day[2];
    while (totalday > month_day[month]){
        totalday -= month_day[month];
        ++month;
    }

    day = totalday;
}

Date::operator int() const
{
    int totaldays = 0;
    int month_day[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    for (int y = 1900; y < year; y++)
        totaldays += is_leapyear(y) ? 366 : 365;
        
    if (is_leapyear(year))
        month_day[2] = 29;
        
    for (int m = 1; m < month; m++)
        totaldays += month_day[m];

    totaldays += day;
    totaldays--;
    
    return totaldays;
}


/* 成员函数实现 */
void Date::set(const int y, const int m, const int d)
{
    int good = 1;
    good &= check_year_legal(y);
    good &= check_month_legal(m);
    if (!good){
        year = 1900;
        month = 1;
        day = 1;
        return;
    }
    else{
        year = y;
        month = m;
        good &= check_day_legal(year, month, d);
    }
    if (!good){
        year = 1900;
        month = 1;
        day = 1;
    }
    else
        day = d;
}

void Date::get(int& y, int& m, int& d)
{
    y = year;
    m = month;
    d = day;
}

void Date::show() const
{
    cout << year << '-'
        << setfill('0') << setw(2) << month << '-'
        << setfill('0') << setw(2) << day << endl;
}


/* 重载运算符实现 */
Date& Date::operator++()
{
    int totaldays = int(*this) + 1;
    totaldays %= MAXDATENUM;
    *this = Date(totaldays);
    return *this;
}

Date Date::operator++(int)
{
    Date temp = *this;
    ++*this;
    return temp;
}

Date& Date::operator--()
{
    int totaldays = int(*this) - 1;
    while (totaldays < 0)
        totaldays += MAXDATENUM;
    *this = Date(totaldays);
    return *this;
}

Date Date::operator--(int)
{
    Date temp = *this;
    --*this;
    return temp;
}

bool Date::operator>(const Date& date)
{
    return int(*this) > int(date);
}

bool Date::operator>=(const Date& date)
{
    return int(*this) >= int(date);
}

bool Date::operator<(const Date& date)
{
    return int(*this) < int(date);
}

bool Date::operator<=(const Date& date)
{
    return int(*this) <= int(date);
}

bool Date::operator==(const Date& date)
{
    return int(*this) == int(date);
}

bool Date::operator!=(const Date& date)
{
    return int(*this) != int(date);
}


/* 友元函数实现 */
Date operator+(const Date& date, const int days)
{
    int totalday = (int)date + days;
    totalday %= MAXDATENUM;
    return Date(totalday);
}

Date operator+(const int days, const Date& date)
{
    int totalday = (int)date + days;
    totalday %= MAXDATENUM;
    return Date(totalday);
}

Date operator-(const Date& date, const int days)
{
    return date + (-days);
}

int operator-(const Date& date1, const Date& date2)
{
    return int(date1) - int(date2);
}

ostream& operator<<(ostream& out, const Date& date)
{
    out << date.year << '-'
        << setfill('0') << setw(2) << date.month << '-'
        << setfill('0') << setw(2) << date.day;
    return out;
}

istream& operator>>(istream& in, Date& date)
{
    int y, m, d;
    in >> y >> m >> d;
    date.set(y, m, d);
    return in;
}
