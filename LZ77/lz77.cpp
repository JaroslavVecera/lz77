#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class lz77 {
	unsigned short buffer_size;
	unsigned short lookahead;
	// asi word_start == word_end;
	int buffer_start = 0;
	int buffer_end = 0;
	int word_start = 0;
	int word_end = 0;
	int min_len;
	int match = 1;

	bool starts_in_buffer(string& input, int* end_len, int* match) {
		for (*end_len = 1; *end_len <= buffer_end - buffer_start; (*end_len)++) {
			if (input[word_start] != input[buffer_end - (*end_len)])
				continue;
			int match_start = buffer_end - (*end_len);
			*match = 1;
			while (input[word_start + *match] == input[match_start + *match] && *match < word_end - word_start)
				(*match)++;
			return true;
		}
		return false;
	}

public:
	lz77(unsigned short buffer_size, unsigned short lookahead) {
		this->buffer_size = buffer_size;
		this->lookahead = lookahead;
		min_len = 0;
	}

	void encode(istream* input, ostream* output) {
		std::string source(std::istreambuf_iterator<char>(*input), {});
		do {
			word_end++;
			bool len_is_in_lookahead = word_end - word_start < lookahead;
			int end_len, m = 0;
			if (len_is_in_lookahead && starts_in_buffer(source, &end_len, &m)) {
				min_len = end_len;
				match = m;
			}
			else {
				cout << min_len << ", " << match << ", " << source[word_end - 1] << endl;
				min_len = 0;
				buffer_end += match;
				if (buffer_end - buffer_start >= buffer_size)
					buffer_start = buffer_end - buffer_size;
				word_start = word_end;
			}
		} while (word_end < source.size());
	}
};