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
    TypedVarScopeTable* intTable;
    TypedVarScopeTable* floatTable;
    VarScopeTable *parent;
public:
    ~VarScopeTable();

    VarScopeTable(VarScopeTable *parent);

    static void pushBlock();
    // { //fixme
    //     VarScopeTable newTable = new VarScopeTable(globalPointer);
    //     globalPointer = newTable;
    // }

    static void popBlock();
    // { //fixme Remember - copy current offsets of int/float tables 
    //     VarScopeTable *currentTable = globalPointer;
    //     globalPointer = currentTable->parent;
    //     delete currentTable;
    // }

    string newTemp(idTypes type);
    /**
     * @brief Allocate a new register for a given variable id
     * @param id - Identifier of the variable 
     * @param type - Type of the variable
     * @return string - Register
     */
    string newVar(string id, idTypes type);

    /**
     * @brief Get the Var object, the symbol table entry of a given id
     * @param var - pointer for the returned entry 
     * @param id - looked-up id
     * @return true/false - found the entry
     */
    bool getVar(varEntry& var, string id);

    void storeIds();

    void loadIds();

};

class TypedVarScopeTable{
protected:
    int _curVarOffset;
    int _curTempOffset;
    idTypes _type;
    map<string,int> _varEntries;
    const int size = 1024;

public:
    TypedVarScopeTable *parent;

    /**
     * @brief Construct a new Register Allocator object
     * 
     * @param startingIndex 
     * @param type 
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
     * @exception Will be thrown for taken id
     * 
     * @param id The id of the var
     * @return varEntry& Allocated new var
     */
    varEntry newVar(string id);

    bool getId(string& var, string id);

    void storeIds();

    void loadIds();    
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
