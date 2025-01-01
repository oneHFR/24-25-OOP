/* 2152131 吴洪蕊 计科 */
#include <iostream>
#include <cmath>
using namespace std;

const double PI = 3.14159;

class Shape {
protected:
    //根据需要加入相应的成员，也可以为空
    virtual ~Shape() {}
public:
    virtual void ShapeName() = 0; //此句不准动
    //根据需要加入相应的成员，也可以为空
    virtual double area() = 0;
};

//此处给出五个类的定义及实现(成员函数采用体外实现形式)

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

/* -- 替换标记行 -- 本行不要做任何改动 -- 本行不要删除 -- 在本行的下面不要加入任何自己的语句，作业提交后从本行开始会被替换 -- 替换标记行 -- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：给出的是main函数的大致框架，允许进行微调或改变初值
***************************************************************************/
int main()
{
    if (1) {
        Circle    c1(5.2);           //半径（如果<=0，面积为0）
        Square    s1(5.2);           //边长（如果<=0，面积为0）
        Rectangle r1(5.2, 3.7);      //长、宽（如果任一<=0，面积为0）
        Trapezoid t1(2.3, 4.4, 3.8); //上底、下底、高（如果任一<=0，面积为0）
        Triangle  t2(3, 4, 5);       //三边长度（如果任一<=0或不构成三角形，面积为0）
        Shape* s[5] = { &c1, &s1, &r1, &t1, &t2 };

        int   i;
        for (i = 0; i < 5; i++) {
            s[i]->ShapeName(); //分别打印不同形状图形的名称（格式参考demo）
            cout << s[i]->area() << endl; //分别打印不同形状图形的面积（格式参考demo）
            cout << endl;
        }
    }

    if (1) {
        Circle    c1(-1);           //半径（如果<=0，面积为0）
        Square    s1(-1);           //边长（如果<=0，面积为0）
        Rectangle r1(5.2, -1);      //长、宽（如果任一<=0，面积为0）
        Trapezoid t1(2.3, -1, 3.8); //上底、下底、高（如果任一<=0，面积为0）
        Triangle  t2(3, 4, -1);       //三边长度（如果任一<=0或不构成三角形，面积为0）
        Shape* s[5] = { &c1, &s1, &r1, &t1, &t2 };

        cout << "============" << endl;
        int   i;
        for (i = 0; i < 5; i++) {
            s[i]->ShapeName(); //分别打印不同形状图形的名称（格式参考demo）
            cout << s[i]->area() << endl; //分别打印不同形状图形的面积（格式参考demo）
            cout << endl;
        }
    }

    return 0;
}

