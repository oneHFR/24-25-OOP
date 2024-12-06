/* 2152131 吴洪蕊 计科 */

/* 允许添加需要的头文件、宏定义等 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <cstring>
#include "16-b5.h"
using namespace std;

/* 给出 TString 类的所有成员函数的体外实现 */

TString::~TString()
{
    if (content)
        delete[] content;
}

TString::TString()
{
    len = 0;
    content = NULL;
}

TString::TString(const char* const input)
{
    if (!input) {
        len = 0;
        content = NULL;
        return;
    }
    len = strlen(input);
    //content = new char[len + 1] {0};
    content = new char[len + 1];
    content[0] = '\0';
    if (!content) {
        cout << "OVERFLOW" << endl; // flag
        return;
    }
    strcpy(content, input);
}

TString::TString(const TString& input)
{
    len = input.len;
    if (!len) {
        content = NULL;
        return;
    }
    content = new char[len + 1];
    content[0] = '\0';
    if (!content) {
        cout << "OVERFLOW" << endl;
        return;
    }
    strcpy(content, input.content);
}

istream& operator>>(istream& in, TString& s)
{
    s.len = 0;
    if (s.content) {
        delete[] s.content;
        s.content = NULL;
    }

    char input;
    char* temp = NULL;
    input = in.get();
    while (input != ' ' && input != '\r' && input != '\n') {
        s.len++;
        if (s.content)
            temp = s.content;
        //s.content = new char[s.len + 1] {0};
        s.content = new char[s.len + 1];
        s.content[0] = '\0';
        if (!s.content) {
            cout << "OVERFLOW" << endl;
            return in;
        }
        if (temp) {
            strcpy(s.content, temp);
            delete[] temp;
        }
        s.content[s.len - 1] = input;
        input = in.get();
    }
    return in;
}

ostream& operator<<(ostream& out, const TString& s)
{
    if (!s.len)
        out << "<EMPTY>";
    else
        out << s.content;
    return out;
}

const char* TString::c_str() const
{
    return content;
}

TString& TString::operator=(const TString& s)
{
    if (this == &s)
        return *this;

    if (content)
        delete[] content;

    len = s.len;
    if (!len) {
        content = NULL;
        return *this;
    }

    //content = new char[len + 1] {0};
    content = new char[len + 1];
    content[0] = '\0';
    if (!content) {
        cout << "OVERFLOW" << endl;
        return *this;
    }
    strcpy(content, s.content);
    return *this;
}

TString& TString::operator=(const char* const s)
{
    if (content)
        delete[] content;

    if (!s) {
        len = 0;
        content = NULL;
        return *this;
    }

    len = strlen(s);
    content = new char[len + 1];
    content[0] = '\0';
    if (!content) {
        cout << "OVERFLOW" << endl;
        return *this;
    }
    strcpy(content, s);
    return *this;
}

const TString operator+(const char* const s1, const TString& s2)
{
    if (!s1)
        return s2;
    if (!s2.len)
        return TString(s1);

    TString result;
    result.len = strlen(s1) + s2.len;
    //result.content = new char[result.len + 1] {0};
    result.content = new char[result.len + 1];
    result.content[0] = '\0';
    if (!result.content) {
        cout << "OVERFLOW" << endl;
        return result;
    }
    strcpy(result.content, s1);
    strcat(result.content, s2.content);
    return result;
}

const TString operator+(const TString& s1, const char* const s2)
{
    if (!s2)
        return s1;
    if (!s1.len)
        return TString(s2);

    TString result;
    result.len = s1.len + strlen(s2);
    //result.content = new char[result.len + 1] {0};
    result.content = new char[result.len + 1];
    result.content[0] = '\0';
    if (!result.content) {
        cout << "OVERFLOW" << endl;
        return result;
    }
    strcpy(result.content, s1.content);
    strcat(result.content, s2);
    return result;
}

const TString operator+(const TString& s1, const TString& s2)
{
    if (!s1.len)
        return s2;
    if (!s2.len)
        return s1;

    TString result;
    result.len = s1.len + s2.len;
    //result.content = new char[result.len + 1] {0};
    result.content = new char[result.len + 1];
    result.content[0] = '\0';
    if (!result.content) {
        cout << "OVERFLOW" << endl;
        return result;
    }
    strcpy(result.content, s1.content);
    strcat(result.content, s2.content);
    return result;
}

const TString operator+(const char ch, const TString& s)
{
    TString temp;
    temp.len = 1;
    temp.content = new char[2] {ch, '\0'};
    if (!temp.content) {
        cout << "OVERFLOW" << endl;
        return s;
    }
    return temp + s;
}

