#ifndef _CODE_H
#define _CODE_H
// #include <map>
#include <iostream>
#include <vector>
#include <string>
#include <list>

using namespace std;

class bpList : public list<int> {};

class codeClass {
protected:

	vector<string> codeVec;
	
public:
	// ~code() {}

	/**
	 * @brief Emit code
	 * 
	 * @param c Code to emit
	 * @return int Line number of emitted code
	 */
	int emit(string c);

	/**
	 * @brief Get next line number of code
	 * 
	 * @return int next line number of code
	 */
	int nextquad();

	/**
	 * @brief Backpatch 'goto 0' commands
	 * 
	 * @param lineList List of line numbers with 'goto 0' to be back-patched
	 * @param lineNo The line number to be filled in
	 */
	void backpatch(bpList lineList, int lineNo);

	/**
	 * @brief Print all code by order
	 * 
	 */
	void print();
};



#endif
