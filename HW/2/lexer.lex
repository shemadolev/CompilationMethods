%{
/*Declaraions section*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "part2_helpers.h"

void printError();
int getTokenFromString(string str);
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
    return getTokenFromString(yytext);
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
    return yytext[0];
}

{whitespace}|{newline}  ;
#[^\n\r]*               ;

.                       printError();

%%

int getTokenFromString(string str){
    switch (str){
        case "int":
            return INT;
        case "float":
            return FLOAT;
        case "void":
            return VOID;
        case "write":
            return WRITE;
        case "read":
            return READ;
        case "while":
            return WHILE;
        case "do":
            return DO;
        case "if":
            return IF;
        case "then":
            return THEN;
        case "else":
            return ELSE;
        case "return":
            return RETURN;
        case "full_while":
            return FULL_WHILE;
        case "break":
            return BREAK;
    }
}

void printError(){
    printf("Lexical error: '%s' in line number %d\n",yytext, yylineno);
    exit(1);
}
