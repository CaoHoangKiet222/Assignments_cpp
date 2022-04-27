#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

const int MaxScope = 50;

class SplayNode {
public:
    SplayNode() : identifier(""), scopeLevel(0), type(""), left(NULL), right(NULL) {};
    SplayNode(std::string i, int s = 0, std::string t = "") : identifier(i), scopeLevel(s), type(t), left(NULL), right(NULL) {};
    std::string identifier;
    int scopeLevel;
    std::string type;
    SplayNode* left;
    SplayNode* right;
};

class SplayTree {
    public:
        SplayTree () {}
        SplayNode* rotateRight(SplayNode*);
        SplayNode* rotateLeft(SplayNode*);
        SplayNode* Insert(SplayNode*, const SplayNode&, int&, int&, const std::string&);
        SplayNode* Insert1(SplayNode*, const SplayNode&, int&, int&, const std::string&);
        SplayNode* Splay(SplayNode*, const SplayNode&, int&, int&, const std::string&);
        SplayNode* DeleteNode(SplayNode*, const SplayNode&, int&, int&, const std::string&);
        SplayNode* findNode(SplayNode*, const std::string&, const int&, int&, int&, const std::string&);
        void PrintTree(SplayNode*);
};

class SymbolTable
{
    public:
        SymbolTable() {}
        void run(string filename);
        friend int checkID(SplayNode*, const SplayNode&, int&);
        bool check_type_Function(const std::string&);
        bool checkValue(const std::string&, std::string&);
private:
    SplayNode* root = NULL;
};
#endif
