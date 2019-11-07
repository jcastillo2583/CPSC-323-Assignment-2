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



int main() {
	char testChar, testWord[20], operators[] = "+-*/%=", separators[] = "'(){}[],.:;!";
	ifstream file("SampleInputFile.txt");
	int i, j = 0;

	ofstream myfile;
	myfile.open("Lexical Analysis");    //makes file named Lexical Analysis
	myfile << "      LEXICAL ANALYSIS" << endl;
	myfile << "------------------------" << endl;
	myfile << "TOKENS \t\tLEXEMES " << endl << endl;


	if (!file.is_open()) {
		//cout << "Error while opening the file, please change file name to SampleInputFile\n";
		exit(0);
	}

	while (!file.eof())
	{
		//We get the first character of each word
		testChar = file.get();

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
				myfile << "OPERATOR:      " << testChar << endl;
				flag = 1;
			}
		}
		//Check seprator
		for (i = 0; i < 11; ++i)
		{
			if (testChar == separators[i]) {
				//cout << testChar << " is separator\n";
				myfile << "SEPARATOR:     " << testChar << endl;
				flag = 1;
			}
		}
		//Check if number is real or integer??????
		if (isdigit(testChar) && !isalnum(testChar - 1))
		{
			//cout << testChar << " is a number\n";
			myfile << "NUMBER:        " << testChar << endl;
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
				myfile << "KEYWORD:       " << testWord << endl;
			}
			else {
				//cout << testWord << " is identifier\n";
				myfile << "IDENTIFIER:    " << testWord << endl;
			}

			flag = 0;
		}
	}

	file.close();
	myfile.close();

	//cout << "Program complete" << endl << endl;

	return 0;
}
