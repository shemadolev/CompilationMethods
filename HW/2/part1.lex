%{
/*Declaraions section*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
    yylval.type = yytext;
    yylval.value = nullptr;
    return yytext;
}

{id} {  
    yylval.type = "id";
    yylval.value = yytext;
    return ID;
}

{integernum}  {
    yylval.type = "integernum";
    yylval.value = yytext;
    return INT;
}

{realnum} {
    yylval.type = "realnum";
    yylval.value = yytext;
    return REAL;
}         
{str} {  
    yylval.type = "str";
    yylval.value = yytext;
    return STR;
}

==|<>|<|<=|>|>= {
    yylval.type = "relop";
    yylval.value = yytext;
    return RELOP;
}

\+|\- {
    yylval.type = "addop";
    yylval.value = yytext;
    return ADDOP;
}
\*|\/ {
    yylval.type = "mulop";
    yylval.value = yytext;
    return MULOP;
}
\= {
    yylval.type = "assign";
    yylval.value = yytext; //null?
    return ASSIGN;
}
\&\& {
    yylval.type = "and";
    yylval.value = yytext; //null?
    return AND;
}
\|\| {
    yylval.type = "or";
    yylval.value = yytext;//null?
    return OR;
}
\! {
    yylval.type = "not";
    yylval.value = yytext;//null?
    return NOT;
}

{symbols} {
    yylval.type = yytext;
    yylval.value = nullptr;
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
