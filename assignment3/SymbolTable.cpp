#include "SymbolTable.h"

std::string SymbolTable::Tokenize(const std::string& s, const char& c, int& start) {
    std::string ans = "";
    for (int i = start; i < (int)s.size(); i++) {
        if (s[i] == c) {
            start = i+1;
            return ans;
        }
        ans += s[i];
    }
    start = (int)s.size();
    return ans;
}

long long SymbolTable::Encode(const std::string& s, int level) {
    std::string temp = "";
    temp += to_string(level);
    for (int i = 0; i < (int)s.size(); i++) {
        temp += std::to_string((int)s[i] - 48);
    }
    int j = 0;
    long long num = 0;
    while (j < (int)temp.size()) {
        num = num * 10 + (int)temp[j] - 48;
        j++;
    }
    return num;
}

bool SymbolTable::CheckId(const std::string& s) {
    regex e("[[:lower:]][[:w:]]*");
    return regex_match(s, e);    
}

bool SymbolTable::CheckNum(const std::string& s) {
    regex e("[[:digit:]]+");
    return regex_match(s, e);
}

bool SymbolTable::isSixNum(const std::string& s) {
    if ((int)s.size() > 6) return false;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] > '9' || s[i] < '0') return false;
    }
    return true;
}

bool SymbolTable::CheckString(const std::string& s) {
    regex e("'[[:alnum:],' ']*'");
    return regex_match(s, e);
}

void SymbolTable::Check_And_CountFunction(Hash_Table& ht, const std::string& s, const int& level, int& countSlot, int& index, const std::string& error, const std::string& command) {
    int sl = 0, end = 0, start = 0, count = 0, idx = 0;
    bool found = false;

    end = (int)s.find('(', start);
    if (end == -1) throw InvalidInstruction(error);

    std::string tp = s.substr(start, end - start);
    for (int i = level; i >= 0; i--) {
        Hash_Symbol temp(tp, Encode(tp, i), -1, i);
        sl = ht.Search<long long>(temp, idx);
        if (idx != -1) break;
    }

    if (idx == -1) throw Undeclared(tp);
    index = idx;
    int numofPara = ht.vec[index].numofParameter;
    if (numofPara == -1) throw TypeMismatch(error);

    countSlot += sl;

    start = end + 1;

    while (s[start] != ')' && !found) {
        end = (int)s.find(',', start); 
        if (end == -1) {
            end = (int)s.find(')');
            if (end == -1) throw InvalidInstruction(error);
            found = true;
        }

        tp = s.substr(start, end - start);

        if (!ht.vec[index].isExists) {
            if (CheckNum(tp) && ht.vec[index].arguments[count] == Null) 
                ht.vec[idx].arguments[count] = Number;
            if (CheckString(tp) && ht.vec[index].arguments[count] == Null) 
                ht.vec[idx].arguments[count] = String;
        }
        else if (ht.vec[index].isExists) {
            if (CheckNum(tp))
                if (ht.vec[index].arguments[count] != Number) 
                    throw TypeMismatch(error);
            if (CheckString(tp)) 
                if (ht.vec[index].arguments[count] != String) 
                    throw TypeMismatch(error);
        }

        if (!CheckNum(tp) && !CheckString(tp)) {
            if (!CheckId(tp)) 
                throw InvalidInstruction(error);

            bool fd = false;
            for (int i = level; i >= 0 && !fd; i--) {
                Hash_Symbol temp1(tp, Encode(tp, i), -1, i);
                
                sl = ht.Search<long long>(temp1, idx);
                /* if (temp1.arguments != nullptr) { */
                /*     delete[] temp1.arguments; */
                /*     temp1.arguments = nullptr; */
                /* } */
                if (idx != -1) {
                    countSlot += sl;
                    fd = true;
                }
            }
            
            if (!fd) 
                throw Undeclared(tp);

            if (ht.vec[idx].numofParameter != -1) 
                throw TypeMismatch(error);

            if (ht.vec[idx].type == Null) {
                if (ht.vec[index].isExists) {
                     if (ht.vec[index].arguments[count] != Null)
                         ht.vec[idx].type = ht.vec[index].arguments[count];
                }
                    /* throw TypeMismatch(error); */
                else throw TypeCannotBeInferred(error);
            }
            else {
                if (ht.vec[index].arguments[count] != Null && ht.vec[index].arguments[count] != ht.vec[idx].type)
                    throw TypeMismatch(error);
            }
        }
        count++;
        start = end + 1;
    } 
    if (count != numofPara) throw TypeMismatch(error);
    ht.vec[index].isExists = true;
    return;
}

