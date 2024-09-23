/* 计科 2152131 吴洪蕊 */
/* 班级 学号 姓名 */
#include <iostream>
using namespace std;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
template <typename T, int N> // T代表数据类型的占位符
void test(T(&array)[N])	//将...替换为相应内容
{// 此函数不返回任何数值
	cout << sizeof(array) << endl;	//array为形参名
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
//template <int N> // 这里数据类型都有指定故可以不用typename T 但是题目要求应当多考虑
////template <typename T, int N>
//char* tj_strcpy_s(char (& dst)[N], const char *src)	//将...替换为相应内容
//{
//	char* d = dst;		//dst为要复制的目标串
//	const char* s = src;	//src为要复制的源串
//
//	while (*s != '\0' && d - dst < int(sizeof(dst)) - 1)
//		*d++ = *s++;
//	*d = '\0';
//
//	return dst;
//}


template <typename T, int N>
T* tj_strcpy_s(T(&dst)[N], const T* src)	// 使用模板参数T代替char
{
	T* d = dst;		
	const T* s = src;	

	while (*s != '\0' && d - dst < N - 1)
		*d++ = *s++;
	*d = '\0';

	return dst;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main()
{
	int t1[10]; // 4*10
	double t2[20]; // 8*20
	test(t1);	//期望输出40
	test(t2);	//期望输出160

	char s1[10], s2[20];
	const char* s = "Welcome to Tongji University!";
	cout << tj_strcpy_s(s1, s) << '*' << endl; //期望输出：Welcome t*
	cout << tj_strcpy_s(s2, s) << '*' << endl; //期望输出：Welcome to Tongji U*

	//short shortArray1[10] = { 0 };
	//short shortArray2[20] = { 0 };
	//const short srcShorts[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
	//tj_strcpy_s(shortArray1, srcShorts);
	//tj_strcpy_s(shortArray2, srcShorts);
	//for (short i = 0; i < 10; ++i) cout << shortArray1[i] << ' ';
	//cout << '*' << endl; 
	//for (short i = 0; i < 20; ++i) cout << shortArray2[i] << ' ';
	//cout << '*' << endl; 

	//long longArray1[10] = { 0 };
	//long longArray2[20] = { 0 };
	//const long srcLongs[] = { 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 0L };
	//tj_strcpy_s(longArray1, srcLongs);
	//tj_strcpy_s(longArray2, srcLongs);
	//for (long i = 0; i < 10; ++i) cout << longArray1[i] << ' ';
	//cout << '*' << endl; 
	//for (long i = 0; i < 20; ++i) cout << longArray2[i] << ' ';
	//cout << '*' << endl; 

	return 0;
}
