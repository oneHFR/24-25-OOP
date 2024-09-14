/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <iomanip>
using namespace std;

#define STUDENT_NUM	4
#define SCORE_NUM	5

/* --- 不允许定义任何形式的全局变量 --- */

/***************************************************************************
  函数名称：
  功    能：求第一门课的平均分
  输入参数：
  返 回 值：
  说    明：相关输出都放在函数中即可
***************************************************************************/
void average(int (*score)[SCORE_NUM])
{
    /* 函数定义语句部分：
       1、本函数中仅允许定义 1个简单变量 + 1个指针变量 */

       /* 函数执行语句部分：
          1、不允许再定义任何类型的变量，包括 for (int i=0;...）等形式定义变量
          2、循环变量必须是指针变量，后续语句中不允许出现[]形式访问数组
             不允许：int a[10], i;
                     for(i=0; i<10; i++)
                         cout << a[i];
             允许  ：int a[10], p;
                     for(p=a; p<a+10; p++)
                         cout << *p;          */
    float average = 0;
    for (int(*p)[SCORE_NUM] = score; p < score + STUDENT_NUM; p++)
        average += **p;
    average = average / STUDENT_NUM;
    cout << "第1门课平均分：" << average << endl << endl;
}

/***************************************************************************
  函数名称：
  功    能：找出有两门以上课程不及格的学生
  输入参数：
  返 回 值：
  说    明：相关输出都放在函数中即可
***************************************************************************/
void fail(int (*score)[SCORE_NUM])
{
    /* 函数定义语句部分：
       1、本函数中仅允许定义 2个简单变量 + 1个行指针变量 + 1个简单指针变量 */
    /*
    2门以上不及格的学生：
    No：2 92 82 72 32 52 平均：66
    */
    float average = 0;
    int helper = 0;
    int(*p)[SCORE_NUM];
    int* p1;
    /* 函数执行语句部分（要求同average）*/
    cout << "2门以上不及格的学生：" << endl;
    for (p = score; p < STUDENT_NUM + score; p++) {
        for (p1 = *p; p1 < *p + SCORE_NUM; p1++) {
            average += *p1;
            if (*p1 <= 60)
                helper++;
        }

        average = average / SCORE_NUM;
        if (helper >= 2) {
            cout << "No：" << (p - score + 1) << " ";
            for (int* p1 = *p; p1 < *p + SCORE_NUM; p1++)
                cout << *p1 << " ";
            cout << "平均：" << average << endl;
        }
        helper = 0;
        average = 0;
    }
    cout << endl;
}

/***************************************************************************
  函数名称：
  功    能：找出平均成绩在90分以上或全部成绩在85分以上的学生
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void good(int (*score)[SCORE_NUM])
{
    /* 函数定义语句部分：
       1、本函数中仅允许定义 2个简单变量 + 1个行指针变量 + 1个简单指针变量 */
    float average = 0;
    int helper = 0;
    int(*p)[SCORE_NUM];
    int* p1;
    /* 函数执行语句部分（要求同average）*/
    cout << "平均90以上或全部85以上的学生：" << endl;
    for (p = score; p < STUDENT_NUM + score; p++) {
        for (p1 = *p; p1 < *p + SCORE_NUM; p1++) {
            average += *p1;
            if (*p1 >= 85)
                helper++;
        }

        average = average / SCORE_NUM;
        if (average >= 90 || helper == SCORE_NUM) {
            cout << "No：" << (p - score + 1) << " ";
            for (int* p1 = *p; p1 < *p + SCORE_NUM; p1++)
                cout << *p1 << " ";
            cout << "平均：" << average << endl;
        }
        helper = 0;
        average = 0;
    }
    /*
        平均90以上或全部85以上的学生：
        No：3 93 83 99 80 95 平均：90
        No：4 97 85 87 91 88 平均：89.6
    */
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main()
{
    int a[STUDENT_NUM][SCORE_NUM] = {
        {91, 81, 71, 61, 51},	//第1个学生的1-5门课的成绩
        {92, 82, 72, 32, 52},   //第2个学生的1-5门课的成绩
        {93, 83, 99, 80, 95},   //第3个学生的1-5门课的成绩
        {97, 85, 87, 91, 88} }; //第4个学生的1-5门课的成绩

    /* 除上面的预置数组外，本函数中仅允许定义 1个行指针变量 + 1个简单指针变量 */
    cout << "初始信息：" << endl;
    for (int(*p)[SCORE_NUM] = a; p < STUDENT_NUM + a; p++) {
        cout << "No." << (p - a + 1) << "的1-5门课成绩：";
        for (int* p1 = *p; p1 < *p + SCORE_NUM; p1++)
            cout << *p1 << " ";
        cout << endl;
    }
    cout << endl;
    /* 函数执行语句部分（要求同average）*/
    average(a);
    fail(a);
    good(a);

    return 0;
    /*
    初始信息：
    No.1的1-5门课成绩：91 81 71 61 51
    No.2的1-5门课成绩：92 82 72 32 52
    No.3的1-5门课成绩：93 83 99 80 95
    No.4的1-5门课成绩：97 85 87 91 88
    */
}
