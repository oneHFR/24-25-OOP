/* 2152131 ����� �ƿ� */
#include <iostream>
#include "16-b4.h"
using namespace std;

/* ���� Date ������г�Ա����������ʵ�� */
/* ������� */
static int fixYear(int year) {
    return (year >= 1900 && year <= 2099) ? year : 2000;
}

static int fixMonth(int month) {
    return (month >= 1 && month <= 12) ? month : 1;
}

static int fixDay(int year, int month, int day) {
    int maxDay = MONTH_DAYS[month];
    if (month == 2 && isLeap(year))
        maxDay = 29;
    return (day >= 1 && day <= maxDay) ? day : 1;
}


/* ���� */
Date::Date() : year(2000), month(1), day(1) {}

Date::Date(int y, int m, int d) {
    year = fixYear(y);
    month = fixMonth(m);
    day = fixDay(year, month, d);
}

Date::Date(int days) {
    if (days < 1) days = 1;
    if (days > 73049) days = 73049;

    int y = 1900;
    while (days > 365 + isLeap(y)) {
        days -= 365 + isLeap(y);
        y++;
    }

    int m = 1;
    while (m <= 12) {
        int monthDays = MONTH_DAYS[m];
        if (m == 2 && isLeap(y))
            monthDays = 29;
        if (days <= monthDays)
            break;
        days -= monthDays;
        m++;
    }

    year = y;
    month = m;
    day = days;
}


/* ��Ա */
void Date::set(int y, int m, int d) {
    if (y != 0)
        year = fixYear(y);
    if (m != 0)
        month = fixMonth(m);
    if (d != 0)
        day = fixDay(year, month, d);
    else
        day = fixDay(year, month, day);
}

void Date::get(int& y, int& m, int& d) {
    y = year;
    m = month;
    d = day;
}

void Date::show() {
    cout << year << "��" << month << "��" << day << "��" << endl;
}


/* ����ת�� */
Date::operator int() const {
    int totalDays = 0;

    for (int y = 1900; y < year; y++)
        totalDays += 365 + isLeap(y);

    for (int m = 1; m < month; m++) {
        totalDays += MONTH_DAYS[m];
        if (m == 2 && isLeap(year))
            totalDays++;
    }

    totalDays += day;

    return totalDays;
}


/* ��������� */
Date operator+(const Date& d, int days) {
    int totalDays = int(d) + days;
    return Date(totalDays);
}

Date operator+(int days, const Date& d) {
    return d + days;
}

Date operator-(const Date& d, int days) {
    return d + (-days);
}

int operator-(const Date& d1, const Date& d2) {
    return int(d1) - int(d2);
}

Date& Date::operator++() {
    *this = *this + 1;
    return *this;
}

Date Date::operator++(int) {
    Date temp = *this;
    ++*this;
    return temp;
}

Date& Date::operator--() {
    *this = *this - 1;
    return *this;
}

Date Date::operator--(int) {
    Date temp = *this;
    --*this;
    return temp;
}


/* �Ƚ� */
bool Date::operator>(const Date& d) const {
    return int(*this) > int(d);
}

bool Date::operator>=(const Date& d) const {
    return int(*this) >= int(d);
}

bool Date::operator<(const Date& d) const {
    return int(*this) < int(d);
}

bool Date::operator<=(const Date& d) const {
    return int(*this) <= int(d);
}

bool Date::operator==(const Date& d) const {
    return int(*this) == int(d);
}

bool Date::operator!=(const Date& d) const {
    return int(*this) != int(d);
}


/* ������� */
ostream& operator<<(ostream& out, const Date& d) {
    out << d.year << "��" << d.month << "��" << d.day << "��";
    return out;
}

istream& operator>>(istream& in, Date& d) {
    int y, m, day;
    in >> y >> m >> day;
    d = Date(y, m, day);
    return in;
}


/* �������Ҫ������ȫ�ֺ�����ʵ�֣�����д�ڴ˴� */
bool isLeap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}