/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <time.h>
/* 如果有需要，此处可以添加头文件 */

using namespace std;

/* 允许定义常变量/宏定义，但不允许定义全局变量 */

/* 可以添加自己需要的函数 */
const char* cards[] = { "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A", "2", "BJ", "RJ"};

#if (__linux__)
const char* modify[] = { "C", "D", "H", "S" };
#else
const char* modify[] = { "\5", "\4", "\3", "\6" };
#endif

int choose()
{
	int landlord = -1;
	while (1) {
		cout << "请选择一个地主[0-2]：" << endl;
		cin >> landlord;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else if (landlord >= 0 && landlord <= 2)
			return landlord;
	}
}

/* 拟定函数 */
/***************************************************************************
  函数名称：
  功    能：打印某个玩家的牌面信息，如果是地主，后面加标记
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int print(const char* prompt, const bool landlord, const unsigned long long player)
{
	/* 只允许定义不超过三个基本类型的简单变量，不能定义数组变量、结构体、string等 */
	cout << prompt;

	for (int i = 0; i < 54; i++){
		if (player & ((unsigned long long) 1 << i)){
			if (i <= 51)
				cout << modify[i % 4] << cards[i / 4] << " ";
			else if (i == 52)
				cout << "BJ" << " ";
			else if (i == 53)
				cout << "RJ" << " ";
		}
	}

	if (landlord)
		cout << " (地主)";
	cout << endl;
	return 0;
}

/***************************************************************************
  函数名称：
  功    能：发牌（含键盘输入地主）
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int deal(unsigned long long* player)
{
	/* 只允许定义不超过十个基本类型的简单变量，不能定义数组变量、结构体、string等 */
	srand((unsigned int)(time(NULL)));
	unsigned long long i_card = ((unsigned long long)1 << 54) - 1;

	for (int i = 0; i < 17; i++){
		for (int j = 0; j < 3;) {
			int index = rand() % 54; 
			if (i_card & ((unsigned long long)1 << index)) { // 查重 
				player[j] |= ((unsigned long long)1 << index);
				i_card &= ~((unsigned long long)1 << index);
				j++;
			}
		}

		cout << "第" << i + 1 << "轮结束：" << endl;
		print("甲的牌：", 0, player[0]);
		print("乙的牌：", 0, player[1]);
		print("丙的牌：", 0, player[2]);

	}
	cout << endl;
	int landlord = choose();
	player[landlord] |= i_card;
	return landlord; //此处修改为选定的地主(0-2)
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数，不准修改
 ***************************************************************************/
//请选择一个地主[0 - 2]：
//乙的牌：3 3 3 6 7 8 9 9 J Q Q K A A A A 2 2 2 2 (地主)
//按回车键开始发牌第1轮结束：
//甲的牌：Q
//乙的牌：6
//丙的牌：T
//第2轮结束： 17轮

int main()
{
	unsigned long long player[3] = { 0 }; //存放三个玩家的发牌信息
	int landlord; //返回0-2表示哪个玩家是地主

	cout << "按回车键开始发牌";
	while (getchar() != '\n')
		;

	landlord = deal(player);
	print("甲的牌：", (landlord == 0), player[0]);
	print("乙的牌：", (landlord == 1), player[1]);
	print("丙的牌：", (landlord == 2), player[2]);

	return 0;
}
