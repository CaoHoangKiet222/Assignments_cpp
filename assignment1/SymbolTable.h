#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
    vector<string> tokenize (string s, string del = " ") {
        vector<string> res;
        int start = 0;
        int end = (int)s.find(del);
        while (end != -1) {
            res.push_back(s.substr(start, end - start));
            start = (int)end + (int)del.size();
            end = (int)s.find(del, start);
        }
        res.push_back(s.substr(start, end - start));
        return res;
    }
    bool check_Identifier(string s);
};
#endif