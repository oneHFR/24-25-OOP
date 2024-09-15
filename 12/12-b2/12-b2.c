/* 2152131 ����� �ƿ� */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

/*
������sinxdx�����ޡ����޼����仮������
sinxdx[0~1/n=100] : 0.463901
������cosxdx�����ޡ����޼����仮������
cosxdx[-1~1/n=20] : 1.68154
������e^xdx�����ޡ����޼����仮������
e^xdx[0~2/n=50] : 6.51769
*/
double definite_integration(double (*mode)(double), double high, double low, int n)
{
	double sigma = 0;
	double delta = (high - low) / n;
	double i = low;
	while (fabs(i - high) > 1e-8) {
		sigma += mode(i + delta) * delta;
		i += delta;
	}
	return sigma;
}

double sin_x(double x)
{
	return sin(x);
}

double cos_x(double x)
{
	return cos(x);
}

double exp_x(double x)
{
	return exp(x);
}

int main()
{
	int n;
	double low, high, value;
	printf("������sinxdx�����ޡ����޼����仮������\n");
	scanf("%lf %lf %d", &low, &high, &n);
	value = definite_integration(sin_x, high, low, n);
	printf("sinxdx[%d~%d/n=%d] : %.6g\n", (int)low, (int)high, n, value);

	printf("������cosxdx�����ޡ����޼����仮������\n");
	scanf("%lf %lf %d", &low, &high, &n);
	value = definite_integration(cos_x, high, low, n);
	printf("cosxdx[%d~%d/n=%d] : %.6g\n", (int)low, (int)high, n, value);

	printf("������e^xdx�����ޡ����޼����仮������\n");
	scanf("%lf %lf %d", &low, &high, &n);
	value = definite_integration(exp_x, high, low, n);
	printf("e^xdx[%d~%d/n=%d] : %.6g\n", (int)low, (int)high, n, value);

	return 0;
}