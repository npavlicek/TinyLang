#include <fstream>
#include <iostream>

#include "Lexer.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
		throw std::runtime_error("Please supply a filename");

	auto file_name = std::string(argv[1]);

	std::fstream input_file(file_name, std::ios::in);
	if (!input_file)
		throw std::runtime_error("Failed to open file");

	std::stringstream str;
	str << input_file.rdbuf();

	input_file.close();

	Lexer lex(str);
	lex.tokenize();

	return EXIT_SUCCESS;
}
