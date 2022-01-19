#include "code_class.hpp"
#include <iostream>
using namespace std;
#include <string>
#include <list>

#define FP string("I3")

int main(){

    CodeClass myCode;
    myCode.emit("JLINK -1");
    myCode.emit("HALT " + FP);

    CodeLineList lineList(1);

    myCode.backpatch(lineList, 25);

    // myCode.print();

    return 0;
}