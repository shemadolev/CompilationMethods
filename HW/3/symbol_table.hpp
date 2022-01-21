#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
// #include <list>

#include "part2_helpers.hpp"
#include "code_class.hpp"

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
    VarScopeTable *_parent;
public:
    ~VarScopeTable();

    VarScopeTable(VarScopeTable *parent);

    /**
     * @brief 
     * 
     */
    void pushBlock();
    
    /**
     * @brief 
     * 
     */
    void popBlock();
        
    /**
     * @brief Allocate a new temporary register for the compiler use.
     *          We assume that there isn't stack overflow.
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
    TypedVarScopeTable *parent;

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

class SymbolTable {
protected:

    //todo second type should prbably be a list of something
	map<string,string> _table;
    SymbolTable* _parent;
public:

    // regTable intTable;
    // regTable floatTable;

    /**
     * @brief Construct a new Symbol Table object
     * 
     */
	SymbolTable();

    /**
     * @brief Construct a new Symbol Table object
     * 
     */
	SymbolTable(SymbolTable* parent);

	/**
	 * @brief Print all contens
	 * 
	 */
	void print();
};

#endif
