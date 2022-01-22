/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.c - Helper functions for project part 2 - implementation  */
#include <stdio.h>
#include "part2_helpers.hpp"
#include "code_class.hpp"

extern ParserNode *parseTree; /* Root of parse tree. Defined in the parser. */
extern CodeClass code;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

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
  string indent(depth * 2,' ');
  if(value.length() > 0)
    cout << indent << "(<" << type << "," << value << ">)" << endl;
  else
    cout << indent << "(<" << type << ">)" << endl;
}

void NodeSymbol::dumpParseTree(int depth){
  string indent(depth * 2,' ');
  cout << indent << "(<" << type << ">" << endl;
  for (ParserNode* curChild = child; curChild != NULL; curChild = curChild->sibling) {
    curChild->dumpParseTree(depth + 1);
  }
  cout << indent << ")" << endl;
}


NodeDeclaration::NodeDeclaration(string type, ParserNode* child, idTypes idType, string id)
 : NodeSymbol(type, child), idType(idType) {
  idList.push_back(id);
}

NodeDeclaration::NodeDeclaration(string type, ParserNode* child, string id, NodeDeclaration prevDeclare)
 : NodeSymbol(type, child), idType(prevDeclare.idType) {
   idList = prevDeclare.idList;
   idList.push_back(id);
}


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
/*                           Main of parser                               */
/**************************************************************************/
int main (int argc, char **argv) {
    int rc;
#if YYDEBUG
    yydebug=1;
#endif
    rc = yyparse();
    if (rc == 0) { // Parsed successfully
        if(PRINT_PARSE_TREE)
          parseTree->dumpParseTree();
        //Create output file
        ofstream outFile;
        string outputName = argv[1];
        bool replaceName = replace(outputName, ".cmm", ".rsk");
        if(!replaceName){
          //Couldn't find .cmm in input file name
          exit(1); //todo exit on specific code?
        }
        outFile.open(outputName); //todo anything on open fail?
        //Print headers (for linker)
        //...
        //Print code
        code.print(outFile);
        outFile.close();
    }
    delete parseTree;
    return rc;
}