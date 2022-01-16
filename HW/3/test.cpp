#include "code_class.hpp"
#include <iostream>
using namespace std;
#include <string>
#include <list>

int main(){

    CodeClass myCode;
    myCode.emit("JLINK 0");
    myCode.emit("HALT");

    BpList lineList(1);

    myCode.backpatch(lineList, 10);

    myCode.print();

    return 0;
}