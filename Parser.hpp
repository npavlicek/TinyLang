#pragma once

#include "Lexer.hpp"

struct _Var
{
	std::string name;
	TokenType type;
	int scopeLevel;
	bool alive;
	Token assignedVal;

	bool operator==(_Var &other)
	{
		if (this->alive != other.alive)
			return false;

		if (this->scopeLevel != other.scopeLevel)
			return false;

		if (this->name != other.name)
			return false;

		if (this->type != other.type)
			return false;

		return true;
	}
};

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
	std::stringstream body;

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
	std::vector<_Var> variables;

	std::vector<_Arg> funcArgs();
	void funcBody(_Function &fn);
	void func();
	void var();
	void document();

	int getFunction(std::string name);
	bool isIdentAvailVar(std::string ident);
	bool isIdentAvailFunc(std::string ident);

	void throwError(std::string error, int lineNumber);

	int currentScopeLevel;

	std::stringstream asmOutput;

  public:
	Parser(std::vector<Token> &tokens, std::vector<std::string> &identifiers)
		: tokens(tokens), identifiers(identifiers){};
	void parse();
};
