/* 046266 Compilation Methods, EE Faculty, Technion                        */
/* part2_helpers.c - Helper functions for project part 2 - implementation  */
#include "part3_helpers.hpp"

extern ParserNode *parseTree; /* Root of parse tree. Defined in the parser. */
extern FunctionTable funcTable; //defined in parser
extern CodeClass code; //defined in parser

extern int yyparse (void);

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

ParserNode::ParserNode(string type) : type(type) {}
ParserNode::ParserNode(string type, ParserNode *sibling) : type(type), sibling(sibling) {}
ParserNode::~ParserNode(){
  if(sibling != nullptr)
    delete sibling;
}
void ParserNode::dumpParseTree(){
  dumpParseTree(0);
}

NodeSymbol::~NodeSymbol(){
  delete child;
}

// NodeSymbol::NodeSymbol(string type, list<NodeSymbol*> children) : ParserNode(type), children(children) {
// }

NodeToken::NodeToken(string type) : ParserNode(type) {
#ifdef DEBUG
  cout << "token: " << type << endl;
#endif
}
NodeToken::NodeToken(string type, string value) : ParserNode(type), value(value){
#ifdef DEBUG
  cout << "token: " << type << ":" << value << endl;
#endif
}


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
      extern FILE *yyin;
      // Open the input file, pass to bison 
      yyin = fopen(argv[1], "r");
      if (yyin == NULL) {
        operational_error("can't open input file");
      }
      rc = yyparse();
      if (rc == 0) { // Parsed successfully
          // if(PRINT_PARSE_TREE)
            // parseTree->dumpParseTree();
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