#ifndef SML_VECTOR_HPP
#define SML_VECTOR_HPP

#ifndef SML_MODULE_VECTOR

#include <array>
#include <algorithm>
#include <iostream>
#include <vector>

#ifndef sml_export
#define sml_export
#endif // !sml_export

#endif // !SML_MODULE_VECTOR

namespace sml {

sml_export template <typename T, size_t elements>
class Vector {
public:
	Vector() { }
	template<typename T2>
	Vector(T2 t) {
		for (size_t i = 0; i < elements; i++) {
			data.at(i) = static_cast<T>(t);
		}
	}
	template<typename ...T2>
	Vector(T2 ...ts) : data{ static_cast<T>(ts)... } {}
	template<typename T2>
	Vector(const Vector<T2, elements>& v2) {
		std::transform(v2.begin(), v2.end(), data.begin(), [](T2 t)->T {return static_cast<T>(t); });
	}
	template<typename T2>
	Vector(const T2 arr[elements]) {
		for (size_t i = 0; i < elements; i++) {
			data.at(i) = static_cast<T>(arr[i]);
		}
	}
	template<typename T2>
	Vector(const std::array<T2, elements> arr) {
		std::transform(arr.begin(), arr.end(), data.begin(), [](T2 t)->T {return static_cast<T>(t); });
	}

	// Access elements with v[i]
	inline T operator [] (size_t i) const { return data[i]; }
	inline T& operator [] (size_t i) { return data[i]; }

	// Access elements with v.at(i)
	inline T at(int i) const { return data.at(i); }
	inline T& at(int i) { return data.at(i); }

	// Unary operators
	inline const Vector& operator + () const { return *this; }
	inline Vector operator - () const {
		Vector<T, elements> ret = *this; 
		std::transform(ret.begin(), ret.end(), ret.begin(), [](T t)->T {return -t; }); 
		return ret;
	}

	// Comparison operators
	inline bool operator == (const Vector<T, elements>& v2) const {
		return std::equal(data.begin(), data.end(), v2.begin(), v2.end());
	}
	inline bool operator != (const Vector<T, elements>& v2) const {
		return !(*this == v2);
	}

	// Arithmetic operators
	
	// Addition
	template<typename T2>
	inline Vector<T, elements>& operator += (const Vector<T2, elements>& v2);
	template<typename T2>
	inline Vector<T, elements>& operator += (const T2& t);

	// Subtraction
	template<typename T2>
	inline Vector<T, elements>& operator -= (const Vector<T2, elements>& v2);
	template<typename T2>
	inline Vector<T, elements>& operator -= (const T2& t);

	// Multiplication
	template<typename T2>
	inline Vector<T, elements>& operator *= (const Vector<T2, elements>& v2);
	template<typename T2>
	inline Vector<T, elements>& operator *= (const T2& t);

	// Division
	template<typename T2>
	inline Vector<T, elements>& operator /= (const Vector<T2, elements>& v2);
	template<typename T2>
	inline Vector<T, elements>& operator /= (const T2& t);

	// Modulus - requires integer operands
	template<typename T2>
	inline Vector<T, elements>& operator %= (const Vector<T2, elements>& v2);
	template<typename T2>
	inline Vector<T, elements>& operator %= (const T2& t);

	// Assignment operator
	template<typename T2>
	inline Vector& operator = (const Vector<T2, elements>& v2) {
		std::transform(v2.begin(), v2.end(), data.begin(), [](T2 t)->T {return static_cast<T>(t); });
		return *this;
	}

	// Accessor functions for accessing uv/coordinate/colour elements - analogous to at()
	// Useful for code readability, but use with caution as all are available for vectors of any length
	// Eg. Calling a() on a 3-vector will throw an error
	inline T& u() { return data.at(0); }
	inline T& v() { return data.at(1); }
	inline T& x() { return data.at(0); }
	inline T& y() { return data.at(1); }
	inline T& z() { return data.at(2); }
	inline T& w() { return data.at(3); }
	inline T& r() { return data.at(0); }
	inline T& g() { return data.at(1); }
	inline T& b() { return data.at(2); }
	inline T& a() { return data.at(3); }
	inline T u() const { return data.at(0); }
	inline T v() const { return data.at(1); }
	inline T x() const { return data.at(0); }
	inline T y() const { return data.at(1); }
	inline T z() const { return data.at(2); }
	inline T w() const { return data.at(3); }
	inline T r() const { return data.at(0); }
	inline T g() const { return data.at(1); }
	inline T b() const { return data.at(2); }
	inline T a() const { return data.at(3); }

