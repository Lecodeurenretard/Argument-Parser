# Argument Parser
## Purpose
This is a library for parsing argument passed with the command line.

## Include
### Build from source
In order to include this library, you'll have to compile it from source (cmake and gcc required):
```bash
git clone "https://github.com/Lecodeurenretard/Argument-Parser.git"
cd "Argument-Parser"
 
./compileLib --install	# Require sudo permission, remove the `--install` argument if you don't have them
```

If you passed the `--install` parameter to `compileLib`, you can now include `<nlibs/Parser>` to use the `Parser` class. 
If not, you'll have to include `Parser.hpp` from where you cloned the repo, to compile your project you can use `compileExe`.

### Use the compiler scripts
There are two of them written in bash:
+ **`compileLibs`**: Compiles the Argument Parser library in the `build` directory, if it already exists will prompt the user what does he want to do.  
If the `--install` argument is passed and the user grants root permissions, will copy the compiled library and header in the `/usr/lib` and `/usr/include/c++/nlibs` folders.
+ **`compileExe`**: Compiles an executable using the compiled dynamic library in the same folder. Pass in first argument first the file to compile and the `g++` args.
+  **`installLibs`**: If you already have the shared libraries and the header, you can use it in order to install the library.

## Documentation
The `Parser`, the `Type` and the `handledType` are defined in the `cmd` namespace.

### The `Type` enum
This enumeration containg all types supported for parsing in the command line:
+ **`boolean`**: A boolean attribute, can have 3 values:
	- An explicit _`true`_ value,
	- An explicit _`false`_ value,
	- no value
  
	The first two are straightforward but the last one is a shorthand for `true` since the absence of a boolean parameter will set it to false.

+ **`integer`**: A signed integer.
+ **`decimal`**: A floating point value.
+ **`string`**: A sequence of characters.

+ **`argument`**: This type is only for internal purposes, it represents an argument.

### The `outputType` variant
This variant provides corresponding types for the `Type` enum, it maps all types from `Type` to a C++ type.  
Here are the links:
+ `Type::boolean`	$\longleftrightarrow$	`bool`
+ `Type::integer`	$\longleftrightarrow$	`int`
+ `Type::float`		$\longleftrightarrow$	`float`
+ `Type::string`	$\longleftrightarrow$	`std::string`
+ `Type::argument` don't have any corresponding type, the `to_value()` method will simply throw an error if you try to do it.

### Isolated functions
#### `std::string cmd::to_string(cmd::Type t)`
Returns the name of the type as a string, used for debugging purposes.

#### `bool cmd::to_bool(const std::string& str)`
Try to convert `str` to a boolean or throws an error if it can't.

#### `cmd::outputType cmd::to_value(std::string value)`
Try to guess the type of `value` and converts it to `value`.

#### `cmd::outputType cmd::to_value(cmd::Type t, std::string value)`
Try to convert `value` to `t`, if `t` is incompatible with `value` or `t` is `cmd::Types::argument`, will throw an error.

#### `bool std::isalpha(char c)`
The safe implementation shown in the docs of [`std::isalpha(int)`](https://en.cppreference.com/w/cpp/string/byte/isalpha).

#### `bool std::isspace(char c)`
The safe implementation shown in the docs of [`std::isspace(int)`](https://en.cppreference.com/w/cpp/string/byte/isspace).

#### `bool std::isdigit(char c)`
The safe implementation shown in the docs of [`std::isdigit(int)`](https://en.cppreference.com/w/cpp/string/byte/isdigit).


### The `cmd::Parser` class
This class is for parsing command line arguments, the purpose of this library.

#### Type aliases
```C++
using arg_t			= std::pair<std::string, cmd::Type>;		//The type of an argument
using argList_t		= std::map<std::string, cmd::Type>;			// A list contaning multiple arguments
using parseReturn_t = std::map<std::string, cmd::outputType>;	//The return type of `parse()`
```
#### Fields (all private)
##### `argList_t knownArguments`
A list of arguments provided by the user.

##### `std::vector<std::string> knownBoolArguments`
All boolean arguments provided by the user.

##### `bool parse0`
If the methods of the class should process the first argument given in an `argList_t`.

#### Methods (all public)
##### `cmd::Parser::Parser(const Parser::argList_t& arguments)`
Shorthand for `Parser(arguments, false)`.

##### `cmd::Parser::Parser(const Parser::argList_t& arguments, bool parseZero)`
Copy all elements of `arguments` into `knownArguments` and all boolean arguments into `knownBoolArguments`, set `parse0` to `parseZero`.

##### `cmd::Parser::parseReturn_t cmd::Parser::parse(int argc, const char* argv[], bool guess = false)`
Try to parse `argv` as an argument sequence entered via terminal. If succeed, return a map associating an `std::string` corresponding to a parsed argument with a `cmd::outputType` corresponding to its value. If an argument is not found, it will not be in the map except if it's a boolean argument (see the section on the [`Type` enum](#the-type-enum)). If `guess` is true, will try to guess unknown arguments' type.
If an invalid token is found during parsing, will throw a `cmd::Parser::parse_error`.  
For more information about parsing see [the edge cases](#edge-cases).

##### `bool cmd::Parser::isName(std::string str)`
Check if `str` is a key of `knownArguments`.

##### `static bool isCorrectName(std::string str)`
Check if `str` is a correct argument name (see [argument syntax](#argument-syntax)).

##### `static cmd::Type guessType(std::string value)`
Try to guess what type could be `value`, if it does not fit any type, return `Type::argument`.

##### `static bool Parser::isCorrectValue(std::string str, Type expectedType = Type::string)`
Check if `str` is a correct value for the given `expectedType` (see [argument syntax](#argument-syntax)).  
If `expectedType` is `Type::argument`, will always return `false`.  
If `expectedType` is `Type::string`, will always return `true`.  


### Edge cases
#### Specifing the same argument multiple times
The returned value will be the last entered.
```bash
argParser --hello 1 --hello 2 # The value of --hello will be 2
```

#### Having an empty string as an argument
The `parse()` method will throw an error because it uses the `"\0"` value as an error value.

## Using the library
Now you know what each function does, you can learn how to use them. A typical use would be this one:
```C++
#include <nlibs/Parser>

int main(int argc, const char* argv[]){
	cmd::Parser parser({
			{"--arg"		, cmd::Type::boolean},
			{"--argument"	, cmd::Type::string},
			//...
	});
	
	auto userInput = parser.parse(argc, argv);

	if(std::get<bool>(userInput["--argument1"])){	//testing if the argument was entered
		//do something
	}

	if(userInput.contains("--argument2")){	//same but --argument2 is not a boolean
		//do something
	}
}
```
In the end you only need to know only the `Parser.parse()` method but I think it's intrestreing to know about the rest.

## Argument syntax
### Argument name syntax
The argument names have to begin with a dash `-` and if they have more than one letter they should have one more. All names must have one dash `-` (appart of the begining one(s)) or letter.  
Namely, all of those are correct names:
+ `-j`
+ `--j`
+ `--word`
+ `--word-and-dash`
+ `--`
+ `---`

### Boolean syntax
The only correct boolean values are `true` and `false` (case-sensitive).

### Number syntax
A number may begin by a `+` or `-` sign, else they are only digits except for one dot `.` for decimal numbers. If the first character is a dot `.`, it is implied that the whole part of the number is zero.  
All of those are correct numbers:
+ `1234`
+ `+1234`
+ `-1234`
+ `1234.56`
+ `-.1234`

### String syntax
All sequence of characters except those above the string size limit are correct strings. This does **not** include empty strings.