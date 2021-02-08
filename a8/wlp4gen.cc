#include <iostream>
#include <vector>
#include <map>
#include <sstream>
using namespace std;
// TODO LIST: memory leak
class Tree {
public:
	string rule; // ex. expr expr PLUS term
	vector<string> tokens; // from scanner
	vector<Tree*> children;

};

typedef map<string, pair<string, int>> symbolTable;
typedef pair<vector<string>, symbolTable> signatureTable;
typedef map<string, signatureTable> topSymbolTable;

string curProc = "empty";
string terminal[] = { "BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID", "IF", "INT", "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS", "PRINTLN", "RBRACE", "RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", "RBRACK", "NEW", "DELETE", "NULL" };

string errorMessage;
bool error = false;
topSymbolTable topSymT;
symbolTable symT;
// list of tokens
int offset = 0;

int loopCounter = 0;
int ifCounter = 0;


Tree *buildT(string rule) {
	
	bool tempBool = false;
	Tree* parseTree = new Tree();
	string tempString;
	istringstream iss(rule);

	// rule
	(*parseTree).rule = rule;


	// tokens
	while (iss >> tempString) {
		(*parseTree).tokens.push_back(tempString);
	}

	tempString = (*parseTree).tokens[0];

	for (int i = 0; i < 35; i++) {
		if (tempString == terminal[i]) {
			tempBool = true;
		}
	}

	// child
	if (!tempBool) {
		for (int i = 0; i < (*parseTree).tokens.size() - 1; i++) {
			string line;
			getline(cin, line);
			(*parseTree).children.push_back(buildT(line));
		}
	}

	return parseTree;
	

}

void deleteParseTree(Tree* parseTree) {

	for (Tree* child : parseTree->children) {
		deleteParseTree(child);
	}
	delete parseTree;
}


void printParseTree(Tree* parseTree) {


	cout << parseTree->rule << endl;
	



	for (Tree* child : parseTree->children) {
		printParseTree(child);
	}
}

//void typeOf(Tree& tree) {
//	for (Tree& child : tree->children) {
//		typeOf(child);
//	}
//}

