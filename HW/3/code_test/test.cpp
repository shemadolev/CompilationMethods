#include "code_class.hpp"
#include <iostream>
using namespace std;
#include <string>
#include <list>

int main(){

    codeClass myCode;
    myCode.emit("JLINK 0");
    myCode.emit("HALT");

    bpList lineList;
    lineList.push_back(1);

    myCode.backpatch(lineList, 10);

    myCode.print();

    return 0;
}