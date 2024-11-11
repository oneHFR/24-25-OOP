#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
    ofstream out("out.txt", ios::out | ios::binary);

    out << "hello" << endl;

    out.close();

    return 0;
}
