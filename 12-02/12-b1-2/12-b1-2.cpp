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
  说    明：
***************************************************************************/
void average(int (*score)[STUDENT_NUM])
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
    for (int* p = *score; p < *score + STUDENT_NUM; p++)
        average += *p;
    average = average / STUDENT_NUM;
    cout << "第1门课平均分：" << average << endl << endl;

}

/***************************************************************************
  函数名称：
  功    能：找出有两门以上课程不及格的学生
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void fail(int (*score)[STUDENT_NUM])
{
    /* 函数定义语句部分：
       1、本函数中仅允许定义 3个简单变量 + 1个行指针变量 + 1个简单指针变量 */

       /* 函数执行语句部分（要求同average）
       2门以上不及格的学生：
        No：2 92 82 72 32 52 平均：66
       */
    float average = 0;
    int helper = 0;
    int(*p)[STUDENT_NUM];
    int* p1;
    /* 函数执行语句部分（要求同average）*/
    cout << "2门以上不及格的学生：" << endl;
    for (p1 = *score; p1 < STUDENT_NUM + *score; p1++) {
        for (p = score; p < score + SCORE_NUM; p++) {
            if (*(*p + (p1 - *score)) < 60)
                helper++;
        }

        average = average / SCORE_NUM;
        if (helper >= 2) {
            cout << "No：" << (p1 - *score + 1) << " ";
            for (p = score; p < score + SCORE_NUM; p++) {
                cout << *(*p + (p1 - *score)) << " ";
                average += *(*p + (p1 - *score));
            }
            cout << "平均：" << average / SCORE_NUM << endl;
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
void good(int (*score)[STUDENT_NUM])
{
    /* 函数定义语句部分：
       1、本函数中仅允许定义 3个简单变量 + 1个行指针变量 + 1个简单指针变量 */

       /* 函数执行语句部分（要求同average）*/
    float average = 0;
    int helper = 0;
    int(*p)[STUDENT_NUM];
    int* p1;
    /* 函数执行语句部分（要求同average）*/
    cout << "平均90以上或全部85以上的学生：" << endl;
    for (p1 = *score; p1 < STUDENT_NUM + *score; p1++) {
        for (p = score; p < score + SCORE_NUM; p++) {
            if (*(*p + (p1 - *score)) >= 85)
                helper++;
            average += *(*p + (p1 - *score));
        }

        average = average / SCORE_NUM;
        if (average >= 90 || helper == SCORE_NUM) {
            cout << "No：" << (p1 - *score + 1) << " ";
            for (p = score; p < score + SCORE_NUM; p++) 
                cout << *(*p + (p1 - *score)) << " ";
            cout << "平均：" << average << endl;
        }
        helper = 0;
        average = 0;
    }
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
    int a[SCORE_NUM][STUDENT_NUM] = {
        {91,92,93,97},  //第1-4个学生的第1门课成绩
        {81,82,83,85},  //第1-4个学生的第2门课成绩
        {71,72,99,87},  //第1-4个学生的第3门课成绩
        {61,32,80,91},  //第1-4个学生的第4门课成绩
        {51,52,95,88} };//第1-4个学生的第5门课成绩
    /* 除上面的预置数组外，本函数中仅允许定义 1个行指针变量 + 1个简单指针变量 */
    cout << "初始信息：" << endl;
    // No.1-4学生的第5门课的成绩：
    for (int(*p)[STUDENT_NUM] = a; p < SCORE_NUM + a; p++) {
        cout << "No.1-4学生的第" << (p - a + 1) << "门课的成绩：";
        for (int* p1 = *p; p1 < *p + STUDENT_NUM; p1++)
            cout << *p1 << " ";
        cout << endl;
    }
    cout << endl;
    /* 函数执行语句部分（要求同average）*/
    average(a);
    fail(a);
    good(a);

    return 0;
}