// modify error bool when type error confronted
//string typeChecking(Tree* t) {
//	if (t->tokens[0] == "ID") {
//		return topSymT[curProc].second[t->tokens[1]];
//
//	}
//	else if (t->tokens[0] == "NUM") {
//		return "int";
//	}
//	else if (t->tokens[0] == "NULL") {
//		return "int*";
//	}
//	else if (t->rule == "factor ID") {
//		return typeChecking(t->children[0]);
//	}
//	else if (t->rule == "factor NUM") {
//		return "int";
//	}
//	else if (t->rule == "factor NULL") {
//		return "int*";
//	}
//	else if (t->rule == "lvalue ID" || t->rule == "expr term" || t->rule == "term factor" || t->rule == "arglist expr") {
//		return typeChecking(t->children[0]);
//	}
//	else if (t->rule == "lvalue LPAREN lvalue RPAREN" || t->rule == "factor LPAREN expr RPAREN") {
//		return typeChecking(t->children[1]);
//	}
//	else if (t->rule == "lvalue STAR factor" || t->rule == "factor STAR factor") {
//		if (typeChecking(t->children[1]) == "int*") {
//			return "int";
//		}
//		else {
//			error = true;
//			return "error";
//		}
//	}
//	else if (t->rule == "factor AMP lvalue") {
//		if (typeChecking(t->children[1]) == "int") {
//			return "int*";
//		}
//		else {
//			error = true;
//			return "error";
//		}
//
//	}
//	else if (t->rule == "dcls dcls dcl BECOMES NUM SEMI") {
//		string ltype = typeChecking(t->children[1]);
//		string rtype = "int";
//
//		if (ltype != rtype) {
//			error = true;
//			return "error";
//		}
//		else {
//			string garbage = typeChecking(t->children[0]);
//			return "int";
//		}
//	}
//	else if (t->rule == "dcls") {
//		return "nothing";
//	}
//	else if (t->rule == "dcls dcls dcl BECOMES NULL SEMI") {
//		string ltype = typeChecking(t->children[1]);
//		string rtype = "int*";
//
//		if (ltype != rtype) {
//			error = true;
//			return "error";
//		}
//		else {
//			
//			string garbage = typeChecking(t->children[0]);
//			return "int*";
//		}
//	}
//	else if (t->rule == "dcl type ID") {
//		return typeChecking(t->children[1]);
//	}
//	else if (t->rule == "factor ID LPAREN RPAREN") {
//		return "int";
//	}
//	else if (t->rule == "arglist expr COMMA arglist") {
//		return typeChecking(t->children[0]);
//	}
//	else if (t->rule == "factor ID LPAREN arglist RPAREN") {
//		Tree* tempT = t;
//
//		for (vector<string>::const_iterator sigs = topSymT[curProc].first.begin(); sigs != topSymT[curProc].first.end(); sigs++) {
//			//cerr << " " << *sigs;
//			tempT = t->children[2];
//			if (typeChecking(tempT) != *sigs) {
//				error = true;
//				errorMessage = "typeChecking factor ID LPAREN arglist";
//				return "error";
//			}
//		}
//		if (tempT->rule == "arglist expr COMMA arglist") {
//			error = true;
//			errorMessage = "typeChecking factor ID LPAREN arglist // extra arg";
//			return "error";
//		}
//
//		return "int";
//	}
//	else if (t->rule == "params") {
//		return "nothing";
//	}
//	else if (t->rule == "params paramlist" || t->rule == "paramlist dcl" || t->rule == "paramlist dcl COMMA paramlist") {
//		return typeChecking(t->children[0]);
//	}
//	else if (t->rule == "factor NEW INT LBRACK expr RBRACK") {
//		string exType = typeChecking(t->children[3]);
//		if (exType != "int") {
//			error = true;
//			return "error";
//		}
//		else {
//			return "int*";
//		}
//	}
//	else if (t->rule == "term term STAR factor" || t->rule == "term term SLASH factor" || t->rule == "term term PCT factor") {
//		string ltype = typeChecking(t->children[0]);
//		string rtype = typeChecking(t->children[2]);
//		if (ltype == "int" && rtype == "int") {
//			return "int";
//		}
//		else {
//			error = true;
//			return "error";
//		}
//	}
//	else if (t->rule == "expr expr PLUS term") {
//		string ltype = typeChecking(t->children[0]);
//		string rtype = typeChecking(t->children[2]);
//
//		if (ltype == "int" && rtype == "int") {
//			return "int";
//		}
//		else if (ltype == "int*" && rtype == "int") {
//			return "int*";
//		}
//		else if (ltype == "int" && rtype == "int*") {
//			return "int*";
//		}
//		else {
//			error = true;
//			return "error";
//		}
//	}
//	else if (t->rule == "expr expr MINUS term") {
//		string ltype = typeChecking(t->children[0]);
//		string rtype = typeChecking(t->children[2]);
//		if (ltype == "int" && rtype == "int") {
//			return "int";
//		}
//		else if (ltype == "int*" && rtype == "int") {
//			return "int*";
//		}
//		else if (ltype == "int*" && rtype == "int*") {
//			return "int";
//		}
//		else {
//			error = true;
//			return "error";
//		}
//	}
//}


