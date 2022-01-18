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

//***************************************
//TODO - I think I'm mixing up 'temp' managements with symbol table
//***************************************

using namespace std;

// #define REG_NUM 1024

// class tempEntry{
// public:
//     string type;
//     string assignedReg;

// };

// class regTable{
// protected:
//     vector<tempEntry&> _table;
// public:

//     regTable(int capacity);

//     /**
//      * @brief Get a new temp register
//      * 
//      * @param type What type it holds
//      * @return tempEntry& 
//      */
//     tempEntry& newTemp(string type, string id);

// };

class varEntry{
public:
    string place;
    idTypes type;
};

class VarScopeTable{
protected:
    int _curOffset;
    idTypes _type;
    vector<varEntry&> _varEntries;

public:
    /**
     * @brief Construct a new Register Allocator object
     * 
     * @param startingIndex 
     * @param type 
     */
    VarScopeTable(int startingIndex, idTypes type);

    /**
     * @brief Allocate a new temp var
     * 
     * @return varEntry& Allocated new var
     */
    varEntry& newTemp();

    /**
     * @brief Allocate a new var for specific id
     * @exception Will be thrown for taken id
     * 
     * @param id The id of the var
     * @return varEntry& Allocated new var
     */
    varEntry& newId(string id);

    /**
     * @brief Get var entry by id
     * @exception Will be thrown if id not found
     * 
     * @param id The id of the var
     * @return varEntry& Entry of the existing var
     */
    varEntry& getId(string id);
};


//Parent class for different types of SymbolTable entries
class SymbolEntry{

};

class SymbolEntry_Function : public SymbolEntry{
protected:
    idTypes _type;
    string _id;
    vector<tuple<idTypes, string>> _args;
public:
    CodeLineList callList;
    unsigned int startingLine;
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
    bool addArg(idTypes type, string name);

    /**
     * @brief Get the Args object
     * 
     * @return vector<tuple<idTypes, string>> 
     */
    vector<tuple<idTypes, string>> getArgs();
    
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
