/* 2152131 ����� �ƿ� */
#pragma once

#include <iostream>
using namespace std;

enum class week { sun, mon, tue, wed, thu, fri, sat };

/* ���������Ӧ�ĺ������� */

ostream& operator<<(ostream& out, const week& w);
istream& operator>>(istream& in, week& w);

week& operator++(week& w);    // ǰ׺++
week operator++(week& w, int);// ��׺++
week& operator--(week& w);    // ǰ׺--
week operator--(week& w, int);// ��׺--

week operator+(const week& w, int n);
week operator-(const week& w, int n);
week& operator+=(week& w, int n);
week& operator-=(week& w, int n);
