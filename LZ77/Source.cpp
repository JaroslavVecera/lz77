#include <iostream>
#include <sstream>
#include <fstream>
#include "FA.cpp"
#include "obstream.cpp"
#include "ibstream.cpp"
#include "lz77.cpp"

using namespace std;

void test_print_binary(string str) {
	for (char c : str) {
		for (int i = 7; i > -1; i--) {
			cout << ((c >> i) & 1);
		}
		cout << endl;
	}
}

void test2() {
	lz77 hnup(10, 3);
	string str = "barbaraabarboraubaru";
	istringstream stream(str);
	hnup.encode(&stream, &cout);
}

void test() {
	std::stringstream ss;
	ss << "k";
	int a;
	ss >> a;
	ss >> a;
	ss >> a;
	ss >> a;
	ss >> a;
	char* b = (char*)calloc(5, sizeof(char));
	ss.read(b, 55);
	fa analyzer(&ss);
	ss.clear();
	obstream bitstream(&ss);
	bitstream << 1;
	bitstream << 1;
	bitstream << 1;
	bitstream << 1;
	bitstream << 1;
	bitstream << 1;
	bitstream << 1;
	bitstream << 1;
	bitstream << 1;
	bitstream << 0;
	bitstream << 1;
	bitstream.flush();

	test_print_binary(ss.str());

	ibstream ibs(&ss);
	bool x = true;
	while (ibs >> x) {
		cout << x;
	}
}

int main() {
	test2();
}