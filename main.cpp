/* 
 * Assignment: CS460 Interpreter
 * Authors: Evan Walters
 * Date: 2/8/24
 * Description:
 * 
 */

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {

	// open file
	ifstream codeFile;
	//string fileName = "programming_assignment_1-test_file_1.c";
	codeFile.open(argv[1]);

	// check if file opened
	if (!codeFile.is_open()) {
		cerr << "Couldn't open file: " << argv[1] << endl;
		exit(0);
	}

	// get char count in file
	codeFile.seekg(0, codeFile.end);
	int charCount = codeFile.tellg();
	codeFile.seekg(0, codeFile.beg);	// go back to beginning of file

	// close file
	codeFile.close();

	return 0;
}
