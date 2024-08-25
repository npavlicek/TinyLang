#pragma once

#include <cstdint>
#include <sstream>
#include <vector>

enum class TokenType
{
	NONE,

	// Special tokens
	IDENTIFIER,

	// types
	U32,
	I32,
	U64,
	I64,
	F32,
	F64,

	// functions
	FN,
	RETURN,

	// symbols
	OPEN_PAREN,
	CLOSE_PAREN,
	OPEN_BRACK,
	CLOSE_BRACK,
	COLON,
	SEMI_COLON,
	EQUALS_SIGN,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
};

struct Token
{
	TokenType type;
	int lineNumber;
	union {
		uint32_t unsigned32val;
		int32_t signed32val;
		uint64_t unsigned64val;
		int64_t signed64val;
		float float_;
		double double_;
	};
};

class Lexer
{
	std::stringstream &stream;
	std::vector<Token> tokens;

	int currentLineNumber;

	bool detectSymbol();
	bool detectComment();
	bool detectIdentifier();
	bool detectKeyword();
	bool detectNumericalConstant();

	// clang-format off
	const std::string tokenTypeStrings[20] = {
	"NONE",

	"IDENTIFIER",

	// Keywords / types
	"U32",
	"I32",
	"U64",
	"I64",
	"F32",
	"F64",

	"FN",
	"RETURN",

	"OPEN_PAREN",
	"CLOSE_PAREN",
	"OPEN_BRACK",
	"CLOSE_BRACK",
	"COLON",
	"SEMI_COLON",
	"EQUALS_SIGN",
	"PLUS",
	"MINUS",
	"MULTIPLY",
	"DIVIDE",
	};
	// clang-format on

  public:
	Lexer(std::stringstream &stream) : stream(stream){};
	bool tokenize();
};
