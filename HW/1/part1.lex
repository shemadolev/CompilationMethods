%{
/*Declaraions section*/
#include <stdio.h>
#include <string.h>
void showToken(char *);
void showOriginal();
void printError();
void showTokenReserved();
void showTokenString();
%}

%option yylineno
%option noyywrap

digit       ([0-9])
letter      ([a-zA-z])
esacapedChars   (\\["nt])
symbols         ([\(\)\{\}\.;,:])
whitespace      ([\t\n ])

str             (\"([^\\\"\n]|{esacapedChars})*\")
integernum      ({digit}+)
realnum         ({digit}+\.{digit}+)
id              ({letter}+({letter}|{digit}|_)*)

%%

int|float|void|write|read|while|do|if|then|else|return|full_while|break   showTokenReserved("reserved_word");
{id}                   showToken("id");
{integernum}            showToken("integernum");
{realnum}               showToken("realnum");
{str}                   showTokenString();

==|<>|<|<=|>|>=         showToken("relop");
\+|\-                   showToken("addop");
\*|\/                   showToken("mulop");
\=                      showToken("assign");
\&\&                    showToken("and");
\|\|                    showToken("or");
\!                      showToken("not");

{symbols}|{whitespace}  showOriginal();

#[^\n]*                 ;

.                       printError();

%%

void showToken(char* name){
    printf("<%s,%s>",name,yytext);
}
void showTokenReserved(){
    printf("<%s>",yytext);
}
void showTokenString(){
    yytext++;
    yytext[strlen(yytext)-1]='\0';
    printf("<str,%s>",yytext);
}
void showOriginal(){
    printf("%s",yytext);
}
void printError(){
    printf("\nLexical error: '%s' in line number %d\n",yytext, yylineno);
    exit(1);
}
