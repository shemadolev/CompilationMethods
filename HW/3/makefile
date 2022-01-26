all: part3

parser.tab.cpp: parser.ypp part2_helpers.hpp
	bison -vd parser.ypp

lex.yy.c: lexer.lex part2_helpers.hpp
	flex lexer.lex 

part3: lex.yy.c part2_helpers.cpp parser.tab.cpp
	g++ -std=c++11 -o part2 parser.tab.cpp lex.yy.c part2_helpers.cpp


code_class.o: code_class.cpp code_class.hpp part3_helpers.hpp
part3_helpers.o: part3_helpers.cpp part3_helpers.hpp code_class.hpp \
 symbol_table.hpp
symbol_table.o: symbol_table.cpp symbol_table.hpp part3_helpers.hpp \
 code_class.hpp
test.o: test.cpp code_class.hpp
code_class.o: code_class.hpp
part3_helpers.o: part3_helpers.hpp code_class.hpp
symbol_table.o: symbol_table.hpp part3_helpers.hpp code_class.hpp


clean:
	rm -f *.o part2 parser.tab.cpp lex.yy.c parser.tab.h