const TString operator+(const TString& s, const char ch)
{
    TString temp;
    temp.len = 1;
    temp.content = new char[2] {ch, '\0'};
    if (!temp.content) {
        cout << "OVERFLOW" << endl;
        return s;
    }
    return s + temp;
}

TString& TString::operator+=(const char ch)
{
    TString temp = *this + ch;
    *this = temp;
    return *this;
}

TString& TString::operator+=(const TString& s)
{
    if (s.content == NULL)
        return *this;
    if (content == NULL){
        *this = s;
        return *this;
    }
    char* tmp = content;
    len += s.len;
    //content = new(nothrow)char[len + 1];
    content = new(nothrow)char[len + 1];
    content[0] = '\0';
    if (content == NULL){
        cout << "OVERFLOW" << endl;
        return *this;
    }
    strcpy(content, tmp);
    strcat(content, s.content);
    delete[]tmp;
    return (*this);
}

TString& TString::append(const TString& s)
{
    if (s.content == NULL)
        return *this;
    if (content == NULL) {
        *this = s;
        return *this;
    }
    char* tmp = content;
    len += s.len;
    //content = new(nothrow)char[len + 1];
    content = new(nothrow)char[len + 1];
    content[0] = '\0';
    if (content == NULL) {
        cout << "OVERFLOW" << endl;
        return *this;
    }
    strcpy(content, tmp);
    strcat(content, s.content);
    delete[]tmp;
    return (*this);
}

TString& TString::append(const char ch)
{
    TString temp = *this + ch;
    *this = temp;
    return *this;
}

const TString operator-(const TString& s1, const TString& s2)
{
    if (strstr(s1.content, s2.content) == NULL)
    {
        return s1;
    }
    TString t;
    char* p = strstr(s1.content, s2.content);
    t.len = s1.len - s2.len;
    t.content = new(nothrow)char[t.len + 1];
    if (t.content == NULL)
    {
        cout << "OVERFLOW" << endl;
        return NULL;
    }
    strncpy(t.content, s1.content, p - s1.content);
    t.content[p - s1.content] = '\0';
    strcat(t.content, p + s2.len);
    return t;
}

const TString operator-(const TString& s, const char ch)
{
    TString temp;
    temp.len = 1;
    temp.content = new char[2] {0};
    if (!temp.content) {
        cout << "OVERFLOW" << endl;
        getchar();
        return s;
    }
    temp.content[0] = ch;
    return s - temp;
}

TString& TString::operator-=(const TString& s)
{
    *this = *this - s;
    return *this;
}

TString& TString::operator-=(const char ch)
{
    *this = *this - ch;
    return *this;
}

const TString TString::operator*(const int n)
{
    if (n <= 0 || !len)
        return TString();

    TString result;
    result.len = len * n;
    //result.content = new char[result.len + 1] {0};
    result.content = new char[result.len + 1];
    result.content[0] = '\0';
    if (!result.content) {
        cout << "OVERFLOW" << endl;
        return result;
    }

    for (int i = 0; i < n; i++) {
        strcat(result.content, content);
    }
    return result;
}

TString& TString::operator*=(const int n)
{
    *this = *this * n;
    return *this;
}

const TString operator!(const TString& s)
{
    if (!s.len)
        return s;

    TString result = s;
    for (int i = 0; i < s.len / 2; i++) {
        char temp = result.content[i];
        result.content[i] = result.content[s.len - 1 - i];
        result.content[s.len - 1 - i] = temp;
    }
    return result;
}

int operator>(const TString& s1, const TString& s2)
{
    if (!s1.len || !s2.len)
        return s1.len > s2.len;
    return strcmp(s1.content, s2.content) > 0;
}

int operator>=(const TString& s1, const TString& s2)
{
    if (!s1.len || !s2.len)
        return s1.len >= s2.len;
    return strcmp(s1.content, s2.content) >= 0;
}

int operator<(const TString& s1, const TString& s2)
{
    if (!s1.len || !s2.len)
        return s1.len < s2.len;
    return strcmp(s1.content, s2.content) < 0;
}

int operator<=(const TString& s1, const TString& s2)
{
    if (!s1.len || !s2.len)
        return s1.len <= s2.len;
    return strcmp(s1.content, s2.content) <= 0;
}

int operator==(const TString& s1, const TString& s2)
{
    if (!s1.len || !s2.len)
        return s1.len == s2.len;
    return strcmp(s1.content, s2.content) == 0;
}

int operator!=(const TString& s1, const TString& s2)
{
    if (!s1.len || !s2.len)
        return s1.len != s2.len;
    return strcmp(s1.content, s2.content) != 0;
}

int TString::length() const
{
    return len;
}

char& TString::operator[](int n)
{
    return content[n];
}

/* 如果有需要的其它全局函数的实现，可以写于此处 */

int TStringLen(const TString& s)
{
    return s.length();
}
