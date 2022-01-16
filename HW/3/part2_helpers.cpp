/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.c - Helper functions for project part 2 - implementation  */
#include <stdio.h>
#include "part2_helpers.hpp"

extern ParserNode *parseTree; /* Root of parse tree. Defined in the parser. */

extern int yyparse (void);

ParserNode::ParserNode(string type) : type(type) {}
ParserNode::ParserNode(string type, ParserNode *sibling) : type(type), sibling(sibling) {}
ParserNode::~ParserNode(){
  if(sibling != nullptr)
    delete sibling;
}
void ParserNode::dumpParseTree(){
  dumpParseTree(0);
}

NodeSymbol::NodeSymbol(string type, ParserNode *child) : ParserNode(type), child(child) {}
NodeSymbol::~NodeSymbol(){
  delete child;
}

NodeToken::NodeToken(string type) : ParserNode(type) {}
NodeToken::NodeToken(string type, string value) : ParserNode(type), value(value){}


void NodeToken::dumpParseTree(int depth){
  string indent(depth,' ');
  if(value.length() > 0)
    cout << indent << "(<" << type << "," << value << ">)" << endl;
  else
    cout << indent << "(<" << type << ">)" << endl;
}

void NodeSymbol::dumpParseTree(int depth){
  string indent(depth,' ');
  cout << indent << "(<" << type << ">" << endl;
  for (ParserNode* curChild = child; curChild != NULL; curChild = curChild->sibling) {
    curChild->dumpParseTree(depth + 1);
  }
  cout << ")" << endl;
}

/**************************************************************************/
/*                             MAKE a node                                */
/**************************************************************************/

// ParserNode *makeNode(const char* type,const char* value, ParserNode *child)
// {
//   ParserNode *p;

//   if ((p = (ParserNode*)(malloc(sizeof(ParserNode)))) == 0)
//     fprintf(stderr, "Failed malloc(struct node)\n");
//   else {
//     p->type = strdup(type);
//     if (value != NULL) {
//       p->value = strdup(value);
//     } else {
//       p->value = NULL;
//     }
//     p->child = child;
//     p->sibling = (ParserNode*)NULL;
//   }
//   return(p);
// }

// ParserNode *makeSymbol(const char* type, ParserNode *child){
//   return makeNode(type, NULL, child);
// }

// ParserNode *makeToken(const char* type,const char* value){
//   return makeNode(type, value, NULL);
// }

/**************************************************************************/
/*                           Concate item to list                         */
/**************************************************************************/

ParserNode *concatList(ParserNode *listHead,ParserNode *newItem)
{
  ParserNode* lastSibling;

  if (listHead == NULL) return newItem;
  lastSibling = listHead;
  while (lastSibling->sibling != (ParserNode*)NULL)
    lastSibling = lastSibling->sibling;  /* finds the last sibling in the list */
  lastSibling->sibling = newItem;  /* append the new item to the list */
  return (listHead);
}


/**************************************************************************/
/*                    Functions to dump parse tree                        */
/**************************************************************************/

// /* Helper function for dumping of parse tree. Recursively invoked. */
// /* Used by dumpParseTree()                                         */
// static void dumpParseSubtree(ParserNode *parentNode, int depth)
// {
//     ParserNode *curChild;
//     char indent[(depth * 2) + 1];
//     memset(indent, ' ', depth * 2); // Indentation is two spaces per depth level
//     indent[depth * 2] = 0; // NULL terminate string.

//     if (parentNode == NULL) // probably reached "child" of a leaf
//         return;
//     printf("%s(<%s", indent, parentNode->type);
//     if (parentNode->value != NULL)
//         printf(",%s", parentNode->value);
//     printf(">");
//     // Handle children
//     if (parentNode->child != NULL) {
//         printf("\n");
//         for (curChild = parentNode->child; curChild != NULL; curChild = curChild->sibling) {
//             dumpParseSubtree(curChild, depth + 1);
//         }
//         printf("%s)\n", indent);
//     } else { // No children - just close node parenthesis
//         printf(")\n");
//     }
// }

// /* This is the entry function to dump of parse tree. Assumes parseTree points to tree root. */
// void dumpParseTree(void)
// {
//     if (parseTree == NULL) {
//         fprintf(stderr,"*** Parse tree is empty!\n");
//     } else {
//         dumpParseSubtree(parseTree, 0);
//     }
// }

/**************************************************************************/
/*                           Main of parser                               */
/**************************************************************************/
int main(void)
{
    int rc;
#if YYDEBUG
    yydebug=1;
#endif
    rc = yyparse();
    if (rc == 0) { // Parsed successfully
        parseTree->dumpParseTree();
    }
    delete parseTree;
    return rc;
}
