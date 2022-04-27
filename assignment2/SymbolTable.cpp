#include "SymbolTable.h"

SplayNode* SplayTree::rotateRight(SplayNode* root) {
    SplayNode* A = root;
    SplayNode* B = root->left;
    SplayNode* C = B->right;
    B->right = root;
    A->left = C;
    return B;
}

SplayNode* SplayTree::rotateLeft(SplayNode* root) {
    SplayNode* A = root;
    SplayNode* B = root->right;
    SplayNode* C = B->left;
    B->left = root;
    A->right = C;
    return B;
}

int checkID(SplayNode* root, const SplayNode& key, int& count_comp) {
    
    count_comp++;

    if (key.scopeLevel == root->scopeLevel) {

        return strcmp(&root->identifier[0], &key.identifier[0]);

    }
    else return (key.scopeLevel < root->scopeLevel) ? 1 : -1; 
}

SplayNode* SplayTree::Splay(SplayNode* root, const SplayNode& key, int& count_splay, int& count_comp, const std::string& error) {

    if (root == NULL) return root;

    if (checkID(root, key, --count_comp) == 1) {  // key < root->key

        if (root->left == NULL) return root;

        if (checkID(root->left, key, --count_comp) == 1) {    // key < root->left->key
            
            root->left->left = Splay(root->left->left, key, count_splay, count_comp, error);
            
            root = rotateRight(root);
            // count_splay++;
        }
        else if (checkID(root->left, key, --count_comp) == -1) {  // key > root->left->key

            root->left->right = Splay(root->left->right, key, count_splay, count_comp, error);

            if (root->left->right != NULL) {
                // count_splay++;
                root->left = rotateLeft(root->left);
            }
        }
        // check NULL after first rotate
        if (root->left == NULL) return root;
        else {
            // count_splay++;
            return rotateRight(root);
        }
    }
    else if (checkID(root, key, --count_comp) == -1) {    // key > root->key
        
        if (root->right == NULL) return root;

        if (checkID(root->right, key, --count_comp) == -1) {  // key > root->right->key

            root->right->right = Splay(root->right->right, key, count_splay, count_comp, error);

            root = rotateLeft(root);
            // count_splay++;
        }
        else if (checkID(root->right, key, --count_comp) == 1) {  // key < root->right->key

            root->right->left = Splay(root->right->left, key, count_splay, count_comp, error);

            if (root->right->left != NULL) {
                // count_splay++;
                root->right = rotateRight(root->right);
            }

        }
        // check NULL after first rotate
        if (root->right == NULL) return root;
        else {
            // count_splay++;
            return rotateLeft(root);
        }
    }
    // else if (checkID(root, key, --count_comp) == 0) throw Redeclared(error);
    // count_comp--;
    return root;
}

SplayNode* SplayTree::Insert1(SplayNode* root, const SplayNode& key, int& count_splay, int& count_comp, const std::string& error) {

    SplayNode* n = new SplayNode(key.identifier, key.scopeLevel, key.type);

    if (root == NULL) {
        root = n;
    }
    else {
        root = Splay(root, key, ++count_splay, count_comp, error);
        if (checkID(root, key, --count_comp) == 0) {
            throw Redeclared(error);
        }
        else if (checkID(root, key, --count_comp) == -1) {
            n->left = root;
            n->right = root->right;
            root->right = NULL; 
        }
        else {
            n->right = root;
            n->left = root->left;
            root->left = NULL;
        }
    }
    return n;

}

SplayNode* SplayTree::Insert(SplayNode* root, const SplayNode& key, int& count_splay, int& count_comp, const std::string& error) {
    
    SplayNode* n = new SplayNode(key.identifier, key.scopeLevel, key.type);
    SplayNode* temp = root, *save = NULL;

    while (temp != NULL) {
        save = temp;
        if (checkID(temp, key, --count_comp) == 1) temp = temp->left; 
        else if (checkID(temp, key, --count_comp) == -1)temp = temp->right;
        else throw Redeclared(error);
        count_comp++;
    }

    if (save == NULL) {
        root = n;
        return root;
    }
    else if (checkID(save, key, --count_comp) == -1) save->right = n;
    else save->left = n;

    return Splay(root, key, ++count_splay, count_comp, error);

}

bool SymbolTable::check_type_Function(const std::string& s) {
    if (s[0] == '(') {
        int end = 0, start = 1;
        bool found = false;
        while (s[start] != ')' && !found) {
            end = (int)s.find(",", start);
            if (end == -1)  {
                end = (int)s.find(")");
                if (end == -1) return false;
                found = true;
            }
            std::string tp = s.substr(start, end - start);
            if (tp != "string" && tp != "number") return false;
            start = end + 1;    
        }
        if (s[start] == ')') start++;

        if (s[start] != '-' && s[start+1] != '>') return false;
        else start += 2;
        std::string tp = s.substr(start, s.size() - start);
        if (tp != "string" && tp != "number") return false;
        return true;
    }
    else return false;
}

