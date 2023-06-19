#pragma once

#include <string>
#include <iostream>
 
using namespace std;

enum class token_type
{
    IDENTIFIER,


    INTEGER_CONST,
    REAL_CONST,
    STRING_CONST,
    CHAR_CONST,

    // boolean constant
    TRUE,
    FALSE,


    CONST,


    // types
    UNDEFINED,

    INT,    // %
    REAL,   // !
    BOOL,   // $
    CHAR,   // @

    // cycles
    WHILE,
    FOR,
    TO,
    DO,

    // cycles addition

    BREAK,
    CONTINUE,


    // conditions
    IF,
    THEN,
    ELSE,


    // relationship operators
    LESS, // <
    GREATER, // >
    LESS_EQUAL, // <=
    GREATER_EQUAL, // >=


    // equal operators
    EQUAL, // =
    NOT_EQUAL, // <>


    // logical operators
    AND, // and
    OR, // or
    UNARY_NO, // not

    // math operators
    PLUS, // +
    MINUS, // -
    STAR, // *
    SLASH, // /

    // brackets
    LPAR, // (
    RPAR, // )


    // assign
    ASSIGN, // ass

    

    // other symbols
    SEMICOLON, // ;
    COLON, // :
    COMMA, // ,
    POINT, // .

    // comment
    ONE_COMMENT,  // {...
    TWO_COMMENT,  // ...}
    

    NEW,
    DELETE,


    PREPROCESSOR_DIRECTIVE,


    ACCESS_OPERATOR, 

    // user
    READ,   // read
    WRITE,  // write

    // strucrure
    PROGRAM, // program
    VAR,     // var
    BEGIN,   // begin
    END,     // end
};

class token
{
public:
	string lexeme;
	token_type type;
    
public:
    explicit token(const string& lexeme)            // конструктор
    {
        this->lexeme = lexeme;
        this->type = which_token_type(lexeme);
    }

public:
    void print()
    {
        cout << lexeme << " with type: " << token_type_to_string(type) << endl;
    }

private:

    static token_type which_token_type(const string& lexeme)
    {
        if (lexeme == "%")
            return token_type::INT;
        if (lexeme == "!")
            return token_type::REAL;
        if (lexeme == "$")
            return token_type::BOOL;
        if (lexeme == "@")
            return token_type::CHAR;
       

        // cycles
        if (lexeme == "for")
            return token_type::FOR;
        if (lexeme == "to")
            return token_type::TO;
        if (lexeme == "while")
            return token_type::WHILE;
        if (lexeme == "do")
            return token_type::DO;
        // cycles addition
        if (lexeme == "break")
            return token_type::BREAK;
        if (lexeme == "continue")
            return token_type::CONTINUE;



        // conditions
        if (lexeme == "if")
            return token_type::IF;
        if (lexeme == "then")
            return token_type::THEN;
        if (lexeme == "else")
            return token_type::ELSE;



        // relationship operators
        if (lexeme == ">")
            return token_type::GREATER;
        if (lexeme == "<")
            return token_type::LESS;
        if (lexeme == "<=")
            return token_type::LESS_EQUAL;
        if (lexeme == ">=")
            return token_type::GREATER_EQUAL;



        // equal operators
        if (lexeme == "=")
            return token_type::EQUAL;
        if (lexeme == "<>")
            return token_type::NOT_EQUAL;

        



        // logical operators
        if (lexeme == "and")
            return token_type::AND;
        if (lexeme == "or")
            return token_type::OR;
        if (lexeme == "not")
            return token_type::UNARY_NO;


        // math operators
        if (lexeme == "+")
            return token_type::PLUS;
        if (lexeme == "-")
            return token_type::MINUS;
        if (lexeme == "*")
            return token_type::STAR;
        if (lexeme == "/")
            return token_type::SLASH;
        

        // brackets
        if (lexeme == "(")
            return token_type::LPAR;
        if (lexeme == ")")
            return token_type::RPAR;


        // assign
        if (lexeme == "ass")
            return token_type::ASSIGN;
       

        // new
        if (lexeme == "new")
            return token_type::NEW;
        if (lexeme == "delete")
            return token_type::DELETE;



        // boolean constants
        if (lexeme == "true")
            return token_type::TRUE;
        if (lexeme == "false")
            return token_type::FALSE;



        // other symbols
        if (lexeme == ";")
            return token_type::SEMICOLON;
        if (lexeme == ":")
            return token_type::COLON;
        if (lexeme == ",")
            return token_type::COMMA;
        if (lexeme == ".")
            return token_type::POINT;
        if (lexeme == "::")
            return token_type::ACCESS_OPERATOR;


        // comment
        if (lexeme == "{")
            return token_type::ONE_COMMENT;
        if (lexeme == "}")
            return token_type::TWO_COMMENT;

        // struct
        if (lexeme[0] == '#')
            return token_type::PREPROCESSOR_DIRECTIVE;
        if (lexeme == "program")
            return token_type::PROGRAM;
        if (lexeme == "var")
            return token_type::VAR;
        if (lexeme == "begin")
            return token_type::BEGIN;
        if (lexeme == "end")
            return token_type::END;


        // const
        if (is_integer(lexeme))
            return token_type::INTEGER_CONST;

        if (is_real(lexeme))
            return token_type::REAL_CONST;

        if (is_string(lexeme))
            return token_type::STRING_CONST;

        if (is_char(lexeme))
            return token_type::CHAR_CONST;


        return token_type::IDENTIFIER;
    }

