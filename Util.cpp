#if __has_include("Util.hpp")
	#include "Util.hpp"
#else
	#include <nlibs/Util>
#endif

namespace util {
	notImplemented::notImplemented(const char cstr[])
		: std::runtime_error(cstr)
	{}

	notImplemented::notImplemented(const std::string& str)
		: std::runtime_error(str)
	{}
}