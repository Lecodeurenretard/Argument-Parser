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
+ **`compileLib`**: Compiles the Argument Parser library in the `build` directory, if it already exists will prompt the user what does he want to do.
If the `--install` argument is passed and the user can grant root permissions, will copy the compiled library and header in the `/usr/lib` and `/usr/include/c++/nlibs` folders.
+ **`compileExe`**: Compiles an executable using the compiled dynamic library in the same folder. Pass in first argument first the file to compile and the `g++` args.  
