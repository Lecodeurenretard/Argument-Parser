#include "Util.hpp"

namespace util {
	/**
	* Count the all chars in a C string except the null terminator.
	*/
	size_t strCount(std::string cstr){
		size_t i = 0;
		while(i < SIZE_MAX){
			if(cstr[i] == '\0')
				return i;
			i++;
		}
		return SIZE_MAX;
	}



	notImplemented::notImplemented(const char cstr[])
		: logic_error(cstr)
	{}

	notImplemented::notImplemented(const std::string& str)
		: logic_error(str)
	{}

	const char* notImplemented::what(void) const noexcept{
		return logic_error::what();
	}

}