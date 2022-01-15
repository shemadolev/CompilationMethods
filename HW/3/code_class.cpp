#include "code_class.hpp"


bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

int codeClass::emit(string c){
    codeVec.push_back(c);
    return codeVec.size();
}


int codeClass::nextquad(){
    return codeVec.size() + 1;
}


void codeClass::backpatch(bpList lineList, int lineNo){
    const string replaceFrom = " 0";
    for(int codeLine : lineList){
        string replaceTo = " " + to_string(lineNo);
        if(!replace(codeVec[codeLine - 1], replaceFrom, replaceTo)){
            cerr << "codeClass::backpatch Error parsing line to backpatch:" << endl
                << "Line " << codeLine << ": " << codeVec[codeLine - 1] << endl;
        }
    }
}

void codeClass::print(){
    for(string c : codeVec){
        cout << c << endl;
    }
}
