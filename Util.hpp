#include <vector>
#include <stdexcept>
#include <map>

#ifndef INT8_MIN
#include <cstdint>
#endif

namespace util {
	size_t strCount(std::string);

	/** When a feature is not implemeted yet. */
	class notImplemented : std::logic_error{
	public:
		notImplemented(const char[]);
		notImplemented(const std::string&);
		
		const char* what(void) const noexcept;
	};

	template<typename T>
	struct ptrLess {
		/** * Check if the value at `v1` is less than the value at `v2`.*/
		constexpr bool operator()(T* v1, T* v2) const{
			return *v1 < *v2;
		}
	};

	template<typename T>
	struct ptrMore {
		/** * Check if the value at `v1` is less than the value at `v2`.*/
		constexpr bool operator()(T* v1, T* v2) const{
			return *v1 > *v2;
		}
	};

	template<typename T>
	struct ptrEqual {
		/** * Check if the value at `v1` is less than the value at `v2`.*/
		constexpr bool operator()(T* v1, T* v2) const{
			return *v1 == *v2;
		}
	};
}