/* 2152131 ����� �ƿ� */
#pragma once

#include <iostream>
using namespace std;

/* ���������ȫ�ֺ�����Ҫ������д�ڴ˴� */
const int total = 86400;

/* Time������� */ 
class Time {
protected:
	/* �����������⣬�������ٶ����κ����ݳ�Ա */ 
	int hour;
	int minute;
	int second;
public:
	/* ������Ҫ�ĳ�Ա��������Ԫ���������� */
	
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


	/* ���������Ԫ����������б�Ҫ�� */
    friend const Time operator+(const Time& time, int sum);
    friend const Time operator+(int sum, const Time& time);
    friend const Time operator-(const Time& time, int sum);
    friend const int operator-(const Time& t1, const Time& t2);
    friend ostream& operator<<(ostream& out, const Time& t);
    friend istream& operator>>(istream& in, Time& d);
};
