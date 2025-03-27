#include <iostream>
#include <nlibs/Parser>

using namespace cmd;

void printRes(std::map<std::string, outputType> map){
	for(auto pair : map){
		std::cout << "map[\"" << pair.first << "\"] = ";
		
		if (std::holds_alternative<bool>(pair.second))
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

int main(int argc, const char* argv[]){
	const Parser::argList_t list = {
			{"-tf"		, Type::boolean},
			{"--false"	, Type::boolean},
			{"--bool"	, Type::boolean},
			{"--string"	, Type::string},
			{"-u"	, Type::integer},
			{"-i"	, Type::integer},
			{"-f"	, Type::decimal},
			{"-d"	, Type::decimal},
	};
	Parser parser(
		list,
		true,
		true
	);

	printRes(parser.parse(argc, argv));
}