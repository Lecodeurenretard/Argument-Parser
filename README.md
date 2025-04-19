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
If you need a documentation to for each function, you can go to the [code documentation](documentation.md).


## Features and parsing edge cases
In order to present some features let's say you have a C++ file named _argParser.cpp_ compiling to the executable _argParser_. The `parser` variable is an instance of `Parser` and `res` the result of parsing arguments from the command line.

<!--Positional arguments disabled-->

### Grouping one letter boolean arguments
Even if you think that's pretty specific, it is useful.

If _argParser.cpp_ contains this code:
```C++
if(std::get<bool>(res["-a"]))
	std::cout << "Success for a!\n";

if(std::get<bool>(res["-b"]))
	std::cout << "Success for b!\n";

if(std::get<bool>(res["-c"]))
	std::cout << "Success for c!\n";
```

and the user enters in the terminal:
```bash
argParser -abc		# all messages will be printed
argParser -ac		# all messages but the second one will be printed
```

### Specifing the same argument multiple times
The returned value will be the last entered.
```bash
argParser --hello 1 --hello 2 # The value of --hello will be 2
```

### Having an empty string as an argument
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
In the end you only need to know only the `Parser.parse()` method but I think it's interesting to know about the rest.

## Argument syntax
### Argument name syntax
The argument names have to begin with a dash `-` and if they have more than one letter they should have one more. All names must have one dash `-` (appart of the begining one(s)) or letter.  
Namely, all of those are correct names:
+ `-j`
+ `--j`
+ `--word`
+ `--mime-and-dash`
+ `--`
+ `---`

### Boolean syntax
The only correct boolean values are `true` and `false` (case-sensitive).

### Number syntax
#### Unsigned integer syntax
An unsigned integer have to contain only digit, sign `+` is not allowed.

#### Signed integer syntax
A signed integer can be negative so signs `+` and `-` are accepted.

#### Decimal number syntax
Follow the rules of the signed integer but can have a decimal part begining by a dot `.`. They can also not contain an integer part, in this case it is implied this part is 0.

All of those are correct numbers:
+ `1234`  (unsigned)
+ `+1234` (signed)
+ `-1234` (signed)
+ `1234.56` (decimal)
+ `-.1234`  (decimal)

### String syntax
All sequence of characters containing at least one non null characterand at most the string size limit are correct strings.