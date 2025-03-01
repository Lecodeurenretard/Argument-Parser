#include <map>
#include <vector>
#include <stdexcept>

/** The theoraticaly maximum number of chars in a string (in reality, it's less) */
#define STRING_MAX std::string().max_size()

#ifndef INT8_MIN
#include <cstdint>
#endif

using cstring=const char*;

namespace cmd {
	/** Types of arguments */
	enum Type {
		undefined,		//should only be used internally & for errors, commented bc it has no use for now
		boolean,
		integer,
		decimal,
		string
	};
	std::string to_string(Type) noexcept(false);

	/** A type union holding matching types for the enum `Types` */
	union handeledType {
		void*	_undefined;
		bool	_bool;
		cstring	_string;
		int		_int;
		float	_float;
	};

	bool to_bool(const std::string&) 				noexcept(false);
	handeledType to_value(const cstring&)	noexcept(false);
	handeledType to_value(Type, const cstring&)	noexcept(false);

	/** A class for parsing command line arguments. */
	class Parser{
		public:
			using arg_t			= std::pair<cstring, Type>;
			using argList_t		= std::map <cstring, Type>;
			using argListPtr_t	= std::map <cstring, Type*>;

			Parser(void);
			Parser(cstring, Type)		noexcept(false);
			Parser(const arg_t&)		noexcept(false);
			Parser(const argList_t&)	noexcept(false);

			Parser(cstring, Type, bool)		noexcept(false);
			Parser(const arg_t&, bool)		noexcept(false);
			Parser(const argList_t&, bool)	noexcept(false);
			
			~Parser() = default;

			std::map<cstring, handeledType> parse(int, cstring[]) noexcept(false);
			
			bool isName(cstring) const;

			static bool isCorrectName(cstring);
			static bool isCorrectValue(cstring, Type=Type::string) noexcept(false);
		
		
			/** When `parse()` is not expecting something. */
			class parse_error : std::runtime_error {
			public: 
				parse_error(const char[]);
				parse_error(const std::string&);
			};
		private:
			argList_t knownArguments;
			std::vector<cstring> knownBoolArguments;	//all boolean arguments' indexes
			bool parse0;

			void construtorAdditions(const arg_t&);
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

	/** When a feature is not implemeted yet. */
	class notImplemented : std::logic_error{
	public:
		notImplemented(const char[]);
		notImplemented(const std::string&);
	};
}