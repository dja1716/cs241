#include <iostream>
#include <vector>
#include <map>
#include <sstream>
using namespace std;


class Tree {
public:
	int nodeValue;
	int numberOfChildren;
	vector<Tree> children;

};



Tree traverse(Tree preTree, istream &in) {
	int node, numberOfChildren;
	in >> node >> numberOfChildren;

	

	preTree.nodeValue = node;
	preTree.numberOfChildren = numberOfChildren;
	
	for (int i = 0; i < numberOfChildren; i++) {
		Tree nextTree;
		preTree.children.push_back(traverse(nextTree, cin));
	}
	



	return preTree;
}



void traverseTree(Tree tree) {
	for (int i = 0; i < tree.children.size(); i++) {
		traverseTree(tree.children.at(i));
	}

	cout << tree.nodeValue << " " << tree.numberOfChildren << endl;
}




int main() {
	Tree myTree;
	string line;
	
	myTree = traverse(myTree, cin);

	traverseTree(myTree);

}
