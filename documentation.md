# Code documentation

## The `Type` enum
This enumeration contains all types supported for parsing in the command line:
+ **`boolean`**: A boolean argument, while parsing accepts 3 values:
	- An explicit _`true`_ value,
	- An explicit _`false`_ value,
	- no value
  
	The first two are straightforward but the last one is a shorthand for `true` since the absence of a boolean parameter will set it to false.

+ **`unsignedInteger`**: A positive integer.
+ **`integer`**: A signed integer.
+ **`decimal`**: A floating point value.
+ **`string`**: A sequence of characters.

+ **`argument`**: This type is only for internal purposes, it represents an argument and is an error value for Types.

## The `outputType` variant
This variant provides corresponding types for the `Type` enum, it mirrors all types from `Type` to C++ types.  
Here are the correspondances:
+ `Type::boolean`		$\longleftrightarrow$	`bool`
+ `Type::unsignedInteger`	$\longleftrightarrow$	`uint`
+ `Type::integer`		$\longleftrightarrow$	`int`
+ `Type::float`			$\longleftrightarrow$	`float`
+ `Type::string`		$\longleftrightarrow$	`std::string`
+ `Type::argument` don't have any corresponding type, the `to_value()` method will simply throw an error if you try to do it.

## Isolated functions
### `std::string cmd::to_string(cmd::Type t)`
Returns the name of the type as a string, used for debugging purposes.

### `bool cmd::to_bool(const std::string& str)`
Try to convert `str` to a boolean or throws an error if it can't.

### `cmd::outputType cmd::to_value(std::string value)`
Try to guess the type of `value` and converts it to `value`.

### `cmd::outputType cmd::to_value(cmd::Type t, std::string value)`
Try to convert `value` to `t`, if `t` is incompatible with `value` or `t` is `cmd::Types::argument`, will throw an error.

### `bool std::isalpha(char c)`
The safe implementation shown in the docs of [`std::isalpha(int)`](https://en.cppreference.com/w/cpp/string/byte/isalpha).

### `bool std::isspace(char c)`
The safe implementation shown in the docs of [`std::isspace(int)`](https://en.cppreference.com/w/cpp/string/byte/isspace).

### `bool std::isdigit(char c)`
The safe implementation shown in the docs of [`std::isdigit(int)`](https://en.cppreference.com/w/cpp/string/byte/isdigit).


## The `cmd::Parser` class
This class is for parsing command line arguments, the purpose of this library.

### Type aliases
```C++
using arg_t			= std::pair<std::string, cmd::Type>;		//The type of an argument
using argList_t		= std::map<std::string, cmd::Type>;			// A list contaning multiple arguments
using parseReturn_t = std::map<std::string, cmd::outputType>;	//The return type of `parse()`
```


### Fields (all private)
#### `argList_t knownArguments`
A list of arguments provided by the user.

#### `std::vector<std::string> knownBoolArguments`
All boolean arguments provided by the user.

#### `bool parse0`
If the methods of the class should process the first argument given in an `argList_t`, should turn be turned off for real command line parsing since `argv[0]` is the executable's name.

#### `bool argumentGuess`
If the parsing method should try to guess the type of an unknown argument. If `false` they will throw an error when finding an unknown argument name.

### Methods (all public)
#### `cmd::Parser::Parser(const Parser::argList_t& arguments)`
Shorthand for `Parser(arguments, false)`.

#### `cmd::Parser::Parser(const Parser::argList_t& arguments, bool parseZero)`
Copy all elements of `arguments` into `knownArguments` and all boolean arguments into `knownBoolArguments`, set `parse0` to `parseZero`.

#### `cmd::Parser::parseReturn_t cmd::Parser::parse(int argc, const char* argv[], bool guess = false)`
Try to parse `argv` as an argument sequence entered via terminal. If succeed, return a map associating an `std::string` corresponding to a parsed argument with a `cmd::outputType` corresponding to its value. If an argument is not found, it will not be in the map except if it's a boolean argument (see the section on the [`Type` enum](#the-type-enum)). If `guess` is true, will try to guess unknown arguments' type.
If an invalid token is found during parsing, will throw a `cmd::Parser::parse_error`.  
For more information about parsing see [the edge cases](#edge-cases).

#### `bool cmd::Parser::isName(std::string str)`
Check if `str` is a key of `knownArguments`.

#### `static bool isCorrectName(std::string str)`
Check if `str` is a correct argument name (see [argument syntax](#argument-syntax)).

#### `static bool Parser::isCorrectValue(std::string str, Type expectedType = Type::string)`
Check if `str` is a correct value for the given `expectedType` (see [argument syntax](#argument-syntax)).  
If `expectedType` is `Type::argument`, will always return `false`.  
If `expectedType` is `Type::string`, will always return `true`.  
