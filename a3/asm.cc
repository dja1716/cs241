#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "scanner.h"

using std::vector;
using std::string;
using std::pair;
using std::map;


/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */

typedef std::map<std::string, int> symbolTable;
std::vector <long> binaryOutput;
symbolTable symT;

int address = 0;


void printAsBinary(int64_t n) {
    char tempChar;

    tempChar = n >> 24;
    std::cout << tempChar;
    tempChar = n >> 16;
    std::cout << tempChar;
    tempChar = n >> 8;
    std::cout << tempChar;
    tempChar = n;
    std::cout << tempChar;
}

bool overFlowInt(int64_t n) {
    return (n >= -2147483648) && (n <= 4294967295);
}

bool overFlowHexInt(int64_t n) {
    return (n <= 0xffffffff) && (n >= 0x00000000);
}
void wordCheck(Token tokenBack) {

    Token::Kind tokenKind = tokenBack.getKind();

    if (tokenKind == Token::INT) {
        int64_t n = tokenBack.toLong();

        if (overFlowInt(n)) {
            binaryOutput.push_back(n);
        }
        else {
            throw ScanningFailure("ERROR word");
        }
    }
    else if (tokenKind == Token::HEXINT) {
        int64_t n = tokenBack.toLong();

        if (overFlowHexInt(n)) {
            binaryOutput.push_back(n);

        }
        else {
            throw ScanningFailure("ERROR word");
        }

    }
    else if (tokenKind == Token::ID) {
        string tempString = tokenBack.getLexeme();
        if (symT.find(tempString) != symT.end()) {
            int64_t n = symT.find(tempString)->second;
            //std::cout << symT.find(tempString)->second << std::endl;
            if (overFlowInt(n)) {
                binaryOutput.push_back(n);
            }
            else {
                throw ScanningFailure("ERROR word");
            }
        }
        else {
            throw ScanningFailure("ERROR label access");
        }
    }
    else {
        throw ScanningFailure("ERROR word");
    }
}


void makeSymbolTable(Token token) {
    std::string label = token.getLexeme();
    
    for (int i = 0; i < label.length(); i++) {
        bool tempCheck;
        char tempChar = label[i];
        if (i == 0) {
            if (tempChar == 'a' || tempChar == 'b' || tempChar == 'c' || tempChar == 'd' || tempChar == 'e' || tempChar == 'f' || tempChar == 'g' ||
                tempChar == 'h' || tempChar == 'i' || tempChar == 'j' || tempChar == 'k' || tempChar == 'l' || tempChar == 'm' || tempChar == 'n' ||
                tempChar == 'o' || tempChar == 'p' || tempChar == 'q' || tempChar == 'r' || tempChar == 's' || tempChar == 't' || tempChar == 'u' ||
                tempChar == 'v' || tempChar == 'w' || tempChar == 'x' || tempChar == 'y' || tempChar == 'z' ||
                tempChar == 'A' || tempChar == 'B' || tempChar == 'C' || tempChar == 'D' || tempChar == 'E' || tempChar == 'F' || tempChar == 'G' ||
                tempChar == 'H' || tempChar == 'I' || tempChar == 'J' || tempChar == 'K' || tempChar == 'L' || tempChar == 'M' || tempChar == 'N' ||
                tempChar == 'O' || tempChar == 'P' || tempChar == 'Q' || tempChar == 'R' || tempChar == 'S' || tempChar == 'T' || tempChar == 'V' ||
                tempChar == 'V' || tempChar == 'W' || tempChar == 'X' || tempChar == 'Y' || tempChar == 'Z') {
                tempCheck = true;
            }
            else {
                tempCheck = false;
            }

        }
        else if (i == label.length() - 1) {
            if (tempChar == ':') {
                tempCheck = true;
            }
            else {
                tempCheck = false;
            }

        } else if (tempChar == 'a' || tempChar == 'b' || tempChar == 'c' || tempChar == 'd' || tempChar == 'e' || tempChar == 'f' || tempChar == 'g' || 
            tempChar == 'h' || tempChar == 'i' || tempChar == 'j' || tempChar == 'k' || tempChar == 'l' || tempChar == 'm' || tempChar == 'n' ||
            tempChar == 'o' || tempChar == 'p' || tempChar == 'q' || tempChar == 'r' || tempChar == 's' || tempChar == 't' || tempChar == 'u' ||
            tempChar == 'v' || tempChar == 'w' || tempChar == 'x' || tempChar == 'y' || tempChar == 'z' ||
            tempChar == 'A' || tempChar == 'B' || tempChar == 'C' || tempChar == 'D' || tempChar == 'E' || tempChar == 'F' || tempChar == 'G' ||
            tempChar == 'H' || tempChar == 'I' || tempChar == 'J' || tempChar == 'K' || tempChar == 'L' || tempChar == 'M' || tempChar == 'N' ||
            tempChar == 'O' || tempChar == 'P' || tempChar == 'Q' || tempChar == 'R' || tempChar == 'S' || tempChar == 'T' || tempChar == 'V' ||
            tempChar == 'V' || tempChar == 'W' || tempChar == 'X' || tempChar == 'Y' || tempChar == 'Z' || tempChar == '1' || tempChar == '2' || 
            tempChar == '3' || tempChar == '4' || tempChar == '5' || tempChar == '6' || tempChar == '7' || tempChar == '8' || tempChar == '9' || tempChar == '0'
            ) {
            tempCheck = true;
        }
        else {
            tempCheck = false;
        }

        if (!tempCheck) {
            throw ScanningFailure("ERROR label naming");
        }
    
    }

    label.erase(label.find(':'));

    if (symT.find(label) == symT.end()) {
        symT.insert(make_pair(label, address));
    }
    else {
        throw ScanningFailure("ERROR label duplicated naming");
    }


}

