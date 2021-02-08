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

        // procedure other than wain
        if(it->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            if (topSymT.find(it->children[1].tokens[1]) == topSymT.end()) {
                curProc = it->children[1].tokens[1];
                topSymT.insert(make_pair(curProc, signatureTable()));
            }
            else {
                cerr << "ERROR: procedure with identical name already exist";
                error = true;

            }
        } else if (it->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            if (topSymT.find("wain") == topSymT.end()) {
                // WAIN first time
                curProc = "wain";
                topSymT.insert(make_pair(curProc, signatureTable()));


                if (it->children[3].children[0].rule == "type INT") {
                    topSymT[curProc].first.push_back(it->children[3].children[0].children[0].tokens[1]);
                }
                else if (it->children[3].children[0].rule == "type INT STAR") {
                    topSymT[curProc].first.push_back(it->children[3].children[0].children[0].tokens[1] + it->children[3].children[0].children[1].tokens[1]);
                }

                if (it->children[5].children[0].rule == "type INT") {
                    topSymT[curProc].first.push_back(it->children[5].children[0].children[0].tokens[1]);
                }
                else if (it->children[5].children[0].rule == "type INT STAR") {
                    topSymT[curProc].first.push_back(it->children[5].children[0].children[0].tokens[1] + it->children[5].children[0].children[1].tokens[1]);
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

        } // signature checking
        else if (it->rule == "paramlist dcl COMMA paramlist") {
            //cerr << "TESTING SIG CALLED" << endl;
            if (it->children[0].children[0].rule == "type INT") {
                topSymT[curProc].first.push_back(it->children[0].children[0].children[0].tokens[1]);
            }
            else if (it->children[0].children[0].rule == "type INT STAR") {
                topSymT[curProc].first.push_back(it->children[0].children[0].children[0].tokens[1] + it->children[0].children[0].children[1].tokens[1]);
            }
        }
        else if (it->rule == "paramlist dcl") {
            //cerr << "TESTING SIG CALLED" << endl;
            if (it->children[0].children[0].rule == "type INT") {
                topSymT[curProc].first.push_back(it->children[0].children[0].children[0].tokens[1]);
            }
            else if (it->children[0].children[0].rule == "type INT STAR") {
                topSymT[curProc].first.push_back(it->children[0].children[0].children[0].tokens[1] + it->children[0].children[0].children[1].tokens[1]);
            }
        } // function delcation check
        else if (it->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            if (topSymT.find(it->children[1].tokens[1]) == topSymT.end()) {
                curProc = it->children[1].tokens[1];

                topSymT.insert(make_pair(curProc, signatureTable()));
            }
            else {
                cerr << "ERROR: functions with identical names";
                error = true;
            }
        } // function declaration check on call
        else if (it->rule == "factor ID LPAREN arglist RPAREN") {
            if (topSymT.find(it->children[0].tokens[1]) == topSymT.end()) {
                cerr << "ERROR: no function with that name" << endl;
                error = true;
            }
            // pp check
            if (topSymT[curProc].second.find(it->children[0].tokens[1]) != topSymT[curProc].second.end()) {
                cerr << "ERROR: P()" << endl;
                error = true;
            }
            /*

                        for (symbolTable::const_iterator jt = topSymT[curProc].second.second.begin(); jt != topSymT[curProc].second.second.end(); jt++) {
                if (it->children[0].tokens[1] == jt->first) {
                    cerr << "ERROR: P(P)" << endl;
                    error = true;
                }
            }
            */

        }
        else if (it->rule == "factor ID LPAREN RPAREN") {
            if (topSymT.find(it->children[0].tokens[1]) == topSymT.end()) {
                cerr << "ERROR: no function with that name" << endl;
                error = true;
            }
            // pp check
            if (topSymT[curProc].second.find(it->children[0].tokens[1]) != topSymT[curProc].second.end()) {
                cerr << "ERROR: P()" << endl;
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
        if (it->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            curProc = "wain";
            //cerr << "TESTING: curProc = " << curProc << endl;
        }
        else if (it->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            
            // curProc = it->children[1].tokens[0];
            curProc = it->children[1].tokens[1];
            //cerr << "TESTING: curProc = " << curProc << endl;
        } else if (it->rule == "factor ID") {
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

void CheckNamingIssueProcedure(Tree parseTree) {
    for(map<string, signatureTable>::const_iterator it = topSymT.begin(); it != topSymT.end(); it++) {
    // it->first is procedure name
        for (symbolTable::const_iterator jt = it->second.second.begin(); jt != it->second.second.end(); jt++) {
            if (it->first == jt->first) {
                cerr << "ERROR: procedure and local variable naming issue" << endl;
                error = true;
            }
        }
    }
}





// return "ERROR" for error
// otherwise type of variable
string ReturnType(Tree parseTree) {
    if (parseTree.tokens[0] == "NUM") {
        return "int";
    }
    else if (parseTree.tokens[0] == "ID") {
        return topSymT[curProc].second[parseTree.tokens[1]];
    }
    else if (parseTree.rule == "factor AMP lvalue") {
        if (ReturnType(parseTree.children[1]) == "int") {
            return "int*";
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "factor STAR factor" || parseTree.rule == "lvalue STAR factor") {
        if (ReturnType(parseTree.children[1]) == "int*") {
            return "int";
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "factor LPAREN expr RPAREN" || parseTree.rule == "lvalue LPAREN lvalue RPAREN") {

        return ReturnType(parseTree.children[1]);
    }
    else if (parseTree.rule == "expr term" || parseTree.rule == "term factor" || parseTree.rule == "factor ID" || parseTree.rule == "lvalue ID" || parseTree.rule == "factor NUM") {

        return ReturnType(parseTree.children[0]);
    }
    else if (parseTree.rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        if (ReturnType(parseTree.children[11]) == "int" ) {

            return ReturnType(parseTree.children[11]);
            
            
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        if (ReturnType(parseTree.children[9]) == "int" ) {
            return ReturnType(parseTree.children[9]);
            
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "dcls dcls dcl BECOMES NULL SEMI") {
        if (ReturnType(parseTree.children[1].children[1]) == "int*") {
            return ReturnType(parseTree.children[1]);
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "test expr LE expr" || parseTree.rule == "test expr GE expr" || parseTree.rule == "test expr LT expr" || parseTree.rule == "test expr GT expr") {
        if (ReturnType(parseTree.children[0]) != ReturnType(parseTree.children[2])) {
            return "ERROR";
        }
        else {

            return "array";

        }
    }
    else if (parseTree.rule == "test expr EQ expr" || parseTree.rule == "test expr EQ expr") {
        if (ReturnType(parseTree.children[2]) == ReturnType(parseTree.children[0])) {

            return ReturnType(parseTree.children[0]);
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "PRINTLN LPAREN expr RPAREN SEMI" ) {
        if (ReturnType(parseTree.children[2]) != "int" ) {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "DELETE LBRACK RBRACK expr SEMI" ) {
        if (ReturnType(parseTree.children[2]) != "int*" ) {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "factor NEW INT LBRACK expr RBRACK") {
        if (ReturnType(parseTree.children[3]) == "int") {

            return "int*";
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "statement lvalue BECOMES expr SEMI") {
        if (ReturnType(parseTree.children[2]) != ReturnType(parseTree.children[0])) {
            return "ERROR";
        }
        else {
            return ReturnType(parseTree.children[2]);
        }
    }
    else if (parseTree.rule == "term term STAR factor" || parseTree.rule == "term term PCT factor" || parseTree.rule == "term term SLASH factor") {
        if (ReturnType(parseTree.children[0]) == "int" && ReturnType(parseTree.children[2]) == "int") {
            return "int";
        }
        else {
            return "ERROR";
        }
    }
    return "undefined";
}



void CheckType(Tree parseTree) {
    for (vector<Tree>::const_iterator it = parseTree.children.begin(); it != parseTree.children.end(); it++) {
        if (it->rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            curProc = "wain";
        }
        else if (it->rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
            curProc = it->children[1].tokens[1];
        }
        else if (ReturnType(*it) == "ERROR") {
            cerr << "ERROR: Type check" << endl;
            error = true;
            return;
        }
        else {
            CheckType(*it);
        }


    }
}


int main()
{
    Tree parseTree;
    string line;
    getline(cin, line);
    parseTree = BuildNode(line);
    MakeSymbolTable(parseTree);
    //CheckNamingIssueProcedure(parseTree);
    //PrintParseTree(parseTree);

    // check for undeclared variables
    CheckUndeclaredVariable(parseTree);

    // check for type
    CheckType(parseTree);

    
    
    
    
    
    
    if (error == false) {
        /*
        for (map<string, signatureTable>::const_iterator it = topSymT.begin(); it != topSymT.end(); it++) {
            cerr << it->first;
            int i = 0;
            for (vector<string>::const_iterator jt = it->second.first.begin(); jt != it->second.first.end(); jt++) {
                cerr << " " << *jt;
                i++;
                //cerr << " TESTING LOOP : " << i << endl;
                //cerr << "LOOP TESTING" << endl;
                // signature is not saved for id
                // signature printing
            }
            cerr << endl;


            //cerr << "wain" << endl;
            for (symbolTable::const_iterator jt = it->second.second.begin(); jt != it->second.second.end(); jt++) {
                cerr << jt->first << " " << jt->second << endl;
            }
            cerr << endl; // new line for procedure
            
        }
        */
    }
    

    return 0;
    
}

// Run program: Ctrl + F Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


