all: part3

parser.tab.cpp: parser.ypp part3_helpers.cpp part3_helpers.hpp
	bison -vd parser.ypp

lex.yy.c: lexer.lex part3_helpers.cpp part3_helpers.hpp
	flex lexer.lex 

part3: lex.yy.c parser.tab.cpp code_class.cpp part3_helpers.cpp symbol_table.cpp part3_helpers.hpp
	g++ -std=c++11 -o rx-cc parser.tab.cpp lex.yy.c part3_helpers.cpp code_class.cpp symbol_table.cpp

clean:
	rm -f *.o parser.tab.cpp lex.yy.c parser.tab.h *.gch rx-cc