void SymbolTable::run(string filename)
{
    ifstream file(filename);
    std::string currentline;
    int countLevel = 0, countInsert = 0;

    while (getline(file, currentline)) {
        int start = 0, countSlot = 0;
        bool isNum = false, isString = false, isFunc = false, passById = false;

        std::string command = Tokenize(currentline, ' ', start);

        if (command == "LINEAR" || command == "DOUBLE") {

            if (command == "LINEAR") ht.type_Method = "Linear";
            else ht.type_Method = "Double";

            command = Tokenize(currentline, ' ', start);
            if (command == "" || !isSixNum(command)) throw InvalidDeclaration(currentline);
            ht.m = Encode(command, 0);
            Vector<Hash_Symbol> v(ht.m);
            ht.vec = v;

            command = Tokenize(currentline, ' ', start);
            if (command == "" || !isSixNum(command)) throw InvalidDeclaration(currentline);
            ht.c = Encode(command, 0);
        }
        else if (command == "QUADRATIC") {
            command = Tokenize(currentline, ' ', start);
            if (command == "" || !isSixNum(command)) throw InvalidDeclaration(currentline);
            ht.m = Encode(command, 0);
            Vector<Hash_Symbol> v(ht.m);
            ht.vec = v;

            command = Tokenize(currentline, ' ', start);
            if (command == "" || !isSixNum(command)) throw InvalidDeclaration(currentline);
            ht.c1 = Encode(command, 0);

            command = Tokenize(currentline, ' ', start);
            if (command == "" || !isSixNum(command)) throw InvalidDeclaration(currentline);
            ht.c2 = Encode(command, 0);

            ht.type_Method = "Quadratic";
        }
        else if (command == "INSERT") {
            std::string id = Tokenize(currentline, ' ', start);
            if (id == "" || !CheckId(id)) throw InvalidDeclaration(id);

            std::string num_para = Tokenize(currentline, ' ', start);

            Hash_Symbol* hs = nullptr;
            
            if (num_para != "") {
                /* if (countLevel != 0) throw InvalidDeclaration(id); */
                hs = new Hash_Symbol(id, Encode(id, countLevel), Encode(num_para, 0), countLevel);
            }
            else hs = new Hash_Symbol(id, Encode(id, countLevel), -1, countLevel);

            if (ht.type_Method == "Linear") {
                std::cout << ht.Detection_Linear<long long>(hs, currentline) << '\n';
            }
            else if (ht.type_Method == "Quadratic") {
                std::cout << ht.Detection_Quadratic<long long>(hs, currentline) << '\n';
            }
            else if (ht.type_Method == "Double") {
                std::cout << ht.Detection_Double<long long>(hs, currentline) << '\n';
            }
            delete hs;
            countInsert++;
        }
        else if (command == "ASSIGN") {
            std::string id = Tokenize(currentline, ' ', start);

            std::string val = Tokenize(currentline, '$', start);

            int indexofId = 0;
            int ed = (int)val.find('(', 0);
            
            if (ed == -1) {
                if (CheckNum(val)) 
                    isNum = true;

                if (!isNum && CheckString(val)) 
                    isString = true;

                if (!isNum && !isString && CheckId(val)) 
                    passById = true;
            }
            else isFunc = true;

            if (isNum || isString) {
                Hash_Symbol temp(id, Encode(id, countLevel), -1, countLevel);
                countSlot += ht.Search<long long>(temp, indexofId);

                if (indexofId == -1) throw Undeclared(id);

                std::cout << countSlot << '\n';

                if (isNum) ht.vec[indexofId].type = Number;
                else ht.vec[indexofId].type = String;
            }
            else if (isFunc) {
                int indexofFunc = 0, sl = 0;
                Check_And_CountFunction(ht, val, countLevel, countSlot, indexofFunc, currentline, command);

                if (id == "" || !CheckId(id)) 
                    throw InvalidDeclaration(id);

                for (int i = countLevel; i >= 0; i--) {
                    Hash_Symbol temp(id, Encode(id, i), -1, i);
                    sl = ht.Search<long long>(temp, indexofId);
                    if (indexofId != -1) break;
                }

                if (indexofId == -1) 
                    throw Undeclared(id);
                countSlot += sl;

                if (ht.vec[indexofId].numofParameter != -1) 
                    throw TypeMismatch(currentline);

                if (ht.vec[indexofFunc].type == Void) 
                    throw TypeMismatch(currentline);

                if (ht.vec[indexofId].type == Null && ht.vec[indexofFunc].type == Null) 
                    throw TypeCannotBeInferred(currentline);

                if (ht.vec[indexofId].type == Number && ht.vec[indexofFunc].type == String)
                    throw TypeMismatch(currentline);

                if (ht.vec[indexofId].type == String && ht.vec[indexofFunc].type == Number)
                    throw TypeMismatch(currentline);

                if (ht.vec[indexofId].type != Null) ht.vec[indexofFunc].type = ht.vec[indexofId].type;
                else ht.vec[indexofId].type = ht.vec[indexofFunc].type;

                std::cout << countSlot << '\n';
            }
            else if (passById) {
                int indexofVal = 0, sl = 0;
                for (int i = countLevel; i >= 0; i--) {
                    Hash_Symbol temp(val, Encode(val, i), -1, i);
                    sl = ht.Search<long long>(temp, indexofVal);
                    if (indexofVal != -1) break;
                }
                
                if (indexofVal == -1) 
                    throw Undeclared(val);

                countSlot += sl;
                for (int i = countLevel; i >= 0; i--) {
                    Hash_Symbol temp(id, Encode(id, i), -1, i);
                    sl = ht.Search<long long>(temp, indexofId);
                    if (indexofId != -1) break;
                }

                if (indexofId == -1) 
                    throw Undeclared(id);

                countSlot += sl;

                if (ht.vec[indexofId].numofParameter != -1) 
                    throw TypeMismatch(currentline);

                if (ht.vec[indexofId].type == Null && ht.vec[indexofVal].type == Null) 
                    throw TypeCannotBeInferred(currentline);
                
                if (ht.vec[indexofId].type == Number && ht.vec[indexofVal].type == String)
                    throw TypeMismatch(currentline);
                if (ht.vec[indexofId].type == String && ht.vec[indexofVal].type == Number)
                    throw TypeMismatch(currentline);

                if (ht.vec[indexofId].type != Null) ht.vec[indexofVal].type = ht.vec[indexofId].type;
                else ht.vec[indexofId].type = ht.vec[indexofVal].type;

                std::cout << countSlot << '\n';
            }
            else throw TypeMismatch(currentline);
        }
        else if (command == "CALL") {
            int indexofFunc = -1;
            std::string func = Tokenize(currentline, '$', start);
            Check_And_CountFunction(ht, func, countLevel, countSlot, indexofFunc, currentline, command);

            if (ht.vec[indexofFunc].type == Null) 
                ht.vec[indexofFunc].type = Void;
            if (ht.vec[indexofFunc].type != Null && ht.vec[indexofFunc].type != Void)
                throw TypeMismatch(currentline);
            /* if (ht.vec[indexofFunc].type != Void) */
            /*     throw TypeMismatch(currentline); */

            std::cout << countSlot << '\n';
        }
        else if (command == "BEGIN") {
            countLevel++;
        }
        else if (command == "END") {
            if (countLevel == 0) throw UnknownBlock();

            while (ht.vec.find(countLevel) != -1) {
                int index = ht.vec.find(countLevel);
                Hash_Symbol temp;
                ht.vec[index] = temp;
                countInsert--;
            }
            countLevel--;
        }
        else if (command == "LOOKUP") {
            std::string id = Tokenize(currentline, ' ', start);
            bool found = false;
            int indexofId = -1, sl = -1;
            for (int i = countLevel; i >= 0 && !found; i--) {
                Hash_Symbol temp(id, Encode(id, i), -1, i);
                sl = ht.Search<long long>(temp, indexofId);
                if (indexofId != -1 || sl != -1) found = true;
            }
            
            if (!found)
                throw Undeclared(id);

            std::cout << indexofId << '\n';
        }
        else if (command == "PRINT") {
            bool found = false;
            int ct = 0;
            for (int i = 0; i < ht.vec.size(); i++) {
                if (ht.vec[i].identifier != "") {
                    std::cout << i << " " << ht.vec[i].identifier << "//" << ht.vec[i].level;
                    ct++;
                    found = true;
                    if (ct != countInsert) std::cout << ";";
                }
            }
            if (found) std::cout << '\n';
        }
        else throw InvalidInstruction(currentline);
        if (countLevel < 0) 
            throw UnknownBlock();

        countSlot = 0;
    }
        if (countLevel > 0) throw UnclosedBlock(countLevel);
}

