#include <iostream>
#include <functional>
#include <map>


using namespace std;

class fa {
	streampos end;
	map<unsigned char, int> counts;
	int count;

	template <typename Functor>
	void reset_pos(Functor fun, istream* stream) {
		streampos pos = stream->tellg();
		fun();
		stream->seekg(pos);
	}

	void analyze_whole_stream(istream* stream) {
		reset_pos([this, stream]() -> void { analyze_interval(stream, stream->beg, end); }, stream);
	}

	void analyze_interval(istream* stream, streampos from, streampos to) {
		reset_pos([this, stream, from, to]() -> void { analyze(stream, from, to); }, stream);
	}

	void set_end(istream* stream) {
		streampos e;
		reset_pos([&e, stream]() -> void {
			stream->seekg(0, SEEK_END);
			e = stream->tellg();
			}, stream);
		end = e;
		end -= 1;
	}

	void analyze(istream* stream, streampos from, streampos to) {
		stream->seekg(from);
		while (stream->tellg() < end) {
			unsigned char c = stream->get();
			counts[c]++;
			count++;
		}
	}

public:
	fa(istream* stream, streampos from, streampos to) {
		if (stream == nullptr)
			throw invalid_argument("Null stream");
		if (from > end || to > end || from < stream->beg || to < stream->beg)
			throw invalid_argument("Invalid boundaries");
		set_end(stream);
		analyze_interval(stream, from, to);
	}

	fa(istream* stream) {
		if (stream == nullptr)
			throw invalid_argument("Null stream");
		set_end(stream);
		analyze_whole_stream(stream);
	}

	int get_count(unsigned char c) const {
		return counts.at(c);
	}

	double get_frequency(unsigned char c) const {
		return count ? ((double)counts.at(c)) / count : 0;
	}
};