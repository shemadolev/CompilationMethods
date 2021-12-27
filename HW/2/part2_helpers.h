/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.h - Helper functions for project part 2 - API definitions */

#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct node {
    char * type;
    char * value;
    struct node *sibling;
    struct node *child;
} ParserNode;

#define YYSTYPE ParserNode* // Define yystype as the wanted struct

//Macro for a special "epsilon" symbol node
#define EPSILON makeSymbol("EPSILON",NULL)

/**
 * @brief Make a new node for the parser tree.
 * 
 * @param type The type for the new node.
 * @param value The value for the new node - optional (Pass NULL if irrelevant).
 * @param child A ParserNode pointer for the first child - optional.
 * @return ParserNode* Pointer for the created node.
 */
ParserNode *makeNode(const char* type,const char* value, ParserNode *child);
//Shorthand for makeNode to create a symbol's node (no type)

/**
 * @brief A shorthand for makeNode for creating a symbol node.
 * 
 * @param type The symbol's type.
 * @param child A ParserNode pointer for the symbol's first child.
 * @return ParserNode* Pointer for the created symbol's node.
 */
ParserNode *makeSymbol(const char* type, ParserNode *child);
//Shorthand for makeNode to create a token's node (no children)

/**
 * @brief A shorthand for makeNode for creating a token node.
 * 
 * @param type The token's type.
 * @param value The token's value (usually its lexeme).
 * @return ParserNode* Pointer for the created token's node.
 */
ParserNode *makeToken(const char* type,const char* value);

/**
 * @brief Concatenate a child to the end of its children list.
 * 
 * @param listHead Pointer to list's head (Its first child)
 * @param newItem Pointer to the new child to be added in the end of the list.
 * @return ParserNode* Pointer to the list's head.
 */
ParserNode *concatList(ParserNode *listHead,ParserNode *newItem);

/**
 * @brief Print the tree which its root is stored in ParserNode* parseTree.
 * 
 */
void dumpParseTree(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //COMMON_H
