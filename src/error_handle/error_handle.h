#pragma once
#include <string>
#include <iostream>
using namespace  std;

enum class error_handle_type		// ������������ ������ �� ������ ������ �������
{
	LEXER,
	PARSER,
	SEMANTIC,
	CODE_GENERATION
};

class error_handle
{
public:
	static void raise(error_handle_type error_type, const string& message)   // �����, ����������� ������ ���������� ��� ������, 
	{																		// � ������ ����������� �� ���������, �������  ����� �������
		string error_begin;

		switch (error_type)
		{
			case error_handle_type::LEXER:
				error_begin = "Lexical error!";
				break;
			case error_handle_type::PARSER:
				error_begin = "Parser error!";
				break;
			case error_handle_type::SEMANTIC:
				error_begin = "Simantic error!";
				break;
			case error_handle_type::CODE_GENERATION:
				error_begin = "Code generation error!";
				break;
		}

		string error_message = error_begin + message;		// ��� ������ + �������������� ���������
		
		throw logic_error(error_message);
	}
};																	 