// TODO: check error case foo2 error detecting not working
void buildSymbolTable(Tree* t) {
	

	/*if (rule == "procedures main") {
		cout << "main WAIN found" << endl;
	}*/
	if (t->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		curProc = t->children[1]->tokens[1];
		if (topSymT.find(curProc) == topSymT.end()) {
			
			topSymT.insert(make_pair(curProc, signatureTable()));
		}
		else {
			error = true;
		}
	}

	if (t->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		//cout << "WAIN FOUND" << endl;

		if (topSymT.find("wain") == topSymT.end()) {
			// wain for the first time
			curProc = "wain";

			topSymT.insert(make_pair(curProc, signatureTable()));

			if (t->children[3]->children[0]->rule == "type INT") {
				topSymT[curProc].first.push_back("int");
			}
			else {
				topSymT[curProc].first.push_back("int*");
			}

			
			topSymT[curProc].first.push_back("int");
				
		}
		else {
			error = true;
		}
	}

	if (t->rule == "paramlist dcl" || t->rule == "paramlist dcl COMMA paramlist") {
		if (t->children[0]->children[0]->rule == "type INT") {
			topSymT[curProc].first.push_back("int");
		}
		else {
			topSymT[curProc].first.push_back("int*");
		}
	}




	if (t->rule == "factor ID") {
		if (topSymT[curProc].second.find(t->children[0]->tokens[1]) == topSymT[curProc].second.end()) {
			error = true;
			

		}
	}

	if (t->rule == "dcl type ID") {
		//cout << "ID Declaration" << endl;
		if (topSymT[curProc].second.find(t->children[1]->tokens[1]) != topSymT[curProc].second.end()) {
			error = true;
		}
		else {

			if (t->children[0]->rule == "type INT") {
				topSymT[curProc].second.insert(make_pair(t->children[1]->tokens[1], make_pair("int", offset)));
			}
			else {
				topSymT[curProc].second.insert(make_pair(t->children[1]->tokens[1], make_pair( "int*", offset)));
			}

			offset -= 4;
		}
		
	}

	if (t->rule == "factor ID LPAREN arglist RPAREN" || t->rule == "factor ID LPAREN RPAREN") {
		if (topSymT.find(t->children[0]->tokens[1]) == topSymT.end()) {
			error = true;
		}
		if (topSymT[curProc].second.find(t->children[0]->tokens[1]) != topSymT[curProc].second.end()) {
			error = true;
		}
	}


	for (Tree* child : t->children) {
		buildSymbolTable(child);
	}
}


//void typeCheckingMain(Tree* t) {
//	if (t->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
//		curProc = t->children[1]->tokens[1];
//
//		// check params
//		int z = 0;
//		for (vector<string>::const_iterator sigs = topSymT[curProc].first.begin(); sigs != topSymT[curProc].first.end(); sigs++) {
//			z++;
//		}
//
//		Tree* tempT = t->children[3];
//		if (z == 0) {
//			if (tempT->rule != "params") {
//				error = true;
//			}
//
//		}
//		else {
//
//			for (vector<string>::const_iterator sigs = topSymT[curProc].first.begin(); sigs != topSymT[curProc].first.end(); sigs++) {
//				cerr << " " << *sigs;
//
//				tempT = t->children[0];
//
//				if (typeChecking(tempT) != *sigs) {
//					error = true;
//					errorMessage = "proc dcl type error";
//				}
//			}
//			if (tempT->rule != "paramlist dcl") {
//				error = true;
//				errorMessage = "proc param num diff";
//			}
//		}
//
//		// check dcls
//		string garbage1 = typeChecking(t->children[6]);
//		// check statements
//		// check expr
//		if (typeChecking(t->children[9]) != "int") {
//			error = true;
//			errorMessage = "procedure type error";
//		}
//	}
//
//	if (t->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
//
//		curProc = "wain";
//
//
//		Tree* tempT = t;
//		// check first dcl
//		// check second dcl
//		//int z = 0;
//		//for (vector<string>::const_iterator sigs = topSymT[curProc].first.begin(); sigs != topSymT[curProc].first.end(); sigs++) {
//		//	//cerr << " " << *sigs;
//		//	if (z == 0) {
//		//		z++;
//		//		tempT = t->children[3];
//		//	}
//		//	else {
//		//		tempT = t->children[5];
//		//	}
//
//		//	if (typeChecking(tempT) != *sigs) {
//		//		error = true;
//		//		errorMessage = "main dcl type error";
//		//	}
//		//}
//		// check dcls
//		string garbage1 = typeChecking(t->children[8]);
//		// check statements
//
//		// check expr
//		if (typeChecking(t->children[11]) != "int") {
//			error = true;
//			errorMessage = "Main typeChecking Error";
//		}
//	}
//
//	for (Tree* child : t->children) {
//		typeCheckingMain(child);
//	}
//}

