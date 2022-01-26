#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <list>

#include "part3_helpers.hpp"
#include "code_class.hpp"
#include <cassert>

using namespace std;

#define SAVED_REGS_INT 4
#define SAVED_REGS_FLOAT 1

//Size of variable in bytes
#define VAR_SIZE 4

class VarEntry{
public:
    int offset;
    idTypes type;
};

class TypedVarScopeTable{
protected:
    int _curVarOffset;
    int _curTempOffset;
    int startingIndex;
    idTypes _type;
    string _typeLetter;
    map<string,int> _varEntries;

public:

    /**
     * @brief Construct a new Register Allocator object
     * 
     * @param startingIndex - the index after the last var in the parent block
     * @param type - the type of the register in this file
     */
    TypedVarScopeTable(int startingIndex, idTypes type);

    /**
     * @brief Allocate a new temp var
     * 
     * @return VarEntry& Allocated new var
     */
    string newTemp();

    /**
     * @brief Allocate a new var for specific id
     * 
     * @param int The offset from $SP for the new variable
     * @return VarEntry& Allocated new var.
     */
    int newVar(string id);

    /**
     * @brief Search id and get the Var object
     * @param var - pointer for the returned entry 
     * @param id - looked-up id
     * @return true/false - found the entry
     */
    bool lookup(VarEntry& var, string id);

    /**
     * @brief Remove all temporary registers.
     */
    void resetTmps();

    // /**
    //  * @brief Get the Last Var Offest pointer.
    //  */
    // int getLastVarOffest();

    // /**
    //  * @brief Get the Last Tmp Offest pointer.
    //  */
    // int getLastTmpOffest();

    /**
     * @brief Get the Size of registers array
     * 
     * @return int 
     */
    int getSize();

    /**
     * @brief Emit the ASM code line for storing the registers in range & 
     *          Update the $SP to the next available block in the stack.
     */
    void emitLoadIds();

    /**
     * @brief Emit the ASM code line for storing the registers in range & 
     *          Update the $SP to the next available block in the stack.
     */
    void emitStoreIds();

    /**
     * @brief De-allocate stack space for vars
     * 
     */
    void freeStack();
};

class VarScopeTable{
protected:
    TypedVarScopeTable intTable;
    TypedVarScopeTable floatTable;
public:

    /**
     * @brief Construct a new Var Scope Table object 
     */
    VarScopeTable();

    /**
     * @brief Allocate a new temporary register to be used until next entrance/exit of a block.
     *          We assume enough registers are available.
     * @param type - type of the register needed.
     * @return string - Register ("-1" indicates an error).
     */
    string newTemp(idTypes type);
    
    /**
     * @brief Allocate a new register for a given variable id.
     *          We assume that there isn't stack overflow. 
     * @param id - Identifier of the variable 
     * @param type - Type of the variable (can be eFLOAT/eINT)
     * @return string - Register ("-1" indicates an error)
     */
    int newVar(string id, idTypes type);

    /**
     * @brief Search id and get the Var object
     * @param var - pointer for the returned entry 
     * @param id - looked-up id
     * @return true/false - found the entry
     */
    bool lookup(VarEntry& var, string id);

    /**
     * @brief remove the temporary register in its innet TypedVarScopeTables
     */
    void resetTmps();

    /**
     * @brief store the variables & temporaries. emits STORE ASM functions
     */
    void storeIds();

    /**
     * @brief load the variables & temporaries. emits LOAD ASM functions
     */
    void loadIds();

    void freeStack();

};

class VariableTable{
protected:
    list<VarScopeTable> _tables;
public:
    list<ArgDeclaration> functionArgs; //todo merge this into _tables on first push. Must always be overwritten on function entry!!
    
    /**
     * @brief Push a new VarScopeTable into the tables list.
     */
    void push();

    /**
     * @brief Pop the VarScopeTable in the head of the tables list.
     */
    void pop();
    
    /**
     * @brief Get the head of the VarScopeTables list.
     * 
     * @return VarScopeTable& 
     */
    VarScopeTable& front();

    /**
     * @brief Search an id among all VarScopeTables and get the first appeared Var object
     * @param var - pointer for the returned entry 
     * @param id - looked-up id
     * @return true/false - found the entry
     */
    bool lookupVarTableList(VarEntry& var, string id);
    
    /**
     * @brief Set the Function Api into the arguments list.
     * @param args 
     */
    void setFunctionApi(list<ArgDeclaration> &args);
    
    /**
     * @brief Execute storeIds of _tables.head VarScopeTable 
     */
    void storeIds();

    /**
     * @brief Execute storeIds of _tables.head VarScopeTable 
     */
    void loadIds();

    /**
     * @brief Indicate if an 'id' of a var exists in the current scope
     * 
     * @param id 
     * @return true 
     * @return false 
     */
    bool isInScope(string id);
};

typedef struct _functionProps {
    idTypes type;
    string id;
    list<ArgDeclaration> args;
} FunctionProps;

class FunctionEntry{
protected:
    int _definitionLine;
public:
    FunctionProps props;
    CodeLineList callList;

    /**
     * @brief Construct a new FunctionEntry object without definition
     * 
     * @param funcProps Props of the function
     */
    FunctionEntry(FunctionProps& funcProps);


    /**
     * @brief Get the Place of definition, or "-1" for backpatching
     * 
     * @param currentLine Needed for backpatching if not defined yet
     * @return string Place of definition / "-1"
     */
    string getPlace(int currentLine);

    /**
     * @brief Store the starting line of definition, backpath all callers
     * 
     * @param definitionLine starting line of definition
     */
    void define(int definitionLine);

    /**
     * @brief Is this function defined yet
     * 
     * @return true 
     * @return false 
     */
    bool isDefined();

    /**
     * @brief Check if given props match this function entry
     * 
     * @param funcProps Function props to compare with entry
     * @return true 
     * @return false 
     */
    bool matchExisting(FunctionProps& funcProps);
};

class FunctionTable{
protected:
    map<string,FunctionEntry> _functionTable;
    FunctionEntry* _current;
public:
    
    /**
     * @brief Find definition of a function in the table
     * 
     * @param id Name of function
     * @return FunctionEntry* If found - pointer to entry. Eitherwise - NULL.
     */
    FunctionEntry* find(string id);

    /**
     * @brief Insert new function entry.
     */
    FunctionEntry* insert(FunctionProps& funcProps);

    /**
     * @brief Get the Current function (last inserted)
     * 
     * @return FunctionEntry* 
     */
    FunctionEntry* getCurrent();

    /**
     * @brief Set the funcEntry as the current function
     * @param funcEntry 
     */
    void setCurrent(FunctionEntry* funcEntry);

    string getUnimplementedCalls();

    string getImplemented();

};

#endif
