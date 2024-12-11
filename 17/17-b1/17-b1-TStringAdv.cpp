/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include "17-b1-TStringAdv.h"
#include "17-b1-TString.h"
#include <iostream>
#include <climits>
#include <cstring>
using namespace std;


TStringAdv::TStringAdv() :TString()
{

}
TStringAdv::TStringAdv(const char* s) :TString(s)
{

}
TStringAdv::TStringAdv(const TString& s) : TString(s)
{

}
TStringAdv::TStringAdv(const TStringAdv& s) : TString(s)
{

}

//assign
TStringAdv& TStringAdv::assign(const TStringAdv& ts2) 
{
    if (this != &ts2)
        TString::operator=(ts2);
    return *this;
}

TStringAdv& TStringAdv::assign(const char* s) 
{
    TString::operator=(s);
    return *this;
}

TStringAdv& TStringAdv::assign(const char& c) 
{
    if (c == '\0') {
        delete[] content;
        len = 0;
        content = new char[1];
        content[0] = '\0';
    }
    else {
        char s[2] = { c, '\0' };
        TString::operator=(s);
    }
    return *this;
}

//append
TStringAdv& TStringAdv::append(const TStringAdv& ts2)  //flag
{
    if (ts2.content == nullptr || ts2.len == 0) {
        return *this;
    }

    if (content == nullptr || len == 0) {
        content = new(nothrow) char[ts2.len + 1];
        if (content == nullptr) {
            cout << "内存 Allocation Failed! " << endl;
            return *this;
        }
        len = ts2.len;
        strcpy(content, ts2.content);
        return *this;
    }

    char* newStr = new(nothrow) char[len + ts2.len + 1];
    if (newStr == nullptr) {
        cout << "内存 Allocation Failed! " << endl;
        return *this;
    }

    strcpy(newStr, content);
    strcat(newStr, ts2.content);
    delete[] content;
    content = newStr;
    len += ts2.len;

    return *this;
}

TStringAdv& TStringAdv::append(const char* s) 
{
    if (s == nullptr || *s == '\0') {
        return *this;
    }

    size_t newLen = len + strlen(s);
    if (content == nullptr || len == 0) {
        content = new(nothrow) char[newLen + 1];
        if (content == nullptr) {
            cout << "内存 Allocation Failed!" << endl;
            return *this;
        }
        strcpy(content, s);
        len = newLen;
        return *this;
    }

    char* newStr = new(nothrow) char[newLen + 1];
    if (newStr == nullptr) {
        cout << "内存 Allocation Failed!" << endl;
        return *this;
    }
    strcpy(newStr, content);
    strcat(newStr, s);
    delete[] content;

    content = newStr;
    len = newLen;

    return *this;
}

TStringAdv& TStringAdv::append(const char& c) 
{
    if (c == '\0')
        return *this;
    char s[2] = { c, '\0' };
    return append(s);
}

// insert
TStringAdv& TStringAdv::insert(const TStringAdv& ts2, int pos) 
{
    if (pos < 1 || pos > len + 1 || ts2.content == NULL || ts2.len == 0)
        return *this;

    if (content == NULL || len == 0){
        content = new(nothrow) char[ts2.len + 1];
        len = ts2.len;
        strcpy(content, ts2.content);
        return *this;
    }

    if (&ts2 == this){
        insert(ts2.content, pos);
        return *this;
    }

    char* old_content = content;

    content = new(nothrow) char[len + ts2.len + 1];
    if (content == NULL){
        cout << "内存 Allocation Failed! " << endl;
        return *this;
    }

    strncpy(content, old_content, pos - 1);
    content[pos - 1] = 0;
    strcat(content, ts2.content);
    if (pos != len + 1)
        strcat(content, old_content + pos - 1);

    len += ts2.len;
    delete[] old_content;

    return *this;
}

TStringAdv& TStringAdv::insert(const char* s, int pos) 
{
    if (pos < 1 || pos > len + 1 || s == NULL || strlen(s) == 0)
        return *this;

    char* newStr = new char[len + strlen(s) + 1];
    strncpy(newStr, content, pos - 1);
    strcpy(newStr + pos - 1, s);
    strcpy(newStr + pos - 1 + strlen(s), content + pos - 1);

    delete[] content;
    content = newStr;
    len += strlen(s);
    return *this;
}

TStringAdv& TStringAdv::insert(const char& c, int pos) 
{
    if (pos < 1 || pos > len + 1)
        return *this;

    if (c == '\0') {
        if (pos == 1) {
            delete[] content;
            content = NULL;
            len = 0;
        }
        else {
            content[pos - 1] = '\0';
            len = pos - 1;
        }
        return *this;
    }

    char* newStr = new char[len + 2];
    strncpy(newStr, content, pos - 1);
    newStr[pos - 1] = c;
    strcpy(newStr + pos, content + pos - 1);

    delete[] content;
    content = newStr;
    len++;
    return *this;
}

// erase
TStringAdv& TStringAdv::erase(const TStringAdv& ts2) 
{
    if (len == 0)
        return *this;

    char* p = strstr(content, ts2.content);
    if (p) {
        strcpy(p, p + ts2.len);
        len -= ts2.len;
    }
    return *this;
}

TStringAdv& TStringAdv::erase(const char* s) 
{
    if (len == 0 || s == NULL)
        return *this;

    char* p = strstr(content, s);
    if (p) {
        strcpy(p, p + strlen(s));
        len -= strlen(s);
    }
    return *this;
}

TStringAdv& TStringAdv::erase(const char& c) 
{
    char s[2] = { c, '\0' };
    return erase(s);
}

// substr
const TStringAdv TStringAdv::substr(const int pos, int len) 
{
    int subLen;
    TStringAdv result;

    if (len <= 0 && len != DEFAULT)
        return "";

    if (len > TString::len)
        len = TString::len;

    if (len == DEFAULT)
        subLen = TString::len - pos + 1;
    else
        subLen = len;

    if (pos < 1 || pos > TStringAdv::len)
        return "";

    result.content = new(nothrow) char[subLen + 1];
    result.len = subLen;

    if (result.content == nullptr)
    {
        cout << "内存 Allocation Failed! " << endl;
        return *this;
    }

    memset(result.content, 0, subLen + 1);
    memcpy(result.content, content + pos - 1, subLen);

    return result;
}

// at
char& TStringAdv::at(const int n) 
{
	return content[n];
}

// TStringAdvLen
int TStringAdvLen(const TString& ts)
{
    return ts.length();
}