/* 2152131 吴洪蕊 计科 */

#pragma once

#include <iostream>
using namespace std;

/* 补全TString类的定义，所有成员函数均体外实现，不要在此处体内实现 */
class TString {
protected:
    char* content;
    int   len;
    /* 根据需要定义所需的数据成员、成员函数、友元函数等 */
public:
    /* 根据需要定义所需的数据成员、成员函数、友元函数等 */

    TString();
    TString(const char* const input);
    TString(const TString& input);
    ~TString();

    //成员函数
    const char* c_str() const;
    int length() const;
    //TString& append(const TString& s);
    //TString& append(const char ch);

    //成员
    TString& operator=(const TString& s);
    TString& operator=(const char* const s);
    TString& operator+=(const TString& s);
    TString& operator+=(const char ch);
    TString& operator-=(const TString& s);
    TString& operator-=(const char ch);
    const TString operator*(const int n);
    TString& operator*=(const int n);
    char& operator[](int n);

    //友元
    friend istream& operator>>(istream& in, TString& s);
    friend ostream& operator<<(ostream& out, const TString& s);
    friend const TString operator+(const char* const s1, const TString& s2);
    friend const TString operator+(const TString& s1, const char* const s2);
    friend const TString operator+(const TString& s1, const TString& s2);
    friend const TString operator+(const char ch, const TString& s);
    friend const TString operator+(const TString& s, const char ch);
    friend const TString operator-(const TString& s1, const TString& s2);
    friend const TString operator-(const TString& s, const char ch);
    friend const TString operator!(const TString& s);
    friend int operator>(const TString& s1, const TString& s2);
    friend int operator>=(const TString& s1, const TString& s2);
    friend int operator<(const TString& s1, const TString& s2);
    friend int operator<=(const TString& s1, const TString& s2);
    friend int operator==(const TString& s1, const TString& s2);
    friend int operator!=(const TString& s1, const TString& s2);
};

/* 如果有其它全局函数需要声明，写于此处 */
int TStringLen(const TString& s);

/* 如果有需要的宏定义、只读全局变量等，写于此处 */