	// Is this a zero vector?
	inline bool IsZero() const {
		return Vector<T, elements>(0) == *this;
	}

	// Make this vector a zero vector
	inline void Clear(void* = 0) {
		for (int i = 0; i < elements; i++) {
			data[i] = T(0);
		}
	}
	inline void Zero() {
		Clear();
	}

	// v1 = v1 + v2 * weight
	void AddWithWeight(Vector const& v2, float weight) {
		for (int i = 0; i < elements; i++) {
			data[i] += v2.data[i] * weight;
		}
	}

	// C++ container named requirements
	inline constexpr std::array<T, elements>::iterator begin() noexcept { return data.begin(); }
	inline constexpr std::array<T, elements>::const_iterator begin() const noexcept { return data.begin(); }
	inline constexpr std::array<T, elements>::const_iterator cbegin() const noexcept { return data.cbegin(); }
	inline constexpr std::array<T, elements>::iterator end() noexcept { return data.end(); }
	inline constexpr std::array<T, elements>::const_iterator end() const noexcept { return data.end(); }
	inline constexpr std::array<T, elements>::const_iterator cend() const noexcept { return data.cend(); }

	inline constexpr std::array<T, elements>::size_type size() const noexcept { return data.size(); }
	inline constexpr std::array<T, elements>::size_type max_size() const noexcept { return data.max_size(); }
	inline constexpr bool empty() const noexcept { return data.empty(); }

