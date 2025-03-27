//test file 

#include <nlibs/Parser>
#include <nlibs/colors.h>	//from misc_modules
#include <cassert>
#include <iostream>

int main(int argc, char* argv[]){
	std::cout << COLOR_FG_BYELLOW << "Checking `cmd::Parser::isCorrectName()`...\n" << COLOR_FG_RESET;
	assert(cmd::Parser::isCorrectName("-j")			== true);
	assert(cmd::Parser::isCorrectName("--j")		== true);
	assert(cmd::Parser::isCorrectName("--jojo")		== true);
	assert(cmd::Parser::isCorrectName("--jojo-koko")== true);
	assert(cmd::Parser::isCorrectName("--")			== true);
	assert(cmd::Parser::isCorrectName("---")		== true);
	assert(cmd::Parser::isCorrectName("\0")			== false);
	assert(cmd::Parser::isCorrectName("-")			== false);
	assert(cmd::Parser::isCorrectName("hello")		== false);
	assert(cmd::Parser::isCorrectName("false")		== false);
	assert(cmd::Parser::isCorrectName("--nani_f")	== false);
	assert(cmd::Parser::isCorrectName("--nani f")	== false);
	assert(cmd::Parser::isCorrectName("--ni\nffg\rds\"")	== false);


	std::cout << COLOR_FG_BYELLOW << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::argument)`...\n" << COLOR_FG_RESET;
	assert(cmd::Parser::isCorrectValue("\0",		cmd::argument)													== false);
	assert(cmd::Parser::isCorrectValue("freqFAN",	cmd::argument)													== false);
	assert(cmd::Parser::isCorrectValue("12\t5000dsqegrkkutsdr<wfb zlay_'é-5ff!prqç_@\\^[à_èp']", cmd::argument)		== false);

	std::cout << COLOR_FG_BYELLOW << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::string)`...\n" << COLOR_FG_RESET;
	assert(cmd::Parser::isCorrectValue("\0")											== false);
	assert(cmd::Parser::isCorrectValue("hi")											== true);
	assert(cmd::Parser::isCorrectValue(std::string(5, '.').c_str())						== true);

	std::cout << COLOR_FG_BYELLOW << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::boolean)`...\n" << COLOR_FG_RESET;
	assert(cmd::Parser::isCorrectValue("true"	, cmd::Type::boolean)					== true);
	assert(cmd::Parser::isCorrectValue("false"	, cmd::Type::boolean)					== true);
	assert(cmd::Parser::isCorrectValue("1"		, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("0"		, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("\0"		, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("idk"	, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("definitly not true"	, cmd::Type::boolean)		== false);
	assert(cmd::Parser::isCorrectValue("falsetrue"	, cmd::Type::boolean)				== false);

	std::cout << COLOR_FG_BYELLOW << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::integer)`...\n" << COLOR_FG_RESET;
	assert(cmd::Parser::isCorrectValue("1234"	, cmd::Type::integer)					== true);
	assert(cmd::Parser::isCorrectValue("+1234"	, cmd::Type::integer)					== true);
	assert(cmd::Parser::isCorrectValue("-1234"	, cmd::Type::integer)					== true);
	assert(cmd::Parser::isCorrectValue("\0"		, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("+"		, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("-"		, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("+ "		, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("- "		, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("string"	, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("1.23"	, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("~12334"	, cmd::Type::integer)					== false);

	std::cout << COLOR_FG_BYELLOW << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::decimal)`...\n" << COLOR_FG_RESET;
	assert(cmd::Parser::isCorrectValue("1234"		, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("+1234"		, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("-1234"		, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("1234.56"	, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("+1234.56"	, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("-1234.56"	, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue(".1234"		, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("+.1234"		, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("-.1234"		, cmd::Type::decimal)				== true);
	assert(cmd::Parser::isCorrectValue("\0"			, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("+"			, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("-"			, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("."			, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("+ "			, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("- "			, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue(". "			, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1234."		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1234."		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1234."		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("string"		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("123%456"	, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("123^.456"	, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("123.'456"	, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("~12334"		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1..234"		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1.2.3.34"	, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1.23.34"	, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("123."		, cmd::Type::decimal)				== false);


	std::cout << COLOR_FG_BYELLOW << "Checking `cmd::to_string(cmd::Type)`...\n" << COLOR_FG_RESET;
	assert(cmd::to_string(cmd::Type::argument)	== "argument"	);
	assert(cmd::to_string(cmd::Type::boolean)	== "boolean"	);
	assert(cmd::to_string(cmd::Type::integer)	== "integer"	);
	assert(cmd::to_string(cmd::Type::decimal)	== "decimal"	);
	assert(cmd::to_string(cmd::Type::string)	== "string"		);
	
	std::cout << COLOR_FG_YELLOW << "NOT checking `guessType()` because it is private\n" << COLOR_FG_RESET;
	/*assert(cmd::Parser::guessType("\0")			== cmd::Type::argument);
	assert(cmd::Parser::guessType("+01")		== cmd::Type::integer);
	assert(cmd::Parser::guessType("-.90")		== cmd::Type::decimal);
	assert(cmd::Parser::guessType("azerty")		== cmd::Type::string);
	assert(cmd::Parser::guessType("--argument")	== cmd::Type::string);*/

	std::cout << COLOR_FG_GREEN << "Success!" << COLOR_FG_RESET << std::endl;
}