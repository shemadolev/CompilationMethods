#include "code_class.hpp"
#include "part3_helpers.hpp"

string CodeClass::spreadString(string str) {
    return str;
}

string CodeClass::spreadString(int num) {
    return to_string(num);
}

template<typename T, typename... Args>
string CodeClass::spreadString(T t, Args... args) {
    return spreadString(t) + string(" ") + spreadString(args...) ;
}

template<typename... Args>
int CodeClass::emit(Args... args){
    string code = spreadString(args...);
    codeVec.push_back(code);
    return codeVec.size();
}

int CodeClass::nextQuad(){
    return codeVec.size() + 1;
}

CodeLineList::CodeLineList(){}

CodeLineList::CodeLineList(int line){
    this->push_back(line);
}

void CodeClass::backpatch(CodeLineList lineList, int lineNo){
    //todo Make sure linker replaces 'goto -1' commands
    const string replaceFrom = " -1";
    string replaceTo = " " + to_string(lineNo);
    for(int codeLine : lineList){
        if(!replace(codeVec[codeLine - 1], replaceFrom, replaceTo)){
            cerr << "codeClass::backpatch Error parsing line to backpatch:" << endl
                << "Line " << codeLine << ": " << codeVec[codeLine - 1] << endl;
            exit(1);
        }
    }
}

void CodeClass::print(ofstream& file){
    for(string c : codeVec){
        file << c << endl;
    }
}
