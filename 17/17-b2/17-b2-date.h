/* 2152131 吴洪蕊 计科 */
#pragma once
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

/* 如果有其它全局函数需要声明，写于此处 */
const int MAXDATENUM = 73049;  // 1900/1/1 到 2099/12/31 的总天数


bool is_leapyear(const int year);
int check_year_legal(const int year);
int check_month_legal(const int month);
int check_day_legal(const int year, const int month, const int day);

/* Date类的声明 */ 
class Date {
protected:
	/* 除这三个以外，不允许再定义任何数据成员 */ 
	int year;
	int month;
	int day;
public:
	/* 允许需要的成员函数及友元函数的声明 */
    Date();
    Date(const int y, const int m, const int d);
    Date(int totaldays);

    void set(const int y = 1900, const int m = 1, const int d = 1);
    void get(int& y, int& m, int& d);
    void show() const;


	/* 允许加入友元声明（如果有必要） */
    operator int() const;

    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);
    bool operator>(const Date& date);
    bool operator>=(const Date& date);
    bool operator<(const Date& date);
    bool operator<=(const Date& date);
    bool operator==(const Date& date);
    bool operator!=(const Date& date);

    friend Date operator+(const Date& date, const int days);
    friend Date operator+(const int days, const Date& date);
    friend Date operator-(const Date& date, const int days);
    friend int operator-(const Date& date1, const Date& date2);
    friend ostream& operator<<(ostream& out, const Date& date);
    friend istream& operator>>(istream& in, Date& date);
};