//// Vector //////////////////////////////////////////////
template<class T>
Vector<T>::Vector() {
    this->Size = 0;
    this->capacity = 50;
    this->arr = new T[capacity];
}

template<class T>
Vector<T>::Vector(const int& newsize) {
    this->Size = newsize;
    this->capacity = newsize+10;
    this->arr = new T[newsize];
}

template<class T>
Vector<T>::~Vector() {
    delete[] this->arr;
    this->arr = nullptr;
    this->Size = 0;
    this->capacity = 0;
}

template<class T>
void Vector<T>::ensureCap() {
    T* old = arr;
    this->arr = new T[this->capacity+10];
    for (int i = 0; i < this->Size; i++) {
        this->arr[i] = old[i];
    }
    delete[] old;
    this->capacity += 10;
}

template<class T>
int Vector<T>::size() {
    return this->Size;
}

template<class T>
T& Vector<T>::operator [] (int index) {
    return this->arr[index];
}

template<class T>
Vector<T>& Vector<T>::operator = (Vector<T>& new_vec) {
    if (this != &new_vec) {
        delete[] this->arr;
        this->arr = new T[new_vec.capacity];
        for (int i = 0; i < new_vec.Size; i++) {
            this->arr[i] = new_vec[i];
        }
        this->Size = new_vec.size();
        this->capacity = new_vec.capacity;
    }
    return *this;
}

