/* 2152131 ����� �ƿ� */
#pragma once

#include "17-b2-date.h"
#include "17-b2-time.h"

/* ���������ȫ�ֺ�����Ҫ������д�ڴ˴� */
const long long sum = MAXDATENUM * (long long)total;


/* DateTime��Ļ���Ҫ��
	1�����������κ����ݳ�Ա
	2�������ٶ����Ա���� 
*/

class DateTime:public Date, public Time {
protected:
	/* �������ٶ����κ����ݳ�Ա */ 

public:
	/* �������ٶ����κ����ݳ�Ա��������Ҫ�ĳ�Ա��������Ԫ���������� */

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


    /* ���������ʵ�� */

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


