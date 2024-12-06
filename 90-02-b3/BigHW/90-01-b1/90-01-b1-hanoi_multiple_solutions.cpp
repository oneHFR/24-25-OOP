/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "90-01-b1-hanoi.h"
#include "../include/cmd_console_tools.h"
#include <Windows.h>

using namespace std;
/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1����ű� hanoi_main.cpp �и��ݲ˵�����ֵ���õĸ��˵����Ӧ��ִ�к���

     ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
            ���ƶ�����                      :1��ȫ�ּ򵥱���
            Բ��������Բ�̵ı��:3��ȫ��һά�����1��ȫ�ֶ�ά����
            Բ��������Բ�̵�����:3��ȫ�ּ򵥱�����1��ȫ��һά����
            ��ʱ                                    :1��ȫ�ּ򵥱���
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
            �����������ۣ�
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */
/*
    n ����
    src ��ʼ��
    dst �յ���
    tmp �м���
*/

int step, speed;
int tower_num[3][maxLayers] = { {0}, {0}, {0} };
int tower_index[3] = { 1, 1, 1 };


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/

void hanoi(int n, char src, char tmp, char dst, int option)
{
    if (n == 1) {
        operation(1, src, tmp, dst,  option);
    }
    else {
        hanoi(n - 1, src, dst, tmp, option);
        operation(n, src, tmp, dst, option);
        hanoi(n - 1, tmp, src, dst, option);
    }
}

void initializa_input() 
{
    //��֪��Ҫ��Ҫ�ӿ������ʼ���ı����಻��
    step = 0;
    for (int i = 0; i < 3; i++) {
        tower_index[i] = 1;
        for (int j = 0; j < 10; j++)
            tower_num[i][j] = 0;
    }
    //ABC[3][maxLayers] = { {0}, {0}, {0} }; Ϊʲô������ֱ�ӻ���ʾ��ʼֵ�趨��ֵ̫���أ�
    //ABC_pointer[3] = { 0 };
}

void initializa_tower(int n, char src) 
{
    if (src == 'A')
        for (int i = n; i >= 1; i--) {
            tower_num[0][tower_index[0]++] = i;
        }
    if (src == 'B')
        for (int i = n; i >= 1; i--) {
            tower_num[1][tower_index[1]++] = i;
        }
    if (src == 'C')
        for (int i = n; i >= 1; i--) {
            tower_num[2][tower_index[2]++] = i;
        }
 }

void speed_delay(int speed) 
{
    if (speed == 0) {
        while (1) {
            int enter;
            enter = getchar();
            if (enter == 10)
                break;
        }
    }
    else if (speed <= 5){
        Sleep((6 - speed) * (6 - speed) * 10); 
    }
    else if (speed == 5 + 6) {
        Sleep(1);
        //Sleep(0.5);
    }
    else if (speed == 4 + 6) {
        Sleep(1);
    }
    else if (speed == 3 + 6) {
        Sleep(4);
    }
    else if (speed == 2 + 6) {
        Sleep(8);
    }
    else if (speed == 1 + 6) {
        Sleep(18);
    }
    else if (speed == 0 + 6) {
        Sleep(25);
    }
}

