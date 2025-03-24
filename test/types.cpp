//test file 

#include <nlibs/Parser>
#include <cassert>
#include <iostream>

int main(int argc, char* argv[]){
	std::cout << "Checking `cmd::Parser::isCorrectName()`...\n";
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


	std::cout << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::argument)`...\n";
	assert(cmd::Parser::isCorrectValue("\0",		cmd::argument)													== false);
	assert(cmd::Parser::isCorrectValue("freqFAN",	cmd::argument)													== false);
	assert(cmd::Parser::isCorrectValue("12\t5000dsqegrkkutsdr<wfb zlay_'é-5ff!prqç_@\\^[à_èp']", cmd::argument)		== false);

	std::cout << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::string)`...\n";
	assert(cmd::Parser::isCorrectValue("\0")											== false);
	assert(cmd::Parser::isCorrectValue("hi")											== true);
	assert(cmd::Parser::isCorrectValue(std::string(5, '.').c_str())						== true);
	// I assume that any string longer than `STRING_MAX` would return `false` since it is unreprodictable

	std::cout << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::boolean)`...\n";
	assert(cmd::Parser::isCorrectValue("true"	, cmd::Type::boolean)					== true);
	assert(cmd::Parser::isCorrectValue("false"	, cmd::Type::boolean)					== true);
	assert(cmd::Parser::isCorrectValue("1"		, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("0"		, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("\0"		, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("idk"	, cmd::Type::boolean)					== false);
	assert(cmd::Parser::isCorrectValue("definitly not bool"	, cmd::Type::boolean)		== false);

	std::cout << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::integer)`...\n";
	assert(cmd::Parser::isCorrectValue("1234"	, cmd::Type::integer)					== true);
	assert(cmd::Parser::isCorrectValue("+1234"	, cmd::Type::integer)					== true);
	assert(cmd::Parser::isCorrectValue("-1234"	, cmd::Type::integer)					== true);
	assert(cmd::Parser::isCorrectValue("\0"		, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("string"	, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("1.23"	, cmd::Type::integer)					== false);
	assert(cmd::Parser::isCorrectValue("~12334"	, cmd::Type::integer)					== false);

	std::cout << "Checking `cmd::Parser::isCorrectValues(*, cmd::Type::decimal)`...\n";
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
	assert(cmd::Parser::isCorrectValue("1234."		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1234."		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1234."		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("string"		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("~12334"		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1..234"		, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1.2.3.34"	, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("1.23.34"	, cmd::Type::decimal)				== false);
	assert(cmd::Parser::isCorrectValue("123."		, cmd::Type::decimal)				== false);


	std::cout << "Checking `cmd::to_string(cmd::Type)`...\n";
	assert(cmd::to_string(cmd::Type::argument)	== "argument"	);
	assert(cmd::to_string(cmd::Type::boolean)	== "boolean"	);
	assert(cmd::to_string(cmd::Type::integer)	== "integer"	);
	assert(cmd::to_string(cmd::Type::decimal)	== "decimal"	);
	assert(cmd::to_string(cmd::Type::string)	== "string"		);
	
	std::cout << "NOT checking `guessType()` because it is private\n";
	/*assert(cmd::Parser::guessType("\0")			== cmd::Type::argument);
	assert(cmd::Parser::guessType("+01")		== cmd::Type::integer);
	assert(cmd::Parser::guessType("-.90")		== cmd::Type::decimal);
	assert(cmd::Parser::guessType("azerty")		== cmd::Type::string);
	assert(cmd::Parser::guessType("--argument")	== cmd::Type::string);*/

	std::cout << "Success!" << std::endl;
}