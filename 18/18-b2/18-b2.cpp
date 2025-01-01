/* 2152131 吴洪蕊 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

/* 从此处到标记替换行之间，给出各种类的定义及实现
	1、不允许定义全局变量（不含const和#define）
	2、不允许添加其它系统头文件
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
	cout << "请输入" << obj.functionName << "dx的下限、上限及区间划分数量" << endl;
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

/* -- 替换标记行 -- 本行不要做任何改动 -- 本行不要删除 -- 在本行的下面不要加入任何自己的语句，作业提交后从本行开始会被替换 -- 替换标记行 -- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：fun_integral不准动，思考一下，integral应如何定义
***************************************************************************/
void fun_integral(integral& fRef)
{
	cin >> fRef;	//输入上下限、划分数
	cout << fRef.value() << endl;
	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：main函数不准动
***************************************************************************/
int main()
{
	integral_sin s1;
	integral_cos s2;
	integral_exp s3;

	fun_integral(s1); //计算sinxdx的值
	fun_integral(s2); //计算cosxdx的值
	fun_integral(s3); //计算expxdx的值

	return 0;
}

//注：矩形计算取右值，输出为正常double格式

