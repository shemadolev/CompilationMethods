# Makefile to build Flex example

all: part1

part1: lex.yy.c
	gcc -ll lex.yy.c -o part1

lex.yy.c: part1.lex
	flex -o lex.yy.c part1.lex 

# Utility targets
.PHONY: clean test
clean:
	rm -f part1 lex.yy.c