void input(int *n, char *src, char *tmp, char *dst, int option)
{
    initializa_input();
    //*n = 4;
    //*src = 'A';
    //*dst = 'C';
    if (option == 5) {
        cct_cls();
        tower_print();
        return;
    }

    while (1) {
        cout << "�����뺺ŵ���Ĳ���(1-10):" << endl;
        cin >> *n;
        if (*n >= 1 && *n <= 10 && cin.good()) {
            cin.ignore(1024, '\n');
            break;
        }
        else {
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }
    while (1) {
        cout << "��������ʼ��(A-C)" << endl;
        cin >> *src;
        if (cin.good() && ((*src >= 'A' && *src <= 'C') || (*src >= 'a' && *src <= 'c'))) {
            if (*src >= 'a' && *src <= 'c') {
               *src = char(*src - 32);
            }
            cin.ignore(1024, '\n');
            break;
        }
        else {
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }
    while (1) {
        cout << "������Ŀ����(A-C)" << endl;
        cin >> *dst;
        if (cin.good() && ((*dst >= 'A' && *dst <= 'C') || (*dst >= 'a' && *dst <= 'c')) && *dst != *src && *dst != char(*src + 32)) {
            if (*dst >= 'a' && *dst <= 'c') { // Сд����д
                *dst = char(*dst - 32);
            }
            cin.ignore(1024, '\n');
            break;
        }
        else if (*dst == *src || *dst == char(*src + 32)) {
            if (*dst >= 'a' && *dst <= 'c') { // Сд����д
                *dst = char(*dst - 32);
            }
            cout << "Ŀ����(" << *dst << ")��������ʼ��(" << *src << ")��ͬ" << endl;
            cin.ignore(1024, '\n');
        }
        else {
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }


    *tmp = char('A'+'B'+'C' - *src - *dst);
    initializa_tower(*n, *src);
    if (option == 4 || option == 8) {
        while (1) {
            cout << "�������ƶ��ٶ�(0-5: 0-���س�������ʾ 1-��ʱ� 5-��ʱ���)";
            cin >> speed;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1024, '\n');
            }
            else {
                break;
            }
        }

        cct_cls();
        cout << "�� " << *src << " �ƶ��� " << *dst << "���� " << *n << " �㣬��ʱ����Ϊ " << speed;
        speed_delay(speed);

        int t_yn;
        if (option == 8)
            t_yn = t_y + 15;
        else
            t_yn = t_y;

        cct_gotoxy(0, t_yn+6);
        cout << "��ʼ:";
        cct_gotoxy(t_a+6, t_yn+6);
        out_3('A', 0);
        out_3('B', 1);
        out_3('C', 2);

        cct_gotoxy(t_a+1, t_yn + 2);
        cout << "A";
        cct_gotoxy(t_b+1, t_yn + 2);
        cout << "B";
        cct_gotoxy(t_c+1, t_yn + 2);
        cout << "C";
        cct_gotoxy(t_a-1, t_yn + 1);
        cout << "=========================";
        print_4('A', option);
        print_4('B', option);
        print_4('C', option);
        if (option == 8) {
            tower_print();
            tower_layer_print(*n, *src);
        }
        speed_delay(speed);
    }
    if (option == 6 || option == 7) {
        cct_cls();
        cout << "�� " << *src << " �ƶ��� " << *dst << "���� " << *n << " ��";
        tower_print();
        tower_layer_print(*n, *src);
        if (option == 7) {
            speed = 1;
        }
    }
    if (option == 9) {
        cct_cls();
        cout << "�� " << *src << " �ƶ��� " << *dst << "���� " << *n << " ��";
        int t_yn;
        t_yn = t_y + 15;
        cct_gotoxy(0, t_yn + 6);
        cout << "��ʼ:";
        cct_gotoxy(t_a + 6, t_yn + 6);
        out_3('A', 0);
        out_3('B', 1);
        out_3('C', 2);

        cct_gotoxy(t_a + 1, t_yn + 2);
        cout << "A";
        cct_gotoxy(t_b + 1, t_yn + 2);
        cout << "B";
        cct_gotoxy(t_c + 1, t_yn + 2);
        cout << "C";
        cct_gotoxy(t_a - 1, t_yn + 1);
        cout << "=========================";
        print_4('A', option);
        print_4('B', option);
        print_4('C', option);

        tower_print();
        tower_layer_print(*n, *src);
        speed = 1;
    }
    if (option == 1 || option == 2 || option == 3 || option == 4 || option == 8)
        hanoi(*n, *src, *tmp, *dst, option);
}

void out_3(char zimu, int k) 
{
    cout << zimu << ":";
    for (int i = 1; i < tower_index[k]; i++)
        cout << setw(2) << tower_num[k][i];
    for (int i = 10; i >= tower_index[k]; i--)
        cout << "  ";
    cout << " ";
}

void print_4(char src, int option) 
{
    int y, t_n, t_yn, k;
    k = int(src - 'A');
    if (option == 8 || option == 9)
        t_yn = t_y + 15;
    else
        t_yn = t_y;

    if (src == 'A')
       t_n = t_a;
    else if (src == 'B')
        t_n = t_b;
    else
        t_n = t_c;

    cct_gotoxy(t_n, t_yn);
    y = t_yn;
    for (int i = 1; i < maxLayers+1; i++) {
        cout << setw(2) << " ";
        y -= 1;
        cct_gotoxy(t_n, y);
    }
    cct_gotoxy(t_n, t_yn);
    y = t_yn;
    for (int i = 1; i < tower_index[k]; i++) {
        cout << setw(2) << tower_num[k][i];
        y -= 1;
        cct_gotoxy(t_n, y);
    }
}

void out_4(char src, char dst, int option) 
{
    out_3('A', 0);
    out_3('B', 1);
    out_3('C', 2);
    print_4(src, option);
    print_4(dst, option);
}

// #define COLOR_HYELLOW	14	//����
// #define COLOR_WHITE		7	//��
// #define COLOR_BLACK		0	//��
 //������Ϊ��Ҳ��һЩ������ʼ����define�������ﲻ�������ظ��İɣ���������
void tower_print() 
{
    cct_showch(t_A, t_Y, ' ', COLOR_HYELLOW, COLOR_WHITE, 23);
    Sleep(5);
    cct_showch(t_B, t_Y, ' ', COLOR_HYELLOW, COLOR_WHITE, 23);
    Sleep(5);
    cct_showch(t_C, t_Y, ' ', COLOR_HYELLOW, COLOR_WHITE, 23);
    Sleep(5);

    for (int j = t_Y; j > 2; j--){
        cct_showch(t_A+11, j, ' ', COLOR_HYELLOW, COLOR_WHITE, 1);
        Sleep(15);
        cct_showch(t_B+11, j, ' ', COLOR_HYELLOW, COLOR_WHITE, 1);
        Sleep(15);
        cct_showch(t_C+11, j, ' ', COLOR_HYELLOW, COLOR_WHITE, 1);
        Sleep(15);
    }
    cct_setcolor(0,7);
}

void tower_layer_print(int n, char src) 
{
    int t_n;

    if (src == 'A')
        t_n = t_A;
    else if (src == 'B')
        t_n = t_B;
    else
        t_n = t_C;

    for (int i = n; i > 0; i--) {
        cct_showch(t_n + 11 - i, t_Y - 1 - n + i, ' ', i, i, 2 * i + 1);
        Sleep(200);
    }
    cct_setcolor(0, 7);
}

void tower_move(int n, char src, char dst) 
{
    int k,  t_n, t_yn;
    int color_helper = 1, c;
    k = int(src - 'A');
    t_yn = t_Y - tower_index[k];

    // up
    if (src == 'A') {
        t_n = t_A + 11;
    }
    else if (src == 'B') {
        t_n = t_B + 11;
    }
    else {
        t_n = t_C + 11;
    }
    for (int i = n; i > -(1+t_yn); i--) {
        if (color_helper) {
            c = i;
            color_helper = 0;
        }
        cct_showch(t_n - n, t_yn - 1 - n + i, ' ', c, c, 2 * c + 1);
        if (t_yn - n + i > 2) {
            cct_showch(t_n - n, t_yn - n + i, ' ', 0, 7, 2 * n + 1);
            cct_showch(t_n, t_yn - n + i, ' ', 14, 0, 1); //���ԭλ����ԭ
            Sleep(1);
            //Sleep(0.5);
        }
        else {
            cct_showch(t_n - n, t_yn - n + i, ' ', 0, 7, 2 * n + 1);
            Sleep(1);
            //Sleep(0.5);
        }
        if (t_yn - n + i < 3) {
            speed_delay(speed + 6);
            Sleep(1);
            break;
        }
        speed_delay(speed + 6);
    }
    speed_delay(speed + 6);

    //right
    if (dst > src) {
        for (int i = 1; i < abs(int(dst - src) * 32); i++) {
            cct_showch(t_n + i + c, 1, ' ', c, c, 1);
            cct_showch(t_n + i - c - 1, 1, ' ', 0, 7, 1);
            speed_delay(speed + 6);
        }
        cct_showch(t_n + abs(int(dst - src) * 32) - c - 1, 1, ' ', 0, 7, 1);
        speed_delay(speed + 6);
    }
    //left
    else {
        for (int i = 1; i < abs(int(dst - src) * 32); i++) {
            cct_showch(t_n - i - c, 1, ' ', c, c, 1);
            cct_showch(t_n - i + c + 1, 1, ' ', 0, 7, 1);
            speed_delay(speed + 6);
        }
        cct_showch(t_n - abs(int(dst - src) * 32) + c + 1, 1, ' ', 0, 7, 1);
        speed_delay(speed + 6);
    }
    speed_delay(speed + 6);

    //down
    k = dst - 'A';
    if (dst == 'A') {
        t_n = t_A + 11;
    }
    else if (dst == 'B') {
        t_n = t_B + 11;
    }
    else {
        t_n = t_C + 11;
    }
    for (int i = 0; i < t_Y+2; i++) {
        cct_showch(t_n - n, i+1, ' ', c, c, 2 * c + 1);
        if (i > 2) {
            cct_showch(t_n - n, i, ' ', 0, 7, 2 * n + 1);
            cct_showch(t_n, i, ' ', 14, 0, 1); //���ԭλ
            Sleep(1);
            //Sleep(0.5);
        }
        else {
            cct_showch(t_n - n, i, ' ', 0, 7, 2 * n + 1);
            //Sleep(0.5);
            Sleep(1);
        }
        if (i +1 > t_Y - tower_index[k]) {
            break;
        }
        speed_delay(speed + 6);

    }
    speed_delay(speed + 6);

    cct_setcolor(0, 7);
}

int game_input_outcome(char *g_s, char *g_d) 
{
    //����20��֮���Զ����
    int first = 0;
    char input_set[20] = { '\0'}; //�������һ������BUG�������ĵ������������ǳ�ʼ�����Ǿͼ�
    cct_gotoxy(0, 36);
    cout << "�������ƶ�������(������ʽ��AC=A���˵������ƶ���C��Q=�˳�) ��";
    while (1) { 
        if (first){
            cin.clear();
            cin.ignore(1024, '\n');
            cct_gotoxy(60, 36);
            for (int i = 0; i < 1024; i++)
                cout << ' ';
            cct_gotoxy(60, 36);
        }
        first = 1;
        char input_set[20] = { '\0' };
        cin.get(input_set, 21, '\n');
        if (input_set[19] != '\0') {
            continue;
        }
        *g_s = input_set[0];
        *g_d = input_set[1];
        if (*g_s >= 'a' && *g_s <= 'c') { // Сд����д
            *g_s = char(*g_s - 32);
        }
        if (*g_d >= 'a' && *g_d <= 'c') { // Сд����д
            *g_d = char(*g_d - 32);
        }

        if (cin.good() && (input_set[2] == '\0') && (input_set[1] != '\0') && (*g_s >= 'A' && *g_s <= 'C') && (*g_d >= 'A' && *g_d <= 'C')) {
            if (*g_s != *g_d) {
                if (tower_index[*g_s - 'A'] == 1) {
                    cct_gotoxy(0, 37);
                    cout << "Դ��Ϊ�գ�";
                    Sleep(1200);
                    cct_gotoxy(0, 37);
                    cout << "                ";
                    continue;
                }
                else if (tower_num[*g_s - 'A'][tower_index[*g_s - 'A'] - 1] > tower_num[*g_d - 'A'][tower_index[*g_d - 'A'] - 1] && tower_num[*g_d - 'A'][tower_index[*g_d - 'A'] - 1] > 0) {
                    cct_gotoxy(0, 37);
                    cout << "����ѹС�̣��Ƿ��ƶ���";
                    Sleep(1200);
                    cct_gotoxy(0, 37);
                    cout << "                ";
                    continue;
                }
                else {
                    return 1;
                    break;
                }
                continue;
            }
            else {
                continue;
            }
        }
        else if (cin.good() && (input_set[1] == '\0') && (*g_s == 'Q' || *g_s == 'q')) {
            return 0;
            break;
        }
        else {
            continue;
        }
    }
}

void operation(int n, char src, char tmp, char dst, int option)
{
    if (option == 1) {
        //cout << setw(2) << setfill(' ') << n << "# " << src << "-->" << dst << endl;
        cout << n << "# " << src << "---->" << dst << endl;
    }

    else if (option == 2) {
        //��   1 ��( 1#: A-->B) 
        step++;
        cout << "��" << setfill(' ') << setw(4) << step;
        cout << " ��( " << n << "#: " << src << "-->" << dst << ")" << endl;
    }

    else if (option == 3) {
        step++;
        cout << "��" << setfill(' ') << setw(4) << step << " ��(" << setw(2) << n << "#: " << src << "-->" << dst << ")  ";
        if (src == 'A' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[0][--tower_index[0]];
        if (src == 'A' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[0][--tower_index[0]];
        if (src == 'B' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[1][--tower_index[1]];
        if (src == 'B' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[1][--tower_index[1]];
        if (src == 'C' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[2][--tower_index[2]];
        if (src == 'C' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[2][--tower_index[2]];
        out_3('A', 0);
        out_3('B', 1);
        out_3('C', 2);
        cout << endl;
    }

    else if (option == 4) {
        cct_gotoxy(0, 17);
        step++;
        cout << "��" << setfill(' ') << setw(4) << step << " ��(" << setw(2) << n << "#: " << src << "-->" << dst << ")  ";
        if (src == 'A' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[0][--tower_index[0]];
        if (src == 'A' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[0][--tower_index[0]];
        if (src == 'B' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[1][--tower_index[1]];
        if (src == 'B' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[1][--tower_index[1]];
        if (src == 'C' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[2][--tower_index[2]];
        if (src == 'C' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[2][--tower_index[2]];
        out_4(src, dst, option);
        speed_delay(speed);
    }

    else if (option == 7) {
        cct_gotoxy(0, 17 + 15);
        if (src == 'A' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[0][--tower_index[0]];
        if (src == 'A' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[0][--tower_index[0]];
        if (src == 'B' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[1][--tower_index[1]];
        if (src == 'B' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[1][--tower_index[1]];
        if (src == 'C' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[2][--tower_index[2]];
        if (src == 'C' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[2][--tower_index[2]];
        tower_move(n, src, dst);
        speed_delay(1);
    }

    else if (option == 8) {
        cct_gotoxy(0, 17 + 15);
        step++;
        cout << "��" << setfill(' ') << setw(4) << step << " ��(" << setw(2) << n << "#: " << src << "-->" << dst << ")  ";
        if (src == 'A' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[0][--tower_index[0]];
        if (src == 'A' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[0][--tower_index[0]];
        if (src == 'B' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[1][--tower_index[1]];
        if (src == 'B' && dst == 'C')
            tower_num[2][tower_index[2]++] = tower_num[1][--tower_index[1]];
        if (src == 'C' && dst == 'B')
            tower_num[1][tower_index[1]++] = tower_num[2][--tower_index[2]];
        if (src == 'C' && dst == 'A')
            tower_num[0][tower_index[0]++] = tower_num[2][--tower_index[2]];
        out_4(src, dst, option);
        tower_move(n, src, dst);
        speed_delay(speed);
    }

    else if (option == 9) {
        int out_come, g_n;
        char g_s, g_d;
        out_come = game_input_outcome(&g_s, &g_d);
        if (!out_come) {
            cout << "\n\nMAN! WHAT A PITY! GAME ABORTS��" << endl;
            return;
        }
        else {
            g_n = tower_num[g_s - 'A'][tower_index[g_s - 'A'] - 1];
            cct_gotoxy(0, 17 + 15);
            step++;
            cout << "��" << setfill(' ') << setw(4) << step << " ��(" << setw(2) << g_n << "#: " << g_s << "-->" << g_d << ")  ";
            if (g_s == 'A' && g_d == 'B')
                tower_num[1][tower_index[1]++] = tower_num[0][--tower_index[0]];
            if (g_s == 'A' && g_d == 'C')
                tower_num[2][tower_index[2]++] = tower_num[0][--tower_index[0]];
            if (g_s == 'B' && g_d == 'A')
                tower_num[0][tower_index[0]++] = tower_num[1][--tower_index[1]];
            if (g_s == 'B' && g_d == 'C')
                tower_num[2][tower_index[2]++] = tower_num[1][--tower_index[1]];
            if (g_s == 'C' && g_d == 'B')
                tower_num[1][tower_index[1]++] = tower_num[2][--tower_index[2]];
            if (g_s == 'C' && g_d == 'A')
                tower_num[0][tower_index[0]++] = tower_num[2][--tower_index[2]];
            out_4(g_s, g_d, option);
            tower_move(g_n, g_s, g_d);
            speed_delay(2);
            cout << endl;
        }

        if (tower_index[dst - 'A'] != n+1) {
            operation(n, src, tmp, dst, 9);
        }
        else {
            cct_gotoxy(0, 37);
            cout << "WELL DONE! GAME OVER!  " << endl;
            return;
        }
    }

    else
        ;
}


void h1()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 1);
    cout << endl;
}

void h2()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 2);
    cout << endl;
}

void h3()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 3);
    cout << endl;
}

void h4()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 4);
    cout << endl;
    cct_gotoxy(0, 35);
}

void h5()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 5);
    cout << endl;
    cct_gotoxy(0, 35);
}

void h6()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 6);
    cout << endl;
    cct_gotoxy(0, 35);
}

void h7()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 7);
    operation(1, src, tmp, dst, 7);
    cout << endl;
    cct_gotoxy(0, 35);
}

void h8()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 8);
    cout << endl;
    cct_gotoxy(0, 35);
}

void h9()
{
    int n;
    char src, dst, tmp;
    input(&n, &src, &tmp, &dst, 9);
    operation(n, src, tmp, dst, 9);
    cct_gotoxy(0, 37);
    cout << endl;

}



/*�������������Ե���ʧ�ܵĴ���*/
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ָ��λ�ã���ָ����ɫ����ʾһ���ַ����ɴ�
  ���������const int X       ��X�����꣨�У�
            const int Y       ��Y�����꣨�У�
            const char ch     ��Ҫ������ַ�ֵ
            const int bg_color������ɫ��ȱʡΪCOLOR_BLACK��
            const int fg_color������ɫ��ȱʡΪCOLOR_WHITE��
            const int rpt     ���ظ�������ȱʡΪ1��
  �� �� ֵ��
  ˵    ����X��Y�ķ�Χ�μ�gotoxy������˵��
***************************************************************************/
/*void cct_showch(const int X, const int Y, const char ch, const int bg_color, const int fg_color, const int rpt)
{
    int i;

    cct_gotoxy(X, Y);
    cct_setcolor(bg_color, fg_color);

     //ѭ��n�Σ���ӡ�ַ�ch
    for (i = 0; i < rpt; i++)
        putchar(ch);
}*/
/*
void Bprint_4()
{
    int y;
    cct_gotoxy(20, 11);
    y = 11;
    for (int i = 1; i < 12; i++) {
        cout << setw(2) << " ";
        y -= 1;
        cct_gotoxy(20, y);
    }
    cct_gotoxy(20, 11);
    y = 11;
    for (int i = 1; i < tower_index[1]; i++) {
        cout << setw(2) << tower_num[1][i];
        y -= 1;
        cct_gotoxy(20, y);
    }
}

void Cprint_4()
{
    int y;
    cct_gotoxy(30, 11);
    y = 11;
    for (int i = 1; i < 12; i++) {
        cout << setw(2) << " ";
        y -= 1;
        cct_gotoxy(30, y);
    }
    cct_gotoxy(30, 11);
    y = 11;
    for (int i = 1; i < tower_index[2]; i++) {
        cout << setw(2) << tower_num[2][i];
        y -= 1;
        cct_gotoxy(30, y);
    }
}
*/

/*    int bottom_helper, b_A=t_Y, b_B=t_Y, b_C=t_Y;
        if (src == 'A') {
        b_A = b_A - k;
        t_n = t_A + 11;
    }
    else if (src == 'B') {
        b_B = b_B - k;
        t_n = t_B + 11;
    }
    else {
        b_C = b_C - k;
        t_n = t_C + 11;
    }
 
    if (dst == 'A') {
        t_n = t_A + 11;
        bottom_helper = b_A;
    }
    else if (dst == 'B') {
        t_n = t_B + 11;
        bottom_helper = b_B;
    }
    else {
        t_n = t_C + 11;
        bottom_helper = b_C;
    }
*/

/*
    //Դ��Ϊ��
    //����ѹС�� �Ƿ��ƶ�
    int x_start, y_start, helper_1 = 0;
    char error;
    cct_gotoxy(0, 36);
    cct_gotoxy(60, 36);
    while (1) {
        cin.clear();
        cin.ignore(1024, '\n');    //���Ի�����������
        cct_gotoxy(60, 36);
        for (int i = 0; i < 100; i++)
            putchar(' ');

        cct_gotoxy(60, 36);
        char s, d;
        s = getchar();
        d = getchar();
        if ((s == 'q' || s == 'Q')) {
            return 0;
            break;
        }
        else if (int(s) == 10 || int(d) == 10) {
            continue;
        }
        else {
            if (s == 'a' || s == 'b' || s == 'c' || s == 'A' || s == 'B' || s == 'C') {
                d = getchar();
                if (d == 'a' || d == 'b' || d == 'c' || d == 'A' || d == 'B' || d == 'C') {
                    if (d != s) {
                        cout << "right" << endl;
                        break;
                    }
                }
                else if (int(d) == 10) {
                    continue;
                }
            }
        }
     }
*/