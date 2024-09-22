/* 2152131 计科 吴洪蕊 */
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cstring>
#include "90-01-b2-magic_ball.h"
#include "../include/cmd_console_tools.h"
#include "../include/common_menu.h"
#include "../include/common_functions.h"
#include <Windows.h>

using namespace std; 

int main()
{
	while (1) {
		reset_cmd();
		int r, c;
		int s[11][11] = { -1 };

		cct_cls();
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << " 1.内部数组，生成初始状态，寻找是否有初始可消除项" << endl;
		cout << " 2.内部数组，消除初始可消除项后非0项下落并用0填充" << endl;
		cout << " 3.内部数组，消除初始可消除项后查找消除提示" << endl;
		cout << " 4.n*n的框架(无分隔线)，显示初始状态" << endl;
		cout << " 5.n*n的框架(有分隔线)，显示初始状态" << endl;
		cout << " 6.n*n的框架(无分隔线)，显示初始状态及初始可消除项" << endl;
		cout << " 7.n*n的框架(有分隔线)，消除初始可消除项后显示消除提示" << endl;
		cout << " 8.cmd图形界面完整版(有分隔线，鼠标移动时显示坐标，右键退出)" << endl;
		cout << " 9.cmd图形界面完整版" << endl;
		cout << " 0.退出" << endl;
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << "[请选择:] ";

		const char* choice = "1234567890";
		int option = menu(choice);
		option = option - '0';

		if (!option) {
			return 0;
		}
		cct_cls();
		input(&r, &c, 5, 9);
		switch (option) {
			case 1:
				h1(s, r, c);
				break;
			case 2:
				h2(s, r, c);
				break;
			case 3:
				h3(s, r, c);
				break;
			case 4:
				h4(s, r, c);
				break;
			case 5:
				h5(s, r, c);
				break;
			case 6:
				h6(s, r, c);
				break;
			case 7:
				h7(s, r, c);
				break;
			case 8:
				h8(s, r, c);
				break;
			case 9:
				h9(s, r, c);
				break;
		}

		int x, y;
		cout << "\n本小题结束，请输入End继续...";
		cct_getxy(x, y);
		while (1) {

			char keepon[1024] = { 0 };
			cin.getline(keepon, 1024);
			for (int i = 0; keepon[i] != 0; i++) {
				if (keepon[i] <= 'z' && keepon[i] >= 'a') {
					keepon[i] = keepon[i] - ('a' - 'A');
				}
			}
			if (!strcmp(keepon, "END")) {
				break;
			}
			else {
				cct_gotoxy(0, y + 1);
				cout << "输入错误，请重新输入";
				cct_gotoxy(x, y);
				cout << setw(50) << " ";
				cct_gotoxy(x, y);
			}
		}
	}

	return 0;
}