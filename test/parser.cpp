#include <nlibs/Parser>
#include <nlibs/colors.h>	//from misc_modules
#include <cassert>
#include <iostream>

using namespace cmd;

/**
 * Print a map resulting from the `Parser.parse()` method.
 */
void printRes(Parser::parseReturn_t map){
	for(auto pair : map){
		std::cout << "map[\"" << pair.first << "\"] = ";
		
		if (std::holds_alternative<bool>(pair.second	))
			std::cout << (std::get<bool>(pair.second)? "true" : "false");

		else if (std::holds_alternative<std::string>(pair.second))
			std::cout << '"' << std::get<std::string>(pair.second) << '"';

		else if (std::holds_alternative<int>(pair.second))
			std::cout << std::get<int>(pair.second);

		else if (std::holds_alternative<float>(pair.second))
			std::cout << std::get<float>(pair.second);

		else
			std::cout << "NULL";
		
		std::cout << '\n';
	}
}

/**
 * Print a map with the `Parser::argList_t` type.
 */
void printArgList(Parser::argList_t map){
	for(auto pair : map)
		std::cout << pair.first << ": " << cmd::to_string(pair.second) << "\n";
}

Parser::argList_t parserList = {
	{"-t"		, Type::boolean},
	{"-b"		, Type::boolean},
	{"-x"		, Type::boolean},
	{"-y"		, Type::boolean},
	{"--false"	, Type::boolean},
	{"--true"	, Type::boolean},

	{"-u"		, Type::integer},
	{"-i"		, Type::integer},

	{"-f"		, Type::decimal},
	{"-d"		, Type::decimal},
	
	{"-z"		, Type::string },
	{"-s"		, Type::string },
	{"--string"	, Type::string }
};
Parser parser(
	parserList,
	true,
	true
);

