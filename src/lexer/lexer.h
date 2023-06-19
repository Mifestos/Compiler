#pragma once
#include <vector>
#include <fstream>
#include "token/token.h"
#include "../error_handle/error_handle.h"

class lexer
{
private:							
	string file_path;           // ���� � �����
	string code;				// ������ � �����

	vector<token> tokens;		// ������ � ��������

	size_t current_token_index;	// ������ ������

public:
	explicit lexer(const string& file_path)			// �����������, ��������� ������ ���� � �����
	{
		this->file_path = file_path;				// ������ ���� � �����
		this->current_token_index = 0;
		this->open();		
	}

	

	void parse()										// ������������ �������
	{
		split();
		merge();
		check();

	}
	void print()
	{
		for (auto& token : tokens)
		{
			token.print();
		}
	}

	token current_token()
	{
		return tokens[current_token_index];			// ������� ������ �� �������
	}

	token_type current_token_type()
	{
		return current_token().type;				// ������� ���� ������ �� ��������
	}

	void next_token()								// ������� �������
	{
		++current_token_index;
	}

private:

	void open()										// ��������� ����, ��������� ��� � ������ ��� � ���������� code
	{
		ifstream in(file_path, ios::binary);		// �������� ��� �������� �����, ����� ��������� �������

		if (!in.is_open())							// ���� ���� �� ������
		{
			error_handle::raise(error_handle_type::LEXER, "File not found");
		}

		size_t size = in.seekg(0, ios::end).tellg();	 // ��������� ������� ����� ����� ������ ���������
		in.seekg(0);									// ����������� ��������� �� ����

		code.resize(size);								// ������ ������ ���� ������ ������� �����
		in.read(&code[0], size);						// ��������� size ���� �� ����� �� ������ ������� � ������� ������ code
		in.close();										// �������� ������ ���������� ������ �� �����
	}


	void check()										// �������� ����������� �������������
	{
		for (auto& token : tokens)						// ������ �� ���� �������
		{
			if (token.type == token_type::IDENTIFIER)
			{
				if (!is_correct_identifier(token.lexeme))			// ���� �� �������� �������������
				{
					error_handle::raise(error_handle_type::LEXER, "Incorrect identifier!");
				}
			}
		}
	}

	static bool is_correct_identifier(const string& lexeme)
	{
		if (!isalpha(lexeme[0]) && lexeme[0] != '_')			// ���� �� ����� � �� �������������
			return false;
		
		for (int i = 1; i < lexeme.size(); ++i)
			if (!isalpha(lexeme[i]) && !isdigit(lexeme[i])  && lexeme[i] != '_')
				return false;

		return true;
	}

	void merge()										// ������� �������
	{
		vector<token> temp_tokens;						// ��������� ������ �������, ���� �� ����� ������ ��������� ������

		for (int i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i].type == token_type::INTEGER_CONST)				// ������� real
			{
				if (i + 1  < tokens.size() && i + 2 < tokens.size() && tokens[i + 1].type == token_type::POINT && tokens[i + 2].type == token_type::INTEGER_CONST)
				{
					string new_lexeme = tokens[i].lexeme + tokens[i + 1].lexeme + tokens[i + 2].lexeme;
					token new_token(new_lexeme);		
					temp_tokens.push_back(new_token);

					i += 3;
					
				}
			}

			temp_tokens.push_back(tokens[i]);	
			

			
		}

		tokens = temp_tokens;
		temp_tokens.clear();

		for (int i = 0; i < tokens.size(); ++i)
		{
			if (i + 1 < tokens.size() && is_complex_operator(tokens[i].lexeme[0], tokens[i + 1].lexeme[0])) // �������� ������� ������� �������
			{
				string new_lexeme = tokens[i].lexeme + tokens[i + 1].lexeme;
				token new_token(new_lexeme);
				temp_tokens.push_back(new_token);

				i += 2;
			}
			
			temp_tokens.push_back(tokens[i]);
						
		}

		tokens = temp_tokens;
		temp_tokens.clear();

		for (int i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i].type == token_type::ONE_COMMENT)
			{

				while (tokens[i].type != token_type::TWO_COMMENT)
				{
					++i;
				}
				++i;
			}

			temp_tokens.push_back(tokens[i]);
		}

		tokens = temp_tokens;
		temp_tokens.clear();
	}

	static bool is_complex_operator(char symbol1, char symbol2)		
	{
		switch (symbol1)
		{
			case '<':
			{
				return symbol2 == '=' || '>';
			}
			case '>':
			{
				return symbol2 == '=';
			}


			default:
				return false;
		}
	}

	void split()										// ������� ��������� ����� �� ������
	{

		string temp_lexeme;								// ���������� �������

		for (const auto& symbol : code)					// ����� �����������
		{
			if (is_separate_symbol(symbol))
			{
				if (!temp_lexeme.empty())				// ���� ��������� ������� �� �����
				{
					token new_token(temp_lexeme);		// ������� ���������� ���� token � ����� ���� �������� ����������
					tokens.push_back(new_token);		// ��������� � ������ tokens ����� �����
					temp_lexeme.clear();				// ������ �������� �������

				}

				if (symbol != ' ' && symbol != '\n' && symbol != '\r')		// ���� ����������� �� ������, 
				{															// ������� ������ ��� �� ������� �������, �� �� �����
					string current_symbol(1, symbol);						// ���� ����������� - ����� ������
					token new_token(current_symbol);
					tokens.push_back(new_token);
				}
			}
			else
			{
				temp_lexeme += symbol;
			}
		}
	}

	

	static bool is_separate_symbol(char symbol)			// true, ���� �����������
	{
		return  symbol == ':' || symbol == ';' ||		// ������ ������������
				symbol == ',' || symbol == '.' ||
				symbol == '{' || symbol == '}' ||
				symbol == '(' || symbol == ')' ||
				symbol == '[' || symbol == ']' ||
				symbol == '*' || symbol == '/' ||
				symbol == '+' || symbol == '-' ||
				symbol == '&' || symbol == '|' ||
				symbol == '=' || symbol == '!' ||
				symbol == '<' || symbol == '>' ||
				symbol == '\'' || symbol == '"' ||
				symbol == '^' || symbol == '?' ||
				symbol == '%' || symbol == '\\' ||
				symbol == '~' || symbol == ' ' ||
				symbol == '\r' || symbol == '\n' ||
				symbol == '#';
	}
};