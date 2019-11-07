#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<type_traits>

using namespace std;
int flag = 0;

bool isKeyword(char input[]) {
	char keyWords[20][10] = { "int", "float", "bool", "if", "else", "then", "endif", "while",
		"whileend", "do", "doend", "for", "forend", "input", "output",
		"and", "or", "function", "void", "main" };
	int i;

	for (i = 0; i < 32; ++i) {
		if (strcmp(keyWords[i], input) == 0) {
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------------------

void syntaxId() {

}

void syntaxSep() {

}

void syntaxKey() {

}

void syntaxNum() {

}

//-----------------------------------------------------------------------------------------


int main() {
	char testChar, testWord[20], operators[] = "+-*/%=", separators[] = "'(){}[],.:;!";
	ifstream file("SampleInputFile.txt");
	int i, j = 0;

	ofstream myfile;
	myfile.open("Syntax Analysis");    //makes file named Lexical Analysis
	myfile << "      SYNTAX ANALYSIS" << endl;
	myfile << "------------------------" << endl;
	//myfile << "TOKENS \t\tLEXEMES " << endl << endl;


	if (!file.is_open()) {
		//cout << "Error while opening the file, please change file name to SampleInputFile\n";
		cout << "EXITED";
		exit(0);
	}

	while (!file.eof())
	{
		//We get the first character of each word
		testChar = file.get();
		//cout << "\tTEST CHAR IS " << testChar;

		//check if comment
		if (testChar == '!') {
			testChar = file.get();
			while (testChar != '!') {
				testChar = file.get();
			}
		}

			//Check operator
			for (i = 0; i < 6; ++i)
			{
				if (testChar == operators[i]) {
					//cout << testChar << " is operator\n";
					myfile << "\nToken:\tOPERATOR" << "\t\tLexme:\t" << testChar << endl;
					cout << "\nToken:\tOPERATOR" << "\t\tLexme:\t" << testChar << endl;
					//operator has no syntax output
					flag = 1;
				}
			}
			//Check seprator
			for (i = 0; i < 11; ++i)
			{
				if (testChar == separators[i]) {
					//cout << testChar << " is separator\n";
					myfile << "\nToken:\tSEPARATOR" << "\t\tLexme:\t" << testChar << endl;
					cout << "\nToken:\tSEPARATOR" << "\t\tLexme:\t" << testChar << endl;
					syntaxSep();
					flag = 1;
				}
			}
			//Check if number is real or integer??????
			if (isdigit(testChar) && !isalnum(testChar - 1))
			{
				//cout << testChar << " is a number\n";
				myfile << "\nToken:\tNUMBER:" << "\t\tLexme:\t" << testChar << endl;
				cout << "\nToken:\tNUMBER:" << "\t\tLexme:\t" << testChar << endl;
				syntaxNum();
			}

			//If the first character is not operator nor the seperator, get all the rest of word until reach space
			if (isalnum(testChar) || testChar == '$')
			{
				testWord[j++] = testChar;
			}
			else if ((testChar == ' ' || flag == 1 || testChar == '\n') && (j != 0))
			{
				//Make the rest of c string become null
				testWord[j] = '\0';
				//reset new index of testWord for next testWord
				j = 0;

				if (isKeyword(testWord)) {
					//cout << testWord << " is keyword\n";
					myfile << "\nToken:\tKEYWORD" << "\t\t\tLexme:\t" << testWord << endl;
					cout << "\nToken:\tKEYWORD" << "\t\t\tLexme:\t" << testWord << endl;
					syntaxKey();
				}
				else {
					//cout << testWord << " is identifier\n";
					myfile << "\nToken:\tIDENTIFIER" << "\t\tLexme:\t" << testWord << endl;
					cout << "\nToken:\tIDENTIFIER" << "\t\tLexme:\t" << testWord << endl;
					syntaxId();
				}

				flag = 0;
			}
	}
	
	file.close();
	myfile.close();

	return 0;
}