Parser::parseReturn_t run(int len, const char* arr[]){
	try {
		return parser.parse(len, arr);
	} catch(const cmd::Parser::parse_error& e) {
		std::cerr << "Parse error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	} catch(const util::notImplemented& e) {
		std::cerr << "Parse error: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	} catch(const std::exception& e){
		std::cerr << "Exception: " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

int main(void){
	std::cout << COLOR_FG_BYELLOW << "Testing return values\n"	<< COLOR_FG_RESET;
	constexpr size_t testLength = 13;
	const char* testArray[testLength] = {
		"-t",
		"--false"	, "false"					,
		"--string"	, "hello"					,
		"-u"		, "123456789"				,
		"-i"		, "-123456789"				,
		"-f"		, "-123456789.987654321"	,
		"-d"		, "-.6"						,
	};

	cmd::Parser::parseReturn_t res = run(testLength, testArray);
	assert(res.contains("--unknown")	== false);
	assert(res.contains("-o")			== false);
	
	std::cout << COLOR_FG_BYELLOW << "Testing bools\n"			<< COLOR_FG_RESET;
	assert(std::get<bool>(res["-t"])			== true);
	assert(std::get<bool>(res["-b"])			== false);
	assert(std::get<bool>(res["--false"])		== false);
	
	std::cout << COLOR_FG_BYELLOW << "Testing ints\n"			<< COLOR_FG_RESET;
	assert(std::get<int>(res["-u"])				==  123456789);
	assert(std::get<int>(res["-i"])				== -123456789);
	
	std::cout << COLOR_FG_BYELLOW << "Testing floats\n"			<< COLOR_FG_RESET;
	assert(std::get<float>(res["-f"])			== (float)(-1.23456789 * 100000000));
	assert(std::get<float>(res["-d"])			== (float)(-0.6));
	
	std::cout << COLOR_FG_BYELLOW << "Testing strings\n"		<< COLOR_FG_RESET;
	assert(std::get<std::string>(res["--string"])	== "hello");




	constexpr int doubleLength = 13;
	const char* doubleArray[doubleLength]= {
		"-b", "false",
		"-s", "jojo",
		"-i", "-15",
		"-s", "1",
		"-i", "43",
		"-b",
		"-i", "45"
	};
	
	std::cout << COLOR_FG_BYELLOW << "Testing redundancy\n"		<< COLOR_FG_RESET;
	res = run(doubleLength, doubleArray);
	
	assert(std::get<std::string>(res["-s"])	== "1");
	assert(std::get<int>(res["-i"])			== 45);
	assert(std::get<bool>(res["-b"])		== true);


	std::cout << COLOR_FG_BYELLOW << "Testing argument guessing\n" << COLOR_FG_RESET;
	constexpr int guessLength = 11;
	const char* guessArray[guessLength] = {
		"--ustr"	, "Hello!"	,
		"--uint"	, "1234"	,
		"--ufloat"	, ".567"	,
		"--ubool"	,
		"--ufalse"	, "false"	,
		"--utrue"	, "true"	,
	};

	res = run(guessLength, guessArray);
	
	//checking types
	assert(std::holds_alternative<std::string>(	res["--ustr"]	)	== true);
	assert(std::holds_alternative<int>(			res["--uint"]	)	== true);
	assert(std::holds_alternative<float>(		res["--ufloat"]	)	== true);
	assert(std::holds_alternative<bool>(		res["--ubool"]	)	== true);
	assert(std::holds_alternative<bool>(		res["--ufalse"]	)	== true);
	assert(std::holds_alternative<bool>(		res["--utrue"]	)	== true);
	
	//checking values
	assert(std::get<std::string>(	res["--ustr"]	) == "Hello!"		);
	assert(std::get<int>(			res["--uint"]	) == 1234			);
	assert(std::get<float>(			res["--ufloat"]	) == (float)0.567	);
	assert(std::get<bool>(			res["--ubool"]	) == true			);
	assert(std::get<bool>(			res["--ufalse"]	) == false			);
	assert(std::get<bool>(			res["--utrue"]	) == true			);




	std::cout << COLOR_FG_YELLOW << "NOT Checking isCombinedName() because it is private.\n" << COLOR_FG_RESET;
	/*assert(parser.isCombinedName("-ba")							== true);
	assert(parser.isCombinedName("-bb")								== true);
	assert(parser.isCombinedName("-aba")							== true);

	assert(parser.isCombinedName("\0")								== false);
	assert(parser.isCombinedName("-b")								== false);
	assert(parser.isCombinedName("-")								== false);
	assert(parser.isCombinedName("-bi")								== false);	// -i is an int
	assert(parser.isCombinedName("--ba")							== false);
	assert(parser.isCombinedName("-b i")							== false);
	assert(parser.isCombinedName("-b-i")							== false);
	assert(parser.isCombinedName("dfhge")							== false);
	assert(parser.isCombinedName("-abcdefghijklmonpqrstuvxyz")		== false);*/




	std::cout << COLOR_FG_BYELLOW << "Testing argument splitting\n" << COLOR_FG_RESET;
	constexpr int splitLength = 4;
	const char* splitArray[splitLength] = {
		"-bt"	,
		"--int"	, "1234",
		"-xya"
	};

	res = run(splitLength, splitArray);
	assert(std::get<bool>(res["-b"])			== true);
	assert(std::get<bool>(res["-t"])			== true);
	assert(std::get<bool>(res["-x"])			== true);
	assert(std::get<bool>(res["-y"])			== true);
	assert(std::get<bool>(res["-a"])			== true);
	assert(std::get<int>(res["--int"])			== 1234);
	
	assert(std::get<bool>(res["--true"])			== false);
	assert(std::get<bool>(res["--false"])			== false);

	std::cout << COLOR_FG_BYELLOW << "Testing empty arrays\n" << COLOR_FG_RESET;
	constexpr int emptyLength = 0;
	const char* emptyArray[emptyLength + 2] = {"--not-an", "--argument"};
	res = run(emptyLength, emptyArray);

	const Parser::parseReturn_t emptyExpected = {};		//the expected result of parse()

	assert(res == emptyExpected);


	std::cout << COLOR_FG_GREEN << "Success!" << COLOR_FG_RESET << std::endl;
}