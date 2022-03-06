#pragma once

#include <iostream>
#include <vector>

using namespace std;

class ibstream {
	istream* stream;
	char bit_buffer = 0;
	char lookahead_buffer = 0;
	char buffer_size = 0;
	char offset = 0;
	bool eof_flag = false;

	void fill_buffer() {
		bit_buffer = lookahead_buffer;
		lookahead_buffer = stream->get();
		buffer_size = 8;
	}

	void set_offset() {
		offset = (get_bit() << 2) + (get_bit() << 1) + get_bit();
	}

	bool get_bit() {
		if (buffer_size == 0)
			fill_buffer();
		char res = (bit_buffer >> (buffer_size-- - 1)) & 1;
		return res;
	}

	void check_end() {
		eof_flag = stream->eof() && (buffer_size == offset);
	}

public:
	ibstream(istream* stream) {
		if (stream == nullptr)
			throw invalid_argument("Null stream");
		this->stream = stream;
		lookahead_buffer = stream->get();
		set_offset();
	}

	bool eof() {
		return eof_flag;
	}

	bool read() {
		bool b = get_bit();
		check_end();
		return b;
	}

	char read_byte() {
		char res = 0;
		for (int i = 0; i < 8; i++) {
			res += read() << i;
		}
		return res;
	}

	friend bool operator>>(ibstream& os, bool &bit);
};

inline bool operator>>(ibstream& os, bool &bit)
{
	bool e = !os.eof();
	bit = os.read();
	return e;
}
