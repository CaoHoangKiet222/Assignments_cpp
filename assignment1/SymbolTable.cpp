#include "SymbolTable.h"

bool SymbolTable::check_Identifier(string s){
    if(s[0] <= 'z' && s[0] >= 'a') {
        bool check = true;
        for(int i = 1; i < s.size(); i++) {
            if((s[i] <= 'z' && s[i] >= 'a') || (s[i] <= 'Z' && s[i] >= 'A') || (s[i] <= '9' && s[i] >= '0') || s[i] == '_') continue;
            else {check = false; break;}
        }
        return check;
    }
    else return false;
}

void SymbolTable::run(string filename) {
    ifstream file(filename);
    string tmp = "";
    vector<string> identifier, type;
    vector<int> block;
    int count_Begin = 0, cur_Begin = 0;
    while(file) {
        vector<string> cmd;
        getline(file, tmp);
        cmd = tokenize(tmp);
        if(cmd[0] == "INSERT") {
            vector<string>::iterator i = find(identifier.begin(), identifier.end(), cmd[1]);
            if(i != identifier.end() || !check_Identifier(cmd[1])) {
                if(count_Begin == cur_Begin || !check_Identifier(cmd[1])) throw Redeclared(tmp);
                else {
                    cout << "success" << endl;
                    identifier.push_back(cmd[1]);
                    type.push_back(cmd[2]);
                    block.push_back(count_Begin);
                    cur_Begin = count_Begin;
                }
            }
            else {
                cout << "success" << endl;
                identifier.push_back(cmd[1]);
                type.push_back(cmd[2]);
                block.push_back(count_Begin);
                cur_Begin = count_Begin;
            }
        }
        else if(cmd[0] == "ASSIGN") {
            if(check_Identifier(cmd[1])) {
                vector<string>::iterator i = find(identifier.begin(), identifier.end(), cmd[1]);
                if(i != identifier.end()){
                    if(cmd.size() == 2) throw Undeclared(tmp); 
                    bool type_num = true, type_string = true;
                    for(int j = 0; j < cmd[2].size() && type_num; j++) {
                        if(cmd[2][j] <= '9' && cmd[2][j] >= '0') continue;
                        else type_num = false;
                    }

                    if(!type_num) {
                        int size = cmd[2].size();
                        if(size <= 1) type_string = false;
                        else {
                            if((int)cmd[2][0] == 39 && (int)cmd[2][size-1] == 39) {
                                for(int j = 1; j < size-1 && type_string; j++) {
                                    if((cmd[2][j] <= '9' && cmd[2][j] >= '0') || (cmd[2][j] <= 'z' && cmd[2][j] >= 'a') || (cmd[2][j] <= 'Z' && cmd[2][j] >= 'A') || cmd[2][j] == ' ') continue;
                                    else type_string = false;
                                }
                            }
                            else type_string = false;
                        }
                    }
                    else type_string = false;

                    if(type_num && type[i-identifier.begin()] == "number") {
                        cout << "success" << endl;
                    }
                    else if(type_string && type[i-identifier.begin()] == "string") {
                        cout << "success" << endl;
                    }
                    else throw TypeMismatch(tmp);
                    
                }
                else throw Undeclared(tmp);
            }
        }
        else if(cmd[0] == "BEGIN") {
            count_Begin++;
        }
        else if(cmd[0] == "END") {
            count_Begin--;
            if(count_Begin < 0) throw UnknownBlock();
        }
        else if(cmd[0] == "LOOKUP") {
            int size1 = identifier.size();
            bool found = false;
            for(int j = size1-1; j >= 0 && !found; j--) {
                if(identifier[j] == cmd[1]) {
                    found = true;
                    cout << block[j] << endl;
                }
            }
            if(!found) throw Undeclared(tmp);
        }
    }
    file.close();
    if(count_Begin != 0) throw UnclosedBlock(count_Begin);
    return;
}