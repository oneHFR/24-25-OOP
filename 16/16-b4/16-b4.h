/* 2152131 ����� �ƿ� */

#pragma once

#include <iostream>
using namespace std;

/* ���������ȫ�ֺ�����Ҫ������д�ڴ˴� */
bool isLeap(int year);

/* �������Ҫ�ĺ궨�塢ֻ��ȫ�ֱ����ȣ�д�ڴ˴� */
const int MONTH_DAYS[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/* ��ȫDate��Ķ��壬���г�Ա����������ʵ�֣���Ҫ�ڴ˴�����ʵ�� */
class Date {
private:
	int year;
	int month;
	int day;
	/* ������������ݳ�Ա */
public:
	/* ������Ҫ��������ĳ�Ա��������Ԫ������(������������ݳ�Ա) */
    Date();                                  // Ĭ�Ϲ��캯��
    Date(int days);                    // ����ת�����캯��
    Date(int y, int m, int d);     // �����չ��캯��

    // ��Ա����
    void set(int y = 0, int m = 0, int d = 0);
    void get(int& y, int& m, int& d);
    void show();

    // ����ת��
    operator int() const;

    // ���������
    Date& operator++();
    Date operator++(int);
    Date& operator--();
    Date operator--(int);

    // �Ƚ������
    bool operator>(const Date& d) const;
    bool operator>=(const Date& d) const;
    bool operator<(const Date& d) const;
    bool operator<=(const Date& d) const;
    bool operator==(const Date& d) const;
    bool operator!=(const Date& d) const;

    // ��Ԫ����
    friend Date operator+(const Date& d, int days);
    friend Date operator+(int days, const Date& d);
    friend Date operator-(const Date& d, int days);
    friend int operator-(const Date& d1, const Date& d2);
    friend ostream& operator<<(ostream& out, const Date& d);
    friend istream& operator>>(istream& in, Date& d);
};