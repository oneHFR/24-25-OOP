/* 2152131 吴洪蕊 计科 */

#pragma once

#include <iostream>
using namespace std;

/* 如果有其它全局函数需要声明，写于此处 */
bool isLeap(int year);

/* 如果有需要的宏定义、只读全局变量等，写于此处 */
const int MONTH_DAYS[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* 补全Date类的定义，所有成员函数均体外实现，不要在此处体内实现 */
class Date {
private:
	int year;
	int month;
	int day;
	/* 不允许添加数据成员 */
public:
	/* 根据需要定义所需的成员函数、友元函数等(不允许添加数据成员) */
    Date();                                  // 默认构造函数
    Date(int days);                    // 天数转换构造函数
    Date(int y, int m, int d);     // 年月日构造函数

    // 成员函数
    void set(int y = 0, int m = 0, int d = 0);
    void get(int& y, int& m, int& d);
    void show();

    // 类型转换
    operator int() const;

    // 重载运算符
    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);

    // 比较运算符
    bool operator>(const Date& d) const;
    bool operator>=(const Date& d) const;
    bool operator<(const Date& d) const;
    bool operator<=(const Date& d) const;
    bool operator==(const Date& d) const;
    bool operator!=(const Date& d) const;

    // 友元函数
    friend Date operator+(const Date& d, int days);
    friend Date operator+(int days, const Date& d);
    friend Date operator-(const Date& d, int days);
    friend int operator-(const Date& d1, const Date& d2);
    friend ostream& operator<<(ostream& out, const Date& d);
    friend istream& operator>>(istream& in, Date& d);
};