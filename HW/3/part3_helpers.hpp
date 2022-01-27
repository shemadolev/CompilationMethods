/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.h - Helper functions for project part 2 - API definitions */

#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>
#include <tuple>
#include <list>
#include <cassert>
#include <exception>

using namespace std;

#define YYSTYPE ParserNode* // Define yystype as the wanted struct

#define PRINT_PARSE_TREE 1


//return address pointer
#define RA "I0"
//frame start pointer
#define FP "I1"
//stack pointer (next write pointer in stack)
#define SP "I2"
//return int type
#define RT_I "I3"
//declarations pointer (points on the first variable in the frame)
#define DP "I4"
//return float type
#define RT_F "F0"

#define FUNC_ARGS_OFFSET 1
#define SAVED_REGS_INT 4
#define SAVED_REGS_FLOAT 1

//Size of variable in bytes
#define VAR_SIZE 4

enum idTypes {eINT, eFLOAT, eVOID};

typedef struct _ArgDeclaration {
    string id;
    idTypes type;
} ArgDeclaration;

typedef struct _functionProps {
    idTypes type;
    string id;
    list<ArgDeclaration> args;
} FunctionProps;

bool replace(std::string& str, const std::string& from, const std::string& to);

class CodeLineList : public list<int> {
public:
	/**
	 * @brief Construct a new empty Code Line List object (for backpatching)
	 * 
	 */
	CodeLineList();

	/**
	 * @brief Construct a new Code Line List object (for backpatching)
	 * 
	 * @param line The first line to insert to the list
	 */
	CodeLineList(int line);
};

class CodeClass {
protected:
	vector<string> codeVec;

    template<typename T>
	string spreadString(T str) {
		return str;
	}
    
    template<typename T, typename... Args>
    string spreadString(T t, Args... args) {
        return spreadString(t) + string(" ") + spreadString(args...) ;
    }
public:

	CodeClass();

	/**
	 * @brief Emit code
	 * 
	 * @tparam Args Arguments of type string
	 * @param args Arguments of type string
	 * @return int Line number of emitted code
	 */
	template<typename... Args>
	int emit(Args... args){
		string code = spreadString(args...);
		codeVec.push_back(code);
		return codeVec.size();
	}

	/**
	 * @brief Get next line number of code
	 * 
	 * @return int next line number of code
	 */
	int nextQuad();

	/**
	 * @brief Backpatch 'goto -1' commands
	 * 
	 * @param lineList List of line numbers with 'goto 0' to be back-patched
	 * @param lineNo The line number to be filled in
	 */
	void backpatch(CodeLineList lineList, int lineNo);

	/**
	 * @brief Print all code by order
	 * 
	 */
	void print(ofstream& file);
};

/**
 * @brief Virtual class for parse tree nodes
 * 
 */
class ParserNode{
public:
    string type;
    ParserNode *sibling;

    ParserNode(){}
    ParserNode(string type);
    ParserNode(string type, ParserNode *sibling);

    virtual ~ParserNode(); //enable polymorphism

    void dumpParseTree();
    virtual void dumpParseTree(int depth) = 0;
};

/**
 * @brief Parse tree node for symbols
 * 
 */
class NodeSymbol : public ParserNode{
    ParserNode *child;
public:
    // NodeSymbol(string type, ParserNode *child);
    ~NodeSymbol();

    virtual void dumpParseTree(int depth);
};

/**
 * @brief Parse tree node for tokens
 * 
 */
class NodeToken : public ParserNode{
public:
    string value;
    NodeToken(string type);
    NodeToken(string type, string value);
    virtual void dumpParseTree(int depth);
};

class NodeExpression : public NodeSymbol{
public:
    NodeExpression(){
        cout << "NodeExpression" << endl;
    }
    idTypes expType;
    string place;
};

class NodeType : public NodeSymbol{
public:
    NodeType(){
        cout << "NodeType" << endl;
    }
    idTypes typeValue;
};

class NodeStatement : public NodeSymbol{
public:
    NodeStatement(){
        cout << "NodeStatement" << endl;
    }
    CodeLineList nextlist;
    CodeLineList breaklist;
};

