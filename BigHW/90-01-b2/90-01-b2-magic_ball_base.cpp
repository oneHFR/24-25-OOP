/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-01-b2-magic_ball.h"
#include "../include/common_functions.h"
#include "../include/cmd_console_tools.h"
#include <Windows.h>

using namespace std;


void h1(int s[11][11], int r, int c)
{
    int h[11][11] = { -12 };
    int helper_1 = 0;
    fill(s, r, c, 9);
    print_out(s, r, c, 1, h);
    enter_option(1, 0);
    helper_1 = check_1(s, r, c, h, 0);
    if (helper_1) {
        print_out(s, r, c, 10, h);
    }
    ;
}

void h2(int s[11][11], int r, int c)
{
    int h[11][11] = { -12 };
    int helper_2 = 0;
    initializa_input(s, r, c);
    fill(s, r, c, 9);
    print_out(s, r, c, 1, h);
    enter_option(1, 0);
    helper_2 = check_1(s, r, c, h, 0);
    if (helper_2) {
        while (helper_2) {
            print_out(s, r, c, 10, h);
            enter_option(2, 0);
            remove_0(s, r, c, h);
            print_out(s, r, c, 11, h);
            enter_option(3, 0);
            fill_new(s, r, c, h, 0);
            print_out(s, r, c, 12, h);
            helper_2 = check_1(s, r, c, h, 0);
        }
    }
    else {
        cout << "��С���޷����ԣ����ٴ�����" << endl;
    }
}

void h3(int s[11][11], int r, int c)
{
    //����д
    int h[11][11] = { -12 };
    int helper_3 = 0;
    initializa_input(s, r, c);
    fill(s, r, c, 9);
    print_out(s, r, c, 1, h);
    enter_option(1, 0);
    helper_3 = check_1(s, r, c, h, 0);
    if (helper_3) {
        while (helper_3) {
            print_out(s, r, c, 10, h);
            enter_option(2, 0);
            remove_0(s, r, c, h);
            print_out(s, r, c, 11, h);
            enter_option(3, 0);
            fill_new(s, r, c, h, 0);
            print_out(s, r, c, 12, h);
            helper_3 = check_1(s, r, c, h, 0);
        }
    }

    int helper_hint = hint(s, r, c, h, 0);
    cout << "���س�����ʾ������ʾ..." << endl;
    print_out(s, r, c, 12, h);

    /*
        helper_hint = hint(s, r, c, h, 1); // �޿��������Ϸ����!
    if (!helper_7 && !helper_hint) {
        cct_getxy(x, y);
        cct_gotoxy(14, 0);
        cout << "���޿��������Ϸ����!��";
        cct_gotoxy(x, y);
    }
    */
}

void h4(int s[11][11], int r, int c)
{
    int h[11][11] = { -12 };
    initializa_input(s, r, c);
    fill(s, r, c, 9);
    print_out(s, r, c, 1, h);
    enter_option(4, 0);

    set_cmd(r);
    print_canvas(s, r, c, 0, 0);
    print_circle(s, r, c, 0, h, 0, 0);
}

void h5(int s[11][11], int r, int c)
{
    int h[11][11] = { -12 };
    initializa_input(s, r, c);
    fill(s, r, c, 9);
    print_out(s, r, c, 1, h);
    enter_option(4, 0);

    set_cmd(r + 8);
    print_canvas(s, r, c, 1, 0);
    print_circle(s, r, c, 1, h, 0, 0);
}

void h6(int s[11][11], int r, int c)
{
    int h[11][11] = { -12 };
    initializa_input(s, r, c);
    fill(s, r, c, 9);

    set_cmd(r);
    check_1(s, r, c, h, 0);
    print_canvas(s, r, c, 0, 1);
    print_circle(s, r, c, 0, h, 1, 0);
}