bool SymbolTable::checkValue(const std::string& s, std::string& type) {
    std::regex ex1("[[:digit:]]+");
    std::regex ex2("'[[:alnum:],' ']*'");
    if (regex_match(s, ex1)) {
        type = "number";
        return true;
    }
    else if (regex_match(s, ex2)) {
        type = "string";
        return true;
    }
    return false;
}

SplayNode* SplayTree::DeleteNode(SplayNode* root, const SplayNode& key, int& count_splay, int& count_comp, const std::string& error) {
    if (root == NULL) return root;
    SplayNode* temp = NULL;
    root = Splay(root, key, ++count_splay, count_comp, error);
    if (checkID(root, key, --count_comp) == 0) {
        if (root->left == NULL) {
            temp = root;
            root = root->right;
        }
        else {
            temp = root;
            root = Splay(root->left, key, ++count_splay, count_comp, error);
            root->right = temp->right;
        }
        free(temp);
        return root;
    }
    else return root;
}

SplayNode* SplayTree::findNode(SplayNode* root, const std::string& id, const int& countScope, int& count_splay, int& count_comp, const std::string& error) {
    if (root == NULL) return root;

    SplayNode* temp = NULL, * save = NULL;
    bool found = false;
    int cur_comp = 0;

    for (int i = countScope; i >= 0 && !found; i--) {
        SplayNode key(id, i);
        cur_comp = 0;
        temp = root;
        // root = Splay(root, key, ++count_splay, count_comp, error);
        // if (checkID(root, key, --count_comp) == 0) {
        //     return root;
        // }
        while (temp != NULL && !found) {
            save = temp;
            if (checkID(temp, key, --count_comp) == 1) temp = temp->left;
            else if (checkID(temp, key, --count_comp) == -1) temp = temp->right;
            else found = true;
            cur_comp++;  
        }
        if (found) root = Splay(root, key, count_splay, count_comp, error);
        
    }
    count_comp += cur_comp;
    return root;
}

void SplayTree::PrintTree(SplayNode* root) {
    if (root == NULL) return;
    std::cout << root->identifier << "//" << root->scopeLevel << " ";
    PrintTree(root->left);
    PrintTree(root->right);
}

