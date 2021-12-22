all: part2

parser.tab.cpp: parser.ypp part2_helpers.h
	bison -d parser.ypp

lex.yy.c: lexer.lex part2_helpers.h
	flex lexer.lex 

part2: lex.yy.c part2_helpers.c parser.tab.cpp
	g++ -o part2 parser.tab.cpp lex.yy.c part2_helpers.c

clean:
	rm -f *.o part2 parser.tab.cpp lex.yy.c parser.tab.h
