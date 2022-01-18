#ifndef _CODE_H
#define _CODE_H
// #include <map>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <fstream>

using namespace std;

class CodeLineList : public list<int> {
public:
	/**
	 * @brief Construct a new empty Code Line List object (for backpatching)
	 * 
	 */
	CodeLineList();

	/**
	 * @brief Construct a new Code Line List object (for backpatching)
	 * 
	 * @param line The first line to insert to the list
	 */
	CodeLineList(int line);
};

class CodeClass {
protected:
	vector<string> codeVec;

	string spreadString(string str);

	template<typename T, typename... Args>
	string spreadString(T t, Args... args);

public:

	/**
	 * @brief Emit code
	 * 
	 * @tparam Args Arguments of type string
	 * @param args Arguments of type string
	 * @return int Line number of emitted code
	 */
	template<typename... Args>
	int emit(Args... args);

	/**
	 * @brief Get next line number of code
	 * 
	 * @return int next line number of code
	 */
	int nextQuad();

	/**
	 * @brief Backpatch 'goto -1' commands
	 * 
	 * @param lineList List of line numbers with 'goto 0' to be back-patched
	 * @param lineNo The line number to be filled in
	 */
	void backpatch(CodeLineList lineList, int lineNo);

	/**
	 * @brief Print all code by order
	 * 
	 */
	void print(ofstream& file);
};



#endif
