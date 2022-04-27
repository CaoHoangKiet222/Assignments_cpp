#include <iostream>
#include <string.h>

int main() {
    std::string s1 = "b1", s2 = "b1";
    int* i = new int[50];

    std::cout << (int)s1.find("");
    delete[] i;
}
