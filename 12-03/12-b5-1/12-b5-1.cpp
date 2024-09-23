/* �ƿ� 2152131 ����� */
/* �༶ ѧ�� ���� */
#include <iostream>
using namespace std;

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
template <typename T, int N> // T�����������͵�ռλ��
void test(T(&array)[N])	//��...�滻Ϊ��Ӧ����
{// �˺����������κ���ֵ
	cout << sizeof(array) << endl;	//arrayΪ�β���
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
//template <int N> // �����������Ͷ���ָ���ʿ��Բ���typename T ������ĿҪ��Ӧ���࿼��
////template <typename T, int N>
//char* tj_strcpy_s(char (& dst)[N], const char *src)	//��...�滻Ϊ��Ӧ����
//{
//	char* d = dst;		//dstΪҪ���Ƶ�Ŀ�괮
//	const char* s = src;	//srcΪҪ���Ƶ�Դ��
//
//	while (*s != '\0' && d - dst < int(sizeof(dst)) - 1)
//		*d++ = *s++;
//	*d = '\0';
//
//	return dst;
//}


template <typename T, int N>
T* tj_strcpy_s(T(&dst)[N], const T* src)	// ʹ��ģ�����T����char
{
	T* d = dst;		
	const T* s = src;	

	while (*s != '\0' && d - dst < N - 1)
		*d++ = *s++;
	*d = '\0';

	return dst;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main()
{
	int t1[10]; // 4*10
	double t2[20]; // 8*20
	test(t1);	//�������40
	test(t2);	//�������160

	char s1[10], s2[20];
	const char* s = "Welcome to Tongji University!";
	cout << tj_strcpy_s(s1, s) << '*' << endl; //���������Welcome t*
	cout << tj_strcpy_s(s2, s) << '*' << endl; //���������Welcome to Tongji U*

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
