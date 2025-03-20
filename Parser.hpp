#include <variant>
#include <algorithm>


/** The theoraticaly maximum number of chars in a string (in reality, it's less) */
#define STRING_MAX std::string().max_size()

#include <nlibs/Util>

namespace cmd {
	/** Types of arguments */
	enum Type {
		argument,		//should only be used internally & for errors
		boolean,
		integer,
		decimal,
		string
	};
	std::string to_string(Type) noexcept(false);

	/** An std::variant containing all corresponding types of the `Type` enum. */
	typedef std::variant<
		bool,
		int,
		float,
		std::string
	> outputType;
	

	bool to_bool(const std::string&) 			noexcept(false);
	outputType to_value(std::string)			noexcept(false);
	outputType to_value(Type, std::string)		noexcept(false);

	/** A class for parsing command line arguments. */
	class Parser{
		public:
			using arg_t			= std::pair<std::string, Type>;
			using argList_t		= std::map <std::string, Type>;

			Parser(const argList_t&)			noexcept(false);
			Parser(const argList_t&, bool)		noexcept(false);
			
			~Parser() = default;

			using parseReturn_t = std::map<std::string, outputType>;
			parseReturn_t parse(int, const char*[]) noexcept(false);
			
			bool isName(std::string) const;

			static bool isCorrectName(std::string);
			static bool isCorrectValue(std::string, Type=Type::string) noexcept(false);
		
			/** When `parse()` is not expecting something. */
			class parse_error : public std::runtime_error {
			public: 
				parse_error(const char[]);
				parse_error(const std::string&);
			};

		private:
			argList_t knownArguments;
			std::vector<std::string> knownBoolArguments;
			bool parse0;

			void argHandle(const arg_t&);
	};
}

//some wrappers
namespace std {
	bool isalpha(char);
	bool isdigit(char);
	bool isspace(char);
}