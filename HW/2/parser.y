%{
    #include <iostream>
    #include "part2_helpers.h"
    using namespace std;

    int yylex();
    void yyerror(const char*);
%}

// reserved words 
%token "int" "float" "void" "write" "read" "while" "do" "if" "then" "else" "return" "full_while" "break"
// other
%token id integernum realnum str
// symbols
%token '(' ')' '{' '}' '.' ';' ',' ':'

%right assign
%left or
%left and
%left relop
%left addop
%left mulop
%right not

%%

PROGRAM : FDEFS                         {$$=makeNode("PROGRAM", NULL, $1);}
;

FDEFS : FDEFS FUNC_API BLK              {$$=makeNode("FDEFS", NULL, $1); concatList($1,$2); concatList($1,$3);}
        | FDEFS FUNC_API ';'            {$$=makeNode("FDEFS", NULL, $1); concatList($1,$2); concatList($1,$3);}
        | /* epsilon */                 {$$=makeNode("FDEFS", NULL, ,makeNode("EPSILON", NULL, NULL));}
;

FUNC_API : TYPE id '(' FUNC_ARGS ')'    {$$=makeNode("FUNC_API", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); concatList($1,$5);} 
;

FUNC_ARGS : FUNC_ARGLIST                {$$=makeNode("FUNC_ARGS", NULL, $1);}       
            | /* epsilon */             {$$=makeNode("FUNC_ARGS", NULL, ,makeNode("EPSILON", NULL, NULL));}
;

FUNC_ARGLIST :  FUNC_ARGLIST ',' DCL    {$$=makeNode("FUNC_ARGLIST", NULL, $1); concatList($1,$2); concatList($1,$3);} 
                | DCL                   {$$=makeNode("FUNC_ARGLIST", NULL, $1);}
;

BLK : '{' STLIST '}'                    {$$=makeNode("BLK", NULL, $1); concatList($1,$2); concatList($1,$3);} 
;

DCL :   id ':' TYPE                     {$$=makeNode("DCL", NULL, $1); concatList($1,$2); concatList($1,$3);}
        | id ',' DCL                    {$$=makeNode("DCL", NULL, $1); concatList($1,$2); concatList($1,$3);}
;

TYPE :  int                             {$$=makeNode("TYPE", NULL, $1);}
        | float                         {$$=makeNode("TYPE", NULL, $1);}
        | void                          {$$=makeNode("TYPE", NULL, $1);} 
;

STLIST : STLIST STMT                    {$$=makeNode("STLIST", NULL, $1); concatList($1,$2);}
         |/* epsilon */                 {$$=makeNode("STLIST", NULL, ,makeNode("EPSILON", NULL, NULL));}
;

STMT :  DCL ';'                         {$$=makeNode("STMT", NULL, $1); concatList($1,$2);}
        | ASSN                          {$$=makeNode("STMT", NULL, $1);}
        | EXP ';'                       {$$=makeNode("STMT", NULL, $1); concatList($1,$2);}
        | CNTRL                         {$$=makeNode("STMT", NULL, $1);}
        | READ                          {$$=makeNode("STMT", NULL, $1);}
        | WRITE                         {$$=makeNode("STMT", NULL, $1);}
        | RETURN                        {$$=makeNode("STMT", NULL, $1);}
        | BLK                           {$$=makeNode("STMT", NULL, $1);}
        | break ';'                     {$$=makeNode("STMT", NULL, $1); concatList($1,$2);}
;

RETURN : return EXP ';'                 {$$=makeNode("RETURN", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | return ';'                    {$$=makeNode("RETURN", NULL, $1); concatList($1,$2); }
;

WRITE : write '(' EXP ')' ';'           {$$=makeNode("WRITE", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); concatList($1,$5);} 
        | write '(' str ')' ';'         {$$=makeNode("WRITE", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); concatList($1,$5);} 
;

READ : read '(' LVAL ')' ';'            {$$=makeNode("READ", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); concatList($1,$5);} 
;

ASSN : LVAL assign EXP ';'              {$$=makeNode("ASSN", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); } 
;

LVAL : id                               {$$=makeNode("LVAL", NULL, $1);}
;

FULLWHILE : full_while BEXP do STMT     {$$=makeNode("FULLWHILE", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); } 
;

CNTRL : if BEXP then STMT else STMT     {$$=makeNode("CNTRL", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); concatList($1,$5); concatList($1,$6);} 
        | if BEXP then STMT             {$$=makeNode("CNTRL", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); } 
        | while BEXP do STMT            {$$=makeNode("CNTRL", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4); } 
        | FULLWHILE                     {$$=makeNode("CNTRL", NULL, $1);}
;

BEXP :  BEXP or BEXP                    {$$=makeNode("BEXP", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | BEXP and BEXP                 {$$=makeNode("BEXP", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | not BEXP                      {$$=makeNode("BEXP", NULL, $1); concatList($1,$2); }
        | EXP relop EXP                 {$$=makeNode("BEXP", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | '(' BEXP ')'                  {$$=makeNode("BEXP", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | FULLWHILE                     {$$=makeNode("BEXP", NULL, $1);}
;

EXP :   EXP addop EXP 	                {$$=makeNode("EXP", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | EXP mulop EXP                 {$$=makeNode("EXP", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | '(' EXP ')'                   {$$=makeNode("EXP", NULL, $1); concatList($1,$2); concatList($1,$3); }
        | '(' TYPE ')' EXP              {$$=makeNode("EXP", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4);}
        | id                            {$$=makeNode("EXP", NULL, $1);}
        | NUM                           {$$=makeNode("EXP", NULL, $1);}
        | CALL                          {$$=makeNode("EXP", NULL, $1);}
;

NUM :   integernum                      {$$=makeNode("NUM", NULL, $1);}
        | realnum                       {$$=makeNode("NUM", NULL, $1);}
;

CALL : id '(' CALL_ARGS ')'             {$$=makeNode("CALL", NULL, $1); concatList($1,$2); concatList($1,$3); concatList($1,$4);}
;

CALL_ARGS : CALL_ARGLIST                {$$=makeNode("CALL_ARGS", NULL, $1);}
            |/* epsilon */              {$$=makeNode("CALL_ARGS", NULL, ,makeNode("EPSILON", NULL, NULL));} 
;

CALL_ARGLIST :  CALL_ARGLIST ',' EXP    {$$=makeNode("CALL_ARGLIST", NULL, $1); concatList($1,$2); concatList($1,$3); }
                | EXP                   {$$=makeNode("CALL_ARGS", NULL, $1);}
;

%%

void yyerror(const char*) {
    cout << "Syntax error: '" << yytext << "' in line number " << yylineno << endl;
}