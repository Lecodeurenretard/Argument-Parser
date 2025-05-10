#include <iostream>

#if __has_include("Parser.hpp")
	#include "Parser.hpp"
#else
	#include <nlibs/Parser>
#endif

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

			case Type::unsignedInteger:
				return "unsigned integer";

			case Type::integer:
				return "integer";

			case Type::decimal:
				return "decimal";

			case Type::string:
				return "string";

			default:
				throw std::logic_error("A new type is not implemented in `to_string(Type)` but passed to the method.");
				//util::notImplemented("A new type is not implemented but used in `cmd::to_string(Type)`.");
		}
	}

	/**
	 * Returns the corresponding boolean or throw an error.
	 */
	[[ nodiscard ]] bool to_bool(const std::string& str) noexcept(false){
		if(str == "true")
			return true;
		if(str == "false")
			return false;

		throw std::invalid_argument("Invalid boolean passed in `to_bool()`.");
	}

	/**
	 * Guess the type of `t` and convert `value` to the guessed type.
	 */
	[[ nodiscard ]] outputType to_value(std::string value) noexcept(false){
		if(value == "\0")
			throw std::invalid_argument("Empty strings are NOT allowed in `to_value()`");


		//could write a for loop but it would break if we add another type, same if I allocate an array
		outputType res;
		if(Parser::isCorrectValue(value, Type::boolean)){
			res = to_bool(value);
			return res;
		}
		if(Parser::isCorrectValue(value, Type::integer)){	//so is for `unsignedInterger`
			if(value[0] != '-')
				return (uint)std::atoi(value.c_str());
			
			res = std::atoi(value.c_str());
			return res;
		}
		if(Parser::isCorrectValue(value, Type::decimal)){
			res = (float)std::atof(value.c_str());
			return res;
		}

		res = value;	//else string
		return res;
	}

	/**
	 * Convert `value` to the corresponding type `Type` (see `outputType` for more info).
	 */
	[[ nodiscard ]] outputType to_value(std::string value, Type t) noexcept(false){
		if(value == "\0")
			throw std::invalid_argument("Empty strings are NOT allowed in `to_value()`");

		outputType res;
		switch (t) {
			case Type::argument:
				throw std::logic_error("Cannot cast a token to the argument type.");

			case Type::boolean:
				if(!Parser::isCorrectValue(value, Type::boolean))
					throw std::invalid_argument("The parameter `value` ("+ value +") in `cmd::to_value()` can't be converted to a " + to_string(Type::boolean) + ".");
				res = to_bool(value);
				break;

			case Type::unsignedInteger:
				if(!Parser::isCorrectValue(value, Type::unsignedInteger))
					throw std::invalid_argument("The parameter `value` ("+ value +") in `cmd::to_value()` can't be converted to an " + to_string(Type::unsignedInteger) + ".");
				res = (uint)std::atoi(value.c_str());
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
				throw std::logic_error("A new type is not implemented in `cmd::to_value()`.");
				//throw util::notImplemented("A new type is not implemented in `cmd::to_value()`.");
		}

		return res;
	}

	[[ nodiscard ]] Parser::Parser(void)
		: Parser(false, false)
	{}

	[[ nodiscard ]] Parser::Parser(bool parseZero)
		: Parser(parseZero, false)
	{}

	[[ nodiscard ]] Parser::Parser(bool parseZero, bool guess)
		: knownArguments(), knownBoolArguments(), parse0(parseZero), argumentGuess(guess)
	{}

	[[ nodiscard ]] Parser::Parser(const argList_t& arguments) noexcept(false)
		: Parser(arguments, false, false)
	{}

	[[ nodiscard ]] Parser::Parser(const argList_t& arguments, bool parseZero) noexcept(false)
		: Parser(arguments, parseZero, false)
	{}

	[[ nodiscard ]] Parser::Parser(const argList_t& arguments, bool parseZero, bool guess) noexcept(false)
		: knownArguments(), knownBoolArguments(), parse0(parseZero), argumentGuess(guess)
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
	[[ nodiscard ]] Parser::parseReturn_t Parser::parse(int argc, const char* argv[]) noexcept(false){
		Parser::parseReturn_t res;
		if(argc < 1 || (argc == 1 && !parse0)) {		//if argv has a length of 0
			for(auto boolArg : knownBoolArguments)
				res[boolArg] = false;
			return res;
		}

		Type expected(Type::argument);

		for(int i = !parse0; i < argc; i++) {
			if(!argv[i] || (!argv[i+1] && i+1 < argc))
				throw std::logic_error("The parameter `argc` ("+ std::to_string(argc) +") is greater than the real size of `argv` (either "+ std::to_string(i) +" or "+ std::to_string(i+1) +")");

			const std::string& previousToken	((i	  == 0		)	? "\0" : argv[i-1]);	//checks for under/overflows and set to an empty string if that's the case
			const std::string& nextToken		((i+1 >= argc	)	? "\0" : argv[i+1]);

			const std::string& currentToken(argv[i]);

			if(currentToken == "\0")
				throw std::invalid_argument("Empty strings are not allowed as arguments.");

			if(!isCorrectName(currentToken) && !isCorrectValue(currentToken, expected))
				throw unknownArgument_error("The `"+ std::string(currentToken) +"` token is not an argument name nor a value of type " + to_string(expected) + ".", currentToken);

			if(expected != Type::argument){
				res[previousToken] = to_value(currentToken, knownArguments[previousToken]);	//assigning values

				expected = Type::argument;
				continue;
			}

			if(isCombinedName(currentToken)){
				//separate all names
				for(size_t j = 1; j < currentToken.size(); j++){
					const char arg[3] = {'-', currentToken[j], '\0'};

					if(!argumentGuess && !isName(arg))
						throw unknownArgument_error("An unknown argument ("+ std::string(arg) +") was found during the splitting of argument at index "+ std::to_string(i) +" ("+ currentToken +").", arg);
					res[arg] = true;
				}
				expected = Type::argument;
				continue;
			}

			if(!isName(currentToken)){
				if (!argumentGuess)
					throw unknownArgument_error("Expected a defined argument name since the `guess` member is false for `argv["+ std::to_string(i) +"]` but got `"+ currentToken +"`.", currentToken);

				if(isCorrectName(nextToken) || i+1 >= argc){
					//implied `expected = Type::argument;`
					knownArguments[currentToken] = Type::boolean;
					res[currentToken] = true;
					continue;
				}

				expected = guessType(nextToken);
				if(expected == Type::argument)
					throw parse_error("The token at index "+ std::to_string(i+1) +" ("+ nextToken +") is not a valid value.");

				knownArguments[currentToken] = expected;
				continue;
			}

			//else `currentToken` is the name of an argument
			
			//checking if the argument is a boolean
			if((nextToken == "\0" || isCorrectName(nextToken)) && util::vecContain(knownBoolArguments, currentToken)) {
				res[currentToken] = true;
				expected = Type::argument;
				continue;
			}

			expected = knownArguments[currentToken];
		}
		//set all boolean left to false
		for(auto boolArg : knownBoolArguments)
			if(!res.contains(boolArg))
				res[boolArg] = false;

		return res;
	}


	/**
	 * Checks if `str` exists in `knownArgument`.
	 */
	[[ nodiscard ]] bool Parser::isName(const std::string& str) const {
		return knownArguments.contains(str);
	}


	/**
	 * Checks if str is a correct argument name.
	 */
	[[ nodiscard ]] bool Parser::isCorrectName(std::string str){
		if(str[0] != '-' || str.size() < 2)
			return false;

		for (size_t i = 1; i < str.size(); i++){
			if(!std::isalpha(str[i]) && str[i] != '-')
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
		if(str == "\0")
			return false;

		switch (expectedType){
		case Type::boolean: 
			return str == "true" || str == "false";

		case Type::unsignedInteger:
			return isCorrectValue(str, Type::integer) && str[0] != '-' && str[0] != '+';

		case Type::integer:{
			//a number can't just be  a `+` or a `-`
			const bool symbolFirst(str[0] == '+' || str[0] == '-');

			if(symbolFirst && str.size() < 2)
				return false;
			if(!symbolFirst && !std::isdigit(str[0]))
				return false;

			for(size_t i = 1; i < str.size(); i++)
				if(!isdigit(str[i]))
					return false;

			return true;
		}

		case Type::decimal: {
			const bool dotFirst(str[0] == '.');
			const bool symbolFirst(str[0] == '+' || str[0] == '-' || dotFirst);
			
			if(symbolFirst && str.size() < 2)
				return false;

			if(!symbolFirst && !std::isdigit(str[0]))
				return false;

			size_t index(1);
			while (std::isdigit(str[index]) && index < str.size())
				index++;

			if(index >= str.size())	//an int is a float
				return true;

			if(str[index] == '.' && dotFirst)
				return false;

			if(str[index] != '.')
				return false;

			if(!std::isdigit(str[++index]))	//skipping the dot
				return false;

			while (std::isdigit(str[index]) && index < str.size())
				index++;

			return str[index] == '\0' && index < SIZE_MAX;
		}

		case Type::string:
			return true;

		case Type::argument:
			return false;	// If this method is used, that's a bug. This makes more easy to track it down

		default:
			throw std::logic_error("A new type is not implemented in `cmd::Parser::isCorrectValue()` but passed to the method.");
			//throw util::notImplemented("A new type is not implemented but used in `cmd::Parser::isCorrectValue()`.");
		}
	}



	[[ nodiscard ]] Parser::parse_error::parse_error(const char cstr[])
		: runtime_error(cstr)
	{}

	[[ nodiscard ]] Parser::parse_error::parse_error(const std::string& str)
		: runtime_error(str)
	{}

	[[ nodiscard ]] Parser::unknownArgument_error::unknownArgument_error(const char cstr[], const char arg[])
		: parse_error(cstr), argName(arg)
	{}

	[[ nodiscard ]] Parser::unknownArgument_error::unknownArgument_error(const std::string& str, const std::string& arg)
		: parse_error(str), argName(arg)
	{}

	[[ nodiscard ]] const std::string& Parser::unknownArgument_error::getArgName(void) const {
		return argName;
	}

	/**
	 * Try to guess what type could be `value` (assumes it is a value), if it does not fit any type, return `Type::argument`.
	 */
	[[ nodiscard ]] Type Parser::guessType(const std::string& value){
		if(value == "\0")
			return Type::argument;


		if (Parser::isCorrectValue(value, Type::boolean))
			return Type::boolean;

		if (Parser::isCorrectValue(value, Type::unsignedInteger))
			return Type::unsignedInteger;
		
		if (Parser::isCorrectValue(value, Type::integer))
			return Type::integer;

		if (Parser::isCorrectValue(value, Type::decimal))
			return Type::decimal;

		if (Parser::isCorrectValue(value, Type::string))	//If I ever add a new condition for the string type, this makes sure I don't forget what's going on
			return Type::string;

		return Type::argument;
	}

	/**
	 * Checks if `name` is multiple single letter boolean arguments names.
	 */
	[[ nodiscard ]] bool Parser::isCombinedName(const std::string& str){
		if(str[0] != '-' || str.size() <= 2)
			return false;

		for(size_t i = 1; i < str.length(); i++){
			//isCorrectName()
			if(!std::isalpha(str[i]))
				return false;

			//isName()
			const std::string currentArg("-" + std::string(1, str[i]));	//the current char with a dash before
			const bool isCurrentArgName(isName(currentArg));

			if(!argumentGuess && !isCurrentArgName)	//if argument guessing is disabled and the current name is not defined
				return false;
			if(!isCurrentArgName){	//it has to be a correct name because it's a dash and a letter
				knownArguments[currentArg] = Type::boolean;
				knownBoolArguments.push_back(currentArg);
			}

			if(knownArguments[currentArg] != Type::boolean)
				return false;
		}
		return true;
	}
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
 * Check if `c` is a whitespace.
 */
[[ nodiscard ]] bool std::isspace(char c){
	return std::isspace(static_cast<unsigned char>(c));
}