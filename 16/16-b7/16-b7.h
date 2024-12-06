/* 2152131 吴洪蕊 计科 */
#pragma once

#include <iostream>
using namespace std;

enum week { sun, mon, tue, wed, thu, fri, sat };

/* 允许添加相应的函数声明 */

week& operator++(week& w);
week operator++(week& w, int);
week& operator--(week& w);
week operator--(week& w, int);

week operator+(week w, int n);
week operator-(week w, int n);
int operator-(week w1, week w2);
week& operator+=(week& w, int n);
week& operator-=(week& w, int n);

ostream& operator<<(ostream& os, const week& w);
istream& operator>>(istream& in, week& w);