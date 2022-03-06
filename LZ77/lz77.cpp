#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <fstream>
#include "obstream.cpp"
#include "ibstream.cpp"

using namespace std;

class lz77 {
	unsigned short buffer_size;
	unsigned short lookahead;
	int buffer_start;
	int buffer_end;
	int word_start;
	int word_end;
	int offset;
	int offset_bit_length;
	int match_bit_length;

	bool equals(string& input, int match_start, int match) {
		for (int i = 0; i < match; i++) {
			if (input[word_start + i] != input[match_start + i])
				return false;
		}
		return true;
	}

	bool find(string source, int* o) {
		*o = word_end - word_start + 1;
		while (*o <= buffer_end - buffer_start + 1) {
			if (equals(source, buffer_end - *o + 1, word_end - word_start + 1)) {
				return true;
			}
			(*o)++;
		}
		return false;
	}

	void initialize() {
		buffer_end = -1;
		buffer_start = 0;
		word_start = 0;
		word_end = 0;
		offset = 0;
	}

	void to_binary(int val, obstream* output, int length) {
		for (int i = 0; i < length; i++) {
			*output << (val % 2);
			val /= 2;
		}
	}

	int from_binary(ibstream* input, int length) {
		int res = 0;
		for (int i = 0; i < length; i++) {
			res += input->read() << i;
		}
		return res;
	}

	void encode_offset(int val, obstream* output) {
		to_binary(val, output, offset_bit_length);
	}

	int decode_offset(ibstream* input) {
		return from_binary(input, offset_bit_length);
	}

	void encode_match(int val, obstream* output) {
		to_binary(val, output, match_bit_length);
	}

	int decode_match(ibstream* input) {
		return from_binary(input, match_bit_length);
	}

	void encode_char(char val, obstream* output) {
		output->put_byte(val);
	}

	char decode_char(ibstream* input) {
		return input->read_byte();
	}

	void lzdecode(ibstream* input, ostream* output) {
		string str = "";
		while (!input->eof()) {
			int o = decode_offset(input);
			int match = decode_match(input);
			for (int i = 0; i < match; i++) {
				str += str[str.size() - o];
			}
			if (!input->eof()) {
				str += decode_char(input);
			}
		}
		*output << str;
	}

	void lzencode(istream* input, obstream* output) {
		string source(std::istreambuf_iterator<char>(*input), {});
		while (word_end < source.size()) {
			int o;
			if (word_end - word_start <= buffer_end - buffer_start && word_end - word_start < lookahead && find(source, &o))
				offset = o;
			else {
				encode_offset(offset, output);
				encode_match(word_end - word_start, output);
				encode_char(source[word_end], output);
				word_start = word_end + 1;
				offset = 0;
				buffer_end = word_end;
				buffer_start = max(0, buffer_end - buffer_size + 1);
			}
			word_end++;
		}
		if (word_end - word_start >= 1) {
			encode_offset(offset, output);
			encode_match(word_end - word_start, output);
		}
		output->flush();
	}

public:
	lz77(unsigned short buffer_size, unsigned short lookahead) {
		this->buffer_size = buffer_size;
		this->lookahead = lookahead;
		offset_bit_length = ceil(log2(buffer_size + 1));
		match_bit_length = ceil(log2(lookahead + 1));
	}

	void encode(istream* input, ostream* output) {
		initialize();
		obstream lzbitstream(output);
		lzencode(input, &lzbitstream);
	}

	void decode(istream* input, ostream* output) {
		initialize();
		ibstream lzibitstream(input);
		lzdecode(&lzibitstream, output);
	}
};