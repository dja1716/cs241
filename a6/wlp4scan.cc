#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <sstream>



using std::vector;
using std::string;
using std::pair;
using std::map;
using std::cin;
using std::cout;
using std::endl;
using std::make_pair;

typedef vector<pair<string, string>> tokenTable;

int main() {
	tokenTable tokens;
	string line;
	while (getline(cin, line)) {
		int tempInt = 0;
		bool tempBool = true;
		for (int i = 0; i < line.length(); i++) {
			if (tempBool && line[i] == '/') {
				if (line.length() > i + 1 || line[i+1] == '/') {
					tempBool = false;

					if (i > 1) {
						tokens.push_back(make_pair(line.substr(tempInt, i - tempInt), line.substr(tempInt, i - tempInt)));
					}
				}
			}

			if (tempBool) {
				if (line[i] == ' ') {
					tokens.push_back(make_pair(line.substr(tempInt, i - tempInt), line.substr(tempInt, i - tempInt)));
					tempInt = i + 1;
				}
				else if (i == line.length() - 1) {
					tokens.push_back(make_pair(line.substr(tempInt, line.length() - tempInt), line.substr(tempInt, line.length() - tempInt)));
				}
			}
			
		}
	}
	for (tokenTable::const_iterator it = tokens.begin(); it != tokens.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}

	return 0;
}
