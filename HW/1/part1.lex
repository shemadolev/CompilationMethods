%{
/*Declaraions section*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
void showToken(char *);
void showOriginal();
void printError();
void showTokenReserved();
void showTokenString();
void showNewLine();

static bool isEmptyLine = true;
//isEmptyLine = true;
%}

%option yylineno
%option noyywrap

digit       ([0-9])
letter      ([a-zA-z])
esacapedChars   (\\["nt])
symbols         ([\(\)\{\}\.;,:])
whitespace      ([\t ])
newline         [\n\r]

str             (\"([^\\\"\n\r]|{esacapedChars})*\")
integernum      ({digit}+)
realnum         ({digit}+\.{digit}+)
id              ({letter}+({letter}|{digit}|_)*)

%%

int|float|void|write|read|while|do|if|then|else|return|full_while|break   showTokenReserved("reserved_word");
{id}                    showToken("id");
{integernum}            showToken("integernum");
{realnum}               showToken("realnum");
{str}                   showTokenString();
{newline}               showNewLine();

==|<>|<|<=|>|>=         showToken("relop");
\+|\-                   showToken("addop");
\*|\/                   showToken("mulop");
\=                      showToken("assign");
\&\&                    showToken("and");
\|\|                    showToken("or");
\!                      showToken("not");

{symbols}|{whitespace}  showOriginal();

#[^\n\r]*                 ;

.                       printError();

%%

void showToken(char* name){
    printf("<%s,%s>",name,yytext);
    isEmptyLine = false;
}
void showTokenReserved(){
    printf("<%s>",yytext);
    isEmptyLine = false;
}
void showTokenString(){
    yytext++;
    yytext[strlen(yytext)-1]='\0';
    printf("<str,%s>",yytext);
    isEmptyLine = false;
}
void showOriginal(){
    printf("%s",yytext);
    isEmptyLine = false;
}
void printError(){
    if(!isEmptyLine)
        printf("\n");
    printf("Lexical error: '%s' in line number %d\n",yytext, yylineno);
    exit(1);
}
void showNewLine(){
    printf("%s",yytext);
    isEmptyLine = true;
}