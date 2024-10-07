/* 计科 2152131 吴洪蕊 */
#include<iostream>
#include<fstream>
#include <iomanip>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

using namespace std;

struct student {
    int no;
    char name[9];
    int score;
    struct student* next;
};

void output(student* head) 
{
    student* stu = head->next;
    while (stu != NULL) {
        std::cout << stu->no << " " << stu->name << " " << stu->score << std::endl;
        student* old = stu;
        stu = stu->next;
        delete old; 
    }
    stu = head;
    delete stu;
}

int main() 
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ifstream infile("list.txt", ios::in);
    if (!infile.is_open()) {
        cout << "打开文件失败\n";
        return -1;
    }

    // 创建头结点
    student* head = new(nothrow) student;
    if (head == NULL) {
        cout << "Memory allocation using new failed!\n";
        infile.close();
        return -1;
    }

    student* start = head;

    while (1) {
        student* p = new(nothrow) student;
        if (p == NULL) {
            cout << "Memory allocation using new failed!\n";
            infile.close();
            return -1;
        }

        infile >> p->no >> p->name >> p->score;

        if (p->no == 9999999) {
            delete p;
            break;
        }

        head->next = p;
        head = p;
        head->next = NULL;
    }

    output(start);

    infile.close();
    return 0;
}
