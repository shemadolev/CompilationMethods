#include "part3_helpers.hpp"

extern CodeClass code; //defined in parser

/**
 * TypedVarScopeTable implementations
 */

TypedVarScopeTable::TypedVarScopeTable(idTypes type):
        _curTempOffset((type == eINT) ? SAVED_REGS_INT : SAVED_REGS_FLOAT),
        _tempStartingIndex((type == eINT) ? SAVED_REGS_INT : SAVED_REGS_FLOAT), 
        _type(type){
    _typeLetter = (type == eINT) ? "I" : "F";            
}

string
TypedVarScopeTable::newTemp(){
    return _typeLetter + to_string(_curTempOffset++);
}

void
TypedVarScopeTable::newVar(string id, int offset){
    _varEntries.insert(pair<string,int>(id,offset));
 //   cout << "new var " << id << ", " << offset << endl; //debug
}

bool 
TypedVarScopeTable::lookup(VarEntry& var, string id){
    auto it = _varEntries.find(id);
    if(it != _varEntries.end()){ // id is within the symbol table
        var.type = _type;
        var.offset = it->second;
   //     cout << "found var " << id << " in offset " << var.offset << ", type=" << _type << endl; //debug
        return true;
    }
    return false;
}

void
TypedVarScopeTable::resetTmps(){
    _curTempOffset = _tempStartingIndex;
}

void
TypedVarScopeTable::emitStoreIds(){
    int regs_num = _curTempOffset - _tempStartingIndex;
    string regSP = SP;
    if(regs_num > 0){
        if(_type == eFLOAT){
            //Cast SP to a SP_F
            code.emit("CITOF",SP_F, SP);
            regSP = SP_F;
        }
        //Store registers
        for (int i=0; i<regs_num; i++){
            code.emit(string("STOR")+_typeLetter + " " +  _typeLetter + to_string(_tempStartingIndex + i) + " " + regSP + " " + to_string(i * VAR_SIZE));
        }
        //Update SP
        code.emit(string("ADD2I ") + SP + " " + SP + " " + to_string(regs_num * VAR_SIZE));
    }
}

void
TypedVarScopeTable::emitLoadIds(){
    int regs_num = _curTempOffset - _tempStartingIndex;
    if(regs_num > 0){
        //Update SP
        code.emit(string("SUBTI ") + SP + " " + SP + " " + to_string(regs_num * VAR_SIZE));
        string regSP = SP;
        if(_type == eFLOAT){
            //Cast SP to a SP_F
            code.emit("CITOF",SP_F, SP);
            regSP = SP_F;
        }
        //Store registers
        for (int i=0; i<regs_num; i++){
            code.emit(string("LOAD")+_typeLetter + " " +  _typeLetter + to_string(_tempStartingIndex + i) + " " + regSP + " " + to_string(i * VAR_SIZE));
        }
    }
}

/**
 * VarScopeTable implementations
 */

VarScopeTable::VarScopeTable(int startingOffset) : 
    _curVarOffset(startingOffset), _varStartingIndex(startingOffset), intTable(eINT), floatTable(eFLOAT) {}

string
VarScopeTable::newTemp(idTypes type){
    if(type == eVOID)
        return "-1";
    return (type == eINT) ? intTable.newTemp() : floatTable.newTemp();
}

int
VarScopeTable::newVar(string id, idTypes type){
    if(type == eVOID)
        return -1;
    (type == eINT) ? intTable.newVar(id, _curVarOffset) : floatTable.newVar(id, _curVarOffset);
    _curVarOffset+=VAR_SIZE;
    return _curVarOffset;
}

bool
VarScopeTable::lookup(VarEntry& var, string id){
    bool inIntTable = intTable.lookup(var, id);
    bool inFloatTable = floatTable.lookup(var, id);
    assert(!(inIntTable && inFloatTable));
    return inIntTable || inFloatTable;
}

void
VarScopeTable::resetTmps(){
    intTable.resetTmps();
    floatTable.resetTmps();
}

void 
VarScopeTable::storeIds(){
    intTable.emitStoreIds();
    floatTable.emitStoreIds();
}

void 
VarScopeTable::loadIds(){
    floatTable.emitLoadIds();
    intTable.emitLoadIds();
}

