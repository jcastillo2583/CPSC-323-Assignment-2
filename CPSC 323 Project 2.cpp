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

string syntaxId() {
	string str;
	str = "<Statement List>";
	return str;
}

string syntaxSep() {
	string str;
	str = "<>";
	return str;
}

string syntaxKey() {
	string str;
	str = "<>";
	return str;
}

string syntaxNum() {
	string str;
	str = "<>";
	return str;
}

string syntaxOp() {
	string str;
	str = "<Epsilon>";
	return str;
}

//-----------------------------------------------------------------------------------------


void lexer(ifstream &file, ofstream &myfile, char &testChar, int &j, char testWord[20], int &lineNum) {
	bool print = false, printline = true, test = false;
	char operators[] = "+-*/%=", separators[] = "'(){}[],.:;!";
	int i;


	//check if comment
	if (testChar == '!') {
		testChar = file.get();
		while (testChar != '!') {
			testChar = file.get();
		}
		return;
	}

	//Check operator
	for (i = 0; i < 6; ++i)
	{
		if (testChar == operators[i]) {
			//cout << testChar << " is operator\n";
			myfile << "\n\nToken:\tOPERATOR" << "\t\tLexme:\t" << testChar;
			if (printline)
				myfile << " at line " << lineNum;
			myfile << endl;
			if (print)
				cout << "\nToken:\tOPERATOR" << "\t\tLexme:\t" << testChar << endl;
			myfile << syntaxOp();
			flag = 1;
			return;
		}
	}

	//Check seprator
	for (i = 0; i < 11; ++i)
	{
		if (testChar == separators[i]) {
			//cout << testChar << " is separator\n";
			myfile << "\n\nToken:\tSEPARATOR" << "\t\tLexme:\t" << testChar;
			if (printline)
				myfile << " at line " << lineNum;
			myfile << endl;
			if (print)
				cout << "\nToken:\tSEPARATOR" << "\t\tLexme:\t" << testChar << endl;
			myfile << syntaxSep();
			flag = 1;
			return;
		}
	}

	//Check if number is real or integer??????**************************************
	if (isdigit(testChar))
	{
		//cout << testChar << " is a number\n";
		char str[5];
		int x = 0;
		testWord[j] = '\0';
		j = 0;
		str[x] = testChar;

		while (isdigit(file.peek())) {
			testChar = file.get();
			str[x] = testChar;
			x++;
		}

		myfile << "\n\nToken:\tNUMBER" << "\t\t\tLexme:\t";
		if (x == 0)
			myfile << str[0];
		else {
			for (int u = 0; u < x; u++) {
				testWord[j] = '\0';
				myfile << str[u];
			}
		}
		if (printline)
			myfile << " at line " << lineNum;
		myfile << endl;
		flag = 0;
		return;
	}

	//If the first character is not operator nor the seperator, get all the rest of word until reach space
	if (isalnum(testChar) || testChar == '$')
	{
		//cout << "IS ALNUM: " << testChar << "\tj = " << j << endl;
		bool stop = false;
		char next;
		if (testChar == ' ' ||  testChar == '\n')
			stop = true;
		while (!stop) {
			testWord[j++] = testChar;
			next = file.peek();
			for (i = 0; i < 11; ++i)
			{
				if (next == separators[i]) {
					stop = true;
				}
			}
			for (i = 0; i < 6; ++i)
			{
				if (next == operators[i]) {
					stop = true;
				}
			}
			if(!stop)
				testChar = file.get();
			if (testChar == ' ' ||  testChar == '\n')
				stop = true;
		}

		test = true;
		//Make the rest of c string become null
		testWord[j] = '\0';
		//reset new index of testWord for next testWord
		j = 0;


		if (isKeyword(testWord)) {
			myfile << "\n\nToken:\tKEYWORD" << "\t\t\tLexme:\t" << testWord;
			if (printline)
				myfile << " at line " << lineNum;
			myfile << endl;
			if (print)
				cout << "\nToken:\tKEYWORD" << "\t\t\tLexme:\t" << testWord << endl;
			myfile << syntaxKey();
		}
		else {
			myfile << "\n\nToken:\tIDENTIFIER" << "\t\tLexme:\t" << testWord;
			if (printline)
				myfile << " at line " << lineNum;
			myfile << endl;
			if (print)
				cout << "\nToken:\tIDENTIFIER" << "\t\tLexme:\t" << testWord << endl;
			myfile << syntaxId();
		}
		flag = 0;
	}
	//cout << test << endl;

}





//-----------------------------------------------------------------------------------------


int main() {
	ifstream file("SampleInputFile.txt");
	char testWord[20], testChar = ' ';
	int lineNum = 1;

	ofstream myfile;
	myfile.open("Syntax Analysis");    //makes file named Syntax Analysis
	myfile << "            SYNTAX ANALYSIS" << endl;
	myfile << "----------------------------------------------------" << endl;

	if (!file.is_open()) {
		//cout << "Error while opening the file, please change file name to SampleInputFile\n";
		cout << "EXITED";
		exit(0);
	}


	
	int i, j = 0;
	while (!file.eof())
	{
		if (testChar == '\n')
			lineNum++;
		testChar = file.get();
		lexer(file, myfile, testChar, j, testWord, lineNum);
	}
	
	file.close();
	myfile.close();

	return 0;
}
