/* 2152131 ����� �ƿ� */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <time.h>
/* �������Ҫ���˴��������ͷ�ļ� */

using namespace std;

/* �����峣����/�궨�壬����������ȫ�ֱ��� */

/* ��������Լ���Ҫ�ĺ��� */
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
		cout << "��ѡ��һ������[0-2]��" << endl;
		cin >> landlord;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1024, '\n');
		}
		else if (landlord >= 0 && landlord <= 2)
			return landlord;
	}
}

/* �ⶨ���� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ӡĳ����ҵ�������Ϣ������ǵ���������ӱ��
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int print(const char* prompt, const bool landlord, const unsigned long long player)
{
	/* ֻ�����岻���������������͵ļ򵥱��������ܶ�������������ṹ�塢string�� */
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
		cout << " (����)";
	cout << endl;
	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ƣ����������������
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
int deal(unsigned long long* player)
{
	/* ֻ�����岻����ʮ���������͵ļ򵥱��������ܶ�������������ṹ�塢string�� */
	srand((unsigned int)(time(NULL)));
	unsigned long long i_card = ((unsigned long long)1 << 54) - 1;

	for (int i = 0; i < 17; i++){
		for (int j = 0; j < 3;) {
			int index = rand() % 54; 
			if (i_card & ((unsigned long long)1 << index)) { // ���� 
				player[j] |= ((unsigned long long)1 << index);
				i_card &= ~((unsigned long long)1 << index);
				j++;
			}
		}

		cout << "��" << i + 1 << "�ֽ�����" << endl;
		print("�׵��ƣ�", 0, player[0]);
		print("�ҵ��ƣ�", 0, player[1]);
		print("�����ƣ�", 0, player[2]);

	}
	cout << endl;
	int landlord = choose();
	player[landlord] |= i_card;
	return landlord; //�˴��޸�Ϊѡ���ĵ���(0-2)
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main��������׼�޸�
 ***************************************************************************/
//��ѡ��һ������[0 - 2]��
//�ҵ��ƣ�3 3 3 6 7 8 9 9 J Q Q K A A A A 2 2 2 2 (����)
//���س�����ʼ���Ƶ�1�ֽ�����
//�׵��ƣ�Q
//�ҵ��ƣ�6
//�����ƣ�T
//��2�ֽ����� 17��

int main()
{
	unsigned long long player[3] = { 0 }; //���������ҵķ�����Ϣ
	int landlord; //����0-2��ʾ�ĸ�����ǵ���

	cout << "���س�����ʼ����";
	while (getchar() != '\n')
		;

	landlord = deal(player);
	print("�׵��ƣ�", (landlord == 0), player[0]);
	print("�ҵ��ƣ�", (landlord == 1), player[1]);
	print("�����ƣ�", (landlord == 2), player[2]);

	return 0;
}
