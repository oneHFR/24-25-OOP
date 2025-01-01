/* 2152131 ����� �ƿ� */
#include <iostream>
#include <cmath>
using namespace std;

const double PI = 3.14159;

class Shape {
protected:
    //������Ҫ������Ӧ�ĳ�Ա��Ҳ����Ϊ��
    virtual ~Shape() {}
public:
    virtual void ShapeName() = 0; //�˾䲻׼��
    //������Ҫ������Ӧ�ĳ�Ա��Ҳ����Ϊ��
    virtual double area() = 0;
};

//�˴����������Ķ��弰ʵ��(��Ա������������ʵ����ʽ)

/***************************************************************************/
class Circle : public Shape {
protected:
    double r;
public:
    Circle(double rr);
    virtual void ShapeName();
    virtual double area();
};

Circle::Circle(double rr)
{
    r = rr;
}

void Circle::ShapeName()
{
    cout << "Circle" << endl;
}

double Circle::area()
{
    return r > 0.0 ? PI * r * r : 0;
}

/***************************************************************************/
class Square : public Shape {
protected:
    double a;
public:
    Square(double aa);
    virtual void ShapeName();
    virtual double area();
};

Square::Square(double aa)
{
    a = aa;
}

void Square::ShapeName()
{
    cout << "Square" << endl;
}

double Square::area()
{
    return a > 0 ? a * a : 0;
}


/***************************************************************************/
class Rectangle : public Shape {
protected:
    double a, b;
public:
    Rectangle(double aa, double bb);
    virtual void ShapeName();
    virtual double area();
};

Rectangle::Rectangle(double aa, double bb)
{
    a = aa;
    b = bb;
}

void Rectangle::ShapeName()
{
    cout << "Rectangle" << endl;
}

double Rectangle::area()
{
    return a > 0 && b > 0 ? a * b : 0;
}

/***************************************************************************/
class Triangle : public Shape {
protected:
    double a, b, c;
public:
    Triangle(double aa, double bb, double cc);
    virtual void ShapeName();
    virtual double area();
};


Triangle::Triangle(double aa, double bb, double cc)
{
    a = aa;
    b = bb;
    c = cc;
}

void Triangle::ShapeName()
{
    cout << "Triangle" << endl;
}

double Triangle::area()
{
    if (a <= 0 || b <= 0 || c <= 0 || a + b <= c || a + c <= b || b + c <= a) return 0;
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}


/***************************************************************************/
class Trapezoid : public Shape {

public:
    double up_bot, low_bot, high;
    Trapezoid(double a, double b, double c);
    virtual void ShapeName();
    virtual double area();
};

Trapezoid::Trapezoid(double a, double b, double c)
{
    up_bot = a;
    low_bot = b;
    high = c;
}

void Trapezoid::ShapeName()
{
    cout << "Trapezoid" << endl;
}

double Trapezoid::area()
{
    return (up_bot > 0 && low_bot > 0 && high > 0) ? (up_bot + low_bot) * high / 2 : 0;
}

/* -- �滻����� -- ���в�Ҫ���κθĶ� -- ���в�Ҫɾ�� -- �ڱ��е����治Ҫ�����κ��Լ�����䣬��ҵ�ύ��ӱ��п�ʼ�ᱻ�滻 -- �滻����� -- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������������main�����Ĵ��¿�ܣ��������΢����ı��ֵ
***************************************************************************/
int main()
{
    if (1) {
        Circle    c1(5.2);           //�뾶�����<=0�����Ϊ0��
        Square    s1(5.2);           //�߳������<=0�����Ϊ0��
        Rectangle r1(5.2, 3.7);      //�����������һ<=0�����Ϊ0��
        Trapezoid t1(2.3, 4.4, 3.8); //�ϵס��µס��ߣ������һ<=0�����Ϊ0��
        Triangle  t2(3, 4, 5);       //���߳��ȣ������һ<=0�򲻹��������Σ����Ϊ0��
        Shape* s[5] = { &c1, &s1, &r1, &t1, &t2 };

        int   i;
        for (i = 0; i < 5; i++) {
            s[i]->ShapeName(); //�ֱ��ӡ��ͬ��״ͼ�ε����ƣ���ʽ�ο�demo��
            cout << s[i]->area() << endl; //�ֱ��ӡ��ͬ��״ͼ�ε��������ʽ�ο�demo��
            cout << endl;
        }
    }

    if (1) {
        Circle    c1(-1);           //�뾶�����<=0�����Ϊ0��
        Square    s1(-1);           //�߳������<=0�����Ϊ0��
        Rectangle r1(5.2, -1);      //�����������һ<=0�����Ϊ0��
        Trapezoid t1(2.3, -1, 3.8); //�ϵס��µס��ߣ������һ<=0�����Ϊ0��
        Triangle  t2(3, 4, -1);       //���߳��ȣ������һ<=0�򲻹��������Σ����Ϊ0��
        Shape* s[5] = { &c1, &s1, &r1, &t1, &t2 };

        cout << "============" << endl;
        int   i;
        for (i = 0; i < 5; i++) {
            s[i]->ShapeName(); //�ֱ��ӡ��ͬ��״ͼ�ε����ƣ���ʽ�ο�demo��
            cout << s[i]->area() << endl; //�ֱ��ӡ��ͬ��״ͼ�ε��������ʽ�ο�demo��
            cout << endl;
        }
    }

    return 0;
}

