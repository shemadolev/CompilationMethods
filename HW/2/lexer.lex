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
    return id;
}

{integernum}  {
    yylval.type = "integernum";
    yylval.value = yytext;
    return int;
}

{realnum} {
    yylval.type = "realnum";
    yylval.value = yytext;
    return real;
}         
{str} {  
    yylval.type = "str";
    yylval.value = yytext;
    return str;
}

==|<>|<|<=|>|>= {
    yylval.type = "relop";
    yylval.value = yytext;
    return relop;
}

\+|\- {
    yylval.type = "addop";
    yylval.value = yytext;
    return addop;
}
\*|\/ {
    yylval.type = "mulop";
    yylval.value = yytext;
    return mulop;
}
\= {
    yylval.type = "assign";
    yylval.value = yytext; //null?
    return assign;
}
\&\& {
    yylval.type = "and";
    yylval.value = yytext; //null?
    return and;
}
\|\| {
    yylval.type = "or";
    yylval.value = yytext;//null?
    return or;
}
\! {
    yylval.type = "not";
    yylval.value = yytext;//null?
    return not;
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
