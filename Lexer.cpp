#include "Lexer.hpp"

#include <iostream>

bool Lexer::tokenize()
{
	currentLineNumber = 0;
	while (!stream.eof())
	{
		// consume blank chars
		while (std::isblank(stream.peek()))
		{
			stream.get();
		}

		// Consume new line chars and increment current line
		if (stream.peek() == '\n' || stream.peek() == '\r')
		{
			char lineTerm = stream.get();
			if (lineTerm == '\r' && stream.peek() == '\n')
				stream.get();
			currentLineNumber++;
			continue;
		}

		if (detectComment())
			continue;

		if (detectSymbol())
			continue;

		// Keyword should always come before identifier
		if (detectKeyword())
			continue;

		if (detectIdentifier())
			continue;

		std::cout << (char)stream.peek() << std::endl;

		stream.get();
	}
	for (auto tok : tokens)
	{
		std::cout << tokenTypeStrings[(int)tok.type] << ": " << tok.lineNumber << std::endl;
	}
	return 0;
}

bool Lexer::detectKeyword()
{
	std::string buffer;
	if (std::isalnum(stream.peek()))
	{
		buffer.push_back(stream.get());
		while (std::isalnum(stream.peek()))
		{
			buffer.push_back(stream.get());
		}

		if (buffer == "u32")
		{
			tokens.push_back({TokenType::U32, currentLineNumber});
		}
		else if (buffer == "i32")
		{
			tokens.push_back({TokenType::I32, currentLineNumber});
		}
		else if (buffer == "u64")
		{
			tokens.push_back({TokenType::U64, currentLineNumber});
		}
		else if (buffer == "i64")
		{
			tokens.push_back({TokenType::I64, currentLineNumber});
		}
		else if (buffer == "f32")
		{
			tokens.push_back({TokenType::F32, currentLineNumber});
		}
		else if (buffer == "f64")
		{
			tokens.push_back({TokenType::F64, currentLineNumber});
		}
		else if (buffer == "fn")
		{
			tokens.push_back({TokenType::FN, currentLineNumber});
		}
		else if (buffer == "return")
		{
			tokens.push_back({TokenType::RETURN, currentLineNumber});
		}
		else
		{
			stream.seekg(-buffer.length(), std::ios::cur);
			return 0;
		}

		return 1;
	}
	return 0;
}

bool Lexer::detectIdentifier()
{
	std::string buffer;
	if (std::isalpha(stream.peek()) || stream.peek() == '_')
	{
		buffer.push_back(stream.get());
		while (std::isalnum(stream.peek()) || stream.peek() == '_')
		{
			buffer.push_back(stream.get());
		}
		tokens.push_back({TokenType::IDENTIFIER, currentLineNumber});
		return 1;
	}
	return 0;
}

bool Lexer::detectComment()
{
	if (stream.peek() == '/')
	{
		stream.get();
		if (stream.peek() == '/')
		{
			stream.get();

			while (stream.peek() != '\n' && stream.peek() != '\r')
				stream.get();
			return 1;
		}
		else
		{
			stream.unget();

			return 0;
		}
	}

	return 0;
}

bool Lexer::detectSymbol()
{
	switch (stream.peek())
	{
	case '(':
		tokens.push_back({TokenType::OPEN_PAREN, currentLineNumber});
		break;
	case ')':
		tokens.push_back({TokenType::CLOSE_PAREN, currentLineNumber});
		break;
	case '{':
		tokens.push_back({TokenType::OPEN_BRACK, currentLineNumber});
		break;
	case '}':
		tokens.push_back({TokenType::CLOSE_BRACK, currentLineNumber});
		break;
	case ':':
		tokens.push_back({TokenType::COLON, currentLineNumber});
		break;
	case ';':
		tokens.push_back({TokenType::SEMI_COLON, currentLineNumber});
		break;
	case '=':
		tokens.push_back({TokenType::EQUALS_SIGN, currentLineNumber});
		break;
	case '+':
		tokens.push_back({TokenType::PLUS, currentLineNumber});
		break;
	case '-':
		tokens.push_back({TokenType::MINUS, currentLineNumber});
		break;
	case '*':
		tokens.push_back({TokenType::MULTIPLY, currentLineNumber});
		break;
	case '/':
		tokens.push_back({TokenType::DIVIDE, currentLineNumber});
		break;
	default:
		return 0;
	}
	stream.get();
	return 1;
}