void assembler(vector<vector<Token>>& tokenLines) {
    for (int i = 0; i < tokenLines.size(); i++) {

        vector<Token> tokenLine = tokenLines[i];

        if (tokenLine.size() > 0) {
            Token& tokenType = tokenLine.front();
            if (tokenType.getKind() == Token::LABEL) {
                if (tokenLine.size() == 3) {
                    //makeSymbolTable(tokenLine.front());

                    if (tokenLine.at(1).getKind() == Token::WORD) {
                        wordCheck(tokenLine.back());
                    }
                    else if (tokenLine.at(1).getKind() == Token::ID) {
                        string op = tokenLine.at(1).getLexeme();
                        // jr or jalr variable is $s
                    // use toLong to make REG as int64_t and use << to make it binary instruction
                    }
                    else {
                        throw ScanningFailure("ERROR word");
                    }   
                }
                else if (tokenLine.size() == 1) {
                    //makeSymbolTable(tokenLine.front());
                }
                else {
                    throw ScanningFailure("ERROR label");
                }
            } // check id for ops
            else if (tokenType.getKind() == Token::WORD) {
                if (tokenLine.size() == 2) {
                    wordCheck(tokenLine.back());
                }
                else {
                    throw ScanningFailure("ERROR word");
                }
            }
            else if (tokenType.getKind() == Token::ID) {
                if (tokenLine.size() == 2) {

                }
                else {
                    throw ScanningFailure("ERROR ID");
                }
            }
            else {
                throw ScanningFailure("ERROR general");
            }
        }
        
    }
}


int main() {
  std::string line;
  std::vector<std::vector<Token>> tokenLines;
  
  

  try {
    while (getline(std::cin, line)) {
      // For example, just print the scanned tokens
      std::vector<Token> tokenLine = scan(line);

      /*if (tokenLine.size() > 0) {
          Token& tokenType = tokenLine.front();
          if (tokenType.getKind() == Token::LABEL) {
              makeSymbolTable(tokenType);
          }
          if (tokenLine.size() > 1) {
              address += 4;
          }
          
      }

      
      tokenLines.push_back(scan(line));*/
      
      
      for (auto &token : tokenLine) {
        std::cout << token << ' ';
      }
      std::cout << std::endl;
    }
  } catch (ScanningFailure &f) {
    std::cerr << f.what() << std::endl;

    return 1;
  }

  assembler(tokenLines);
  /*for (std::map<std::string, int>::const_iterator it = symT.begin(); it != symT.end(); it++) {
      std::cerr << it->first << " " << it->second << std::endl;
  }*/
  for (auto i : binaryOutput) {
      printAsBinary(i);
  }

  return 0;
}

