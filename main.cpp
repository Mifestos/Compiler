#include <iostream>

#include "src/parser/parser.h"

int main()
{
/*
	try
	{
		lexer lex("test.cpp");
		lex.open();
	}
	catch (logic_error& error)
	{
		cout << error.what() << endl;
	}
*/
	try
	{
		lexer lex("test.cpp");
		lex.parse();
		lex.print();
	}
	catch (logic_error& error)
	{
		cout << error.what() << endl;
	}

	return 0;
}