    static bool is_string(const string& current_lexeme)
    {
        return current_lexeme.front() == '"' && current_lexeme.back() == '"';       // признак строки

    }
    static bool is_char(const string& current_lexeme)
    {
        return current_lexeme.front() == '\'' && current_lexeme.back() == '\'';       // признак символа
       
    }      
    static bool is_integer(const string& lexeme)                                      // признак целого числа
    {
        for (const auto& symbol : lexeme)       
            if (symbol < '0' || symbol > '9')
                return false;
        return true;
        
    }
    static bool is_real(const string& lexeme)                                      // признак нецелого числа
    {
        bool has_point = false;

        for (const auto& symbol : lexeme)
        {
            
            if (symbol == '.')                                                      // проверка на количество точек
            {
                if (has_point)
                {
                    return false;
                }
                else
                {
                    has_point = true;
                }
            }
            else if (symbol < '0' || symbol > '9')
            {
                return false;
            }
        }

        return true;
    }

    static string token_type_to_string(token_type type)         // вывод коментария к типу токена
    {
        switch (type)
        {
        case token_type::IDENTIFIER:
        {
            return "identifier";
        }
        case token_type::INTEGER_CONST:
        {
            return "integer constant";
        }
        case token_type::REAL_CONST:
        {
            return "real constant";
        }
        case token_type::STRING_CONST:
        {
            return "string constant";
        }
        case token_type::CHAR_CONST:
        {
            return "char constant";
        }
        case token_type::TRUE:
        {
            return "true";
        }
        case token_type::FALSE:
        {
            return "false";
        }
        case token_type::CONST:
        {
            return "const";
        }
        case token_type::UNDEFINED:
        {
            return "undefined";
        }
        case token_type::INT:
        {
            return "int";
        }
        case token_type::REAL:
        {
            return "real";
        }
        case token_type::BOOL:
        {
            return "bool";
        }
        case token_type::CHAR:
        {
            return "char";
        }
        //case token_type::DO_WHILE:
        //{
        //    return "do while";
        // }
        case token_type::WHILE:
        {
            return "while";
        }
        case token_type::FOR:
        {
            return "for";
        }
        case token_type::TO:
        {
            return "to";
        }
        case token_type::DO:
        {
            return "do";
        }
        case token_type::BREAK:
        {
            return "break";
        }
        case token_type::CONTINUE:
        {
            return "continue";
        }
        case token_type::IF:
        {
            return "if";
        }
        case token_type::THEN:
        {
            return "then";
        }
        case token_type::ELSE:
        {
            return "else";
        }
        case token_type::LESS:
        {
            return "less";
        }
        case token_type::GREATER:
        {
            return "greater";
        }
        case token_type::LESS_EQUAL:
        {
            return "less and equal";
        }
        case token_type::GREATER_EQUAL:
        {
            return "greater and equal";
        }
        case token_type::EQUAL:
        {
            return "equal";
        }
        case token_type::NOT_EQUAL:
        {
            return "not equal";
        }
        case token_type::AND:
        {
            return "logic and";
        }
        case token_type::OR:
        {
            return "logic or";
        }
        case token_type::UNARY_NO:
        {
            return "unary no";
        }
        case token_type::PLUS:
        {
            return "plus";
        }
        case token_type::MINUS:
        {
            return "minus";
        }
        case token_type::STAR:
        {
            return "star";
        }
        case token_type::SLASH:
        {
            return "true";
        }
        case token_type::LPAR:
        {
            return "lpar";
        }
        case token_type::RPAR:
        {
            return "rpar";
        }
        case token_type::ASSIGN:
        {
            return "assign";
        }
        case token_type::SEMICOLON:
        {
            return "semicolon";
        }
        case token_type::COLON:
        {
            return "colon";
        }
        case token_type::COMMA:
        {
            return "comma";
        }
        case token_type::POINT:
        {
            return "point";
        }
        case token_type::ONE_COMMENT:
        {
            return "one comment";
        }
        case token_type::TWO_COMMENT:
        {
            return "two comment";
        }
        case token_type::NEW:
        {
            return "new";
        }
        case token_type::DELETE:
        {
            return "delete";
        }
        case token_type::PREPROCESSOR_DIRECTIVE:
        {
            return "preprocessor directive";
        }
        case token_type::ACCESS_OPERATOR:
        {
            return "access operator";
        }
        case token_type::PROGRAM:
        {
            return "program";
        }
        case token_type::VAR:
        {
            return "var";
        }
        case token_type::BEGIN:
        {
            return "begin";
        }
        case token_type::END:
        {
            return "end";
        }
        case token_type::READ:
        {
            return "read";
        }
        case token_type::WRITE:
        {
            return "write";
        }

        default:
            return "";
        }
    }

 };