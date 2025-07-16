#ifndef SML_UTILITY_HPP
#define SML_UTILITY_HPP

namespace sml {

	sml_export template<typename T>
	concept arithmetic = std::integral<T> or std::floating_point<T>;

	constexpr double DegToRad = std::numbers::pi / 180.0;
	constexpr double RadToDeg = std::numbers::inv_pi * 180.0;

	sml_export template<arithmetic T>
	double DegreesToRadians(const T& theta) {
		return (theta * DegToRad);
	}

	sml_export template<arithmetic T>
		double RadiansToDegrees(const T& rads) {
		return (rads * RadToDeg); 
	}

}
#endif // SML_UTILITY_HPP