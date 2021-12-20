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

PROGRAM : FDEFS 
;

FDEFS : FDEFS FUNC_API BLK            {$$=makeNode($1.type, $1.value, $1) concatList($1,$2) }
        | FDEFS FUNC_API ';'
        | /* epsilon */ 
;

FUNC_API : TYPE id '(' FUNC_ARGS ')' 
;

FUNC_ARGS : FUNC_ARGLIST
            | /* epsilon */ 
;

FUNC_ARGLIST :  FUNC_ARGLIST ',' DCL
                | DCL 
;

BLK : '{' STLIST '}' 
;

DCL :   id ':' TYPE
        | id ',' DCL 
;

TYPE :  int
        | float
        | void 
;

STLIST : STLIST STMT 
         |/* epsilon */  
;

STMT :  DCL ';'
        | ASSN 
        | EXP ';' 
        | CNTRL 
        | READ 
        | WRITE 
        | RETURN 
        | BLK 
        | break ';' 
;

RETURN : return EXP ';'
         | return ';' 
;

WRITE : write '(' EXP ')' ';'
        | write '(' str ')' ';' 
;

READ : read '(' LVAL ')' ';' 
;

ASSN : LVAL assign EXP ';' 
;

LVAL : id 
;

FULLWHILE : full_while BEXP do STMT 
;

CNTRL : if BEXP then STMT else STMT 
        | if BEXP then STMT 
        | while BEXP do STMT 
        | FULLWHILE 
;

BEXP :  BEXP or BEXP
        | BEXP and BEXP
        | not BEXP
        | EXP relop EXP
        | '(' BEXP ')'
        | FULLWHILE 
;

EXP :   EXP addop EXP 	
        | EXP mulop EXP 
        | '(' EXP ')' 
        | '(' TYPE ')' EXP 
        | id
        | NUM 
        | CALL 
;

NUM :   integernum 
        | realnum 
;

CALL : id '(' CALL_ARGS ')' 
;

CALL_ARGS : CALL_ARGLIST 
            |/* epsilon */ 
;

CALL_ARGLIST :  CALL_ARGLIST ',' EXP 
                | EXP 
;

%%

void yyerror(const char*) {
    cout << "Syntax error: '" << yytext << "' in line number " << yylineno << endl;
}