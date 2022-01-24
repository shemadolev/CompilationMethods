#include "symbol_table.hpp"
#include "code_class.hpp"

extern CodeClass code; //defined in parser

//TODO: change the global pointer to stack <varscopetable>
// update lookup func, typedTables don't need *parent.

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
    return inIntTable || inFloatTable;
}

void
VarScopeTable::resetTmps(){
    intTable.resetTmps();
    floatTable.resetTmps();
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
    return false;
}

void
TypedVarScopeTable::resetTmps(){
    _curTempOffset = size-1;
}

bool VariableTable::lookupVarTableList(varEntry& var, string id){
    // Assuming a new table is always inserted in the front of the list, 
        // iterating from the head of the list ensures the right ordering for looking-up
    for (auto it = _tables.begin(); it != _tables.end(); it++){
        if(it->lookup(var, id)) //found the variable within this table
            return true;
    }
    //didn't find the variable in any var table from the list
    return false;
}

SymbolEntry_Function::SymbolEntry_Function(FunctionProps& funcProps) : _definitionLine(-1) , props(funcProps) {}

string SymbolEntry_Function::getPlace(int currentLine){
    if(!isDefined()){
        callList.push_back(currentLine);
    }
    return to_string(_definitionLine); //will be "-1" if not defined
}

void SymbolEntry_Function::define(int definitionLine){
    _definitionLine = definitionLine;
    code.backpatch(callList, definitionLine);
}

bool SymbolEntry_Function::isDefined(){
    return (_definitionLine != -1);
}

bool SymbolEntry_Function::matchExisting(FunctionProps& funcProps){
    if(funcProps.type != props.type || funcProps.id != props.id || funcProps.args.size() != props.args.size())
        return false;
    
    for(auto myIt = props.args.begin(), otherIt = funcProps.args.begin();myIt != props.args.end(); myIt++,otherIt++){
        if(myIt->type != otherIt->type) //different arg names allowed
            return false;
    }

    return true;
}