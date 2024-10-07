/* 计科 2152131 吴洪蕊 */
#include <iostream>
#include <fstream>
#include <iomanip>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
using namespace std;

struct student {
    int no;
    char name[9];
    int score;
    int rank;
};

void sort(struct student* stu, int num, int metric)
{
    struct student temp;
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num - i - 1; j++) {
            bool swapNeeded = false;
             
            if (metric == 1) {
                if (stu[j].score < stu[j + 1].score)
                    swapNeeded = true;
            }
            else if (metric == 2) {
                if (stu[j].no > stu[j + 1].no)
                    swapNeeded = true;
            }

            if (swapNeeded) {
                temp = stu[j];
                stu[j] = stu[j + 1];
                stu[j + 1] = temp;
            }
        }
    }
}

void Rank(struct student* stu, int num)
{
    int temp_rank = 1, temp_score = stu[0].score;
    for (int i = 0; i < num; i++) {
        if (stu[i].score == temp_score)
            stu[i].rank = temp_rank;
        else {
            temp_rank = i + 1;
            stu[i].rank = temp_rank;
            temp_score = stu[i].score;
        }
    }
}

void output(struct student* stu, int num)
{
    for (int i = 0; i < num; i++)
        cout << stu[i].no << " " << stu[i].name << " " << stu[i].score << " " << stu[i].rank << endl;
}

int main()
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    ifstream infile;
    infile.open("./student.txt", ios::in);
    if (!infile) {
        cout << "打开文件失败" << endl;
        return -1;
    }

    int num;
    infile >> num;

    student* p = new(nothrow) student[num];
    if (p == nullptr) {
        cout << "Memory allocation failed!" << endl;
        return -1;
    }

    for (int i = 0; i < num; i++) {
        infile >> p[i].no >> p[i].name >> p[i].score;
    }

    sort(p, num, 1);
    Rank(p, num);
    sort(p, num, 2);
    output(p, num);

    infile.close();
    delete[] p;

    return 0;
}
