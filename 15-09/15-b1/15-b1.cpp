/* 2152131 吴洪蕊 计科 */
#include<iostream>
#include<fstream>
#include <iomanip>
#include <sstream>
#include<cstdlib>
#include<cstring>
using namespace std;

#define LEN 256

//Usage: d1 --infile 原始文件[--outfile hex格式文件]
//d1 --infile a.docx
//d1 --infile a.docx --outfile a.hex
//00000000  43 68 61 6e 67 65 20 79 - 6f 75 72 20 66 2a 2a 6b     Change.your.f * *k
//00000010  69 6e 67 20 63 61 72 21                               ing.car!
//输入文件hh.txt打开失败!

int usage(const char* procname)
{
	cout << "Usage: " << procname << " --infile 原始文件[--outfile hex格式文件]" << endl;
	cout << "       " << procname << " --infile a.docx" << endl;
	cout << "       " << procname << " --infile a.docx --outfile a.hex" << endl;

	return 0;
}

void print_out(ifstream& in, ostringstream& output_stream) 
{
	int byteValue, offset = 0;
	int asciiValues[16];
	bool isValid[16] = { false };
	bool div_flag = false;
	while ((byteValue = in.get()) != EOF) {
		if (offset % 16 == 0)
			output_stream << setw(8) << setfill('0') << hex << offset << "  ";
		if (div_flag) {
			div_flag = false;
			output_stream << "- ";
		}
		output_stream << setw(2) << setfill('0') << hex << byteValue << " ";

		asciiValues[offset % 16] = byteValue;
		isValid[offset % 16] = true;
		offset++;

		if (offset % 8 == 0 && offset % 16)
			div_flag = true;

		if (offset % 16 == 0) {
			output_stream << "    ";
			for (int i = 0; i < 16; i++) {
				if (isValid[i] && asciiValues[i] >= 33 && asciiValues[i] <= 126)
					output_stream << char(asciiValues[i]);
				else
					output_stream << '.';
				isValid[i] = false;
			}
			output_stream << endl;
		}
	}
	
	output_stream << "  ";

	if (offset % 16 != 0) {
		int remainingBytes = offset % 16;
		for (int i = remainingBytes; i < 16; i++)
			output_stream << "   ";
		output_stream << "    ";
		for (int i = 0; i < remainingBytes; i++) {
			if (isValid[i] && asciiValues[i] >= 33 && asciiValues[i] <= 126)
				output_stream << char(asciiValues[i]);
			else
				output_stream << '.';
		}
		output_stream << endl;
	}
}

int d2h(ifstream& in, ofstream& out, char** argv, bool have_out, bool wrap)
{
	int index_in = 2;
	int index_out = 4;
	if (wrap) {
		index_in = 4;
		index_out = 2;
	}

	in.open(argv[index_in], ios::in | ios::binary);
	if (!in.is_open()) {
		cout << "输入文件" << argv[index_in] << "打开失败!" << endl;
		return -1;
	}
	ostringstream oss;
	if (have_out) {
		out.open(argv[index_out], ios::out);
		if (!out.is_open()) {
			cout << "文件打开失败" << endl;
			return -1;
		}
		print_out(in, oss);
		out << oss.str();
		//cout << oss.str();
	}
	print_out(in, oss);
	return 0;
}

int main(int argc, char* argv[])
{
	char file_name[LEN];
	ifstream in(file_name, ios::in | ios::binary);
	ofstream out;
	//if (!in) {
	//	cout << "输入文件" << file_name << "打开失败" << endl;
	//	return 0;
	//}
	if ((argc != 3) && (argc != 5)) {
		usage(argv[0]);
		return 0;
	}

	if (argc == 3) {
		if (strcmp(argv[1], "--infile")) { // 每次用strcmp都觉得返回值搞混了
			usage(argv[0]);
			return 0;
		}
	d2h(in, out, argv, 0, 0);
	}

	else if (argc == 5) {
		if (strcmp(argv[1], "--infile") == 0 && strcmp(argv[3], "--outfile") == 0)
			d2h(in, out, argv, 1, 0);
		else if (strcmp(argv[3], "--infile") == 0 && strcmp(argv[1], "--outfile") == 0)
			d2h(in, out, argv, 1, 1);
		else {
			usage(argv[0]);
			return 0;
		}
	}

	out.close();
	in.close();

	return 0;
}
