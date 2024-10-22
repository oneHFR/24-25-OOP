/* 2152131 ����� �ƿ� */
#include<iostream>
#include<string.h>
#include<iomanip>

using namespace std;

#define MAX 10

int tj_strlen(const char str[])
{
	/* ע�⣺�����ڲ��������κ���ʽ�����飨������̬���飩 */
	int i = 0;
	while (str[i] != '\0') {
		i++;
	}
	return i; //returnֵ�ɸ�����Ҫ�޸�
}

int tj_strupr(char str[])
{
	/* ע�⣺�����ڲ��������κ���ʽ�����飨������̬���飩 */
	int w = tj_strlen(str);
	for (int i = 0; i < w; i++) {
		if (str[i] != '\0') {
			if (str[i] >= 'a' && str[i] <= 'z')
				str[i] -= 'a' - 'A';
		}
	}
	return 0; //returnֵ�ɸ�����Ҫ�޸�
}

int tj_strstr(const char str[], const char substr[])
{
	/* ע�⣺�����ڲ��������κ���ʽ�����飨������̬���飩 */
	int w = strlen(str);
	int ws = strlen(substr);
	int helper = 0;

	for (int i = 0; i < w; i++) {
		if (str[i] == substr[0]) {
			helper = 1;
			for (int j = 1; j < ws; j++) {
				if (str[j + i] != substr[j]) {
					helper = 0;
					break;
				}
			}
			if (helper) {
				return i + 1;
			}
		}
	}


	return 0; //returnֵ�ɸ�����Ҫ�޸�
}

/* ON��1��OFF��0 *, Quit��-1 */
void input(int& No, int& state)
{
	char in_no;
	char in_state[MAX];

	while (1) {
		cout << "����(\"A On /J Off\"��ʽ���룬����\"Q on/off\"�˳�)" << endl;
		cin >> in_no;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
		} // ���cin�������޷��������
		
		else {
			if (in_no == 'Q' || in_no == 'q') {
				state = -1;
				break;
			}

			else {
				if (in_no >= 'a' && in_no <= 'j')
					in_no += 'A' - 'a';

				cin >> in_state;
				No = in_no - 'A';
				tj_strupr(in_state);
				if (tj_strstr(in_state, "ON")) {
					state = 1;
					return;
				}
				else if (tj_strstr(in_state, "OFF")) {
					state = 0;
					return;
				}
			}
		}
	}
}

void update(short& s, int No, int state)
{
	if (state == 1)
		s |= (1 << No);
	else if (state == 0)
		s &= ~(1 << No);
}

void show_state(short s)
{
	cout << "��ǰ״̬��0x" << setfill('0') << setw(4) << hex << s << dec << endl;
	cout << "A   B   C   D   E   F   G   H   I   J" << endl;
	for (int i = 0; i < 10; ++i)
		cout << ((s & (1 << i)) ? "ON " : "OFF") << ' ';
	cout << endl << endl;
}
/*
��ʼ״̬��0x0000
A   B   C   D   E   F   G   H   I   J
OFF OFF OFF OFF OFF OFF OFF OFF OFF OFF

����("A On /J Off"��ʽ���룬����"Q on/off"�˳�)
��ǰ״̬��0x0001
A   B   C   D   E   F   G   H   I   J
ON  OFF OFF OFF OFF OFF OFF OFF OFF OFF

����("A On /J Off"��ʽ���룬����"Q on/off"�˳�)
*/

int main()
{
	cout << "��ʼ״̬��0x0000" << endl;
	cout << "A   B   C   D   E   F   G   H   I   J" << endl;
	cout << "OFF OFF OFF OFF OFF OFF OFF OFF OFF OFF" << endl << endl;

	short s = 0;

	while (1) {
		int in_no;
		int in_state;
		input(in_no, in_state);
		if (in_state == -1)
			return -1;
		update(s, in_no, in_state);
		show_state(s);
	}

	return 0;
}