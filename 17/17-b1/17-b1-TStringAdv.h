/* 2152131 Œ‚∫È»Ô º∆ø∆ */
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define DEFAULT INT_MIN
#include "17-b1-TString.h"
#include <climits>

using namespace std;

class TStringAdv : public TString {
protected:

public:
	TStringAdv(const char* s);
	TStringAdv();
	TStringAdv(const TStringAdv& s);
	TStringAdv(const TString& s);

	TStringAdv& assign(const TStringAdv& ts2);
	TStringAdv& assign(const char* s);
	TStringAdv& assign(const char& c);

	TStringAdv& append(const TStringAdv& ts2);
	TStringAdv& append(const char* s);
	TStringAdv& append(const char& c);

	TStringAdv& insert(const TStringAdv& ts2, int pos);
	TStringAdv& insert(const char* s, int pos);
	TStringAdv& insert(const char& c, int pos);

	TStringAdv& erase(const TStringAdv& ts2);
	TStringAdv& erase(const char* s);
	TStringAdv& erase(const char& c);

	const TStringAdv substr(const int pos, const int len = DEFAULT);
	char& at(const int n);
};

int TStringAdvLen(const TString&);
