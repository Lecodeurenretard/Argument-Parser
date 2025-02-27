#include <vector>
#include <map>
#include <cctype>
#include <stdexcept>
#include <iostream>

#ifndef INT8_MIN
#include <cstdint>
#endif

using cstring=const char*;

namespace cmd {
	enum Type {
		boolean,
		integer,
		decimal,
		string,
		//undefined		//should only be used internally & for errors, commented bc it has no use for now
	};
	union handeledType{
		bool	b;
		cstring	s;
		int		i;
		float	f;
	};

/** A class for parsing command line arguments. */
class Parser{
	public:
		using arg_t = std::pair<cstring, Type>;
		using argList_t = std::map<cstring, Type>;

		Parser(void);
		Parser(cstring, Type)		noexcept(false);
		Parser(const arg_t&)		noexcept(false);
		Parser(const argList_t&)	noexcept(false);
		
		~Parser() = default;

		//std::map<cstring, handeledType> parse(int, cstring[]) noexcept(false);
		static bool isCorrectName(cstring);
		static bool isCorrectValue(cstring, Type=Type::string) noexcept(false);
	
	private:
		argList_t knowArguments;
	};
}

//some wrappers
namespace std {
	bool isalpha(char);
	bool isdigit(char);
	bool isspace(char);
}

namespace util {
	size_t strCount(cstring);

	class notImplemented : std::logic_error{ public: notImplemented(const char[]); };
}