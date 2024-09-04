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

void Parser::checkFnSig(_Function &_fn)
{
	// Check if function def exists and make sure they match
	for (auto &curFn : functions)
	{
		if (_fn.name == curFn.name)
		{
			if (!(_fn == curFn))
			{
				throwError("function signatures must match", currentToken->lineNumber);
			}
		}
	}
}

void Parser::func()
{
	if (currentToken->type == TokenType::FN)
	{
		_Function _fn;
		currentToken++;

		if (currentToken->type == TokenType::IDENTIFIER)
			throwError("fn must be followed by identifier", currentToken->lineNumber);
		// TODO: make sure identifier isnt already taken by a variable
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
			checkFnSig(_fn);
		}
	}
}

void Parser::var()
{
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
	currentLexLevel = 0;
	document();
}