class NodeBexp : public NodeSymbol{
public:
    NodeBexp(){
        cout << "NodeBexp" << endl;
    }
    CodeLineList truelist;
    CodeLineList falselist;
};

class NodeDeclaration : public NodeSymbol{
public:
    NodeDeclaration(){
        cout << "NodeDeclaration" << endl;
    }
    list<string> idList;
    idTypes idType;
};

class NodeFuncApi : public NodeSymbol{
public:
    NodeFuncApi(){
        cout << "NodeFuncApi" << endl;
    }
    FunctionProps funcProps;
};

class NodeCallArgs : public NodeSymbol{
public:
    NodeCallArgs(){
        cout << "NodeCallArgs" << endl;
    }
    list<NodeExpression> expList;
};

class NodeFuncArgs : public NodeSymbol{
public:
    NodeFuncArgs(){
        cout << "NodeFuncArgs" << endl;
    }
    list<ArgDeclaration> argList;
};

class NodeMarkerM : public NodeSymbol{
public:
    NodeMarkerM(){
        cout << "NodeMarkerM" << endl;
    }
    int quad;
};

class NodeMarkerN : public NodeSymbol{
public:
    NodeMarkerN(){
        cout << "NodeMarkerN" << endl;
    }
    CodeLineList nextlist;
};

/**
 * @brief code to use when there is a string compare hit
 */
enum string_code {
    eEq,
    eNeq,
    eLt, //less than
    eLtEq,
    eGt, // greater than
    eGtEq,
    eAdd,
    eSub,
    eMul,
    eDiv,
    eDefault
};

string_code RelopHit (std::string const& inString);

string_code ArithHit (std::string const& inString);

//Macro for a special "epsilon" symbol node
#define EPSILON new NodeToken("EPSILON")

/**
 * @brief Concatenate a child to the end of its children list.
 * 
 * @param listHead Pointer to list's head (Its first child)
 * @param newItem Pointer to the new child to be added in the end of the list.
 * @return ParserNode* Pointer to the list's head.
 */
ParserNode *concatList(ParserNode *listHead,ParserNode *newItem);

/**
 * @brief Failed to allocate memory, wrong input etc.
 * 
 * @param err Error message
 */
void operational_error(const char* err);

class VarEntry{
public:
    int offset;
    idTypes type;
};

class TypedVarScopeTable{
protected:
    int _curTempOffset;
    int _tempStartingIndex;
    idTypes _type;
    string _typeLetter;
    map<string,int> _varEntries;

public:

    /**
     * @brief Construct a new Register Allocator object
     * 
     * @param type - the type of the register in this file
     */
    TypedVarScopeTable(idTypes type);

    /**
     * @brief Allocate a new temp var
     * 
     * @return VarEntry& Allocated new var
     */
    string newTemp();

    /**
     * @brief Allocate a new var for specific id
     * 
     * @param id the name of the variable
     * @param int Stack offset - the offset from SP for the new variable (according to VAR_SIZE)
     */
    void newVar(string id, int offset);

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

    /**
     * @brief Emit the ASM code line for storing the registers in range & 
     *          Update the SP to the next available block in the stack.
     */
    void emitLoadIds();

    /**
     * @brief Emit the ASM code line for storing the registers in range & 
     *          Update the SP to the next available block in the stack.
     */
    void emitStoreIds();

    /**
     * @brief Get the number of allocated vars in this scope
     * 
     * @return int 
     */
    int getVarCount();
};

class VarScopeTable{
protected:
    int _curVarOffset;
    int _varStartingIndex;
    TypedVarScopeTable intTable;
    TypedVarScopeTable floatTable;
public:

    /**
     * @brief Construct a new Var Scope Table object 
     */
    VarScopeTable(int startingOffset);

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

    /**
     * @brief Free the memory that was used by this scope.
     * 
     */
    void freeStack();

    /**
     * @brief Get the offset of the next available cell in the stack
     * @return int 
     */
    int getCurOffset();

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


#endif //COMMON_H
