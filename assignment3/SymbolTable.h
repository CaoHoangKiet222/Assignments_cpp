#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

template<class T>
class Vector {
public:
    Vector();
    Vector(const int&);
    ~Vector();
    int size();
    T& operator [] (int); // get value of object[index]
    Vector& operator = (Vector&); // assign Vector for Vector
    void push_back(T);
    int find(const int&);

private:
    int Size;
    int capacity;
    T* arr;
    void ensureCap();
};

enum DataType {
    Number,
    String,
    Void,
    Null
};

struct Hash_Symbol {
    std::string identifier;
    long long encode_id;
    int numofParameter;
    DataType* arguments = nullptr;
    int level;
    int index;
    DataType type;
    bool isExists;
    
    Hash_Symbol() : identifier(""), encode_id(0), numofParameter(-1), level(0), index(-1), type(Null), isExists(false) {}
    Hash_Symbol(std::string id, long long e_id = -1, int num = -1, int lv = 0, int in = -1, DataType t = Null, bool iE = false) : identifier(id), encode_id(e_id), numofParameter(num), level(lv), index(in), type(t), isExists(false) {
        if (num != -1) {
            this->arguments = new DataType[num];
            for (int i = 0; i < num; i++) {
                this->arguments[i] = Null;
            }
        }
    }
    bool operator == (Hash_Symbol);
    bool operator != (Hash_Symbol);
};

class Hash_Table {
public:
    template<class T>
    T Hash_Function(const T&, const T&);
    template<class T>
    T Hash_Function_Double(const T&, const T&);
    template<class T>
    T Detection_Linear(Hash_Symbol*&, const std::string&);
    template<class T>
    T Detection_Quadratic(Hash_Symbol*&, const std::string&);
    template<class T>
    T Detection_Double(Hash_Symbol*&, const std::string&);
    template<class T>
    T Search(Hash_Symbol&, int&);
    friend class SymbolTable;
    ~Hash_Table();
private:
    Hash_Symbol data;
    int m; // the size of hash array
    int c, c1, c2; // detection variable
    std::string type_Method;
    Vector<Hash_Symbol> vec;
};

class SymbolTable
{
public:
    SymbolTable() {}
    void run(string filename);

    long long Encode(const std::string&, int);
    std::string Tokenize(const std::string&, const char&, int&);
    bool CheckId(const std::string&);
    bool CheckNum(const std::string&);
    bool CheckString(const std::string&);
    bool isSixNum(const std::string&);
    void Check_And_CountFunction(Hash_Table&, const std::string&, const int&, int&, int&, const std::string&, const std::string&);

private:
    Hash_Table ht;
};
#endif
