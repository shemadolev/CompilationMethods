all: part2

part2_helpers.c part2_helpers.h: parser.ypp
	bison -d parser.ypp

lex.yy.c: lexer.lex part2_helpers.h
	flex lex.yy.c lexer.lex 

part2: lex.yy.c part2_helpers.c part2_helpers.h
	g++ part2 parser.tab.cpp lex.yy.c

clean:
	rm -f *.o part2 parser.tab.cpp lex.yy.c parser.tab.h
