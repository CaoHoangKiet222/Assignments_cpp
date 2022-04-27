#include <exception>
#include <iostream>

using namespace std;

class myException : public exception {
    public:

    myException() {}
    void run(){
        // cout << &throw myException() << endl;
        throw myException();
        return;
    }
    const char* what() const throw() {
        return "C++ exception";
    }
};

void test(int num){
    try {
        if(num > 18) cout << "Access granted - you are old enough." << endl;
        else throw num;
    }
    catch(int myNum){
        cout << "Access denied - You must be at least 18 years old.\n";
        cout << "Error number: " << myNum << endl;
    }
}
void test1(){
    myException* st = new myException();
    try{
        st->run();
    }
    catch(myException& ex){
        cout << ex.what() << endl;
    }
    delete st;
}

int main(){
    test(17);
    test1();
}


