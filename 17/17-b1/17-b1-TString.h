/* 2152131 ����� �ƿ� */

#pragma once

#include <iostream>
using namespace std;

/* ��ȫTString��Ķ��壬���г�Ա����������ʵ�֣���Ҫ�ڴ˴�����ʵ�� */
class TString {
protected:
    char* content;
    int   len;
    /* ������Ҫ������������ݳ�Ա����Ա��������Ԫ������ */
public:
    /* ������Ҫ������������ݳ�Ա����Ա��������Ԫ������ */

    TString();
    TString(const char* const input);
    TString(const TString& input);
    ~TString();

    //��Ա����
    const char* c_str() const;
    int length() const;
    //TString& append(const TString& s);
    //TString& append(const char ch);

    //��Ա
    TString& operator=(const TString& s);
    TString& operator=(const char* const s);
    TString& operator+=(const TString& s);
    TString& operator+=(const char ch);
    TString& operator-=(const TString& s);
    TString& operator-=(const char ch);
    const TString operator*(const int n);
    TString& operator*=(const int n);
    char& operator[](int n);

    //��Ԫ
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

/* ���������ȫ�ֺ�����Ҫ������д�ڴ˴� */
int TStringLen(const TString& s);

/* �������Ҫ�ĺ궨�塢ֻ��ȫ�ֱ����ȣ�д�ڴ˴� */

