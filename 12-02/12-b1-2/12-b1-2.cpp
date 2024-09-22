/* 2152131 ����� �ƿ� */
#include <iostream>
#include <iomanip>
using namespace std;

#define STUDENT_NUM	4
#define SCORE_NUM	5

/* --- ���������κ���ʽ��ȫ�ֱ��� --- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ����һ�ſε�ƽ����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void average(int (*score)[STUDENT_NUM])
{
    /* ����������䲿�֣�
       1���������н������� 1���򵥱��� + 1��ָ����� */

       /* ����ִ����䲿�֣�
          1���������ٶ����κ����͵ı��������� for (int i=0;...������ʽ�������
          2��ѭ������������ָ���������������в��������[]��ʽ��������
             ������int a[10], i;
                     for(i=0; i<10; i++)
                         cout << a[i];
             ����  ��int a[10], p;
                     for(p=a; p<a+10; p++)
                         cout << *p;          */
    float average = 0;
    for (int* p = *score; p < *score + STUDENT_NUM; p++)
        average += *p;
    average = average / STUDENT_NUM;
    cout << "��1�ſ�ƽ���֣�" << average << endl << endl;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ҳ����������Ͽγ̲������ѧ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void fail(int (*score)[STUDENT_NUM])
{
    /* ����������䲿�֣�
       1���������н������� 3���򵥱��� + 1����ָ����� + 1����ָ����� */

       /* ����ִ����䲿�֣�Ҫ��ͬaverage��
       2�����ϲ������ѧ����
        No��2 92 82 72 32 52 ƽ����66
       */
    float average = 0;
    int helper = 0;
    int(*p)[STUDENT_NUM];
    int* p1;
    /* ����ִ����䲿�֣�Ҫ��ͬaverage��*/
    cout << "2�����ϲ������ѧ����" << endl;
    for (p1 = *score; p1 < STUDENT_NUM + *score; p1++) {
        for (p = score; p < score + SCORE_NUM; p++) {
            if (*(*p + (p1 - *score)) < 60)
                helper++;
        }

        average = average / SCORE_NUM;
        if (helper >= 2) {
            cout << "No��" << (p1 - *score + 1) << " ";
            for (p = score; p < score + SCORE_NUM; p++) {
                cout << *(*p + (p1 - *score)) << " ";
                average += *(*p + (p1 - *score));
            }
            cout << "ƽ����" << average / SCORE_NUM << endl;
        }
        helper = 0;
        average = 0;
    }
    cout << endl;

}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ҳ�ƽ���ɼ���90�����ϻ�ȫ���ɼ���85�����ϵ�ѧ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void good(int (*score)[STUDENT_NUM])
{
    /* ����������䲿�֣�
       1���������н������� 3���򵥱��� + 1����ָ����� + 1����ָ����� */

       /* ����ִ����䲿�֣�Ҫ��ͬaverage��*/
    float average = 0;
    int helper = 0;
    int(*p)[STUDENT_NUM];
    int* p1;
    /* ����ִ����䲿�֣�Ҫ��ͬaverage��*/
    cout << "ƽ��90���ϻ�ȫ��85���ϵ�ѧ����" << endl;
    for (p1 = *score; p1 < STUDENT_NUM + *score; p1++) {
        for (p = score; p < score + SCORE_NUM; p++) {
            if (*(*p + (p1 - *score)) >= 85)
                helper++;
            average += *(*p + (p1 - *score));
        }

        average = average / SCORE_NUM;
        if (average >= 90 || helper == SCORE_NUM) {
            cout << "No��" << (p1 - *score + 1) << " ";
            for (p = score; p < score + SCORE_NUM; p++) 
                cout << *(*p + (p1 - *score)) << " ";
            cout << "ƽ����" << average << endl;
        }
        helper = 0;
        average = 0;
    }
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main()
{
    int a[SCORE_NUM][STUDENT_NUM] = {
        {91,92,93,97},  //��1-4��ѧ���ĵ�1�ſγɼ�
        {81,82,83,85},  //��1-4��ѧ���ĵ�2�ſγɼ�
        {71,72,99,87},  //��1-4��ѧ���ĵ�3�ſγɼ�
        {61,32,80,91},  //��1-4��ѧ���ĵ�4�ſγɼ�
        {51,52,95,88} };//��1-4��ѧ���ĵ�5�ſγɼ�
    /* �������Ԥ�������⣬�������н������� 1����ָ����� + 1����ָ����� */
    cout << "��ʼ��Ϣ��" << endl;
    // No.1-4ѧ���ĵ�5�ſεĳɼ���
    for (int(*p)[STUDENT_NUM] = a; p < SCORE_NUM + a; p++) {
        cout << "No.1-4ѧ���ĵ�" << (p - a + 1) << "�ſεĳɼ���";
        for (int* p1 = *p; p1 < *p + STUDENT_NUM; p1++)
            cout << *p1 << " ";
        cout << endl;
    }
    cout << endl;
    /* ����ִ����䲿�֣�Ҫ��ͬaverage��*/
    average(a);
    fail(a);
    good(a);

    return 0;
}