void h7(int s[11][11], int r, int c)
{
    int h[11][11] = { -12 };
    int helper_7 = 0;
    int helper_hint = 0;
    int x, y;
    initializa_input(s, r, c);
    fill(s, r, c, 9);
    set_cmd(r + 8);
    helper_7 = check_1(s, r, c, h, 0);
    print_canvas(s, r, c, 1, 1);
    print_circle(s, r, c, 1, h, 1, 0);

    if (helper_7) {
        while (helper_7) {
            //ȫ��ע��hʹ�õĳ�ʼ��
            enter_option(5, r); //Ҫ����ȷ��λ��
            //������ʼ��
            remove_circle(s, r, c, h);
            falling_down(s, r, c, h); // ���䶯��
            remove_0(s, r, c, h); //����ȡ�� Ϊ����ṩʶ����
            //���
            enter_option(6, r);
            fill_new(s, r, c, h, 1);
            helper_7 = check_1(s, r, c, h, 0);
        }
    }
    enter_option(7, r);
    //��ʾ��ʾ
    helper_hint = hint(s, r, c, h, 1); // �޿��������Ϸ����!
    if (!helper_7 && !helper_hint) {
        cct_getxy(x, y);
        cct_gotoxy(14, 0);
        cout << "���޿��������Ϸ����!��";
        cct_gotoxy(x, y);
    }
}

void h8(int s[11][11], int r, int c)
{
    int h[11][11] = { -12 };
    int helper_8 = 0;
    int helper_hint = 0;
    int x, y;
    initializa_input(s, r, c);
    fill(s, r, c, 9);
    set_cmd(r + 8);
    helper_8 = check_1(s, r, c, h, 0);
    print_canvas(s, r, c, 1, 1);
    print_circle(s, r, c, 1, h, 1, 0);

    if (helper_8) {
        while (helper_8) {
            remove_circle(s, r, c, h);
            falling_down(s, r, c, h); // ���䶯��
            remove_0(s, r, c, h); //����ȡ�� Ϊ����ṩʶ����
            fill_new(s, r, c, h, 1);
            helper_8 = check_1(s, r, c, h, 0);
        }
    }
    helper_hint = hint(s, r, c, h, 1); // �޿��������Ϸ����!
    if (!helper_8 && !helper_hint) {
        cct_getxy(x, y);
        cct_gotoxy(14, 0);
        cout << "���޿��������Ϸ����!��";
        cct_gotoxy(x, y);
        return;
    }
    else {
        cct_getxy(x, y);
        cct_gotoxy(14, 0);
        cout << "����ǰ������0 �Ҽ��˳���";
        cct_gotoxy(x, y);
        location(s, r, c, h, 8);
    }
}

void h9(int s[11][11], int r, int c)
{
    //�޿��������Ϸ����!���շ���:51)
    // ��¼����

    int h[11][11] = { -12 };
    int helper_9 = 0;
    int helper_hint = 0;
    int x, y;
    initializa_input(s, r, c);
    fill(s, r, c, 9);
    set_cmd(r + 8);
    helper_9 = check_1(s, r, c, h, 0);
    print_canvas(s, r, c, 1, 1);
    print_circle(s, r, c, 1, h, 1, 0);

    if (helper_9) {
        while (helper_9) {
            remove_circle(s, r, c, h);
            falling_down(s, r, c, h); // ���䶯��
            remove_0(s, r, c, h); //����ȡ�� Ϊ����ṩʶ����
            fill_new(s, r, c, h, 1);
            helper_9 = check_1(s, r, c, h, 0);
        }
    }
    helper_hint = hint(s, r, c, h, 1); // �޿��������Ϸ����!
    if (!helper_9 && !helper_hint) {
        cct_getxy(x, y);
        cct_gotoxy(14, 0);
        cout << "���޿��������Ϸ����!��";
        cct_gotoxy(x, y);
        return;
    }
    else {
        cct_getxy(x, y);
        cct_gotoxy(14, 0);
        cout << "����ǰ������0 �Ҽ��˳���";
        cct_gotoxy(x, y);
        location(s, r, c, h, 9);
    }
}
