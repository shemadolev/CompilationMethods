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
    yylval = makeNode(yytext, nullptr, nullptr);
    return yytext;
}

{id} {
    yylval = makeNode("id", yytext, nullptr);  
    return id;
}

{integernum}  {
    yylval = makeNode("integernum", yytext, nullptr);
    return int;
}

{realnum} {
    yylval = makeNode("realnum", yytext, nullptr);
    return real;
}         
{str} {  
    yylval = makeNode("str", yytext, nullptr);
    return str;
}

==|<>|<|<=|>|>= {
    yylval = makeNode("relop", yytext, nullptr);
    return relop;
}

\+|\- {
    yylval = makeNode("addop", yytext, nullptr);
    return addop;
}
\*|\/ {
    yylval = makeNode("mulop", yytext, nullptr);
    return mulop;
}
\= {
    yylval = makeNode("assign", nullptr, nullptr);
    return assign;
}
\&\& {
    yylval = makeNode("and", nullptr, nullptr);
    return and;
}

\|\| {
    yylval = makeNode("or", nullptr, nullptr);
    return or;
}

\! {
    yylval = makeNode("not", nullptr, nullptr);
    return not;
}

{symbols} {
    yylval = makeNode(yytext, nullptr, nullptr);
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
