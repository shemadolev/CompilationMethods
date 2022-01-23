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

bool lookupVarTableList(list<VarScopeTable>& tables, varEntry& var, string id);

class SymbolEntry_Function{
protected:
    idTypes _type;
    string _id;
    vector<tuple<idTypes, string>> _args;
public:
    CodeLineList callList;
    int startingLine;
    bool isDefinition;

    /**
     * @brief Construct a new SymbolEntry_Function object
     * 
     * @param type Type of function return value
     */
    SymbolEntry_Function(idTypes type, string id);

    /**
     * @brief Add an argument for function API
     * 
     * @param type Type of the argument
     * @param name Name of the argument
     * @return true Added successfully 
     * @return false Failed - either duplicate name or invalid type
     */

    /**
     * @brief Set the Args object
     * 
     * @param args Vector of pairs of arguments (type, id)
     */
    void setArgs(vector<tuple<idTypes, string>> args);

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

};

class GlobalSymbolTable{
    map<string,SymbolEntry_Function> functionTable;
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
	 * 
     * 
     * @return SymbolEntry_Function* 
     */
    SymbolEntry_Function* getCurrent();
};

#endif