// i is register#
void pop(int i) {
	cout << "add $30, $30, $4" << endl;
	cout << "lw $" << i << ", -4($30)" << endl;

}

void push(int i) {
	cout << "sw $" << i << ", -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
}

void code(int i) {
	cout << "lw $3, " << i << "($29)" << endl;
}

void code(string s) {
	cout << "lw $3, " << s << "($29)" << endl;
}


void CodeGen(Tree* parseTree) {
	if (parseTree->rule == "start BOF procedures EOF") {
		CodeGen(parseTree->children[1]);
	}
	else if (parseTree->rule == "procedures main") {
		CodeGen(parseTree->children[0]);
	}
	else if (parseTree->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
		curProc = "wain";
		CodeGen(parseTree->children[3]);
		CodeGen(parseTree->children[5]);
		CodeGen(parseTree->children[8]);
		CodeGen(parseTree->children[9]);
		CodeGen(parseTree->children[11]);
		
	}
	else if (parseTree->rule == "dcls") {
		// nothing
	}
	else if (parseTree->rule == "statements") {
		// nothing
	}
	else if (parseTree->rule == "type INT") {
		CodeGen(parseTree->children[0]);
	}
	else if (parseTree->rule == "expr term") {
		CodeGen(parseTree->children[0]);
	}
	else if (parseTree->rule == "term factor") {
		CodeGen(parseTree->children[0]);
	}
	else if (parseTree->rule == "factor ID") {
		CodeGen(parseTree->children[0]);
	}
	else if (parseTree->tokens[0] == "ID") {
		int temp = topSymT[curProc].second[parseTree->tokens[1]].second;
		code(temp);
	}
	else if (parseTree->rule == "factor LPAREN expr RPAREN") {
		CodeGen(parseTree->children[1]);
	}
	else if (parseTree->rule == "expr expr PLUS term") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		cout << "add $3, $5, $3" << endl;
	}
	else if (parseTree->rule == "expr expr MINUS term") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		cout << "sub $3, $5, $3" << endl;
	}
	else if (parseTree->rule == "term term STAR factor") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		cout << "mult $3, $5" << endl;
		cout << "mflo $3" << endl;
	}
	else if (parseTree->rule == "term term SLASH factor") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		cout << "div $5, $3" << endl;
		cout << "mflo $3" << endl;
	}
	else if (parseTree->rule == "term term PCT factor") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		cout << "div $5, $3" << endl;
		cout << "mfhi $3" << endl;
	}
	else if (parseTree->rule == "factor NUM") {
		CodeGen(parseTree->children[0]);
	}
	else if (parseTree->tokens[0] == "NUM") {
		cout << "lis $3" << endl;
		cout << ".word " << parseTree->tokens[1] << endl;
	}
	else if (parseTree->rule == "statements statements statement") {
		CodeGen(parseTree->children[0]);
		CodeGen(parseTree->children[1]);
	}
	else if (parseTree->rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
		cout << "add $15, $1, $0" << endl;
		CodeGen(parseTree->children[2]);
		cout << "add $1, $3, $0" << endl;
		push(31);
		cout << "lis $10" << endl;
		cout << ".word print" << endl << "jalr $10" << endl;
		pop(31);
		cout << "add $1, $15, $0" << endl;
	}
	else if (parseTree->rule == "dcls dcls dcl BECOMES NUM SEMI") {
		CodeGen(parseTree->children[0]);
		CodeGen(parseTree->children[1]);
		cout << "lis $3" << endl;
		cout << ".word " << parseTree->children[3]->tokens[1] << endl;
		int temp = topSymT[curProc].second[parseTree->children[1]->children[1]->tokens[1]].second;
		cout << "sw $3, " << temp << "($29)" << endl;

		cout << "sub $30, $30, $4" << endl;
	}
	else if (parseTree->rule == "dcls dcls dcl BECOMES NULL SEMI") {
		CodeGen(parseTree->children[0]);
		CodeGen(parseTree->children[1]);

		cout << "add $3, $11, $0" << endl;
		int temp = topSymT[curProc].second[parseTree->children[1]->children[1]->tokens[1]].second;
		cout << "sw $3, " << temp << "($29)" << endl;

		cout << "sub $30, $30, $4" << endl;
	}
	else if (parseTree->rule == "statement lvalue BECOMES expr SEMI") {
		if (parseTree->children[0]->rule == "lvalue STAR factor") {
			CodeGen(parseTree->children[0]);
			cout << "add $12, $3, $0" << endl;
			CodeGen(parseTree->children[2]);
			cout << "sw $3, 0($12)" << endl;
		}
		else {
			CodeGen(parseTree->children[2]);
			int temp = topSymT[curProc].second[parseTree->children[0]->children[0]->tokens[1]].second;
			cout << "sw $3, " << temp << "($29)" << endl;
		}
		
	}
	else if (parseTree->rule == "lvalue LPAREN lvalue RPAREN") {
		CodeGen(parseTree->children[1]);
	}

	else if (parseTree->rule == "test expr LT expr") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		cout << "slt $3, $5, $3" << endl;
	}
	else if (parseTree->rule == "test expr EQ expr") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		// maybe store $6 and $7 if used
		cout << "slt $6, $3, $5" << endl;
		cout << "slt $7, $5, $3" << endl;
		cout << "add $3, $6, $7" << endl;
		cout << "sub $3, $11, $3" << endl;
	}
	else if (parseTree->rule == "test expr NE expr") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		// maybe store $6 and $7 if used
		cout << "slt $6, $3, $5" << endl;
		cout << "slt $7, $5, $3" << endl;
		cout << "add $3, $6, $7" << endl;
	}
	else if (parseTree->rule == "test expr LE expr") {
		CodeGen(parseTree->children[2]);
		push(3);
		CodeGen(parseTree->children[0]);
		pop(5);
		cout << "slt $3, $5, $3" << endl;
		cout << "sub $3, $11, $3" << endl;
	}
	else if (parseTree->rule == "test expr GE expr") {
		CodeGen(parseTree->children[0]);
		push(3);
		CodeGen(parseTree->children[2]);
		pop(5);
		cout << "slt $3, $5, $3" << endl;
		cout << "sub $3, $11, $3" << endl;
	}
	else if (parseTree->rule == "test expr GT expr") {
		CodeGen(parseTree->children[2]);
		push(3);
		CodeGen(parseTree->children[0]);
		pop(5);
		cout << "slt $3, $5, $3" << endl;

	}
	else if (parseTree->rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
		int temp = loopCounter;
		loopCounter++;
		cout << "loop" << temp << ": ";
		CodeGen(parseTree->children[2]);
		cout << "beq $3, $0, endWhile" << temp << endl;
		CodeGen(parseTree->children[5]);
		cout << "beq $0, $0, loop" << temp << endl;
		cout << "endWhile" << temp << ": " << endl;

	}
	else if (parseTree->rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
		int temp = ifCounter;
		ifCounter++;
		CodeGen(parseTree->children[2]);
		cout << "beq $3, $0, else" << temp << endl;
		CodeGen(parseTree->children[5]);
		cout << "beq $0, $0, endif" << temp << endl;
		cout << "else" << temp << ": ";
		CodeGen(parseTree->children[9]);
		cout << "endif" << temp << ": ";

	}
	else if (parseTree->rule == "factor NULL") {
		cout << "add $3, $11, $0; recall $11 is always 1" << endl;

	}
	else if (parseTree->rule == "factor STAR factor") {
		CodeGen(parseTree->children[1]);
		cout << "lw $3, 0($3)" << endl;
	}
	else if (parseTree->rule == "lvalue STAR factor") {
		CodeGen(parseTree->children[1]);
		//cout << "lw $3, 0($3)" << endl;
	}
	else if (parseTree->rule == "factor AMP lvalue") {
		if (parseTree->children[1]->rule == "lvalue ID") {
			int temp = topSymT[curProc].second[parseTree->children[1]->children[0]->tokens[1]].second;
			cout << "lis $3" << endl;
			cout << ".word " << temp << endl;
			cout << "add $3, $29, $3" << endl;
		}
		else {
			CodeGen(parseTree->children[1]->children[1]);
		}
	}
	else if (parseTree->rule == "lvalue ID") {
		CodeGen(parseTree->children[0]);
	}
	else if (parseTree->rule == "factor NEW INT LBRACK expr RBRACK") {
		CodeGen(parseTree->children[3]);
		cout << "add $1, $3, $0" << endl;
		push(31);
		cout << "lis $7" << endl;
		cout << ".word new" << endl;
		cout << "jalr $7" << endl;
		pop(31);
		cout << "bne $3, $0, 1" << endl;
		cout << "add $3, $11, $0" << endl;
	}
	else if (parseTree->rule == "statement DELETE LBRACK RBRACK expr SEMI") {
		CodeGen(parseTree->children[3]);
		cout << "beq $3, $11, skipDelte" << endl;
		cout << "add $1, $3, $0" << endl;
		push(31);
		cout << "lis $7" << endl;
		cout << ".word delete" << endl;
		cout << "jarl $7" << endl;
		pop(31);
		cout << "skipDelte: " << endl;

	}
}



