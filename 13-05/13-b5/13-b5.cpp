/* �ƿ� 2152131 ����� */
#define _CRT_SECURE_NO_WARNINGS
#define buffer 1024
#include<iostream>
#include<fstream>
#include <iomanip>
#include<ctime>
#include<cstring>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h> 

using namespace std;

struct student {
    int no;
    char* name;
    char* school;
};

void free_all(struct student* list, int list_num)
{
	if (list == NULL) return;  // �ȼ��ָ���Ƿ�Ϊ��
	for (int i = 0; i < list_num; i++){
		delete list[i].name;
		delete list[i].school;
	}
	delete[] list;
}

int main() 
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ifstream fin;
	ofstream fout;
	fin.open("stulist.txt", ios::in);
	fout.open("result.txt", ios::out);
	if (!fin.is_open()){
		cout << "��stulist.txt�ļ�ʧ��" << endl;
		return -1;
	}
	if (!fout.is_open()){
		cout << "��result.txt�ļ�ʧ��" << endl;
		fin.close();
		return -1;
	}
	

	int real = 0, num = 0;
	fin >> num;
	fin >> real;
	struct student* r = new(nothrow) student[real];
	if (r == NULL) {
		cout << "Memory allocation using new failed!" << endl;
		fin.close();
		fout.close();
		return -1;
	}
	struct student* final = new(nothrow) student[num];
	if (final == NULL) {
		cout << "Memory allocation using new failed!" << endl;
		fin.close();
		fout.close();
		return -1;
	}
	int* index_list = new(nothrow) int[real];
	if (index_list == NULL) {
		cout << "Memory allocation using new failed!" << endl;
		fout.close();
		fin.close();
		return -1;
	}
	for (int i = 0; i < real; i++)
		index_list[i] = 0;

	for (int i = 0; i < real; i++) {
		int no;
		char name[buffer], school[buffer];
		fin >> no >> name >> school;
		r[i].no = no;
		if (r[i].no == 0) {
			fout.close();
			fin.close();
			return -1;
		}
		r[i].name = new(nothrow) char[strlen(name) + 1];
		if (r[i].name == NULL){
			fout.close();
			fin.close();
			return -1;
		}
		r[i].school = new(nothrow) char[strlen(school) + 1];
		if (r[i].school == NULL) {
			fout.close();
			fin.close();
			return -1;
		}
		strcpy(r[i].name, name);
		strcpy(r[i].school, school);
	}
	 
	// random select
	srand((unsigned int)(time(0)));
	int r_index = -1;
	for (int i = 0; i < num; i++) {
			
		r_index = rand() % real;
		// check
		if (index_list[r_index]) {
			i--;
			continue;
		}
		index_list[r_index] = 1;
		

		final[i].no = r[r_index].no;
		if (final[i].no == 0) {
			fout.close();
			fin.close();
			return -1;
		}
		// ����
		final[i].school = new(nothrow) char[strlen(r[r_index].school) + 1];
		if (final[i].school == NULL) {
			fout.close();
			fin.close();
			return -1;
		}
		strcpy(final[i].school, r[r_index].school);
		
		final[i].name = new(nothrow) char[strlen(r[r_index].name) + 1];
		if (final[i].name == NULL) {
			fout.close();
			fin.close();
			return -1;
		}
		strcpy(final[i].name, r[r_index].name);

	}

	for (int i = 0; i < num; i++)
		fout << final[i].no << ' ' << final[i].name << ' ' << final[i].school << endl;

	free_all(r, real);
	free_all(final, num);
	delete[] index_list;

	fout.close();
	fin.close();
	cout << "over!" << endl;
	return 0;
}
/*
�����ͱ�ҵѧУ�ĳ��Ȳ��̶�����Ҫ��̬�����ڴ�
��������һ�����͡�ֱ���� int ���ͱ�ʾ
���ڴ�������ϵ����ṹ����ʡ�ڴ��������ڶ�̬��ɾ�ڵ�ʱ����һ������

�����ǵ�����ֻ�漰һ���Ե������ȡ������������Ҫ��Ч��������ʣ������Ǹ����ʵ�ѡ��
����ǰ�����γɻ����ܹ����ø��� ����ʵ�ʿ����ϸ���ļ���������Ƶ����
��ζ���������֧�� O(1) �ķ���ʱ�䣬��������Ҫ O(M) �ı���ʱ������������Ǹ�Ϊ���ʵġ�
*/
