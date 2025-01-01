/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

/* �Ӵ˴�������滻��֮�䣬����������Ķ��弰ʵ��
	1����������ȫ�ֱ���������const��#define��
	2���������������ϵͳͷ�ļ�
*/
class integral
{
protected:
	int p_n;
	double lo, up;
	double (*targetFunction)(double x);
	char functionName[32];

public:
	integral(double (*func)(double x), const char* func_name);
	double value();
	friend istream& operator>>(istream& in, integral& obj);
};

integral::integral(double (*func)(double x), const char* func_name)
{
	p_n = 0;
	lo = up = 0.0;
	targetFunction = func;
	strcpy(functionName, func_name);
}

istream& operator>>(istream& in, integral& obj)
{
	cout << "������" << obj.functionName << "dx�����ޡ����޼����仮������" << endl;
	in >> obj.lo >> obj.up >> obj.p_n;
	return in;
}

double integral::value()
{
	double x = lo;
	double stepSize = (up - lo) / p_n;
	double sum = 0.0;
	for (int i = 0; i < p_n; i++)
	{
		x += stepSize;
		sum += stepSize * targetFunction(x);
	}
	cout << functionName << "dx[" << lo << "~" << up << "/n=" << p_n << "] : "<< sum <<endl;
	return sum;
}



class integral_sin : public integral
{
public:
	integral_sin() : integral(sin, "sinx") {}
};


class integral_cos : public integral
{
public:
	integral_cos() : integral(cos, "cosx") {}
};


class integral_exp : public integral
{
public:
	integral_exp() : integral(exp, "e^x") {}
};

/* -- �滻����� -- ���в�Ҫ���κθĶ� -- ���в�Ҫɾ�� -- �ڱ��е����治Ҫ�����κ��Լ�����䣬��ҵ�ύ��ӱ��п�ʼ�ᱻ�滻 -- �滻����� -- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����fun_integral��׼����˼��һ�£�integralӦ��ζ���
***************************************************************************/
void fun_integral(integral& fRef)
{
	cin >> fRef;	//���������ޡ�������
	cout << fRef.value() << endl;
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����main������׼��
***************************************************************************/
int main()
{
	integral_sin s1;
	integral_cos s2;
	integral_exp s3;

	fun_integral(s1); //����sinxdx��ֵ
	fun_integral(s2); //����cosxdx��ֵ
	fun_integral(s3); //����expxdx��ֵ

	return 0;
}

//ע�����μ���ȡ��ֵ�����Ϊ����double��ʽ

