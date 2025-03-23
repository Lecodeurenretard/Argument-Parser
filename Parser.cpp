#include "Parser.hpp"
#include <iostream>
using namespace util;

namespace cmd {
	/**
	 * Return the name of the type `t` as a string.
	 */
	[[ nodiscard ]] std::string to_string(Type t) noexcept(false){
		switch (t) {
			case Type::argument:
				return "argument";

			case Type::boolean:
				return "boolean";

			case Type::integer:
				return "integer";
			
			case Type::decimal:
				return "decimal";
			
			case Type::string:
				return "string";
			
			default:
				throw notImplemented("A new type is not implemented but used in `cmd::to_string(Type)`.");
		}
	}

	/**
	 * Returns the corresponding boolean.
	 */
	[[ nodiscard ]] bool to_bool(const std::string& str) noexcept(false){
		if(str == "true")
			return true;
		if(str == "false")
			return false;
		if(Parser::isCorrectValue(str, Type::boolean))
			throw std::logic_error("`cmd::to_bool` doesn't cover all cases permitted by `cmd::Parser::isCorrectValue()`.");

		throw std::invalid_argument("Invalid boolean.");
	}

	/**
	 * Guess the type of `t` and convert to it
	 */
	[[ nodiscard ]] outputType to_value(std::string value) noexcept(false){
		if(value == "\0")
			throw std::runtime_error("Empty strings are NOT allowed in `to_value()`");
		
		outputType res;

		if(Parser::isCorrectValue(value, Type::boolean)){
			res = to_bool(value);
			return res;
		}
		if(Parser::isCorrectValue(value, Type::integer)){
			res = std::atoi(value.c_str());
			return res;
		}
		if(Parser::isCorrectValue(value, Type::decimal)){
			res = (float)std::atof(value.c_str());
			return res;
		}

		res = value;
		return res;
	}

	/**
	 * Convert `value` to the corresponding type `Type` (see `outputType` for more info).
	 */
	[[ nodiscard ]] outputType to_value(Type t, std::string value) noexcept(false){
		if(value == "\0")
			throw std::runtime_error("Empty strings are NOT allowed in `to_value()`");
		
		outputType res;
		switch (t) {
			case Type::argument:
				throw std::logic_error("Trying to cast a token to the argument type.");

			case Type::boolean:
				if(!Parser::isCorrectValue(value, Type::boolean))
					throw std::invalid_argument("The parameter `value` ("+ value +") in `cmd::to_value()` can't be converted to a " + to_string(Type::boolean) + ".");
				res = to_bool(value);
				break;

			case Type::integer:
				if(!Parser::isCorrectValue(value, Type::integer))
					throw std::invalid_argument("The parameter `value` ("+ value +") in `cmd::to_value()` can't be converted to an " + to_string(Type::integer) + ".");
				res = std::atoi(value.c_str());
				break;
			
			case Type::decimal:
				if(!Parser::isCorrectValue(value, Type::decimal))
					throw std::invalid_argument("The parameter `value` ("+ value +") in `cmd::to_value()` can't be converted to a " + to_string(Type::decimal) + ".");
				res = (float)std::atof(value.c_str());
				break;
			
			case Type::string:
				if(!Parser::isCorrectValue(value, Type::string))
					throw std::invalid_argument("The parameter `value` ("+ value +") in `cmd::to_value()` can't be converted to a " + to_string(Type::string) + ".");
				res = value;
				break;
			
			default:
				throw notImplemented("A new type is not implemented but used in `cmd::to_value()`.");
		}

		return res;
	}

	[[ nodiscard ]] Parser::Parser(const argList_t& arguments) noexcept(false)
		: Parser(arguments, false)
	{}

	[[ nodiscard ]] Parser::Parser(const Parser::argList_t& arguments, bool parseZero) noexcept(false)
		: knownArguments(), parse0(parseZero)
	{
		for(auto pair : arguments){
			if(!isCorrectName(pair.first))
				throw std::invalid_argument("The name `"+ pair.first +"` is an invalid argument name.");
			
			if(pair.second == Type::boolean)
				knownBoolArguments.push_back(pair.first);
			
			knownArguments[pair.first] = pair.second;
		}
	}

