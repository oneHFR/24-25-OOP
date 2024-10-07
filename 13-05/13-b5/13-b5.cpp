/* 计科 2152131 吴洪蕊 */
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
	if (list == NULL) return;  // 先检查指针是否为空
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
		cout << "打开stulist.txt文件失败" << endl;
		return -1;
	}
	if (!fout.is_open()){
		cout << "打开result.txt文件失败" << endl;
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
		// ？？
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
姓名和毕业学校的长度不固定→需要动态分配内存
报名号是一个整型→直接用 int 类型表示
从内存的消耗上单个结构体会节省内存且链表在动态增删节点时具有一定优势

但考虑到本题只涉及一次性的随机抽取，并且我们需要高效的随机访问，数组是更合适的选择。
由于前几题形成基本架构懒得改了 但是实际考虑上该题的检索需求是频繁的
意味着面对数组支持 O(1) 的访问时间，而链表需要 O(M) 的遍历时间情况下数组是更为合适的。
*/