template<class T>
void Vector<T>::push_back(T newElement) {
    if (this->Size+1 >= this->capacity) {
        ensureCap();
    }
    this->Size += 1;
    this->arr[Size-1] = newElement;
}

template<class T>
int Vector<T>::find(const int& level) {
    for (int i = 0; i < this->Size; i++) {
        if (this->arr[i].level == level) return i;
    }
    return -1;
}
//// Hash_Symbol ////////////////////////////////////////
bool Hash_Symbol::operator == (Hash_Symbol hs) {
    if (this->level != hs.level) return false;
    return (this->identifier.compare(hs.identifier) == 0);
}

bool Hash_Symbol::operator != (Hash_Symbol hs) {
    if (this->level != hs.level) return true;
    return (this->identifier.compare(hs.identifier) != 0);
}

// Hash_Symbol::~Hash_Symbol() {
//     if (this->arguments != nullptr) {
//         delete[] this->arguments;
//         this->arguments = nullptr;
//     }
// }
//// Hash_Table /////////////////////////////////////////
template<class T>
T Hash_Table::Hash_Function(const T& key, const T& m) {
    return key % m;
}

template<class T>
T Hash_Table::Hash_Function_Double(const T& key, const T& m) {
    return 1 + (key % (m - 2));
}

template<class T>
T Hash_Table::Detection_Linear(Hash_Symbol*& key, const std::string& error) {
    T hfunc = this->Hash_Function<T>(key->encode_id, this->m);
    T hp;
    bool found = false;
    T i = 0;

    for (; i < this->m && !found; i++) {
        hp = (hfunc + this->c*i) % m;
        if (this->vec[hp] == *key) {
            delete[] key->arguments;
            delete key;
            throw Redeclared(this->vec[hp].identifier);
        }        
        if (this->vec[hp].index == -1) {
            this->vec[hp] = *key;
            this->vec[hp].index = hp;
            found = true;
            /* delete[] key->arguments; */
            /* delete key; */
        }
    }
    if ((*key).numofParameter != -1 && (*key).level != 0) {
        delete key;
        throw InvalidDeclaration(this->vec[hp].identifier);
    }

    if (!found) {
        delete[] key->arguments;
        delete key;
        throw Overflow(error);
    }    
    return --i;
}

