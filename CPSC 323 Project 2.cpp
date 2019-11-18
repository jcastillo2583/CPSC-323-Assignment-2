#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<type_traits>

using namespace std;
char testChar = ' ', stack[20] = " ", testWord[20];
char testCharList[20];
char keyWords[20][10] = { "int", "float", "bool", "if", "else", "then", "endif", "while",
		"whileend", "do", "doend", "for", "forend", "input", "output",
		"and", "or", "function", "void", "main" };
bool conditionset;
int flag = 0, lineNum = 1, stackindex = 0;
ifstream file("SampleInputFile.txt");
ofstream myfile;

bool isKeyword(char input[]) {	
	int i;

	for (i = 0; i < 32; ++i) {
		if (strcmp(keyWords[i], input) == 0) {
			return true;
		}
	}

	return false;
}

void syntaxError(string str)
{
	//Clear output file
	myfile.close();
	myfile.open("Syntax Analysis", ios::out|ios::trunc);
	// Output error message
	myfile << "ERROR: " << str << " at line: " << lineNum;
	exit(EXIT_FAILURE);
}


//-----------------------------------------------------------------------------------------

string syntaxId() {
	string str;
	char operators[] = "+-*/%";
	bool rightHandSide = true, firstOfLHS = true;

	for (int i = 0; i < strlen(testCharList); i++)
	{
		if (testCharList[i] == '=')
			rightHandSide = false;
	}

	for (int i = 0; i < strlen(testCharList); i++)
	{
		for (int j = 0; j < 5; j++)
			if (testCharList[i] == operators[j])
				firstOfLHS = false;
	}

	if (!rightHandSide)
	{
		if (firstOfLHS)
			myfile << "<Expression> -> <Term> <Expression Prime>\n";
		myfile << "<Term> -> <Factor><TermPrime>\n";
		myfile << "<Factor> -> <Identifier>\n";
	}
	else 
	{

		myfile << "<Statement> -> <Assign>\n";
		myfile << "<Assign> -> <Identifier> = <Expression>\n";
	}
	return str;
}

string syntaxSep() {
	string str = " <Separator> -> ";

	char openers[5] = { "([{'"}, closers[] = { ")]}'"};
	//openers[5] += '"';
	for (int a = 0; a < 4; a++) {
		if (testChar == openers[a] && stack[stackindex] != openers[a]) {//know testChar is a closing separator
			stackindex++;
			stack[stackindex] = testChar;//add separator to the stack

			if (testChar == '(') {
				str += " <Condition>\n";
				str += " <Condition> -> ( <StatementList>";
			}
			else {
				str += " <OpeningSeparator> <StatementList>\n";
				str += " <OpeningSeparator> -> " + testChar;
			}
			return str;
		}
		else if (testChar == closers[a]) {//know testChar is a closing separator
			if (openers[a] == stack[stackindex]) {
				//continue
				stack[stackindex] = ' ';
				stackindex--;
				if (testChar == ')') {
					str += " <Condition>\n";
					str += " <Condition> -> <StatementList> )";
					str += "\n <StatementList> -> Epsilon";
				}
				else {
					str += " <StatementList> <ClosingSeparator>\n";
					str += " <ClosingSeparator> -> " + testChar;
				}
				return str;
			}
			else
				syntaxError("Closing separator incompatible");
		}
	}
	str += " <EndSeparator>\n";
	return str;
}

string syntaxKey() {
	string str;

	char wordsWithParenthese[7][10] = { "if", "while", "for", "forend","function", "main" };
	for (int z = 0; z < 7; z++) {
		if (strcmp(testWord, wordsWithParenthese[z]) == 0)
			conditionset = true;
	}

	for (int a = 0; a < 20; a++) {
		if (strcmp(testWord, keyWords[a]) == 0) {
			string key = (string)keyWords[a];

			if (a < 3) { //is a variable
				str += " <KeyWord> -> <Variable>\n";
				str += " <Variable> -> <" + key + ">";
			}
			else if (a > 2 && a < 13) {//is a conditional
				str += " <KeyWord> -> <Conditional>\n";
				str += " <Conditional> -> <" + key + "> + <Separator>";
			}
			else {//is a function
				str += " <KeyWord> -> <Function>\n";
				str += " <Function> -> <" + key + ">";
			}
		}
	}
	return str;
}

string syntaxNum() {
	string str;
	str = " <Number> -> <Assign>";
	str = " <Assign> -> <" + (string)testWord + ">";
	return str;
}

string syntaxOp() {
	string str;
	if (testChar == '*') {
		myfile << " <TermPrime> -> * <Factor> <TermPrime>\n";
		myfile << " <ExpressionPrime> -> <Empty>\n";
	}
	else if (testChar == '/'){
		myfile << " <TermPrime> -> / <Factor> <TermPrime>\n";
		myfile << " <ExpressionPrime> -> <Empty>\n";
		}
	if (testChar == '+') {
		myfile << " <TermPrime> -> <Empty>\n";
		myfile << " <ExpressionPrime> -> + <Term> <ExpressionPrime>\n";
	}
	else if (testChar == '-') {
		myfile << " <TermPrime> -> <Empty>\n";
		myfile << " <ExpressionPrime> -> - <Term> <ExpressionPrime>\n";
	}
	myfile << " <Empty>->Epsilon\n";
		
		return str;
}

//-----------------------------------------------------------------------------------------


void lexer(int &j) {
	bool print = false, printline = false, test = false;
	char operators[] = "+-*/%=", separators[] = "'(){}[],.:;!";
	int i;

	//discard all spaces
	while (testChar == ' ')
		testChar = file.get();

	if (conditionset) {
		if (testChar != '(') {
			syntaxError("Condition not set");
		}
		conditionset = false;
	}

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
	for (int m = 0; m < 11; m++)
	{
		if (testChar == separators[m]) {
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

	//Check if number
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
		myfile << syntaxNum();
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
		testCharList[i] = testChar;
		i++;
		
		lexer(j);
	}
	if (stackindex != 0)
		syntaxError("Closing argument not found");
	
	file.close();
	myfile.close();

	return 0;
}
