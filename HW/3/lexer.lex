%{
/*Declaraions section*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "part3_helpers.hpp"
#include "parser.tab.hpp"

void printError();
int getTokenFromString(const char*);

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
    yylval = new NodeToken(yytext);
    return getTokenFromString(yytext);
}

{id} {
    yylval = new NodeToken("id",yytext);  
    return ID;
}

{integernum}  {
    yylval = new NodeToken("integernum",yytext);
    return INTEGERNUM;
}

{realnum} {
    yylval = new NodeToken("realnum",yytext);
    return REALNUM;
}         
{str} { 
    //Remove opening & closing " 
    char* str_val = yytext + 1;
    str_val[strlen(str_val)-1]='\0';

    yylval = new NodeToken("str",str_val);
    return STR;
}

==|<>|<|<=|>|>= {
    yylval = new NodeToken("relop",yytext);
    return RELOP;
}

\+|\- {
    yylval = new NodeToken("addop",yytext);
    return ADDOP;
}
\*|\/ {
    yylval = new NodeToken("mulop",yytext);
    return MULOP;
}
\= {
    yylval = new NodeToken("assign",yytext);
    return ASSIGN;
}
\&\& {
    yylval = new NodeToken("and",yytext);
    return AND;
}

\|\| {
    yylval = new NodeToken("or",yytext);
    return OR;
}

\! {
    yylval = new NodeToken("not",yytext);
    return NOT;
}

{symbols} {
    yylval = new NodeToken(yytext);
    return yytext[0]; //Use the char itself as the token
}

{whitespace}|{newline}  ;
#[^\n\r]*               ;

.                       printError();

%%

//"Switch-case" of identifier from the given yytext
int getTokenFromString(const char* str){
    if(strcmp(str, "int") == 0)
        return INT;
    else if(strcmp(str, "float") == 0)
        return FLOAT;
    else if(strcmp(str, "void") == 0)
        return VOID;
    else if(strcmp(str, "write") == 0)
        return WRITE;
    else if(strcmp(str, "read") == 0)
        return READ;
    else if(strcmp(str, "while") == 0)
        return WHILE;
    else if(strcmp(str, "do") == 0)
        return DO;
    else if(strcmp(str, "if") == 0)
        return IF;
    else if(strcmp(str, "then") == 0)
        return THEN;
    else if(strcmp(str, "else") == 0)
        return ELSE;
    else if(strcmp(str, "return") == 0)
        return RETURN;
    else if(strcmp(str, "full_while") == 0)
        return FULL_WHILE;
    else if(strcmp(str, "break") == 0)
        return BREAK;
    else /* default - shouldn't get here */
        return 0;
}

void printError(){
    printf("Lexical error: '%s' in line number %d\n",yytext, yylineno);
    exit(1);
}
