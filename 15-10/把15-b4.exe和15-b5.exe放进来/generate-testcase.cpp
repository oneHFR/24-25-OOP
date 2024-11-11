#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main()
{
    ofstream outfile;
    for (int i = 1; i < 31; i++){
        string filename = "test" + to_string(i) + ".txt";
        outfile.open(filename, ios::out | ios::binary);
        int charNum = rand()%2000;
        for (int j = 0; j < charNum; j++){
            char c = rand()%256;
            outfile << c;
        }
        outfile.close();
    }
}