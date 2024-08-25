#include "Lexer.hpp"

#include <iostream>

bool Lexer::tokenize()
{
	currentLineNumber = 1;
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

		if (detectNumericalConstant())
			continue;

		if (std::isgraph(stream.peek()))
		{
			std::cout << "unconsumed char: " << (char)stream.peek() << std::endl;
		}
		else
		{
			std::cout << "unconsumed char: " << stream.peek() << std::endl;
		}

		stream.get();
	}
	for (auto tok : tokens)
	{
		std::cout << tokenTypeStrings[(int)tok.type] << ": " << tok.lineNumber << std::endl;
	}
	return 0;
}

void Lexer::throwError(std::string error)
{
	std::stringstream errorString;
	errorString << "Error: " << error << ". at line: " << currentLineNumber;
	throw std::runtime_error(errorString.str());
}

bool Lexer::detectNumericalConstant()
{
	std::string buffer;
	bool floatingPoint;
	TokenType literalType = TokenType::NONE;
	while (std::isdigit(stream.peek()) || stream.peek() == '_')
	{
		if (stream.peek() == '_')
		{
			stream.get();
			continue;
		}
		buffer.push_back(stream.get());
	}
	if (buffer.length() > 0)
	{
		if (stream.peek() == '.')
		{
			floatingPoint = true;
			buffer.push_back(stream.get());
			while (std::isdigit(stream.peek()) || stream.peek() == '_')
			{
				if (stream.peek() == '_')
				{
					stream.get();
					continue;
				}

				buffer.push_back(stream.get());
			}
		}

		std::string suffixBuffer;
		for (int i = 0; i < 3; i++)
		{
			suffixBuffer.push_back(stream.get());
		}

		if (suffixBuffer == "f32")
		{
			literalType = TokenType::LITERAL_F32;
		}
		else if (suffixBuffer == "f64")
		{
			literalType = TokenType::LITERAL_F64;
		}
		else if (suffixBuffer == "u32")
		{
			literalType = TokenType::LITERAL_U32;
		}
		else if (suffixBuffer == "i32")
		{
			literalType = TokenType::LITERAL_I32;
		}
		else if (suffixBuffer == "u64")
		{
			literalType = TokenType::LITERAL_U64;
		}
		else if (suffixBuffer == "i64")
		{
			literalType = TokenType::LITERAL_I64;
		}
		else
		{
			stream.seekg(-3, std::ios::cur);
		}

		if (literalType != TokenType::NONE)
		{
			if (floatingPoint && !(literalType == TokenType::LITERAL_F64 || literalType == TokenType::LITERAL_F32))
			{
				throwError("Invalid literal/suffix combo");
			}
		}

		if (literalType == TokenType::NONE)
		{
			if (floatingPoint)
			{
				double val = std::strtod(buffer.c_str(), nullptr);
				tokens.push_back(
					{.type = TokenType::LITERAL_F64, .lineNumber = currentLineNumber, .floating64Bit = val});
				return 1;
			}
			else
			{
				long long val = std::strtoll(buffer.c_str(), nullptr, 10);
				tokens.push_back({.type = TokenType::LITERAL_I64, .lineNumber = currentLineNumber, .signed64Val = val});
				return 1;
			}
		}
		else
		{
			switch (literalType)
			{
			case TokenType::LITERAL_F64: {
				double val = std::strtod(buffer.c_str(), nullptr);
				tokens.push_back({.type = literalType, .lineNumber = currentLineNumber, .floating64Bit = val});
				return 1;
			}
			case TokenType::LITERAL_F32: {
				float val = std::strtof(buffer.c_str(), nullptr);
				tokens.push_back({.type = literalType, .lineNumber = currentLineNumber, .floating32Bit = val});
				return 1;
			}
			case TokenType::LITERAL_I32: {
				int32_t val = std::stoi(buffer, 0, 10);
				tokens.push_back({.type = literalType, .lineNumber = currentLineNumber, .signed32Val = val});
				return 1;
			}
			case TokenType::LITERAL_U32: {
				uint32_t val = std::stoi(buffer, 0, 10);
				tokens.push_back({.type = literalType, .lineNumber = currentLineNumber, .unsigned32Val = val});
				return 1;
			}
			case TokenType::LITERAL_I64: {
				int64_t val = std::strtol(buffer.c_str(), nullptr, 10);
				tokens.push_back({.type = literalType, .lineNumber = currentLineNumber, .signed64Val = val});
				return 1;
			}
			case TokenType::LITERAL_U64: {
				uint64_t val = std::strtoul(buffer.c_str(), nullptr, 10);
				tokens.push_back({.type = literalType, .lineNumber = currentLineNumber, .unsigned64Val = val});
				return 1;
			}
			default:
				return 0;
			}
			return 1;
		}
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
