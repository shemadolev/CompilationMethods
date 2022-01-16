/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.h - Helper functions for project part 2 - API definitions */

#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <string>

#include "code_class.hpp"

using namespace std;

enum idTypes {eINT, eFLOAT, eVOID};

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

class NodeSymbol : public ParserNode{
    ParserNode *child;
public:
    NodeSymbol(string type, ParserNode *child);
    ~NodeSymbol();

    void dumpParseTree(int depth);
};

class NodeToken : public ParserNode{
    string value;
public:
    NodeToken(string type);
    NodeToken(string type, string value);
    void dumpParseTree(int depth);
};

// class NodeExpression : public NodeSymbol{
// public:
//     idTypes expType;
//     string place;
// };

// class NodeStatement : public NodeSymbol{
// public:
//     BpList nextList;
//     BpList breakList;
// };

// class NodeBexp : public NodeSymbol{
// public:
//     BpList trueList;
//     BpList falseList;
// };

#define YYSTYPE ParserNode* // Define yystype as the wanted struct

//todo continue here

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

// /**
//  * @brief Print the tree which its root is stored in ParserNode* parseTree.
//  * 
//  */
// void dumpParseTree(void);

#endif //COMMON_H
