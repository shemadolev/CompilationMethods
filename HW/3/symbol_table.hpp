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

class varEntry{
public:
    string place;
    idTypes type;
};

class VarScopeTable{
protected:
    TypedVarScopeTable intTable;
    TypedVarScopeTable floatTable;
public:

    /**
     * @brief Construct a new Var Scope Table object, 
     *          Call the constructor of its inner tables (for int & float)
     */
    VarScopeTable();

    ~VarScopeTable();

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
    string newVar(string id, idTypes type);

    /**
     * @brief Search id and get the Var object
     * @param var - pointer for the returned entry 
     * @param id - looked-up id
     * @return true/false - found the entry
     */
    bool lookup(varEntry& var, string id);

    void storeIds();

    void loadIds();

    void resetTmps();

};

class TypedVarScopeTable{
protected:
    int _curVarOffset;
    int _curTempOffset;
    idTypes _type;
    string _typeLetter;
    map<string,int> _varEntries;
    const int size = 1024;

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
     * @return varEntry& Allocated new var
     */
    string newTemp();

    /**
     * @brief Allocate a new var for specific id
     * @exception Will be thrown for taken id. NOTE: happens in ypp file?
     * 
     * @param id The id of the var
     * @return varEntry& Allocated new var. NOTE: seems that it can be void
     */
    string newVar(string id);

    /**
     * @brief Search id and get the Var object
     * @param var - pointer for the returned entry 
     * @param id - looked-up id
     * @return true/false - found the entry
     */
    bool lookup(varEntry& var, string id);

    void storeIds();

    void loadIds(); 

    void resetTmps();   
};


class VariableTable{
protected:
    list<VarScopeTable> _tables;
public:
    bool lookupVarTableList(varEntry& var, string id);
    
    void storeIds();

    void loadIds(); 

    VarScopeTable& front();

    void push(VarScopeTable& newTable);
    void pop();
};

typedef struct _functionProps {
    idTypes type;
    string id;
    list<argDeclaration> args;
} FunctionProps;

class SymbolEntry_Function{
protected:
    int _definitionLine;
public:
    FunctionProps props;
    CodeLineList callList;

    /**
     * @brief Construct a new SymbolEntry_Function object without definition
     * 
     * @param funcProps Props of the function
     */
    SymbolEntry_Function(FunctionProps& funcProps);

    /**
     * @brief Get the Args object
     * 
     * @return vector<tuple<idTypes, string>> 
     */
    vector<tuple<idTypes, string>> getArgs();

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
    map<string,SymbolEntry_Function> _functionTable;
    SymbolEntry_Function* _current;
public:
    
    /**
     * @brief Find definition of a function in the table
     * 
     * @param id Name of function
     * @return SymbolEntry_Function* If found - pointer to entry. Eitherwise - NULL.
     */
    SymbolEntry_Function* find(string id);

    /**
     * @brief Insert new function entry. Updates the 'current' function.
     * 
     * @param funcProps 
     * @return SymbolEntry_Function* 
     */
    SymbolEntry_Function* insert(FunctionProps& funcProps);

    /**
     * @brief Get the Current function (last inserted)
     * 
     * @return SymbolEntry_Function* 
     */
    SymbolEntry_Function* getCurrent();
};

#endif
