#pragma once

#include <iostream>
#include <vector>

using namespace std;

class obstream {
	ostream* stream;
	char bit_buffer = 0;
	char buffer_size = 0;
	char first_byte = 0;
	bool is_first_byte = true;

	void clear_buffer() {
		if (is_first_byte) {
			is_first_byte = false;
			first_byte = bit_buffer;
		}
		else
			stream->put(bit_buffer);
		buffer_size = 0;
		bit_buffer = 0;
	}

	void set_offset() {
		buffer_size = 3;
	}

public:
	obstream(ostream* stream) {
		if (stream == nullptr)
			throw invalid_argument("Null stream");
		this->stream = stream;
		set_offset();
		stream->put((char)0);
	}

	obstream& put(const char bit) {
		return put(bit != 0);
	}

	obstream& put(const int bit) {
		return put(bit != 0);
	}

	obstream& put(const bool bit) {
	    bit_buffer |= bit << (7 - buffer_size);
		buffer_size++;
		if (buffer_size == 8)
			clear_buffer();
		return *this;
	}

	obstream& put_byte(char byte) {
		for (int i = 0; i < 8; i++) {
			put(byte & 1);
			byte >>= 1;
		}
		return *this;
	}

	obstream& put(vector<bool> arr) {
		for (bool i : arr)
			put(i);
		return *this;
	}

	void flush() {
		int rest = 8 - buffer_size;
		if (buffer_size != 0)
			clear_buffer();
		char encoded_rest = rest << 5;
		stream->seekp(stream->beg, SEEK_SET);
		stream->put(first_byte | encoded_rest);
	}

	friend obstream& operator<<(obstream& os, const char bit);
	friend obstream& operator<<(obstream& os, const bool bit);
	friend obstream& operator<<(obstream& os, const int bit);
	friend obstream& operator<<(obstream& os, const vector<bool> arr);
};

inline obstream& operator<< (obstream& os, const char bit) {
	return os.put(bit);
}

inline obstream& operator<< (obstream& os, const bool bit) {
	return os.put(bit);
}

inline obstream& operator<< (obstream& os, const int bit) {
	return os.put(bit);
}

inline obstream& operator<< (obstream& os, const vector<bool> arr) {
	return os.put(arr);
}