/* 2152131 计科 吴洪蕊 */
#include <iostream>
#include "../include/cmd_console_tools.h"
#include <iomanip>
#include <conio.h>
#include <Windows.h>
#include "../include/common_menu.h"
#include "../include/common_functions.h"
#include "../include/cmd_gmw_tools.h"
#include "90-01-b2-gmw.h"

using namespace std;

int main()
{
	while (1) {
		srand((unsigned int)time(0));
		CONSOLE_GRAPHICS_INFO MagicBall_CGI;
		gmw_init(&MagicBall_CGI);

		int s[MAX_ROW][MAX_COL] = { -1 };
		reset_cmd();
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << " 1.内部数组，生成初始状态，寻找是否有初始可消除项" << endl;
		cout << " 2.内部数组，消除初始可消除项后非0项下落并用0填充" << endl;
		cout << " 3.内部数组，消除初始可消除项后查找消除提示" << endl;
		cout << " 4.n*n的框架(无分隔线)，显示初始状态" << endl;
		cout << " 5.n*n的框架(有分隔线)，显示初始状态" << endl;
		cout << " 6.n*n的框架(无分隔线)，显示初始状态及初始可消除项" << endl;
		cout << " 7.n*n的框架(有分隔线)，消除初始可消除项后显示消除提示" << endl;
		cout << " 8.cmd图形界面完整版(有分隔线，鼠标移动时显示坐标，右键退出)" << endl;
		cout << " 9.cmd图形界面完整版【只有这个有用！】" << endl;
		cout << " 0.退出" << endl;
		cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;
		cout << "[请选择:] ";

		const char* choice = "1234567890";
		//int option = menu(choice);
		//option = option - '0';

		//if (!option) {
		//	return 0;
		//}
		cct_cls();
		MagicBall_CGI.row_num = 7;
		MagicBall_CGI.col_num = 7;
		h9(&MagicBall_CGI, s);
		//input(&MagicBall_CGI, 5, 7);

		//switch (option) {
		//	case 9:
		//		h9(&MagicBall_CGI, s);
		//		break;
		//}

		int x, y;
		cout << endl;
		int l = strlen("本小题结束，请输入End继续...");
		cct_gotoxy((MagicBall_CGI.cols - l) / 2, MagicBall_CGI.SLI.lower_start_y + 1);//光标设到指定位置
		cout << "本小题结束，请输入End继续...";
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