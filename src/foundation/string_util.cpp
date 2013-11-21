#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>
#include <string>
#include <sstream>
#include <vector>

#include <string.h>

using std::string;
using std::vector;
using std::stringstream;


string & ltrim(string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
					std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

char const * ltrim(char const * s, char const * end) {
	if (!s) return nullptr;
	if (end == nullptr) {
		end = strchr(s, 0);
	}
	while (s < end && isspace(*s)) {
		++s;
	}
	return s;
}

string & rtrim(string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
					std::not1(std::ptr_fun<int, int>(std::isspace))).base(),
							s.end());
	return s;
}

string & trim(string &s) {
	return ltrim(rtrim(s));
}

vector<string> & split(string const & s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> & split(char const * s, char delim, vector<string> &elems) {
	if (s) {
		while (true) {
			if (*s == 0) {
				break;
			}
			char const * next = strchr(s, delim);
			if (next) {
				elems.push_back(string(s, next));
				s = next + 1;
			} else {
				elems.push_back(s);
				break;
			}
		}

	}
	return elems;
}

vector<string> split(string const & s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

vector<string> split(char const * s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

void parse_exec_hosts(string const &exec_hosts,
		vector<string> & exec_host_vector) {

	vector<string> tmp_list;
	split(exec_hosts, '+', tmp_list);

	for (size_t i = 0; i < tmp_list.size(); i++) {
		string tmp_host = tmp_list[i];
		size_t pos = tmp_host.find("/");

		if (pos != string::npos)
			tmp_host.erase(pos);

		exec_host_vector.push_back(tmp_host);
	}
}
