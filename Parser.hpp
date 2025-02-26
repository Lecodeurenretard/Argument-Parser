#include <vector>
#include <map>
#include <cctype>

#ifndef INT8_MIN
#include <cstdint>
#endif

using cstring=char*;

namespace cmd{
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
		Parser(cstring, Type)		noexcept(true);
		Parser(const arg_t&)		noexcept(true);
		Parser(const argList_t&)	noexcept(true);
		
		~Parser() = default;

		std::map<cstring, handeledType> parse(int, cstring[]) noexcept(true);
	
	private:
		argList_t knowArguments;

		static bool isCorrectName(cstring);
		static bool isCorrectValue(cstring, Type=Type::string);
		
		static cstring toCorrectName(cstring);
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
}