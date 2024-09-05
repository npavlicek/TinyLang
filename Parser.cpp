#include "Parser.hpp"

void Parser::throwError(std::string error, int lineNumber)
{
	std::stringstream errorString;
	errorString << "[Error] " << error << ". at line: " << lineNumber;
	throw std::runtime_error(errorString.str());
}

std::vector<_Arg> Parser::funcArgs()
{
	std::vector<_Arg> args;

	while (Lexer::isPrimitiveType(currentToken->type))
	{
		_Arg curArg;
		curArg.type = currentToken->type;
		currentToken++;

		if (currentToken->type != TokenType::IDENTIFIER)
			throwError("arg type must be followed by name", currentToken->lineNumber);
		curArg.name = identifiers.at(currentToken->identIndex);
		args.push_back(curArg);
		currentToken++;

		if (currentToken->type != TokenType::COMMA)
			break;

		currentToken++;
	}

	return args;
}

bool Parser::isIdentAvailVar(std::string ident)
{
	for (auto &var : variables)
	{
		if (var.name == ident && var.alive)
			return false;
	}
	return true;
}

bool Parser::isIdentAvailFunc(std::string ident)
{
	for (auto &func : functions)
	{
		if (func.name == ident)
			return false;
	}
	return true;
}

int Parser::getFunction(std::string name)
{
	for (int i = 0; i < functions.size(); i++)
	{
		if (functions.at(i).name == name)
			return i;
	}
	return -1;
}

void Parser::funcBody(_Function &fn)
{
	currentScopeLevel++;
	currentScopeLevel--;
}

void Parser::func()
{
	if (currentToken->type == TokenType::FN)
	{
		_Function _fn;
		currentToken++;

		if (currentToken->type == TokenType::IDENTIFIER)
			throwError("fn must be followed by identifier", currentToken->lineNumber);
		if (!isIdentAvailVar(identifiers.at(currentToken->identIndex)))
			throwError("identifier is already used as variable name", currentToken->lineNumber);
		_fn.name = identifiers.at(currentToken->identIndex);
		currentToken++;

		if (currentToken->type != TokenType::OPEN_PAREN)
			throwError("function identifier must be followed by a parenthesis", currentToken->lineNumber);
		currentToken++;

		_fn.args = funcArgs();

		if (currentToken->type != TokenType::CLOSE_PAREN)
			throwError("function args must be followed by closing parenthesis", currentToken->lineNumber);
		currentToken++;

		if (currentToken->type == TokenType::SEMI_COLON)
		{
			currentToken++;
			int funcIdx = getFunction(_fn.name);
			if (funcIdx != -1)
			{
				if (!(_fn == functions.at(funcIdx)))
					throwError("function signature mismatch", currentToken->lineNumber);
			}

			_fn.defined = false;
			functions.push_back(_fn);
			return;
		}

		if (currentToken->type == TokenType::OPEN_BRACK)
		{
			currentToken++;
			funcBody(_fn);
			if (currentToken->type != TokenType::CLOSE_BRACK)
				throwError("function body must be closed with matching bracket", currentToken->lineNumber);
			currentToken++;

			return;
		}

		throwError("function declaration must be followed by semicolon or body", currentToken->lineNumber);
	}
}

void Parser::var()
{
	if (Lexer::isPrimitiveType(currentToken->type))
	{
		_Var curVar;
		curVar.type = currentToken->type;
		currentToken++;

		if (currentToken->type != TokenType::IDENTIFIER)
			throwError("var declaration must be followed by identifier", currentToken->lineNumber);

		if (!isIdentAvailVar(identifiers.at(currentToken->identIndex)))
			throwError("var already declared", currentToken->lineNumber);
		curVar.name = identifiers.at(currentToken->identIndex);
		currentToken++;

		if (currentToken->type == TokenType::EQUALS_SIGN)
		{
			currentToken++;
			// TODO: IMPLEMENT EXPRESSIONS BECAUSE GLOBAL VARS CAN BE ASSIGNED TO EXPRESSIONS
			// TODO: emit the initialization code at the top of the assembly
		}

		if (currentToken->type != TokenType::SEMI_COLON)
			throwError("global variable declaration must end with a semicolon or assignment", currentToken->lineNumber);
	}
}

void Parser::document()
{
	while (currentToken != tokens.end())
	{
		func();
		var();
	}
}

void Parser::parse()
{
	currentToken = tokens.begin();
	currentScopeLevel = 0;
	document();
}