void SymbolTable::run(string filename)
{
    std::string str = "";
    int countScope = 0, count_splay = 0, count_comp = 0, save = 0;
    SplayTree tree;

    SplayNode* saveNode[MaxScope];
    for (int i = 0; i < MaxScope; i++) saveNode[i] = NULL;
    int sizeScope[MaxScope]{};

    std::ifstream file(filename);

    while (getline(file, str)) {
        int start = 0, end = 0;
        bool isValid = false, isNum = false, isString = false, isFunc = false, passById = false;

        end = (int)str.find(" ");
        std::string tmp = str.substr(start, end);

        if (tmp == "INSERT") {
            start = end + 1;    end = (int)str.find(" ", start);
            std::string id = str.substr(start, end - start);

            bool checkId = [=]() ->bool {
                std::regex ex("[[:lower:]][[:w:]]*");
                return regex_match(id, ex);
            }();

            if (!checkId) throw InvalidInstruction(str);

            start = end + 1;    end = (int)str.find(" ", start);
            std::string tp = str.substr(start, end - start);

            start = end + 1;
            std::string sc = str.substr(start, str.size() - start);

            if (tp == "number" || tp == "string") isValid = true;
            if (!check_type_Function(tp) && !isValid) throw InvalidInstruction(str);

            if (sc == "true") {
                SplayNode node(id, 0, tp);

                if (saveNode[0] == NULL) {
                    saveNode[0] = new SplayNode[MaxScope];
                }
                saveNode[0][sizeScope[0]] = node;
                sizeScope[0]++;

                root = tree.Insert(root, node, count_splay, count_comp, str);
            }
            else if (sc == "false") {
                if (countScope > 0 && check_type_Function(tp)) throw InvalidInstruction(str);
                
                SplayNode node(id, countScope, tp);

                if (saveNode[countScope] == NULL) {
                    saveNode[countScope] = new SplayNode[MaxScope];
                }
                saveNode[countScope][sizeScope[countScope]] = node;
                sizeScope[countScope]++;

                root = tree.Insert(root, node, count_splay, count_comp, str);
            }
            std::cout << count_comp << " " << count_splay << std::endl;
        }
        else if (tmp == "ASSIGN") {
            start = end + 1;  end = (int)str.find(" ", start);
            std::string id = str.substr(start, end - start);
            std::string funcId = "", val = "";

            start = end + 1;  end = (int)str.find("(", start);
            if (end == -1) {
                val = str.substr(start, str.size() - start);

                isNum = [=]() ->bool {
                    std::regex ex("[[:digit:]]+");
                    return regex_match(val, ex);
                }();

                if (!isNum) {
                    isString = [=]() ->bool {
                        std::regex ex("'[[:alnum:],' ']*'");
                        return regex_match(val, ex);
                    }();
                }

                if (!isNum && !isString) {
                    passById = [=]() ->bool {
                        std::regex ex("[[:lower:]][[:w:]]*");
                        return regex_match(val, ex);
                    }();
                }
            }
            else isFunc = true;

            if (isNum) {
                root = tree.findNode(root, id, countScope, ++count_splay, count_comp, str);

                // root = tree.Splay(root, key, count_splay, count_comp, str);
                if (root->identifier != id) throw Undeclared(str);
                if (root->type != "number") throw TypeMismatch(str);
            }
            else if (isString) {
                root = tree.findNode(root, id, countScope, ++count_splay, count_comp, str);

                // root = tree.Splay(root, key, count_splay, count_comp, str);
                if (root->identifier != id) throw Undeclared(str);
                if (root->type != "string") throw TypeMismatch(str);
            }
            else if (isFunc) {
                funcId = str.substr(start, end - start);
                root = tree.findNode(root, funcId, countScope, ++count_splay, count_comp, str);
                // root = tree.Splay(root, SplayNode(funcId, 0), count_splay, count_comp, str);

                if (root->identifier == funcId) {
                    std::string tp = root->type, temp = "", temp1 = "", type = "";
                    bool found = false;
                    int st = 1, ed = 0;
                    start = end + 1;

                    while(str[start] != ')' && tp[st] != ')') {
                        end = (int)str.find(",", start);
                        ed = (int)tp.find(",", st);

                        if (end == -1 && ed == -1) {
                            end = (int)str.find(")", start);
                            ed = (int)tp.find(")", st);
                            if (end == -1) throw TypeMismatch(str);
                            found = true;
                        }
                        else if (end == -1 && ed != -1) throw TypeMismatch(str);
                        else if (end != -1 && ed == -1) throw TypeMismatch(str);

                        temp = str.substr(start, end - start);
                        temp1 = tp.substr(st, ed - st);

                        if (checkValue(temp, type)) {
                            if (type != temp1) throw TypeMismatch(str);
                        }
                        if (!found) {
                            start = end + 1;
                            st = ed + 1;
                        }
                        else {
                            start = end;  st = ed;
                        }
                    }

                    st += 3;
                    temp1 = tp.substr(st, tp.size() - st);

                    root = tree.findNode(root, id, countScope, count_splay, count_comp, str);

                    // root = tree.Splay(root, key, count_splay, count_comp, str);

                    if (root->identifier != id) throw Undeclared(str);
                    if (temp1 != root->type) throw TypeMismatch(str);

                }
                else throw Undeclared(str);
            }
            else if (passById) {
                root = tree.findNode(root, val, countScope, ++count_splay, count_comp, str);
                // root = tree.Splay(root, key, count_splay, count_comp, str);

                if (root->identifier != val) throw Undeclared(str);

                std::string saveType = root->type;

                root = tree.findNode(root, id, countScope, count_splay, count_comp, str);
                // root = tree.Splay(root, key, count_splay, count_comp, str);

                if (root->identifier != id) throw Undeclared(str);
                if (root->type != saveType) throw TypeMismatch(str);
            }
            else throw TypeMismatch(str);
            std::cout << count_comp << " " << count_splay << std::endl;
            
        }
        else if (tmp == "BEGIN") {
            countScope++;
        }
        else if (tmp == "END") {
            
            for (int i = 0; i < sizeScope[countScope]; i++) {
                root = tree.DeleteNode(root, saveNode[countScope][i], count_splay, count_comp, str);
            }
            delete[] saveNode[countScope];
            countScope--;
        }
        else if (tmp == "LOOKUP") {
            start = end + 1;
            std::string id = str.substr(start, str.size() - start);
            root = tree.findNode(root, id, countScope, ++count_splay, count_comp, str);
            if (root->identifier != id) throw Undeclared(str);
            std::cout << root->scopeLevel << std::endl;
        }
        else if (tmp == "PRINT") {
            tree.PrintTree(root);
            std::cout << std::endl;
        }

        count_comp = count_splay = 0;

        if (countScope < 0) throw UnknownBlock();

    }
    if (countScope > 0) throw UnclosedBlock(countScope);
    
    for (int i = 0; i < sizeScope[countScope]; i++) {
        root = tree.DeleteNode(root, saveNode[countScope][i], count_splay, count_comp, str);
    }
    delete[] saveNode[countScope];

    file.close();
    return;
}
