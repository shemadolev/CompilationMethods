#include "symbol_table.hpp"
extern VarScopeTable *varScopeTable;

//TODO: change the global pointer to stack <varscopetable>
// update lookup func, typedTables don't need *parent.

void
VarScopeTable::pushBlock(){
    VarScopeTable* newTable = new VarScopeTable(this);
    newTable->intTable.parent = &intTable;
    newTable->floatTable.parent = &floatTable;
    varScopeTable = newTable; 
}

void
VarScopeTable::popBlock(){
    VarScopeTable *oldTable = varScopeTable;
    varScopeTable = varScopeTable->parent;  
    delete oldTable;
    varScopeTable->intTable.resetTmps();
    varScopeTable->floatTable.resetTmps();
}

string 
VarScopeTable::newVar(string id, idTypes type){
    if(type == eVOID)
        return "-1";
    return (type == eINT) ? intTable.newVar(id) : floatTable.newVar(id);
}

string
VarScopeTable::newTemp(idTypes type){
    if(type == eVOID)
        return "-1";
    return (type == eINT) ? intTable.newTemp() : floatTable.newTemp();
}

bool
VarScopeTable::lookup(varEntry& var, string id){
    bool inIntTable = intTable.lookup(var, id);
    bool inFloatTable = floatTable.lookup(var, id);
    assert(inIntTable && inFloatTable);
    return intTable || inFloatTable;
}

TypedVarScopeTable::TypedVarScopeTable(int startingIndex, idTypes type):
        _curVarOffset(startingIndex), _curTempOffset(size-1), _type(type){
    _typeLetter = (type == eINT) ? "I" : "F";            
}

string
TypedVarScopeTable::newVar(string id){
    string reg = _typeLetter + to_string(_curVarOffset);
    _varEntries.insert(pair<string,int>(reg,_curVarOffset));
    _curVarOffset++;
    return reg;
}

string
TypedVarScopeTable::newTemp(){
    string reg = _typeLetter + to_string(_curTempOffset);
    _varEntries.insert(pair<string,int>(reg,_curTempOffset));
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
        return (parent) ? parent->lookup(var, id) : false;
    }
}

void
TypedVarScopeTable::resetTmps(){
    _curTempOffset = size-1;
}

