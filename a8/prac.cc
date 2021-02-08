// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

class Tree {
public:
    string rule; //eg. expr expr PLUS term
    vector<string> tokens;
    vector<Tree> children;
};

typedef map<string, string> symbolTable; // name -> type
typedef pair<vector<string>, symbolTable> signatureTable;
typedef map<string, pair<vector<string>, map<string, string>>> topSymbolTable;

// pro tip
string curProc = "empty";
topSymbolTable topSymT;
symbolTable symT;
bool error = false;

// q1 ~
void MakeSymbolTable(Tree parseTree) {
    for (vector<Tree>::const_iterator it = parseTree.children.begin(); it != parseTree.children.end(); ++it) {



    

        if (it->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            if (topSymT.find("wain") == topSymT.end()) {
                // WAIN first time
                curProc = "wain";
                topSymT.insert(make_pair(curProc, signatureTable()));


                if (it->children[3].children[0].rule == "type INT") {
                    topSymT[curProc].first.push_back(it->children[3].children[0].children[0].tokens[1]);
                }
                else if (it->children[3].children[0].rule == "type INT STAR") {
                    topSymT[curProc].first.push_back(it->children[3].children[0].children[0].tokens[1] + it->children[3].children[1].tokens[1]);
                }

                if (it->children[5].children[0].rule == "type INT") {
                    topSymT[curProc].first.push_back(it->children[5].children[0].children[0].tokens[1]);
                }
                else if (it->children[5].children[0].rule == "type INT STAR") {
                    topSymT[curProc].first.push_back(it->children[5].children[0].children[0].tokens[1] + it->children[5].children[1].tokens[1]);
                }
            }
            else {
                cerr << "ERROR: multiple wain" << endl;
                error = true;
            }
            
            


        }
        else if (it->rule == "dcl type ID") { // for q1
            if (topSymT[curProc].second.find(it->children[1].tokens[1]) == topSymT[curProc].second.end()) {
                if (it->children[0].rule == "type INT") {
                    topSymT[curProc].second.insert(make_pair(it->children[1].tokens[1], it->children[0].children[0].tokens[1]));
                }
                else if (it->children[0].rule == "type INT STAR") {
                    //cout << "INT STAR" << endl;
                    topSymT[curProc].second.insert(make_pair(it->children[1].tokens[1], it->children[0].children[0].tokens[1] + it->children[0].children[1].tokens[1]));
                }
            }
            else {
                cerr << "ERROR: variable multiple declaration" << endl;
                error = true;
            }

        }
        MakeSymbolTable(*it);
    }
    
}
// for testing purpose
void PrintParseTree(Tree parseTree) {
    for (vector<Tree>::const_iterator it = parseTree.children.begin(); it != parseTree.children.end(); ++it) {
        cout << (*it).rule << endl;
        PrintParseTree(*it);
    }
}

bool CheckTerminal(string word) {
    string terminals[] = { "BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID", "IF", "INT", "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS", "PRINTLN", "RBRACE", "RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", "RBRACK", "NEW", "DELETE", "NULL" };
    for (int i = 0; i < 35; i++) {
        if (word == terminals[i]) return true;
    }
    return false;
}


Tree BuildNode(string rule) {
    Tree parseNode;
    parseNode.rule = rule;
    
    istringstream iss(rule);
    string word;

    while (iss >> word) {
        parseNode.tokens.push_back(word);
    }
    if (CheckTerminal(parseNode.tokens[0]) == false) {
        for (int i = 1; i < parseNode.tokens.size(); ++i) {
            string line;
            getline(cin, line);
            parseNode.children.push_back(BuildNode(line));
        }
    }
    return parseNode;
}



// TODO
// make a symbol table for identifiers
// error check
// 1. multiple declaration of identifiers
// 2. 


// undeclared
void CheckUndeclaredVariable(Tree parseTree) {
    for (vector<Tree>::const_iterator it = parseTree.children.begin(); it != parseTree.children.end(); it++) {
        if (it->rule == "factor ID") {
            //cerr << "TESTING: " << it->children[0].tokens[1] << endl;

            for (map<string, signatureTable>::const_iterator jt = topSymT.begin(); jt != topSymT.end(); jt++) {
                if (topSymT[curProc].second.find(it->children[0].tokens[1]) == topSymT[curProc].second.end()) {
                    cerr << "ERROR: undeclared vairables" << endl;
                    error = true;
                }
            }
        }

        CheckUndeclaredVariable(*it);
    }
}

int main()
{
    Tree parseTree;
    string line;
    getline(cin, line);
    parseTree = BuildNode(line);
    MakeSymbolTable(parseTree);
    //PrintParseTree(parseTree);

    // check for undeclared variables
    CheckUndeclaredVariable(parseTree);

    
    
    if (error == false) {
        for (map<string, signatureTable>::const_iterator it = topSymT.begin(); it != topSymT.end(); ++it) {
            cerr << it->first;
            //for (vector<string>::const_iterator jt = it->second.first.begin(); jt != it->second.first.end(); jt++) {
            //    cerr << " " << *jt;
            //}
            cerr << endl;

            if (it->first == "wain") {
                //cerr << "wain" << endl;
                for (symbolTable::const_iterator jt = it->second.second.begin(); jt != it->second.second.end(); jt++) {
                    cerr << jt->first << " " << jt->second << endl;
                }
            }
        }
    }
    

    return 0;
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

