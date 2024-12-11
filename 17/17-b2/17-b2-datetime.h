/* 2152131 吴洪蕊 计科 */
#pragma once

#include "17-b2-date.h"
#include "17-b2-time.h"

/* 如果有其它全局函数需要声明，写于此处 */
const long long sum = MAXDATENUM * (long long)total;


/* DateTime类的基本要求：
	1、不允许定义任何数据成员
	2、尽量少定义成员函数 
*/

class DateTime:public Date, public Time {
protected:
	/* 不允许再定义任何数据成员 */ 

public:
	/* 不允许再定义任何数据成员，允许需要的成员函数及友元函数的声明 */

    DateTime(int y, int m, int d, int h, int min, int s);
    DateTime();
    DateTime(long long sum);

    operator long long()const;

    void set(int y, int m, int d, int h, int min, int s);
    void set(int y, int m, int d, int h, int min);
    void set(int y, int m, int d, int h);
    void set(int y, int m, int d);
    void set(int y, int m);
    void set(int y);
    void set();
    void show()const;
    void get(int& y, int& mon, int& d, int& h, int& m, int& s);


    /* 运算符重载实现 */

    DateTime& operator++();
    DateTime operator++(int);
    DateTime& operator--();
    DateTime operator--(int);

    bool operator>(const DateTime& t);
    bool operator>=(const DateTime& t);
    bool operator<(const DateTime& t);
    bool operator<=(const DateTime& t);
    bool operator==(const DateTime& t);
    bool operator!=(const DateTime& t);

    friend const DateTime operator-(const DateTime& time, long long sum);
    friend const DateTime operator-(const DateTime& time, long sum);
    friend const DateTime operator-(const DateTime& time, int sum);
    friend const long long operator-(DateTime& t1, DateTime& t2);
    friend ostream& operator<<(ostream& out, const DateTime& d);
    friend istream& operator>>(istream& in, DateTime& dt);
    friend const DateTime operator+(const DateTime& time, long long sum);
    friend const DateTime operator+(long long sum, const DateTime& time);
    friend const DateTime operator+(const DateTime& time, int sum);
    friend const DateTime operator+(int sum, const DateTime& time);
    friend const DateTime operator+(const DateTime& time, long sum);
    friend const DateTime operator+(long sum, const DateTime& time);
};


