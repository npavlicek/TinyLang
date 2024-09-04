#pragma once

#include "Lexer.hpp"

struct _Arg
{
	std::string name;
	TokenType type;

	bool operator==(_Arg &other)
	{
		if (other.type == this->type)
			return true;
		return false;
	}
};

struct _Function
{
	std::string name;
	bool defined;
	TokenType returnType;
	std::vector<_Arg> args;

	bool operator==(_Function &other)
	{
		if (other.name != this->name)
			return false;

		if (other.returnType != this->returnType)
			return false;

		if (args.size() == other.args.size())
		{
			for (int i = 0; i < args.size(); i++)
			{
				if (!(args.at(i) == other.args.at(i)))
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}
};

class Parser
{
	std::vector<Token> &tokens;
	std::vector<std::string> &identifiers;
	std::vector<Token>::iterator currentToken;

	std::vector<_Function> functions;

	std::vector<_Arg> funcArgs();
	void func();
	void var();
	void document();

	void checkFnSig(_Function &_fn);

	void throwError(std::string error, int lineNumber);

	int currentLexLevel;

	std::stringstream asmOutput;

  public:
	Parser(std::vector<Token> &tokens, std::vector<std::string> &identifiers)
		: tokens(tokens), identifiers(identifiers){};
	void parse();
};
