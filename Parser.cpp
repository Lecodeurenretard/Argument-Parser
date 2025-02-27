#include "Parser.hpp"

/** The theoreticaly maximum number of chars in a string (in reality, it's less) */
#define STRING_MAX std::string().max_size()

using namespace cmd;
using namespace util;

[[ nodiscard ]] Parser::Parser(void)
	: knowArguments()
{}

[[ nodiscard ]] Parser::Parser(cstring argName, Type t) noexcept(false)
	: knowArguments()
{
	if(!isCorrectName(argName))
		throw std::invalid_argument("Invalid name, make sure to write `--` before your argument name and use only letters and `-`.");
	knowArguments[argName] = t;
}

[[ nodiscard ]] Parser::Parser(const arg_t& argument) noexcept(false)
	: Parser(argument.first, argument.second)
{}

[[ nodiscard ]] Parser::Parser(const argList_t& arguments) noexcept(false)
	: knowArguments(arguments)
{}

/**
 * Returns a map with the parameter name as key and its value
 
[[ nodiscard ]] std::map<cstring, handeledType> Parser::parse(int argc, cstring argv[]) noexcept(false){
	
}*/

/**
 * Checks if str is a correct argumant name.
 */
[[ nodiscard ]] bool Parser::isCorrectName(cstring str){
	if(str[0] != '-' || str[1] == '\0')
		return false;

	for (uint i = 1; str[i] != '\0' ; i++){		//until reaching the null terminator
		if(str[i] == ' ' || (!std::isalpha(str[i]) && str[i] != '-'))
			return false;
	}
	return true;
}

/**
 * Checks if str has a correct argument value syntax.
 */
[[ nodiscard ]] bool Parser::isCorrectValue(cstring str, Type expectedType /*= Type::string*/) noexcept(false){
	switch (expectedType){
	case Type::boolean: {
		const std::string s = std::string(str);
		return s == "true" || s == "false" || (s == "0" || s == "1") ;
	}

	case Type::integer:
		if(str[0] != '+' && str[0] != '-' && !std::isdigit(str[0]))
			return false;
		
		for(size_t i = 1; str[i] != '\0'; i++)
			if(i == SIZE_MAX || !isdigit(str[i]))
				return false;
		
		return true;
	
	case Type::decimal: {
		const bool dotFirst(str[0] == '.');
		if(str[0] != '+' && str[0] != '-' && !dotFirst && !std::isdigit(str[0]))
			return false;

		size_t index(1);
		while (std::isdigit(str[index]) && index < SIZE_MAX)
			index++;
		
		if(str[index] == '\0')	//an int is a float
			return true;
		
		if(str[index] != '.' || (str[index] == '.' && dotFirst) || index >= SIZE_MAX)
			return false;
		
		if(!std::isdigit(str[++index]))	//skipping the dot
			return false;

		while (std::isdigit(str[index]) && index < SIZE_MAX)
			index++;

		return str[index] == '\0' && index < SIZE_MAX;
	}
	
	case Type::string:
		for(size_t i = 0; i < STRING_MAX; i++)
			if(str[i] == '\0')
				return true;
		return false;
	
	default:
		throw util::notImplemented("New type not implemented.");
	}
}





/**
 * Check if `c` is an upper or lowercase letter.
 */
bool std::isalpha(char c){
	return std::isalpha(static_cast<unsigned char>(c));
}

/**
 * Check if `c` is a number.
 */
bool std::isdigit(char c){
	return std::isdigit(static_cast<unsigned char>(c));
}

/**
 * Check if `c` is a whitespace  (' ', a newline, a tab, ...).
 */
bool std::isspace(char c){
	return std::isspace(static_cast<unsigned char>(c));
}

/**
 * Count the all chars in a C string except the null terminator.
 */
size_t util::strCount(cstring str){
	size_t i = 0;
	while(i < SIZE_MAX){
		if(str[i] != '\0')
			return i;
		i++;
	}
	return SIZE_MAX;
}



util::notImplemented::notImplemented(const char str[])
	: logic_error(str)
{}
