%{
/*Declaraions section*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "part2_helpers.h"

void printError();
%}

%option yylineno
%option noyywrap

digit       ([0-9])
letter      ([a-zA-Z])
esacapedChars   (\\["nt])
symbols         ([\(\)\{\}\.;,:])
whitespace      ([\t ])
newline         [\n\r]

str             (\"([^\\\"\n\r]|{esacapedChars})*\")
integernum      ({digit}+)
realnum         ({digit}+\.{digit}+)
id              ({letter}+({letter}|{digit}|_)*)

%%

int|float|void|write|read|while|do|if|then|else|return|full_while|break {  
    yylval = makeNode(yytext, NULL, NULL);
    return yytext;
}

{id} {
    yylval = makeNode("id", yytext, NULL);  
    return ID;
}

{integernum}  {
    yylval = makeNode("integernum", yytext, NULL);
    return INT;
}

{realnum} {
    yylval = makeNode("realnum", yytext, NULL);
    return REAL;
}         
{str} {  
    yylval = makeNode("str", yytext, NULL);
    return STR;
}

==|<>|<|<=|>|>= {
    yylval = makeNode("relop", yytext, NULL);
    return RELOP;
}

\+|\- {
    yylval = makeNode("addop", yytext, NULL);
    return ADDOP;
}
\*|\/ {
    yylval = makeNode("mulop", yytext, NULL);
    return MULOP;
}
\= {
    yylval = makeNode("assign", NULL, NULL);
    return ASSIGN;
}
\&\& {
    yylval = makeNode("and", NULL, NULL);
    return AND;
}

\|\| {
    yylval = makeNode("or", NULL, NULL);
    return OR;
}

\! {
    yylval = makeNode("not", NULL, NULL);
    return NOT;
}

{symbols} {
    yylval = makeNode(yytext, NULL, NULL);
    return yytext;
}

{whitespace}|{newline}  ;
#[^\n\r]*               ;

.                       printError();

%%

void printError(){
    printf("Lexical error: '%s' in line number %d\n",yytext, yylineno);
    exit(1);
}
