/* 2152131 �ƿ� ����� */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include "../include/cmd_console_tools.h"

using namespace std;

int menu(const char *choice)
{
	char option;
	while (1) {
		option = _getch();
		if (strchr(choice, option) != NULL)
			break;
	}

	return option;
}

void reset_cmd()
{
	cct_cls();
	cct_setfontsize("������", 16);
	cct_setconsoleborder(128, 30, 150, 1000);
	//cct_setconsoleborder(120, 40, 120, 9000);
}
/* ----------------------------------------------------------------------------------

1.�˵������ʾ
	�� �ַ�ָ������
	�� ��ά�ַ�����
	�� һά�ַ�����/ָ��
2.ѡ����ж�
	�� ��������(0-9��A-Q��)
	�� ������������(A-G��Q)
	�� �ַ��Ĵ�С����/������
	�� ����/��ĸ���
3.ѡ��Ĵ���
	�� menu������
	�� menu�������غ�

   ----------------------------------------------------------------------------------- */