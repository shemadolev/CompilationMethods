/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.c - Helper functions for project part 2 - implementation  */
#include <stdio.h>
#include "part3_helpers.hpp"
#include "code_class.hpp"
#include "symbol_table.hpp"
#include <exception>


extern ParserNode *parseTree; /* Root of parse tree. Defined in the parser. */
extern FunctionTable funcTable; //defined in parser
extern CodeClass code; //defined in parser

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
    try{
      rc = yyparse();
      if (rc == 0) { // Parsed successfully
          if(PRINT_PARSE_TREE)
            parseTree->dumpParseTree();
          //Create output file
          ofstream outFile;
          string outputName = argv[1];
          bool replaceName = replace(outputName, ".cmm", ".rsk");
          if(!replaceName){
            operational_error("invalid file name. must end with .cmm");
          }
          outFile.open(outputName); 
          //Print headers (for linker)
          outFile << "<header>" << endl;
          outFile << "<unimplemented>" << funcTable.getUnimplementedCalls() << endl;
          outFile << "<implemented>" << funcTable.getImplemented() << endl;
          outFile << "</header>" << endl;
          //Print code
          code.print(outFile);
          outFile.close();
      }
    } catch (exception e){
      operational_error(e.what());
    }
    delete parseTree;
    return rc;
}

void operational_error(const char* err){
    cerr << "Operational error: " << err <<endl;
    exit(9);
}