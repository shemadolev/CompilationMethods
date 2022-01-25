#include "symbol_table.hpp"
#include "code_class.hpp"

extern CodeClass code; //defined in parser

/**
 * VarScopeTable implementations
 */

VarScopeTable::VarScopeTable() : 
    intTable(SAVED_REGS_INT, eINT), floatTable(SAVED_REGS_FLOAT, eFLOAT) {}

VarScopeTable::VarScopeTable(VarScopeTable& prevTable) : 
    intTable(prevTable.intTable.getLastVarOffest(), eINT), floatTable(prevTable.floatTable.getLastVarOffest(), eFLOAT) {}

string
VarScopeTable::newTemp(idTypes type){
    if(type == eVOID)
        return "-1";
    return (type == eINT) ? intTable.newTemp() : floatTable.newTemp();
}

string
VarScopeTable::newVar(string id, idTypes type){
    if(type == eVOID)
        return "-1";
    return (type == eINT) ? intTable.newVar(id) : floatTable.newVar(id);
}

bool
VarScopeTable::lookup(VarEntry& var, string id){
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


/**
 * @brief Emit the ASM code line for storing the registers in range & 
 *          Update the $SP to the next available block in the stack.
 * 
 * @param firstReg 
 * @param lastReg 
 * @param table_type 
 */
void emitStoreIds(int firstReg, int lastReg, idTypes table_type){
    string type = (table_type == eINT) ? "I" : "F";
    int regs_num = lastReg - firstReg + 1;
    //Store registers
    for (int i=0; i<regs_num; i++){
        code.emit(string("STOR")+type, type+to_string(i), $SP, to_string(i));
    }
    //Update $SP
    code.emit("ADD2I", $SP, $SP, regs_num);
}

void 
VarScopeTable::storeIds(){
    // int vars
    emitStoreIds(SAVED_REGS_INT, intTable.getLastVarOffest(), eINT);
    // float vars
    emitStoreIds(SAVED_REGS_FLOAT, floatTable.getLastVarOffest(), eFLOAT);
    // int tmps
    emitStoreIds(intTable.getLastTmpOffest(), intTable.getSize()-1, eINT);
    // float tmps
    emitStoreIds(floatTable.getLastTmpOffest(), floatTable.getSize()-1, eFLOAT);
}


/**
 * @brief Emit the ASM code line for storing the registers in range & 
 *          Update the $SP to the next available block in the stack.
 * 
 * @param firstReg 
 * @param lastReg 
 * @param table_type 
 */
void emitLoadIds(int firstReg, int lastReg, idTypes table_type){
    string type = (table_type == eINT) ? "I" : "F";
    int regs_num = lastReg - firstReg + 1;
    //Update $SP
    code.emit("SUBTI", $SP, $SP, regs_num);
    //Store registers
    for (int i=0; i<regs_num; i++){
        code.emit(string("LOAD")+type, type+to_string(i), $SP, to_string(i));
    }
}


void 
VarScopeTable::loadIds(){
    // float tmps
    emitLoadIds(floatTable.getLastTmpOffest(), floatTable.getSize()-1, eFLOAT);
    // int tmps
    emitLoadIds(intTable.getLastTmpOffest(), intTable.getSize()-1, eINT);
    // float vars
    emitLoadIds(SAVED_REGS_FLOAT, floatTable.getLastVarOffest(), eFLOAT);
    // int vars
    emitLoadIds(SAVED_REGS_INT, intTable.getLastVarOffest(), eINT);
}

/**
 * TypedVarScopeTable implementations
 */

TypedVarScopeTable::TypedVarScopeTable(int startingIndex, idTypes type):
        _curVarOffset(startingIndex), _curTempOffset(size-1), _type(type){
    _typeLetter = (type == eINT) ? "I" : "F";            
}

string
TypedVarScopeTable::newTemp(){
    return _typeLetter + to_string(_curTempOffset--);
}

string
TypedVarScopeTable::newVar(string id){
    _varEntries.insert(pair<string,int>(id,_curVarOffset));
    return _typeLetter + to_string(_curVarOffset++);
}

bool 
TypedVarScopeTable::lookup(VarEntry& var, string id){
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

int 
TypedVarScopeTable::getLastVarOffest(){
    return _curVarOffset-1; //last reg that stores variable
}

int 
TypedVarScopeTable::getLastTmpOffest(){
    return _curTempOffset+1; //last reg that stores tmp
}

/**
 * VariableTable implementations
 */
void 
VariableTable::push(){
    if(_tables.size() == 0){ 
        VarScopeTable newTable;
        _tables.push_front(newTable);
    } else {
        VarScopeTable newTable(_tables.front());
        _tables.push_front(newTable);
    }
}

void 
VariableTable::pop(){
    _tables.pop_front();
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
    return front().lookup(VarEntry(),id);
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
        str += " ";
        str += func.props.id;
        for(int line : func.callList){
            str += ",";
            str += to_string(line);
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
