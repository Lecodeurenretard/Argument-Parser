#include "Parser.hpp"

using namespace cmd;	//no one is including those files anyway
using namespace util;

/**
 * Return the name of the type `t` as a string.
 */
[[ nodiscard ]] std::string cmd::to_string(Type t) noexcept(false){
	switch (t) {
		case Type::undefined:
			return "undefined";

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
[[ nodiscard ]] bool cmd::to_bool(const std::string& str) noexcept(false){
	if(str == "true" || str == "1")
		return true;
	if(str == "false" || str == "0")
		return false;
	throw std::invalid_argument("Invalid boolean.");
}

/**
 * Guess the type of `t` and convert to it
 */
[[ nodiscard ]] handeledType cmd::to_value(const cstring& value) noexcept(false){
	handeledType res; res._undefined = nullptr;
	try { to_value(Type::boolean, value); }
	catch (const std::invalid_argument&) {  }
	if(res._undefined != nullptr)
		return res;

	if(Parser::isCorrectValue(value, Type::decimal)){
		res._float = std::atof(value);
		return res;
	}
	if(Parser::isCorrectValue(value, Type::integer)){
		res._int = std::atoi(value);
		return res;
	}

	res._string = value;
	return res;
}

/**
 * Convert `value` to the corresponding type `Type` (see `handeledType` for more info).
 */
[[ nodiscard ]] handeledType cmd::to_value(Type t, const cstring& value) noexcept(false){
	handeledType res;
	switch (t) {
		case Type::undefined:
			res._undefined = (void*)&value;
			break;

		case Type::boolean:
			if(!Parser::isCorrectValue(value, Type::boolean))
				throw std::invalid_argument("The parameter `value` in `cmd::to_value()` can't be converted to a " + to_string(Type::boolean) + ".");
			res._bool = to_bool(value);
			break;

		case Type::integer:
			if(!Parser::isCorrectValue(value, Type::integer))
				throw std::invalid_argument("The parameter `value` in `cmd::to_value()` can't be converted to a " + to_string(Type::integer) + ".");
			res._int = std::atoi(value);
			break;
		
		case Type::decimal:
			if(!Parser::isCorrectValue(value, Type::decimal))
				throw std::invalid_argument("The parameter `value` in `cmd::to_value()` can't be converted to a " + to_string(Type::decimal) + ".");
			res._float = std::atof(value);
			break;
		
		case Type::string:
			if(!Parser::isCorrectValue(value, Type::string))
				throw std::invalid_argument("The parameter `value` in `cmd::to_value()` can't be converted to a " + to_string(Type::string) + ".");
			res._string = value;
			break;
		
		default:
			throw notImplemented("A new type is not implemented but used in `cmd::to_value()`.");
	}

	return res;
}


[[ nodiscard ]] Parser::Parser(void)
	: knownArguments()
{}

[[ nodiscard ]] Parser::Parser(cstring argName, Type t) noexcept(false)
	: Parser(std::make_pair(argName, t))
{}

[[ nodiscard ]] Parser::Parser(const arg_t& argument) noexcept(false)
	: Parser(argument, false)
{}

[[ nodiscard ]] Parser::Parser(const argList_t& arguments) noexcept(false)
	: Parser(arguments, false)
{}

[[ nodiscard ]] Parser::Parser(cstring argName, Type t, bool parseZero) noexcept(false)
	: Parser(std::make_pair(argName, t), parseZero)
{}

[[ nodiscard ]] Parser::Parser(const arg_t& argument, bool parseZero) noexcept(false)
	: knownArguments(), parse0(parseZero)
{
	if(!isCorrectName(argument.first))
		throw std::invalid_argument("Invalid name, make sure to write `--` before your argument name and use only letters and `-`.");
	knownArguments[argument.first] = argument.second;

	construtorAdditions(argument);
}

[[ nodiscard ]] Parser::Parser(const argList_t& arguments, bool parseZero) noexcept(false)
	: knownArguments(), parse0(parseZero)
{
	for(auto pair : arguments)
		construtorAdditions(pair);
}

/**
 * Returns a map with the parameter name as key and its value, `argv[0]` is not parsed.
 */
[[ nodiscard ]] std::map<cstring, handeledType> Parser::parse(int argc, cstring argv[]) noexcept(false){
	std::map<cstring, handeledType> res;
	
	Type expected(Type::undefined);	//What type to expect next? if == Type::undefined, the next token should be a parameter name
	for(int i = !parse0; i < argc; i++){
		if(!argv[i])
			throw std::overflow_error("The parameter `argc` = "+ std::to_string(argc) +" does not match the size of `argv` = "+ std::string(argv[i]) +".");
		const cstring lastArg	(argv[i-1]);
		const cstring currentArg(argv[i  ]);
		const cstring nextArg	(argv[i+1]);
		

		if(expected == Type::undefined && !isCorrectName(currentArg))
			throw parse_error("Expected an argument name in `argv["+ std::to_string(i) +"]`.");
		
		if(!isCorrectName(currentArg) && !isCorrectValue(currentArg, expected))
			throw parse_error("The current token ("+ std::string(currentArg) +") is not an argument name nor a value of type " + to_string(expected) + ".");

		if(expected == Type::undefined && !isName(currentArg))
			throw notImplemented("Unknown name handling will require argument guessing which is not implemented yet.");

		
		
		if(expected != Type::undefined) {
			res[lastArg] = to_value(knownArguments[lastArg], currentArg);
		
		} else if(knownArguments[currentArg] == Type::boolean && !isCorrectName(nextArg))
			res[currentArg]._bool = true;
	}
	for(auto boolArg : knownBoolArguments)
		if(!res.contains(boolArg))
			res[boolArg]._bool = false;
	return res;
}

/**
 * Checks if `cstr` exists in `knownArgument`.
 */
bool Parser::isName(cstring cstr) const {
	return knownArguments.find(cstr) == knownArguments.end();
}


/**
 * Checks if str is a correct argument name.
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
 * Checks if `str` has a correct argument value syntax.
 * @param str The string to verify.
 * @param expectedType What should str represent? If set to `Type::undefined`, return `true`.
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
	
	case Type::undefined:
		return true;
	
	default:
		throw util::notImplemented("A new type is not implemented but used in `cmd::Parser::isCorrectValue()`.");
	}
}


cmd::Parser::parse_error::parse_error(const char cstr[])
	: runtime_error(cstr)
{}

cmd::Parser::parse_error::parse_error(const std::string& str)
	: runtime_error(str)
{}

/**
 * Function called by constructors. It's the code they have in common.
 */
void Parser::construtorAdditions(const arg_t& pair){
	//Registering bool elems in `knownBoolArguments`
	if(pair.second == Type::boolean)
		knownBoolArguments.push_back(pair.first);
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



util::notImplemented::notImplemented(const char cstr[])
	: logic_error(cstr)
{}

util::notImplemented::notImplemented(const std::string& str)
	: logic_error(str)
{}