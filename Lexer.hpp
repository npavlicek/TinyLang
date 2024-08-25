#pragma once

#include <cstdint>
#include <sstream>
#include <vector>

enum class TokenType
{
	NONE,

	// Special tokens
	IDENTIFIER,

	// Constants/Literals
	LITERAL_F64,
	LITERAL_F32,
	LITERAL_U32,
	LITERAL_I32,
	LITERAL_U64,
	LITERAL_I64,

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
		uint32_t unsigned32Val;
		int32_t signed32Val;
		uint64_t unsigned64Val;
		int64_t signed64Val;
		float floating32Bit;
		double floating64Bit;
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

	void throwError(std::string error);

	// clang-format off
	const std::string tokenTypeStrings[27] = {
	"NONE",

	"IDENTIFIER",

	"LITERAL_F64",
	"LITERAL_F32",
	"LITERAL_U32",
	"LITERAL_I32",
	"LITERAL_U64",
	"LITERAL_I64",

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
