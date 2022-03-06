#include <iostream>
#include <sstream>
#include <fstream>
#include "FA.cpp"
#include "obstream.cpp"
#include "ibstream.cpp"
#include "lz77.cpp"

using namespace std;

bool debug = false;

void test_print_binary(string str) {
	for (char c : str) {
		for (int i = 7; i > -1; i--) {
			cout << ((c >> i) & 1);
		}
		cout << endl;
	}
}

void test() {
	lz77 comp(10, 3);
	string str = "barbaraabarboraubaru";
	istringstream stream(str);
	stringstream lz;
	comp.encode(&stream, &lz);
	comp.decode(&lz, &cout);
}

void print_help() {
	cout << "usage: lz77 <mode> [<options>] <input-file> <output-file>" << endl << endl
		<< "       <mode>    ::= -encode | -decode" << endl
		<< "       <options> ::= --l <pi> [--s <pi>] | --s <pi> [--l <pi>]" << endl
		<< "       <pi>          is arbitrary positive integer" << endl
		<< endl
		<< "--l is parametrized optional argument setting lookahead size" << endl
		<< "--s is parametrized optional argument setting buffer size" << endl
		<< " (buffer size must be lesser or equal to lookahead) " << endl;
}

void error() {
	cout << "Bad arguments. Try -help to see all command arguments.";
}

void invalid_number(const char *arg) {
	cout << "\"" << arg << "\"" << " is not valid positive integer.";
}

bool read_positive(int* val, const char *arg) {
	int x = 0;
	int i = 0;
	if (arg[0] == '0') {
		invalid_number(arg);
		return false;
	}
	while (arg[i]) {
		if (!isdigit(arg[i])) {
			invalid_number(arg);
			return false;
		}
		x *= 10;
		x += arg[i] - '0';
		i++;
	}
	*val = x;
	return i > 0;
}

void encode_file(int lookahead, int size, const char* input_file, const char* output_file) {
	ifstream ifs;
	ofstream ofs;
	ifs.open(input_file, ios::binary);
	if (!ifs.is_open()) {
		cout << "Input file \"" << input_file << "\" cannot be opened.";
		return;
	}
	ofs.open(output_file, ios::binary);
	if (!ofs.is_open()) {
		cout << "Output file \"" << output_file << "\" cannot be opened.";
		return;
	}
	lz77 comp(size, lookahead);
	comp.encode(&ifs, &ofs);
	ifs.close();
	ofs.close();
}

void decode_file(int lookahead, int size, const char* input_file, const char* output_file) {
	ifstream ifs;
	ofstream ofs;
	ifs.open(input_file, ios::binary);
	if (!ifs.is_open()) {
		cout << "Input file \"" << input_file << "\" cannot be opened.";
		return;
	}
	ofs.open(output_file, ios::binary);
	if (!ofs.is_open()) {
		cout << "Output file \"" << output_file << "\" cannot be opened.";
		return;
	}
	lz77 comp(size, lookahead);
	comp.decode(&ifs, &ofs);
	ifs.close();
	ofs.close();
}

void parse_compression(int argc, char* argv[]) {
	bool encode;
	if (strcmp(argv[1], "-encode") == 0)
		encode = true;
	else if (strcmp(argv[1],"-decode") == 0)
		encode = false;
	else {
		error();
		return;
	}
	int lookahead = 3;
	int buffer_size = 10;
	if (argc >= 5) {
		if (strcmp(argv[2],"--l") == 0) {
			if (!read_positive(&lookahead, argv[3]))
				return;
			if (argc >= 7) {
				if (strcmp(argv[4], "--s") == 0) {
					if (!read_positive(&buffer_size, argv[5]))
						return;
				}
				else {
					error();
					return;
				}
			}
		}
		else if (strcmp(argv[2], "--s") == 0) {
			if (!read_positive(&buffer_size, argv[3]))
				return;
			if (argc >= 7) {
				if (strcmp(argv[4], "--l") == 0) {
					if (!read_positive(&lookahead, argv[5]))
						return;
				}
				else {
					error();
					return;
				}
			}
		}
		else {
			error();
			return;
		}
	}
	if (lookahead > buffer_size) {
		cout << "Lookahead must not be greater than buffer size.";
		return;
	}
	if (encode)
		encode_file(lookahead, buffer_size, argv[argc - 2], argv[argc - 1]);
	else
		decode_file(lookahead, buffer_size, argv[argc - 2], argv[argc - 1]);
}

void parse(int argc, char *argv[]) {
	if (argc == 2) {
		if (string(argv[1]) == "-help") {
			print_help();
		}
		else
			error();
	}
	else if (4 <= argc && argc <= 8 && argc != 5 && argc != 7) {
		parse_compression(argc, argv);
	}
	else {
		cout << "Bad number of arguments. Try -help to see all command arguments.";
	}
}

void test_e() {
	encode_file(3, 10, "i.txt", "o.txt");
}

void test_d() {
	decode_file(3, 10, "o.txt", "r.txt");
}

int main(int argc, char *argv[]) {
	if (debug) {
		test_e();
		test_d();
	}
	else
		parse(argc, argv);
	return 0;
}