// parseTree is built
// build symboltable

int main() {

	

	Tree *parseTree;
	string line;
	getline(cin, line);
	parseTree = buildT(line);

	//printParseTree(parseTree);
	buildSymbolTable(parseTree);
	//typeCheckingMain(parseTree);


	cout << ".import init" << endl;
	cout << ".import print" << endl;
	cout << ".import new" << endl;
	cout << ".import delete" << endl;
	cout << "lis $11" << "; for true" << endl;
	cout << ".word 1" << endl;
	cout << "lis $4" << endl;
	cout << ".word 4" << endl;
	cout << "sub $29, $30, $4" << endl;
	cout << "sw $1, -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;
	cout << "sw $2, -4($30)" << endl;
	cout << "sub $30, $30, $4" << endl;

	if (topSymT["wain"].first.front() == "int") {
		cout << "add $2, $0, $0" << endl;
	}
	push(31);
	cout << "lis $7" << endl;
	cout << ".word init" << endl;
	cout << "jalr $7" << endl;
	pop(31);


	CodeGen(parseTree);
	cout << "add $30, $30, $4" << endl;
	cout << "add $30, $30, $4" << endl;
	cout << "jr $31" << endl;


	//if (error) {
	//	cerr << "ERROR" << endl;
	//	//cerr << errorMessage << endl;
	//}
	//else {
	//	/*for (topSymbolTable::const_iterator procedure = topSymT.begin(); procedure != topSymT.end(); procedure++) {
	//		if (procedure != topSymT.begin()) {
	//			cerr << endl;
	//		}

	//		cerr << procedure->first;

	//		for (vector<string>::const_iterator sigs = procedure->second.first.begin(); sigs != procedure->second.first.end(); sigs++) {
	//			cerr << " " << *sigs;
	//		}
	//		cerr << endl;
	//		if (procedure->first == "wain") {
	//			for (symbolTable::const_iterator variable = procedure->second.second.begin(); variable != procedure->second.second.end(); variable++) {
	//				cerr << variable->first << " " << variable->second << endl;
	//			}
	//		}	
	//	}*/
	//}

	deleteParseTree(parseTree);


}
