/* 计科 2152131 吴洪蕊 */
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include <iomanip>
#include<cstring>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

using namespace std;

struct student {
    int* no;
    char* name;
    int* score;
    struct student* next;
};


void output(student* head) {
	student* stu = head->next;
	while (stu != NULL) {
		cout << *(stu->no) << " " << stu->name << " " << *(stu->score) << endl;
		student* old = stu;
		stu = stu->next;
		delete old->no;
		delete old->score;
		delete[] old->name;
		delete old;
	}
}

int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	ifstream infile("./list.txt", ios::in);
	if (!infile) {
		cout << "打开文件失败" << endl;
		return -1;
	}

	student* head = new (nothrow) student;
	if (head == NULL) {
		cout << "Memory allocation using new failed!" << endl;
		infile.close();
		return -1;
	}

	student* start = head;

	while (true) {
		student* p = new (nothrow) student;
		if (p == NULL) {
			cout << "Memory allocation using new failed!" << endl;
			infile.close();
			return -1;
		}

		int no = 0, score = 0;
		char name[9];

		infile >> no >> name >> score;
		if (no == 9999999) {
			delete p;
			break;
		}

		// 二次申请 成员
		p->no = new (nothrow) int;
		if (p->no == NULL) {
			cout << "Memory allocation for no using new failed!" << endl;
			infile.close();
			return -1;
		}
		p->score = new (nothrow) int;
		if (p->score == NULL) {
			cout << "Memory allocation for score using new failed!" << endl;
			infile.close();
			return -1;
		}
		p->name = new (nothrow) char[strlen(name) + 1];
		if (p->name == NULL) {
			cout << "Memory allocation for name using new failed!" << endl;
			infile.close();
			return -1;
		}

		// 赋值
		*(p->no) = no;
		*(p->score) = score;
		strcpy(p->name, name);

		head->next = p;
		head = p;
		head->next = NULL;
	}

	output(start);
	delete start;
	infile.close();

	return 0;
}
