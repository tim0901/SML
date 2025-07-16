#ifndef SML_UTILITY_HPP
#define SML_UTILITY_HPP

namespace sml {

	sml_export template<typename T>
	concept arithmetic = std::integral<T> or std::floating_point<T>;

}
#endif // SML_UTILITY_HPP