#include "code_class.hpp"
#include "part3_helpers.hpp"

CodeClass::CodeClass(){}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
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
