/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.h - Helper functions for project part 2 - API definitions */

#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <string>
#include <fstream>

#include "code_class.hpp"
#include "symbol_table.hpp"

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
//declaration pointer
#define DP "I4"
//return float type
#define RT_F "F0"

#define FUNC_ARGS_OFFSET 1

enum idTypes {eINT, eFLOAT, eVOID};

bool replace(std::string& str, const std::string& from, const std::string& to);

/**
 * @brief Virtual class for parse tree nodes
 * 
 */
class ParserNode{
public:
    string type;
    ParserNode *sibling;

    ParserNode(string type);
    ParserNode(string type, ParserNode *sibling);

    ~ParserNode();

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
    NodeSymbol(string type, ParserNode *child);
    ~NodeSymbol();

    void dumpParseTree(int depth);
};

/**
 * @brief Parse tree node for tokens
 * 
 */
class NodeToken : public ParserNode{
    string value;
public:
    NodeToken(string type);
    NodeToken(string type, string value);
    void dumpParseTree(int depth);
};

class NodeExpression : public NodeSymbol{
public:
    idTypes expType;
    string place;
};

class NodeType : public NodeSymbol{
public:
    idTypes typeValue;
};

class NodeStatement : public NodeSymbol{
public:
    CodeLineList nextlist;
    CodeLineList breaklist;
};

class NodeBexp : public NodeSymbol{
public:
    CodeLineList truelist;
    CodeLineList falselist;
};

class NodeDeclaration : public NodeSymbol{
public:
    list<string> idList;
    idTypes idType;
};

class NodeFuncApi : public NodeSymbol{
public:
    FunctionProps funcProps;
};

class NodeArgsList : public NodeSymbol{
public:
    list<NodeExpression> exprList;
};

class NodeMarkerM : public NodeSymbol{
public:
    string quad;
    NodeMarkerM(); //todo use global code pointer: store quad
};

class NodeMarkerN : public NodeSymbol{
public:
    CodeLineList nextlist;
    NodeMarkerN(); //todo use global code pointer: emit goto
};

class ArgDeclaration{
public:
    string id;
    idTypes type;

    ArgDeclaration(string id, idTypes type) : id(id), type(type){}
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

string_code RelopHit (std::string const& inString) {
    if (inString == "==") return eEq;
    if (inString == "<>") return eNeq;
    if (inString == "<") return eLt;
    if (inString == "<=") return eLtEq;
    if (inString == ">") return eGt;
    if (inString == ">=") return eGtEq;
    return eDefault;
}

string_code ArithHit (std::string const& inString) {
    if (inString == "+") return eAdd;
    if (inString == "-") return eSub;
    if (inString == "*") return eMul;
    if (inString == "/") return eDiv;
    return eDefault;
}

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
void operational_error(const char* err){

#endif //COMMON_H
