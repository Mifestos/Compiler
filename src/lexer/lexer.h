#pragma once
#include <vector>
#include <fstream>
#include "token/token.h"
#include "../error_handle/error_handle.h"

class lexer
{
private:							
	string file_path;           // путь к файлу
	string code;				// строка с кодом

	vector<token> tokens;		// вектор с токенами

	size_t current_token_index;	// индекс токена

public:
	explicit lexer(const string& file_path)			// конструктор, принимает только путь к файлу
	{
		this->file_path = file_path;				// запись пути к файлу
		this->current_token_index = 0;
		this->open();		
	}

	

	void parse()										// интерфейсная функция
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
		return tokens[current_token_index];			// возврат токена по индексу
	}

	token_type current_token_type()
	{
		return current_token().type;				// возврат типа токена по ниндексу
	}

	void next_token()								// бегунок индекса
	{
		++current_token_index;
	}

private:

	void open()										// открывает файл, считывает код и кладет его в переменную code
	{
		ifstream in(file_path, ios::binary);		// бинарный тип открытия файла, чтобы считывать целиком

		if (!in.is_open())							// если файл не открыт
		{
			error_handle::raise(error_handle_type::LEXER, "File not found");
		}

		size_t size = in.seekg(0, ios::end).tellg();	 // получение размера файла путем сдвига указателя
		in.seekg(0);									// возвращение указателя на ноль

		code.resize(size);								// делаем размер кода равный размеру файла
		in.read(&code[0], size);						// считываем size байт из файла по адресу начиная с первого строки code
		in.close();										// закрытие потока считывания данных из файла
	}


	void check()										// проверка коректности индефекаторов
	{
		for (auto& token : tokens)						// проход по всем токенам
		{
			if (token.type == token_type::IDENTIFIER)
			{
				if (!is_correct_identifier(token.lexeme))			// если не коректен идентификатор
				{
					error_handle::raise(error_handle_type::LEXER, "Incorrect identifier!");
				}
			}
		}
	}

	static bool is_correct_identifier(const string& lexeme)
	{
		if (!isalpha(lexeme[0]) && lexeme[0] != '_')			// если не буква и не водчеркивание
			return false;
		
		for (int i = 1; i < lexeme.size(); ++i)
			if (!isalpha(lexeme[i]) && !isdigit(lexeme[i])  && lexeme[i] != '_')
				return false;

		return true;
	}

	void merge()										// склейка токенов
	{
		vector<token> temp_tokens;						// временный массив токенов, куда мы будем класть склеееные токены

		for (int i = 0; i < tokens.size(); ++i)
		{
			if (tokens[i].type == token_type::INTEGER_CONST)				// склейка real
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
			if (i + 1 < tokens.size() && is_complex_operator(tokens[i].lexeme[0], tokens[i + 1].lexeme[0])) // передачи первого символа лексемы
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

	void split()										// функция разбиения файла на токены
	{

		string temp_lexeme;								// времеенная лексема

		for (const auto& symbol : code)					// обход посимвольно
		{
			if (is_separate_symbol(symbol))
			{
				if (!temp_lexeme.empty())				// если временная лексема не пуста
				{
					token new_token(temp_lexeme);		// создаем переменную типа token и ложим туда темповую переменную
					tokens.push_back(new_token);		// добавляем в массив tokens новый токен
					temp_lexeme.clear();				// очиста темповой лексемы

				}

				if (symbol != ' ' && symbol != '\n' && symbol != '\r')		// если разделитель не пробел, 
				{															// перенос строки или не возврат каретки, то он токен
					string current_symbol(1, symbol);						// если разделитель - часть токена
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

	

	static bool is_separate_symbol(char symbol)			// true, если разделитель
	{
		return  symbol == ':' || symbol == ';' ||		// список разделителей
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