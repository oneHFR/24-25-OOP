/* 2152131 吴洪蕊 计科 */
#pragma once

#include <iostream>
using namespace std;

enum class week { sun, mon, tue, wed, thu, fri, sat };

/* 允许添加相应的函数声明 */

ostream& operator<<(ostream& out, const week& w);
istream& operator>>(istream& in, week& w);

week& operator++(week& w);    // 前缀++
week operator++(week& w, int);// 后缀++
week& operator--(week& w);    // 前缀--
week operator--(week& w, int);// 后缀--

week operator+(const week& w, int n);
week operator-(const week& w, int n);
week& operator+=(week& w, int n);
week& operator-=(week& w, int n);
