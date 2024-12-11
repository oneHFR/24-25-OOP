/* 2152131 ����� �ƿ� */
#pragma once
#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

/* ���������ȫ�ֺ�����Ҫ������д�ڴ˴� */
const int MAXDATENUM = 73049;  // 1900/1/1 �� 2099/12/31 ��������


bool is_leapyear(const int year);
int check_year_legal(const int year);
int check_month_legal(const int month);
int check_day_legal(const int year, const int month, const int day);

/* Date������� */ 
class Date {
protected:
	/* �����������⣬�������ٶ����κ����ݳ�Ա */ 
	int year;
	int month;
	int day;
public:
	/* ������Ҫ�ĳ�Ա��������Ԫ���������� */
    Date();
    Date(const int y, const int m, const int d);
    Date(int totaldays);

    void set(const int y = 1900, const int m = 1, const int d = 1);
    void get(int& y, int& m, int& d);
    void show() const;


	/* ���������Ԫ����������б�Ҫ�� */
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