template<class T>
T Hash_Table::Detection_Quadratic(Hash_Symbol*& key, const std::string& error) {
    T hfunc = this->Hash_Function<T>(key->encode_id, this->m);
    T hp, i = 0;
    Hash_Symbol hs;
    bool found = false;

    for (; i < this->m && !found; i++) {
        hp = (hfunc + this->c1*i + this->c2*i*i) % m;
        if (this->vec[hp] == *key) {
            delete[] key->arguments;
            delete key;
            throw Redeclared(this->vec[hp].identifier);

        }        
        if (this->vec[hp].index == -1) {
            if ((*key).numofParameter != -1) {
                /* Hash_Symbol temp((*key).identifier, (*key).encode_id, (*key).numofParameter, 0); */
                /* hs = temp; */
            }
            this->vec[hp] = *key;
            // std::cout << this->vec[hp].arguments[0] << '\n';
            this->vec[hp].index = hp;
            found = true;
            // delete[] key->arguments;
            // std::cout << this->vec[hp].arguments[0] << '\n';
            // delete key;
            // std::cout << this->vec[hp].arguments[0] << '\n';
        }
    }
    if ((*key).numofParameter != -1 && (*key).level != 0) {
        delete key;
        throw InvalidDeclaration(this->vec[hp].identifier);
    }

    if (!found) {
        delete[] key->arguments;
        delete key;
        throw Overflow(error);
    }    
    return --i;
}

template<class T>
T Hash_Table::Detection_Double(Hash_Symbol*& key, const std::string& error) {
    T hfunc1 = this->Hash_Function<T>(key->encode_id, this->m);
    T hfunc2 = this->Hash_Function_Double<T>(key->encode_id, this->m);
    T hp, i = 0;
    bool found = false;

    for (; i < this->m && !found; i++) {
    hp = (hfunc1 + hfunc2*i*this->c) % m;
        if (this->vec[hp] == *key) {
            delete[] key->arguments;
            delete key;
            throw Redeclared(this->vec[hp].identifier);
        }        
        if (this->vec[hp].index == -1) {
            this->vec[hp] = *key;
            this->vec[hp].index = hp;
            found = true;
            // delete[] key->arguments;
            // delete key;
        }
    }
    if ((*key).numofParameter != -1 && (*key).level != 0) {
        delete key;
        throw InvalidDeclaration(this->vec[hp].identifier);
    }

    if (!found) {
        delete[] key->arguments;
        delete key;
        throw Overflow(error);
    }    
    return --i;
}

template<class T>
T Hash_Table::Search(Hash_Symbol& key, int& index) {
    if (this->type_Method == "Linear") {
        T hfunc = this->Hash_Function<T>(key.encode_id, this->m);
        T hp, i = 0;

        for (; i < this->m; i++) {
            hp = (hfunc + this->c*i) % m;
            if (this->vec[hp] == key) {
                index = hp;
                return i;
            }        
        }
        index = -1;
        return index;
    }
    else if (this->type_Method == "Quadratic") {
        T hfunc = this->Hash_Function<T>(key.encode_id, this->m);
        T hp, i = 0;

        for (; i < this->m; i++) {
            hp = (hfunc + this->c1*i + this->c2*i*i) % m;
            if (this->vec[hp] == key) {
                index = hp;
                return i;
            }        
        };
        index = -1;
        return index;
    }
    else if (this->type_Method == "Double") {
        T hfunc1 = this->Hash_Function<T>(key.encode_id, this->m);
        T hfunc2 = this->Hash_Function_Double<T>(key.encode_id, this->m);
        T hp, i = 0;

        for (; i < this->m; i++) {
            hp = (hfunc1 + hfunc2*i*this->c) % m;
            if (this->vec[hp] == key) {
                index = hp;
                return i;
            }        
        }
        index = -1;
        return index;
    }
    return -1;
}

Hash_Table::~Hash_Table() {
    for (int i = 0; i <= vec.size(); i++) {
        if (this->vec[i].numofParameter != -1) {
            delete[] this->vec[i].arguments;
        }
    }
}
