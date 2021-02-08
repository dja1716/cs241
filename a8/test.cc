// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <utility>
using namespace std;

class Tree {
public:
    string rule; //eg. expr expr PLUS term
    vector<string> tokens;
    vector<Tree> children;
};

typedef map<string, pair<string, int>> symbolTable; // name -> type, location
typedef pair<vector<string>, symbolTable> signatureTable;
typedef map<string, pair<vector<string>, map<string, pair<string, int>>>> topSymbolTable;

// pro tip
string curProc = "empty";
topSymbolTable topSymT;
symbolTable symT;
bool error = false;
int loopCounter = 0;
int ifCounter = 0;
bool lvalueBecomeFlag = false;
bool init = false;


int offset = 0;

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
                    // below is iffy
                    init = true;
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
                    topSymT[curProc].second.insert(make_pair(it->children[1].tokens[1], make_pair(it->children[0].children[0].tokens[1], offset)));
                }
                else if (it->children[0].rule == "type INT STAR") {
                    //cout << "INT STAR" << endl;
                    topSymT[curProc].second.insert(make_pair(it->children[1].tokens[1], make_pair(it->children[0].children[0].tokens[1] + it->children[0].children[1].tokens[1], offset)));
                }
                offset -= 4;
                
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
        if (word == terminals[i]) { 
            return true; 
        }
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
        return topSymT[curProc].second[parseTree.tokens[1]].first;
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
        if (ReturnType(parseTree.children[11]) == "int") {

            return ReturnType(parseTree.children[11]);


        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "procedure INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        if (ReturnType(parseTree.children[9]) == "int") {
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
    else if (parseTree.rule == "PRINTLN LPAREN expr RPAREN SEMI") {
        if (ReturnType(parseTree.children[2]) != "int") {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "DELETE LBRACK RBRACK expr SEMI") {
        if (ReturnType(parseTree.children[2]) != "int*") {
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
        if (ReturnType(parseTree.children[0]) != ReturnType(parseTree.children[2])) {
            return "ERROR";
        }
        return ReturnType(parseTree.children[0]);
    }
    else if (parseTree.rule == "term term STAR factor" || parseTree.rule == "term term PCT factor" || parseTree.rule == "term term SLASH factor") {
        if (ReturnType(parseTree.children[0]) == "int" && ReturnType(parseTree.children[2]) == "int") {
            return "int";
        }
        else {
            return "ERROR";
        }
    }
    else if (parseTree.rule == "expr expr PLUS term") {
        if (ReturnType(parseTree.children[0]) == "int" && ReturnType(parseTree.children[2]) == "int") {
            return "int";
        }
        else if (ReturnType(parseTree.children[0]) == "int*" && ReturnType(parseTree.children[2]) == "int*") {
            return "ERROR";
        }
        else {
            return "int*";
        }

    }
    else if (parseTree.rule == "expr expr MINUS term") {
        if (ReturnType(parseTree.children[0]) == "int" && ReturnType(parseTree.children[2]) == "int") {
            return "int";
        }
        else if (ReturnType(parseTree.children[0]) == "int*" && ReturnType(parseTree.children[2]) == "int*") {
            return "int";
        }
        else if (ReturnType(parseTree.children[0]) == "int*" && ReturnType(parseTree.children[2]) == "int") {
            return "int*";
        }
        else {
            return "ERROR";
        }
    }



    // IMPORTANT!!!! SIGNATURE CHECK
    else if (parseTree.rule == "factor ID LPAREN arglist RPAREN") {

        return ReturnType(parseTree.children[2]);
    }
    else if (parseTree.rule == "arglist expr COMMA arglist") {

        return ReturnType(parseTree.children[2]);


    }
    return "ERROR";
}

// i is register#
void pop(int i) {
    cout << "add $30, $30, $4" << "; pop" << endl;
    cout << "lw $" << i << ", -4($30)" << "; pop" << endl;
    //cout << "sw $17, -4($30)" << "; pop" << endl;

}

void push(int i) {
    //cout << "lw $17, -4($30)" << "; push" << endl;
    cout << "sw $" << i << ", -4($30)" << "; push" << endl;
    cout << "sub $30, $30, $4" << "; push" << endl;
}

void code(int i) {
    cout << "lw $3, " << i << "($29)" << "; code" << endl;
}

void code(string s) {
    cout << "lw $3, " << s << "($29)" << endl;
}



// final assessment
void CodeGen(Tree parseTree) {
    // if cases are not found, then do nothing?
    if (parseTree.rule == "start BOF procedures EOF") {
        CodeGen(parseTree.children[1]);
    }
    else if (parseTree.rule == "procedures main") {
        //curProc = "main"
        CodeGen(parseTree.children[0]);

    }
    else if (parseTree.rule == "main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE") {
        CodeGen(parseTree.children[3]);
        CodeGen(parseTree.children[5]);
        CodeGen(parseTree.children[8]);
        CodeGen(parseTree.children[9]);
        CodeGen(parseTree.children[11]);
        curProc = "wain";
    }
    else if (parseTree.rule == "type INT") {
        CodeGen(parseTree.children[0]);
    }
    else if (parseTree.rule == "dcls") {
        // nothing
    }
    else if (parseTree.rule == "statements") {
        // nothing
    }
    else if (parseTree.rule == "expr term") {
        CodeGen(parseTree.children[0]);
    }
    else if (parseTree.rule == "term factor") {
        CodeGen(parseTree.children[0]);
    }
    else if (parseTree.rule == "factor ID") {
        CodeGen(parseTree.children[0]);
    }
    else if (parseTree.tokens[0] == "ID") {
        //cout << curProc << endl;
        //cout << parseTree.tokens[0] << " " << parseTree.tokens[1] << endl;
        cout << endl << ";ID: " << parseTree.tokens[1] << endl << endl;
        int temp = topSymT[curProc].second[parseTree.tokens[1]].second;
        code(temp);
    }
    else if (parseTree.rule == "factor LPAREN expr RPAREN") {
        CodeGen(parseTree.children[1]);
    } // q3 pop, push function needed
    else if (parseTree.rule == "expr expr PLUS term") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        if (ReturnType(parseTree.children[0]) == "int*") {
            cout << "mult $5, $4" << endl;
            cout << "mflo $5" << endl;
        }
        else if (ReturnType(parseTree.children[2]) == "int*") {
            cout << "mult $3, $4" << endl;
            cout << "mflo $3" << endl;
        }
        
         cout << "add $3, $5, $3" << endl;


    }
    else if (parseTree.rule == "expr expr MINUS term") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        if (ReturnType(parseTree.children[0]) == "int*") {
            if (ReturnType(parseTree.children[2]) == "int*") {
                cout << "div $3, $4" << endl;
                cout << "mflo $3" << endl;
                cout << "div $5, $4" << endl;
                cout << "mflo $5" << endl;

            }
            else {
                cout << "mult $5, $4" << endl;
                cout << "mflo $5" << endl;
            }
        }
        cout << "sub $3, $5, $3" << endl;
    }
    else if (parseTree.rule == "term term STAR factor") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        cout << "mult $3, $5" << endl;
        cout << "mflo $3" << endl;
    }
    else if (parseTree.rule == "term term SLASH factor") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        cout << "div $5, $3" << endl;
        cout << "mflo $3" << endl;
    }
    else if (parseTree.rule == "term term PCT factor") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        cout << "div $5, $3" << endl;
        cout << "mfhi $3" << endl;
    } 
    else if (parseTree.rule == "factor NUM") {
        CodeGen(parseTree.children[0]);
    }
    else if (parseTree.tokens[0] == "NUM") {
        // code
        //code(parseTree.tokens[1]);
        //cout << "lw $3, " << parseTree.tokens[1] << "($29)" << endl;
        cout << "lis $3" << endl;
        cout << ".word " << parseTree.tokens[1] << endl;
    }
    else if (parseTree.rule == "statements statements statement") {
        CodeGen(parseTree.children[0]);
        CodeGen(parseTree.children[1]);
    }
    else if (parseTree.rule == "statement PRINTLN LPAREN expr RPAREN SEMI") {
        cout << "add $15, $1, $0" << endl;
        CodeGen(parseTree.children[2]);

        cout << "add $1, $3, $0" << endl;
        push(31);
        cout << "jalr $10" << endl;
        pop(31);
        cout << "add $1, $15, $0" << endl;
    }
    // iffy codes
    else if (parseTree.rule == "dcls dcls dcl BECOMES NUM SEMI") {
        CodeGen(parseTree.children[0]);
        CodeGen(parseTree.children[1]);
        cout << "lis $3" << endl;
        cout << ".word " << parseTree.children[3].tokens[1] << endl; // correct this line
        int temp = topSymT[curProc].second[parseTree.children[1].children[1].tokens[1]].second;
        cout << "sw $3, " << temp << "($29)" << endl; // error this line
        //topSymT[curProc].second.find(it->children[0].tokens[1]
        

        
        //PrintParseTree(topSymT[curProc]);
        //cout << endl << endl;
        cout << "sub $30, $30, $4" << endl;
    }
    else if (parseTree.rule == "dcls dcls dcl BECOMES NULL SEMI") {
        CodeGen(parseTree.children[0]);
        CodeGen(parseTree.children[1]);
        
        cout << "add $3, $11, $0" << endl;
        
    }
    else if (parseTree.rule == "statement lvalue BECOMES expr SEMI") {
        CodeGen(parseTree.children[2]);
        
        if (parseTree.children[0].rule == "lvalue STAR factor") {
            
            //cout << endl << ";lvalue STAR factor begin" << endl;
             // error line
            push(3);
            //lvalueBecomeFlag = true;
            CodeGen(parseTree.children[0].children[1]);
            pop(5);
            //lvalueBecomeFlag = false;
            cout << "sw $5, 0($3)" << endl;
            //cout << endl << ";lvalue STAR factor end" << endl;
            

        }
        else {
            int temp = topSymT[curProc].second[parseTree.children[0].children[0].tokens[1]].second;
            cout << "sw $3, " << temp << "($29)" << "; variable: " << parseTree.children[0].children[0].tokens[1] << " value: " << topSymT[curProc].second[parseTree.children[0].children[0].tokens[1]].second << endl;

        
        
        }
        


        
    }
    else if (parseTree.rule == "lvalue LPAREN lvalue RPAREN") {
        CodeGen(parseTree.children[1]);

    }
    else if (parseTree.rule == "test expr LT expr") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        if (ReturnType(parseTree.children[0]) == "int") {
            cout << "slt $3, $5, $3" << endl;
        }
        else {
            cout << "sltu $3, $5, $3" << endl;
        }
        
    }
    else if (parseTree.rule == "test expr EQ expr") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        // maybe store $6 and $7 if used
        if(ReturnType(parseTree.children[0]) == "int") {
            cout << "slt $6, $3, $5" << endl;
            cout << "slt $7, $5, $3" << endl;
        }
        else {
            cout << "sltu $6, $3, $5" << endl;
            cout << "sltu $7, $5, $3" << endl;
        }

        cout << "add $3, $6, $7" << endl;
        cout << "sub $3, $11, $3" << endl;
    }
    else if (parseTree.rule == "test expr NE expr") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        // maybe store $6 and $7 if used
        if (ReturnType(parseTree.children[0]) == "int") {
            cout << "slt $6, $3, $5" << endl;
            cout << "slt $7, $5, $3" << endl;
        }
        else {
            cout << "sltu $6, $3, $5" << endl;
            cout << "sltu $7, $5, $3" << endl;
        }

        cout << "add $3, $6, $7" << endl;
    }
    else if (parseTree.rule == "test expr LE expr") {
        CodeGen(parseTree.children[2]);
        push(3);
        CodeGen(parseTree.children[0]);
        pop(5);
        if (ReturnType(parseTree.children[0]) == "int") {
            cout << "slt $3, $5, $3" << endl;
        }
        else {
            cout << "sltu $3, $5, $3" << endl;
        }

        cout << "sub $3, $11, $3" << endl;
    }
    else if (parseTree.rule == "test expr GE expr") {
        CodeGen(parseTree.children[0]);
        push(3);
        CodeGen(parseTree.children[2]);
        pop(5);
        if (ReturnType(parseTree.children[0]) == "int") {
            cout << "slt $3, $5, $3" << endl;
        }
        else {
            cout << "sltu $3, $5, $3" << endl;
        }

        cout << "sub $3, $11, $3" << endl;
    }
    else if (parseTree.rule == "test expr GT expr") {
        CodeGen(parseTree.children[2]);
        push(3);
        CodeGen(parseTree.children[0]);
        pop(5);
        cout << "slt $3, $5, $3" << endl;

    }
    else if (parseTree.rule == "statement WHILE LPAREN test RPAREN LBRACE statements RBRACE") {
        int temp = loopCounter;
        loopCounter++;
        cout << "loop" << temp << ": ";
        CodeGen(parseTree.children[2]);
        cout << "beq $3, $0, endWhile" << temp << endl;
        CodeGen(parseTree.children[5]);
        cout << "beq $0, $0, loop" << temp << endl;
        cout << "endWhile" << temp << ": " << endl;

    }
    else if (parseTree.rule == "statement IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE") {
        int temp = ifCounter;
        ifCounter++;
        CodeGen(parseTree.children[2]);
        cout << "beq $3, $0, else" << temp << endl;
        CodeGen(parseTree.children[5]);
        cout << "beq $0, $0, endif" << temp << endl;
        cout << "else" << temp << ": ";
        CodeGen(parseTree.children[9]);
        cout << "endif" << temp << ": ";

    }
    else if (parseTree.rule == "factor NULL") {
        cout << "add $3, $11, $0; NULL" << endl;
    }
    else if (parseTree.rule == "factor STAR factor") {
        CodeGen(parseTree.children[1]);
        cout << "lw $3, 0($3); factor STAR factor" << endl;
    }
    else if (parseTree.rule == "lvalue STAR factor") {
        cout << "; lvalue STAR factor";
        CodeGen(parseTree.children[1]);
        //cout << "lw $3, 0($3)" << endl;
        //if (!lvalueBecomeFlag) {
        //    cout << "lw $3, 0($3); lvalueBECOME false" << endl;
        //}

    }
    else if (parseTree.rule == "factor AMP lvalue") {

        if (parseTree.children[1].rule == "lvalue ID") {
            int temp = topSymT[curProc].second[parseTree.children[1].children[0].tokens[1]].second;
            //int temp = topSymT[curProc].second[parseTree.children[1].tokens[1]].second;
            cout << "lis $3; AMP lvalue case " << parseTree.children[1].children[0].tokens[1] << endl;
            cout << ".word " << temp << endl;
            cout << "add $3, $29, $3" << endl;
        }
        else {
            CodeGen(parseTree.children[1].children[1]);
        }
    }
    else if (parseTree.rule == "lvalue ID") {
        CodeGen(parseTree.children[0]);

    }
    else if (parseTree.rule == "factor NEW INT LBRACK expr RBRACK") {
        CodeGen(parseTree.children[3]);
        cout << "add $1, $3, $0" << endl;
        push(31);
        cout << "lis $7" << endl;
        cout << ".word new" << endl;
        cout << "jalr $7" << endl;
        pop(31);
        cout << "bne $3, $0, 1" << endl;
        cout << "add $3, $11, $0" << endl;
    }
    else if (parseTree.rule == "statement DELETE LBRACK RBRACK expr SEMI") {
        CodeGen(parseTree.children[3]);
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


int main()
{
    cout << ".import init" << endl;
    Tree parseTree;
    string line;
    getline(cin, line);
    parseTree = BuildNode(line);
    MakeSymbolTable(parseTree);
    //CheckNamingIssueProcedure(parseTree);
    //PrintParseTree(parseTree);

    // check for undeclared variables
    //CheckUndeclaredVariable(parseTree);

    // check for type
    //CheckType(parseTree);
    
    cout << ".import print" << endl;
    cout << ".import new" << endl;
    cout << ".import delete" << endl;
    cout << "lis $11" << "; for true" << endl;
    cout << ".word 1" << endl;
    cout << "lis $4" << endl;
    cout << ".word 4" << endl;
    cout << "lis $10" << endl;
    cout << ".word print" << endl;\

    cout << "sub $29, $30, $4" << endl;
    cout << "sw $1, -4($30)" << endl;
    cout << "sub $30, $30, $4" << endl;

    
    // init,,,,
    if (init) {
        cout << "add $2, $0, $0" << endl;
    }
    cout << "sw $2, -4($30)" << endl;
    cout << "sub $30, $30, $4" << endl;
    push(31);
    cout << "lis $7" << endl;
    cout << ".word init" << endl;
    cout << "jalr $7" << endl;
    pop(31);

    CodeGen(parseTree);
    cout << "add $30, $30, $4" << endl;
    cout << "add $30, $30, $4" << endl;
    cout << "jr $31" << endl;
    
    
    
    
    
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




