all: part2

parser.tab.cpp: parser.ypp part2_helpers.hpp
	bison -vd parser.ypp

lex.yy.c: lexer.lex part2_helpers.hpp
	flex lexer.lex 

part2: lex.yy.c part2_helpers.cpp parser.tab.cpp
	g++ -std=c++11 -o part2 parser.tab.cpp lex.yy.c part2_helpers.cpp

clean:
	rm -f *.o part2 parser.tab.cpp lex.yy.c parser.tab.h
