#include <iostream>
using namespace std;


void traverse(istream &in) {
	int node, child;
	in >> node >> child;
	if(cin.eof()) {
		return;
	}
	if(child != 0) {
		for(int i = 0; i < child; i++) {
			traverse(in);
		}
	}
	cout << node << " " << child << endl;
}



int main(void) {
	traverse(cin);
	return 0;
}
