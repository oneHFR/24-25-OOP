/* 2152131 ����� �ƿ� */
#include<iostream>
using namespace std;
int main()
{
	int sel = 0;
	const char* month[] = {"January", "February", "March", "April", "May" , "June" , "July" , "August" , "September" , "October" , "November" , "December"};
	cout << "�������·�(1-12)" << endl;
	cin >> sel;
	if (sel < 1 || sel > 12 || !cin.good())
		cout << "Invalid" << endl;
	else
		cout << *(month + sel - 1) << endl;
	return 0;
}