%{
    #include <iostream>
    #include "part2_helpers.h"
    using namespace std;

    int yylex();
    void yyerror(const char*);
%}

%token ...

%%

PROGRAM : FDEFS ;

FDEFS : FDEFS FUNC_API BLK | 
        FDEFS FUNC_API ';' |
        /* epsilon */ ;

FUNC_API : TYPE id '(' FUNC_ARGS ')' ;

FUNC_ARGS : FUNC_ARGLIST | 
            /* epsilon */ ;

FUNC_ARGLIST :  FUNC_ARGLIST ',' DCL | 
                DCL ;

BLK : '{' STLIST '}' ;

DCL :   id ':' TYPE |
        id ',' DCL ;

TYPE :  int | 
        float | 
        void ;

STLIST : STLIST STMT | 
        /* epsilon */  ;

STMT :  DCL ';' |
        ASSN | 
        EXP ';' | 
        CNTRL | 
        READ | 
        WRITE | 
        RETURN | 
        BLK | 
        break ';' ;

RETURN : return EXP ';' |
	     return';' ;

WRITE : write '(' EXP ')' ';' |
	    write '(' str ')' ';' ;

READ : read '(' LVAL ')' ';' ;

ASSN : LVAL assign EXP ';' ;

LVAL : id ;

FULLWHILE : full_while BEXP do STMT ;

CNTRL : if BEXP then STMT else STMT |
        if BEXP then STMT |
        while BEXP do STMT |
        FULLWHILE ;

BEXP :  BEXP or BEXP |
        BEXP and BEXP |
        not BEXP |
        EXP relop EXP |
        '(' BEXP ')' |
        FULLWHILE ;

EXP :   EXP addop EXP |	
        EXP mulop EXP |
        '(' EXP ')' |
        '(' TYPE ')' EXP |
        id |
        NUM |
        CALL ;

NUM :   integernum |
	    realnum ;

CALL : id '(' CALL_ARGS ')' ;

CALL_ARGS : CALL_ARGLIST |
	 /* epsilon */ ;

CALL_ARGLIST :  CALL_ARGLIST ',' EXP |
	            EXP ;

%%

void yyerror(const char*) {
    cout << "Syntax error: '" << yytext << "' in line number " << yylineno << endl;
}