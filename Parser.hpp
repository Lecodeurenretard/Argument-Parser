#include <variant>
#include <vector>
#include <map>

#if __has_include(<nlibs/Util>)
	#include <nlibs/Util>
#else
	#include "Util.hpp"		//if the user has not the library installed yet
#endif


namespace cmd {
	/** Types of arguments */
	enum Type {
		argument,		//should only be used internally & for errors
		boolean,
		integer,
		unsignedInteger,
		decimal,
		string
	};
	std::string to_string(Type) noexcept(false);

	/** An std::variant containing all corresponding types of the `Type` enum. */
	typedef std::variant<
		bool,
		int,
		uint,
		float,
		std::string
	> outputType;
	

	bool to_bool(const std::string&) 			noexcept(false);
	outputType to_value(std::string)			noexcept(false);
	outputType to_value(std::string, Type)		noexcept(false);

	/** A class for parsing command line arguments. */
	class Parser{
		public:
			using arg_t			= std::pair<std::string, Type>;
			using argList_t		= std::map <std::string, Type>;

			Parser(void);
			Parser(bool);
			Parser(bool, bool);
			Parser(const argList_t&)				noexcept(false);
			Parser(const argList_t&, bool)			noexcept(false);
			Parser(const argList_t&, bool, bool)	noexcept(false);
			
			~Parser() = default;

			using parseReturn_t = std::map<std::string, outputType>;
			parseReturn_t parse(int, const char*[]) noexcept(false);

			bool isName(const std::string&) const;

			static bool isCorrectName(std::string);
			static bool isCorrectValue(std::string, Type=Type::string) noexcept(false);
		
			/** When `parse()` is not expecting something. */
			class parse_error : public std::runtime_error {
			public: 
				parse_error(const char[]);
				parse_error(const std::string&);
			};

			/** When `parse()` is not expecting something. */
			class unknownArgument_error : public parse_error {
			private:
				std::string argName;
			public: 
				unknownArgument_error(const char[], const char[]);
				unknownArgument_error(const std::string&, const std::string&);

				const std::string& getArgName(void) const;
			};

		private:
			argList_t knownArguments;
			std::vector<std::string> knownBoolArguments;
			bool parse0;
			bool argumentGuess;
			
			static Type guessType(const std::string&);

			bool isCombinedName(const std::string&);

	};
}

//some wrappers
namespace std {
	bool isalpha(char);
	bool isdigit(char);
	bool isspace(char);
}

namespace util {
	/**
	 * Check if `vec` contains `toSearch`.
	 */
	template<typename T>
	bool vecContain(std::vector<T> vec, T toSearch){
		return std::find(vec.begin(), vec.end(), toSearch) != vec.end();
	}
}