	/**
	 * Parses `argv` up to `argv[argc]`, if the instance is initialized with `parse0` set to `false` will ignore `argv[0]`.
	 * @param argc The number of token to parse.
	 * @param argv An array containing all tokens.
	 * @param guess If the function should try to guess the type of unknown arguments, if set to false will throw an error in this situation.
	 */
	[[ nodiscard ]] Parser::parseReturn_t Parser::parse(int argc, const char* argv[], bool guess /*=false*/) noexcept(false){
		Parser::parseReturn_t res;

		Type expected(Type::argument);
		if(!parse0 && argc <= 1)
			throw std::overflow_error("The member `Parser.parse0` = "+ std::to_string(parse0) +" creates an overflow (try to set it to true).");
		for(int i = !parse0; i < argc; i++){			
			if(argv[i] == nullptr || (argv[i+1] == nullptr && i+1 < argc))
				throw std::logic_error("The parameter `argc` ("+ std::to_string(argc) +") is greater than the real size of `argv` (either "+ std::to_string(i) +" or "+ std::to_string(i+1) +")");

			const std::string& lastToken	((i == 0)		? "\0" : argv[i-1]);
			const std::string& nextToken	((i+1 >= argc)	? "\0" : argv[i+1]);
			
			const std::string& currentToken(argv[i]);

			if(currentToken == "\0")
				throw std::invalid_argument("Empty strings are not allowed as arguments.");
			if(!isCorrectName(currentToken) && !isCorrectValue(currentToken, expected))
				throw parse_error("The current token ("+ std::string(currentToken) +") is not an argument name nor a value of type " + to_string(expected) + ".");
			
			if(expected != Type::argument){
				if(lastToken == "\0")
					throw parse_error("Expected the argument at index "+ std::to_string(i) +" to be a value but it is an argument.");
				res[lastToken] = to_value(knownArguments[lastToken], currentToken);	//assigning values
				
				expected = Type::argument;
				continue;
			}


			if(!isName(currentToken)){
				if (!guess)
					throw parse_error("Expected a defined argument name since the `guess` argument is false in `argv["+ std::to_string(i) +"]` but got `"+ currentToken +"`.");
				
				if(isCorrectName(nextToken)){
					//expected = Type::argument;	//implied
					knownArguments[currentToken] = Type::boolean;
					res[currentToken] = true;
					continue;
				}

				expected = guessType(nextToken);
				if(expected == Type::argument)
					throw parse_error("The token at index "+ std::to_string(i+1) +" ("+ nextToken +") is not a valid argument.");
					
				knownArguments[currentToken] = expected;
				continue;
				
			}
			
			if(std::find(knownBoolArguments.begin(), knownBoolArguments.end(), currentToken) != knownBoolArguments.end() && isCorrectName(nextToken)) {	//if the argument is a boolean
				res[currentToken] = true;
				expected = Type::argument;
				continue;
			}
			
			//else `currentToken` is the name of an argument
			expected = knownArguments[currentToken];
		}
		for(auto boolArg : knownBoolArguments)
			if(!res.contains(boolArg))
				res[boolArg] = false;
		
		return res;
	}

	/**
	 * Checks if `str` exists in `knownArgument`.
	 */
	[[ nodiscard ]] bool Parser::isName(std::string str) const {
		return knownArguments.find(str) != knownArguments.end();
	}


	/**
	 * Checks if str is a correct argument name.
	 */
	[[ nodiscard ]] bool Parser::isCorrectName(std::string str){
		if(str[0] != '-' || str.size() < 2)
			return false;

		for (uint i = 1; str[i] != '\0' ; i++){		//until reaching the null terminator
			if(str[i] == ' ' || (!std::isalpha(str[i]) && str[i] != '-'))
				return false;
		}
		return true;
	}

	/**
	 * Checks if `str` has a correct argument value syntax.
	 * @param str The string to verify.
	 * @param expectedType What should str represent? If set to `Type::argument`, return `false`.
	 */
	[[ nodiscard ]] bool Parser::isCorrectValue(std::string str, Type expectedType /*= Type::string*/) noexcept(false){
		switch (expectedType){
		case Type::boolean: 
			return str == "true" || str == "false";

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
			/*old method from back then `str` was a char*
			for(size_t i = 0; i < STRING_MAX; i++)
				if(str[i] == '\0')
					return true;
			*/
			return true;
		
		case Type::argument:
			return false;
		
		default:
			throw util::notImplemented("A new type is not implemented but used in `cmd::Parser::isCorrectValue()`.");
		}
	}


	/**
	 * Try to guess what type could be `value` (assumes it is a value), if it does not fit any type, return `Type::argument`.
	 */
	[[ nodiscard ]] Type Parser::guessType(std::string value){
		if(value == "\0")
			return Type::argument;


		if (Parser::isCorrectValue(value, Type::boolean))
			return Type::boolean;
		
		if (Parser::isCorrectValue(value, Type::integer))
			return Type::integer;

		if (Parser::isCorrectValue(value, Type::decimal))
			return Type::decimal;
		
		if (Parser::isCorrectValue(value, Type::string))	//If I ever add a new condition for the string type, this makes sure I don't forget what's going on
			return Type::string;
		
		return Type::argument;
	}

	[[ nodiscard ]] Parser::parse_error::parse_error(const char str[])
		: runtime_error(str)
	{}

	[[ nodiscard ]] Parser::parse_error::parse_error(const std::string& str)
		: runtime_error(str)
	{}
}



/**
 * Check if `c` is an upper or lowercase letter.
 */
[[ nodiscard ]] bool std::isalpha(char c){
	return std::isalpha(static_cast<unsigned char>(c));
}

/**
 * Check if `c` is a number.
 */
[[ nodiscard ]] bool std::isdigit(char c){
	return std::isdigit(static_cast<unsigned char>(c));
}

/**
 * Check if `c` is a whitespace (' ', a newline, a tab, ...).
 */
[[ nodiscard ]] bool std::isspace(char c){
	return std::isspace(static_cast<unsigned char>(c));
}