void
VarScopeTable::freeStack(){
    if(_curVarOffset > _varStartingIndex)
        code.emit(string("SUBTI ") + SP + " " + SP + " " + to_string(_curVarOffset));
}

int VarScopeTable::getCurOffset(){
    return _curVarOffset;
}

/**
 * VariableTable implementations
 */
void 
VariableTable::push(){
    int stackOffset = (_tables.size() > 0) ? _tables.front().getCurOffset() : 0; 
    VarScopeTable newTable(stackOffset);
    _tables.push_front(newTable);
   // cout << "Block #" << _tables.size() << " w/ stackOffset " << stackOffset << endl;
}

void 
VariableTable::pop(){
    _tables.front().freeStack();
    _tables.pop_front();
    if(_tables.size())
        _tables.front().resetTmps();
}

VarScopeTable& 
VariableTable::front(){
    return _tables.front();
}

bool 
VariableTable::lookupVarTableList(VarEntry& var, string id){
    // Assuming a new table is always inserted in the front of the list, 
        // iterating from the head of the list ensures the right ordering for looking-up
    for (auto it = _tables.begin(); it != _tables.end(); it++){
        if(it->lookup(var, id)) //found the variable within this table
            return true;
    }
    //didn't find the variable in any var table from the list
    return false;
}

void 
VariableTable::setFunctionApi(list<ArgDeclaration> &args){
    functionArgs = args;
}

void 
VariableTable::storeIds(){
    // get the offset of named & temp from each int/float table, for i: store all in mem
    _tables.front().storeIds();
}

void 
VariableTable::loadIds(){
    _tables.front().loadIds();
}

bool
VariableTable::isInScope(string id){
    VarEntry tmp;
    return front().lookup(tmp,id);
}

/**
 * FunctionEntry implementation
 */
FunctionEntry::FunctionEntry(FunctionProps& funcProps) : _definitionLine(-1) , props(funcProps) {}

string 
FunctionEntry::getPlace(int currentLine){
    if(!isDefined()){
        callList.push_back(currentLine);
    }
    return to_string(_definitionLine); //will be "-1" if not defined
}

void 
FunctionEntry::define(int definitionLine){
    _definitionLine = definitionLine;
    code.backpatch(callList, definitionLine);
}

bool 
FunctionEntry::isDefined(){
    return (_definitionLine != -1);
}

bool 
FunctionEntry::matchExisting(FunctionProps& funcProps){
    if(funcProps.type != props.type || funcProps.id != props.id || funcProps.args.size() != props.args.size())
        return false;
    
    for(auto myIt = props.args.begin(), otherIt = funcProps.args.begin();myIt != props.args.end(); myIt++,otherIt++){
        if(myIt->type != otherIt->type) //different arg names allowed
            return false;
    }

    return true;
}

/**
 * FunctionTable implemetation
 */
FunctionEntry*
FunctionTable::find(string id){
    auto it = _functionTable.find(id);
    if(it != _functionTable.end()) //found the function entry
        return &it->second;
    return nullptr;
}

FunctionEntry*
FunctionTable::insert(FunctionProps& funcProps){
    FunctionEntry newFuncEntry(funcProps);
    _functionTable.insert( pair<string,FunctionEntry>(funcProps.id, newFuncEntry));
    // get a pointer to the funcEntry in the map
    auto it = _functionTable.find(funcProps.id);
    assert(it != _functionTable.end());
    return &it->second;
}

FunctionEntry* 
FunctionTable::getCurrent(){
    return _current;
}

void
FunctionTable::setCurrent(FunctionEntry *funcEntry){
    _current = funcEntry;
}

string FunctionTable::getUnimplementedCalls(){
    string str;
    for (auto const& funcIt : _functionTable){
        FunctionEntry func = funcIt.second;
        if(func.callList.size() != 0){
            str += " ";
            str += func.props.id;
            for(int line : func.callList){
                str += ",";
                str += to_string(line);
            }
        }
    }
    return str;
}

string FunctionTable::getImplemented(){
    string str;
    for (auto const& funcIt : _functionTable){
        FunctionEntry func = funcIt.second;
        if(func.isDefined()){
            str += " ";
            str += func.props.id;
            str += ",";
            str += func.getPlace(0);
        }
    }
    return str;
}
