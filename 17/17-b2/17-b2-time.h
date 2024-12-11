/* 2152131 吴洪蕊 计科 */
#pragma once

#include <iostream>
using namespace std;

/* 如果有其它全局函数需要声明，写于此处 */
const int total = 86400;

/* Time类的声明 */ 
class Time {
protected:
	/* 除这三个以外，不允许再定义任何数据成员 */ 
	int hour;
	int minute;
	int second;
public:
	/* 允许需要的成员函数及友元函数的声明 */
	
    Time(int h, int m, int s);
    Time();
    Time(int sum);

    operator int()const;

    void set(int h, int m, int s);
    void set(int h);
    void set(int h, int m);
    void set();
    void show()const;
    void get(int& y, int& m, int& d);

    Time& operator++();
    Time operator++(int);
    Time& operator--();
    Time operator--(int);


    bool operator>(const Time& t);
    bool operator>=(const Time& t);
    bool operator<(const Time& t);
    bool operator<=(const Time& t);
    bool operator==(const Time& t);
    bool operator!=(const Time& t);


	/* 允许加入友元声明（如果有必要） */
    friend const Time operator+(const Time& time, int sum);
    friend const Time operator+(int sum, const Time& time);
    friend const Time operator-(const Time& time, int sum);
    friend const int operator-(const Time& t1, const Time& t2);
    friend ostream& operator<<(ostream& out, const Time& t);
    friend istream& operator>>(istream& in, Time& d);
};
