#include "symbol_table.hpp"


string 
VarScopeTable::newVar(string id, idTypes type){
    if(type == eVOID)
        return "-1";
    return (type == eINT) ? intTable->newVar(id) : floatTable->newVar(id);
}

string
VarScopeTable::newTemp(idTypes type){
    if(type == eVOID)
        return "-1";
    return (type == eINT) ? intTable->newTemp() : floatTable->newTemp();
}

bool
VarScopeTable::lookup(varEntry& var, string id){
    //NOTE: maybe can add a check that the same id isn't stored in both table
    bool inIntTable = intTable->lookup(var, id);
    bool inFloatTable = intTable->lookup(var, id);
    //if inInt & inFloat --> operational error
    return intTable || inFloatTable;
}


TypedVarScopeTable::TypedVarScopeTable(int startingIndex, idTypes type):
        _curVarOffset(startingIndex), _curTempOffset(size-1), _type(type){
    _typeLetter = (type == eINT) ? "I" : "F";            
}

string
TypedVarScopeTable::newVar(string id){
    string reg = _typeLetter + to_string(_curVarOffset);
    _verEntries.insert(pair<string,int>(reg,_curVarOffset));
    _curVarOffset++;
    return reg;
}

string
TypedVarScopeTable::newTemp(){
    string reg = _typeLetter + to_string(_curTempOffset);
    _verEntries.insert(pair<string,int>(reg,_curTempOffset));
    _curTempOffset--;
    return reg;
}

bool 
TypedVarScopeTable::lookup(varEntry& var, string id){
    auto it = _varEntries.find(id);
    if(it != _varEntries.end()){ // id is within the symbol table
        var.type = _type;
        string reg = _typeLetter + to_string(it->second);
        var.place = reg;
        return true;
    }
    else{
        // search at the parent
        bool found = (parent) ? parent->lookup(var, id) : false;
        return found;
    }
}