	std::array<T, elements> data = {};
};

// Write vector to ostream
sml_export template<typename T, size_t elements>
inline std::ostream& operator << (std::ostream& os, const Vector<T, elements>& v) {
	for (size_t i = 0; i < elements; i++) {
		os << v[i];
		if (i < (elements - 1))
			os << " ";
	}
	return os;
}

// Write vector to wostream
sml_export template<typename T, size_t elements>
inline std::wostream& operator << (std::wostream& os, const Vector<T, elements>& v) {
	for (size_t i = 0; i < elements; i++) {
		os << v[i];
		if (i < (elements - 1))
			os << " ";
	}
	return os;
}

// Vector addition
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator += (const Vector<T2, elements>& v2) {
	for (size_t i = 0; i < elements; i++) {
		data[i] += static_cast<T>(v2[i]);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator + (Vector<T, elements> v1, const Vector<T2, elements>& v2) {
	v1 += v2;
	return v1;
}

// Scalar addition
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator+= (const T2& t) {
	for (size_t i = 0; i < elements; i++) {
		data[i] += static_cast<T>(t);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator + (Vector<T, elements> v1, const T2& t) {
	v1 += t;
	return v1;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator + (const T2& t, Vector<T, elements> v1) {
	v1 += t;
	return v1;
}

// Vector subtraction
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator -= (const Vector<T2, elements>& v2) {
	for (size_t i = 0; i < elements; i++) {
		data[i] -= static_cast<T>(v2[i]);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator - (Vector<T, elements> v1, const Vector<T2, elements>& v2) {
	v1 -= v2;
	return v1;
}

// Scalar subtraction
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator-= (const T2& t) {
	for (size_t i = 0; i < elements; i++) {
		data[i] -= static_cast<T>(t);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator - (Vector<T, elements> v1, const T2& t) {
	v1 -= t;
	return v1;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator - (const T2& t, Vector<T, elements> v1) {
	for (size_t i = 0; i < elements; i++) {
		v1[i] = static_cast<T>(t) - v1[i];
	}
	return v1;
}

// Vector multiplication
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator *= (const Vector<T2, elements>& v2) {
	for (size_t i = 0; i < elements; i++) {
		data[i] *= static_cast<T>(v2[i]);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator * (Vector<T, elements> v1, const Vector<T2, elements>& v2) {
	v1 *= v2;
	return v1;
}

// Scalar multiplication
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator*= (const T2& t) {
	for (size_t i = 0; i < elements; i++) {
		data[i] *= static_cast<T>(t);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator * (Vector<T, elements> v1, const T2& t) {
	v1 *= t;
	return v1;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator * (const T2& t, Vector<T, elements> v1) {
	v1 *= t;
	return v1;
}

// Vector division
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator /= (const Vector<T2, elements>& v2) {
	for (size_t i = 0; i < elements; i++) {
		data[i] /= static_cast<T>(v2[i]);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator / (Vector<T, elements> v1, const Vector<T2, elements>& v2) {
	v1 /= v2;
	return v1;
}

// Scalar division
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator /= (const T2& t) {
	double inverse = 1.0 / static_cast<T>(t);
	for (size_t i = 0; i < elements; i++) {
		data[i] *= inverse;
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator / (Vector<T, elements> v1, const T2& t) {
	v1 /= t;
	return v1;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator / (const T2& t, Vector<T, elements> v1) {
	for (size_t i = 0; i < elements; i++) {
		v1[i] = static_cast<T>(t) / v1[i];
	}
	return v1;
}

// Vector modulus - requires integer operands
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator %= (const Vector<T2, elements>& v2) {
	for (size_t i = 0; i < elements; i++) {
		data[i] %= static_cast<T>(v2[i]);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator % (Vector<T, elements> v1, const Vector<T2, elements>& v2) {
	v1 %= v2;
	return v1;
}

// Scalar modulus - requires integer operands
sml_export template<typename T, size_t elements>
template<typename T2>
inline Vector<T, elements>& Vector<T, elements>::operator%= (const T2& t) {
	for (size_t i = 0; i < elements; i++) {
		data[i] %= static_cast<T>(t);
	}
	return *this;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator % (Vector<T, elements> v1, const T2& t) {
	v1 %= t;
	return v1;
}
sml_export template<typename T, size_t elements, typename T2>
inline Vector<T, elements> operator % (const T2& t, Vector<T, elements> v1) {
	v1 %= t;
	return v1;
}

// Perform the dot product on two equal-length vectors
sml_export template<typename T, size_t elements, typename T2>
double constexpr dot(const Vector<T, elements>& v1, const Vector<T2, elements>& v2) {
	double ret = 0.0;
	for (int i = 0; i < elements; i++) {
		ret += v1[i] * static_cast<T>(v2[i]);
	}
	return ret;
}

// Calculate the length of the vector
sml_export template<typename T, size_t elements>
double length(const Vector<T, elements>& v) {
	return sqrt(squared_length(v));
}

// Calculate the square of the length of the vector
sml_export template<typename T, size_t elements>
double squared_length(const Vector<T, elements>& v) {
	double ret = 0.0;
	for (auto i : v) {
		ret += (i * i);
	}
	return ret;
}

// Normalise the vector
sml_export template<typename T, size_t elements>
Vector<T, elements> unit_vector(const Vector<T, elements>& v) {
	return v / length(v);
}

// Calculate the cross product of two 3-vectors
// The cross product only exists in 3 and 7 dimensions
sml_export template<typename T>
Vector<T, 3> constexpr cross_product(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
	return Vector<T, 3>((v1[1] * v2[2] - v1[2] * v2[1]), (v1[2] * v2[0] - v1[0] * v2[2]), (v1[0] * v2[1] - v1[1] * v2[0]));
}

using std::abs;
sml_export template<typename T>
Vector<T, 3> abs(const Vector<T, 3>& v) {
	return Vector<T, 3>(abs(v[0]), abs(v[1]), abs(v[2]));
}

sml_export template<typename T, size_t elements, typename T2, typename T3>
Vector<T, elements> lerp(const Vector<T, elements>& v1, const Vector<T2, elements>& v2, const T3 t) {
	return (v1 + ((v2 - v1) * static_cast<T>(t)));
}

// Return the index containing the largest value
sml_export template<typename T, size_t elements>
size_t max_element(const Vector<T, elements>& v) {
	return std::distance(v.begin(), std::max_element(v.begin(), v.end()));
}

// Return the index containing the smallest value
sml_export template<typename T, size_t elements>
size_t min_element(const Vector<T, elements>& v) {
	return std::distance(v.begin(), std::min_element(v.begin(), v.end()));
}

// Return the largest element
sml_export template<typename T, size_t elements>
T max(const Vector<T, elements>& v) {
	return *std::max_element(v.begin(), v.end());
}
// Given two equal-length vectors, return a vector containing the largest value for each dimension, 
sml_export template<typename T, size_t elements>
Vector<T, elements> max(const Vector<T, elements>& v1, const Vector<T, elements>& v2) {
	Vector<T, elements> ret;
	for (size_t i = 0; i < elements; i++) {
		ret[i] = v1[i] > v2[i] ? v1[i] : v2[i];
	}
	return ret;
}
// Given two 2-vectors, return a 2-vector containing the largest value for each dimension
sml_export template<typename T>
Vector<T, 2> max(const Vector<T, 2>& v1, const Vector<T, 2>& v2) {
	return Vector<T, 2>(v1[0] > v2[0] ? v1[0] : v2[0],
		v1[1] > v2[1] ? v1[1] : v2[1]);
}
// Given two 3-vectors, return a 3-vector containing the largest value for each dimension
sml_export template<typename T>
Vector<T, 3> max(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
	return Vector<T, 3>(v1[0] > v2[0] ? v1[0] : v2[0],
		v1[1] > v2[1] ? v1[1] : v2[1],
		v1[2] > v2[2] ? v1[2] : v2[2]);
}
// Given two 4-vectors, return a 4-vector containing the smallest value for each dimension
sml_export template<typename T>
Vector<T, 4> max(const Vector<T, 4>& v1, const Vector<T, 4>& v2) {
	return Vector<T, 4>(v1[0] > v2[0] ? v1[0] : v2[0],
		v1[1] > v2[1] ? v1[1] : v2[1],
		v1[2] > v2[2] ? v1[2] : v2[2],
		v1[3] > v2[3] ? v1[3] : v2[3]);
}

// Return the smallest element
sml_export template<typename T, size_t elements>
T min(const Vector<T, elements>& v) {
	return *std::min_element(v.begin(), v.end());
}
// Given two equal-length vectors, return a vector containing the smallest value for each dimension, 
sml_export template<typename T, size_t elements>
Vector<T, elements> min(const Vector<T, elements>& v1, const Vector<T, elements>& v2) {
	Vector<T, elements> ret;
	for (size_t i = 0; i < elements; i++) {
		ret[i] = v1[i] < v2[i] ? v1[i] : v2[i];
	}
	return ret;
}
// Given two 2-vectors, return a 2-vector containing the smallest value for each dimension
sml_export template<typename T>
Vector<T, 2> min(const Vector<T, 2>& v1, const Vector<T, 2>& v2) {
	return Vector<T, 2>(v1[0] < v2[0] ? v1[0] : v2[0],
		v1[1] < v2[1] ? v1[1] : v2[1]);
}
// Given two 3-vectors, return a 3-vector containing the smallest value for each dimension
sml_export template<typename T>
Vector<T, 3> min(const Vector<T, 3>& v1, const Vector<T, 3>& v2) {
	return Vector<T, 3>(v1[0] < v2[0] ? v1[0] : v2[0],
		v1[1] < v2[1] ? v1[1] : v2[1],
		v1[2] < v2[2] ? v1[2] : v2[2]);
}
// Given two 4-vectors, return a 4-vector containing the smallest value for each dimension
sml_export template<typename T>
Vector<T, 4> min(const Vector<T, 4>& v1, const Vector<T, 4>& v2) {
	return Vector<T, 4>(v1[0] < v2[0] ? v1[0] : v2[0],
		v1[1] < v2[1] ? v1[1] : v2[1],
		v1[2] < v2[2] ? v1[2] : v2[2],
		v1[3] < v2[3] ? v1[3] : v2[3]);
}

// Reorder vector indices as desired
sml_export template<typename T, size_t elements, typename ...Ts>
Vector<T, elements> permute(const Vector<T, elements>& v, Ts ...ts) {
	Vector<T, elements> ret;
	std::vector<size_t>indices({ static_cast<size_t>(ts)... });
	for (size_t i = 0; i < indices.size(); i++) {
		ret[i] = v[indices[i]];
	}
	indices.clear();
	return ret;
}
sml_export template<typename T, size_t elements>
Vector<T, elements> permute(const Vector<T, elements>& v, const Vector<size_t, elements> indices) {
	Vector<T, elements> ret;
	for (size_t i = 0; i < indices.size(); i++) {
		ret[i] = v[indices[i]];
	}
	return ret;
}
sml_export template<typename T>
Vector<T, 2> permute(const Vector<T, 2>& v, size_t x, size_t y) {
	return Vector<T, 2>(v[x], v[y]);
}
sml_export template<typename T>
Vector<T, 3> permute(const Vector<T, 3>& v, size_t x, size_t y, size_t z) {
	return Vector<T, 3>(v[x], v[y], v[z]);
}
sml_export template<typename T>
Vector<T, 4> permute(const Vector<T, 4>& v, size_t x, size_t y, size_t z, size_t w) {
	return Vector<T, 4>(v[x], v[y], v[z], v[w]);
}

// Shorthand definitions
sml_export using Vec2d = Vector<double, 2>;
sml_export using Vec3d = Vector<double, 3>;
sml_export using Vec4d = Vector<double, 4>;
sml_export using Vec2f = Vector<float, 2>;
sml_export using Vec3f = Vector<float, 3>;
sml_export using Vec4f = Vector<float, 4>;
sml_export using Vec2i = Vector<int, 2>;
sml_export using Vec3i = Vector<int, 3>;
sml_export using Vec4i = Vector<int, 4>;

} //!namespace sml
#endif // !SML_VECTOR_HPP
