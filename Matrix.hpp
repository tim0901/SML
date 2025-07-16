#ifndef SML_MATRIX_HPP
#define SML_MATRIX_HPP

#ifndef SML_MODULE_MATRIX
#include <array>
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#ifndef sml_export
#define sml_export
#endif // !sml_export

#endif // !SML_MODULE_MATRIX

namespace sml {

// This class uses row-major memory ordering
sml_export template<arithmetic T, size_t nrows, size_t ncols>
class Matrix {
public:
	Matrix() {};
	template<arithmetic T2>
	Matrix(T2 t) {
		for (size_t i = 0; i < nrows*ncols; i++) {
			data[i] = static_cast<T>(t);
		}
	}
	template<arithmetic ... T2>
	Matrix(T2 ...ts) : data{ static_cast<T>(ts)... } {}
	template<arithmetic T2>
	Matrix(const Matrix<T2, nrows, ncols>& m2) {
		std::transform(m2.begin(), m2.end(), data.begin(), [](T2 t)->T {return static_cast<T>(t); });
	}
	template<arithmetic T2>
	Matrix(const T2 arr[nrows][ncols]) {
		for (size_t i = 0; i < nrows; i++) {
			for (size_t j = 0; j < ncols; j++) {
				data.at((i * ncols) + j) = static_cast<T>(arr[i][j]);
			}
		}
	}
	template<arithmetic T2>
	Matrix(const T2 arr[nrows * ncols]) {
		for (size_t i = 0; i < nrows; i++) {
			for (size_t j = 0; j < ncols; j++) {
				data.at((i * ncols) + j) = static_cast<T>(arr[(i * ncols) + j]);
			}
		}
	}
	template<arithmetic T2>
	Matrix(const std::array<T2, nrows* ncols> arr) {
		std::transform(arr.begin(), arr.end(), data.begin(), [](T2 t)->T {return static_cast<T>(t); });
	}
	
	// TODO: Construct from sub-matricecs (rows/column vectors, squares eg. Pauli matrices)

	// Access elements with M[row][column]
	inline std::array<T, nrows * ncols>::iterator operator [] (size_t i) { return data.begin() + (i * ncols); }
	inline std::array<T, nrows* ncols>::const_iterator operator [] (size_t i) const { return data.begin() + (i * ncols); }
	// (This class uses row-major memory ordering)

	// Access elements with m.at(i)
	inline T at(size_t i) const { return data.at(i); }
	inline T& at(size_t i) { return data.at(i); }
	// Access elements with m.at(row, column)
	inline T at(size_t r, size_t c) const { return data.at((r * ncols) + c); }
	inline T& at(size_t r, size_t c) { return data.at((r * ncols) + c); }

	// Fetch an individual row as a Matrix
	inline Matrix<T, 1, ncols> row_matrix(int r) const {
		Matrix<T, 1, ncols> ret;
		for (size_t i = 0; i < ncols; i++) {
			ret[0][i] = data[(r * ncols) + i];
		}
		return ret;
	}

	// Fetch an individual row as a Vector
	inline Vector<T, ncols> row_vector(int r) const {
		Vector<T, ncols> ret;
		for (size_t i = 0; i < ncols; i++) {
			ret[i] = data[(r * ncols) + i];
		}
		return ret;
	}

	// Fetch an individual column as a Matrix
	inline Matrix<T, nrows, 1> col_matrix(int c) const {
		Matrix<T, nrows, 1> ret;
		for (size_t i = 0; i < nrows; i++) {
			ret[i][0] = data[(i * ncols) + c];
		}
		return ret;
	}

	// Fetch an individual column as a Vector
	inline Vector<T, nrows> col_vector(int c) const {
		Vector<T, nrows> ret;
		for (size_t i = 0; i < nrows; i++) {
			ret[i] = data[(i * ncols) + c];
		}
		return ret;
	}

	// Unary operators
	inline const Matrix& operator + () const { return *this; }
	inline Matrix operator - () const {
		Matrix<T, nrows, ncols> ret = *this;
		std::transform(ret.begin(), ret.end(), ret.begin(), [](T t)->T {return -t; });
		return ret;
	}

	// Comparison operators
	inline bool operator == (const Matrix<T, nrows, ncols>& m2) const {
		return std::equal(data.begin(), data.end(), m2.begin(), m2.end());
	}
	inline bool operator != (const Matrix<T, nrows, ncols>& m2) const {
		return !(*this == m2);
	}

	// Arithmetic operators

	// Addition
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator += (const Matrix<T2, nrows, ncols>& m2);
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator += (const T2& t);

	// Subtraction
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator -= (const Matrix<T2, nrows, ncols>& m2);
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator -= (const T2& t);

	// Matrix product
	template<arithmetic T2, size_t ncols2>
	inline Matrix<T, nrows, ncols>& operator *= (const Matrix<T2, ncols, ncols2>& m2);
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator *= (const T2& t);
	
	// Division
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator /= (const T2& t);

	// Modulus - requires integer operands
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator %= (const Matrix<T2, nrows, ncols>& m2);
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator %= (const T2& t);

	// Assignment operator
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& operator = (const Matrix<T2, nrows, ncols>& m2) {
		std::transform(m2.begin(), m2.end(), data.begin(), [](T2 t)->T {return static_cast<T>(t); });
		return *this;
	}

	// Add a row to each row of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& add_row(const Matrix<T2, 1, ncols>& m2);
	// Subtract a row from each row of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& sub_row(const Matrix<T2, 1, ncols>& m2);
	// Multiply each row of a Matrix with a row
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& mul_row(const Matrix<T2, 1, ncols>& m2);
	// Divide a each row of a Matrix by a row
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& div_row(const Matrix<T2, 1, ncols>& m2);

	// Add a column to each column of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& add_col(const Matrix<T2, nrows, 1>& m2);
	// Subtract a column from each column of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& sub_col(const Matrix<T2, nrows, 1>& m2);
	// Multiply each column of a Matrix with a column
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& mul_col(const Matrix<T2, nrows, 1>& m2);
	// Divide a each column of a Matrix by a column
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& div_col(const Matrix<T2, nrows, 1>& m2);

	// Add a Vector to each row of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& add_row(const Vector<T2, ncols>& v);
	// Subtract a Vector from each row of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& sub_row(const Vector<T2, ncols>& v);
	// Multiply each row of a Matrix with a Vector
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& mul_row(const Vector<T2, ncols>& v);
	// Divide a each row of a Matrix by a Vector
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& div_row(const Vector<T2, ncols>& v);

	// Add a Vector to each column of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& add_col(const Vector<T2, nrows>& v);
	// Subtract a Vector from each column of a Matrix
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& sub_col(const Vector<T2, nrows>& v);
	// Multiply each column of a Matrix with a Vector
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& mul_col(const Vector<T2, nrows>& v);
	// Divide a each column of a Matrix by a Vector
	template<arithmetic T2>
	inline Matrix<T, nrows, ncols>& div_col(const Vector<T2, nrows>& v);

	// C++ container named requirements
	inline constexpr std::array<T, nrows * ncols>::iterator begin() noexcept { return data.begin(); }
	inline constexpr std::array<T, nrows * ncols>::const_iterator begin() const noexcept { return data.begin(); }
	inline constexpr std::array<T, nrows * ncols>::const_iterator cbegin() const noexcept { return data.cbegin(); }
	inline constexpr std::array<T, nrows * ncols>::iterator end() noexcept { return data.end(); }
	inline constexpr std::array<T, nrows * ncols>::const_iterator end() const noexcept { return data.end(); }
	inline constexpr std::array<T, nrows * ncols>::const_iterator cend() const noexcept { return data.cend(); }

	inline constexpr std::array<T, nrows * ncols>::size_type size() const noexcept { return data.size(); }
	inline constexpr std::array<T, nrows * ncols>::size_type max_size() const noexcept { return data.max_size(); }
	inline constexpr bool empty() const noexcept { return data.empty(); }

	inline constexpr size_t rows() const { return nrows; }
	inline constexpr size_t cols() const { return ncols; }
	std::array<T, nrows * ncols> data = {};
};

// Ostream << operator. Allows matrices to be printed to the console using eg cout
sml_export template<class T, size_t nrows, size_t ncols>
inline std::ostream& operator << (std::ostream& os, const Matrix<T, nrows, ncols> m) {
	// For each column, find the longest element

	std::vector<size_t> column_widths(ncols);
	Matrix<std::string, nrows, ncols> stringMatrix;

	// Find the longest element in each column to determine their widths
	for (int i = 0; i < ncols; i++) {
		std::vector<std::string> stringVector(0);
		// Convert Ts to std::strings
		for (int j = 0; j < nrows; j++) {
			stringMatrix[j][i] = std::to_string(m[j][i]);
			// Trim trailing zeroes / decimal points from floating point types
			if (std::is_floating_point_v<T>) {
				stringMatrix[j][i].erase(stringMatrix[j][i].find_last_not_of('0') + 1, std::string::npos);
				stringMatrix[j][i].erase(stringMatrix[j][i].find_last_not_of('.') + 1, std::string::npos);
			}
			stringVector.push_back(stringMatrix[j][i]);
		}
		column_widths[i] = std::max_element(stringVector.begin(), stringVector.end(), [](std::string& const lhs, std::string& const rhs) {
			return lhs.size() < rhs.size();
			}
		)->size();
	}

	// Write matrix to os
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			// Pad extra spaces to ensure correct column spacing
			while (stringMatrix[i][j].size() < column_widths[j])
			{
				stringMatrix[i][j] += " ";
			}

			os << stringMatrix[i][j] << " ";
		}
		if (i < (nrows - 1))
			os << "\n";
	}

	return os;
}

// Wostream << operator. Allows matrices to be printed to the console using eg wcout
sml_export template<class T, size_t nrows, size_t ncols>
inline std::wostream& operator << (std::wostream& os, const Matrix<T, nrows, ncols> m) {
	// For each column, find the longest element

	std::vector<size_t> column_widths(ncols);
	Matrix<std::wstring, nrows, ncols> stringMatrix;

	// Find the longest element in each column to determine their widths
	for (int i = 0; i < ncols; i++) {
		std::vector<std::wstring> stringVector(0);
		// Convert Ts to std::strings
		for (int j = 0; j < nrows; j++) {
			stringMatrix[j][i] = std::to_wstring(m[j][i]);
			// Trim trailing zeroes / decimal points from floating point types
			if (std::is_floating_point_v<T>) {
				stringMatrix[j][i].erase(stringMatrix[j][i].find_last_not_of('0') + 1, std::wstring::npos);
				stringMatrix[j][i].erase(stringMatrix[j][i].find_last_not_of('.') + 1, std::wstring::npos);
			}
			stringVector.push_back(stringMatrix[j][i]);
		}
		column_widths[i] = std::max_element(stringVector.begin(), stringVector.end(), [](std::wstring& const lhs, std::wstring& const rhs) {
			return lhs.size() < rhs.size();
			}
		)->size();
	}

	// Write matrix to os
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			// Pad extra spaces to ensure correct column spacing
			while (stringMatrix[i][j].size() < column_widths[j])
			{
				stringMatrix[i][j] += L" ";
			}

			os << stringMatrix[i][j] << L" ";
		}
		if (i < (nrows - 1))
			os << L"\n";
	}

	return os;
}

// Matrix addition
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator += (const Matrix<T2, nrows, ncols>& m2) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] += static_cast<T>(m2.at(i,j));
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator + (Matrix<T, nrows, ncols> m1, const Matrix<T2, nrows, ncols>& m2) {
	m1 += m2;
	return m1;
}

// Scalar addition
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator += (const T2& t) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] += static_cast<T>(t);
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator + (Matrix<T, nrows, ncols> m1, const T2& t) {
	m1 += t;
	return m1;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator + (const T2& t, Matrix<T, nrows, ncols> m1) {
	m1 += t;
	return m1;
}

// Matrix subtraction
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator -= (const Matrix<T2, nrows, ncols>& m2) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] -= static_cast<T>(m2.at(i, j));
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator - (Matrix<T, nrows, ncols> m1, const Matrix<T2, nrows, ncols>& m2) {
	m1 -= m2;
	return m1;
}

// Scalar subtraction
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator -= (const T2& t) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] -= static_cast<T>(t);
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator - (Matrix<T, nrows, ncols> m1, const T2& t) {
	m1 -= t;
	return m1;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator - (const T2& t, Matrix<T, nrows, ncols> m1) {
	m1 -= t;
	return m1;
}

// Matrix product
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2, size_t ncols2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator *= (const Matrix<T2, ncols, ncols2>& m2) {
	return (*this) * m2;
}
sml_export template<arithmetic T, size_t outside_rows, size_t inside_dim, size_t outside_cols, arithmetic T2>
inline Matrix<T, outside_rows, outside_cols>& operator * (const Matrix<T, outside_rows, inside_dim>& m1, const Matrix<T2, inside_dim, outside_cols>& m2) {
	Matrix<T, outside_rows, outside_cols> ret(0);
	for (int i = 0; i < outside_rows; i++) {
		for (int j = 0; j < outside_cols; j++) {
			for (int k = 0; k < inside_dim; k++) {
				ret[i][j] += (m1[i][k] * static_cast<T>(m2[k][j]));
			}
		}
	}
	return ret;
}

// Unrolled for small, common combinations
sml_export template<class T>
inline Matrix<T, 2, 2> operator * (const Matrix<T, 2, 2>& m1, const Matrix<T, 2, 2>& m2) {
	return Matrix<T, 2, 2>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(2), m1.at(0) * m2.at(1) + m1.at(1) * m2.at(3),
		m1.at(2) * m2.at(0) + m1.at(3) * m2.at(2), m1.at(2) * m2.at(1) + m1.at(3) * m2.at(3) });
}
sml_export template<class T>
inline Matrix<T, 3, 3> operator * (const Matrix<T, 3, 3>& m1, const Matrix<T, 3, 3>& m2) {
	return Matrix<T, 3, 3>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(3) + m1.at(2) * m2.at(6), m1.at(0) * m2.at(1) + m1.at(1) * m2.at(4) + m1.at(2) * m2.at(7), m1.at(0) * m2.at(2) + m1.at(1) * m2.at(5) + m1.at(2) * m2.at(8),
		m1.at(3) * m2.at(0) + m1.at(4) * m2.at(3) + m1.at(5) * m2.at(6), m1.at(3) * m2.at(1) + m1.at(4) * m2.at(4) + m1.at(5) * m2.at(7), m1.at(3) * m2.at(2) + m1.at(4) * m2.at(5) + m1.at(5) * m2.at(8),
		m1.at(6) * m2.at(0) + m1.at(7) * m2.at(3) + m1.at(8) * m2.at(6), m1.at(6) * m2.at(1) + m1.at(7) * m2.at(4) + m1.at(8) * m2.at(7), m1.at(6) * m2.at(2) + m1.at(7) * m2.at(5) + m1.at(8) * m2.at(8) });
}
sml_export template<class T>
inline Matrix<T, 4, 4> operator * (const Matrix<T, 4, 4>& m1, const Matrix<T, 4, 4>& m2) {
	return Matrix<T, 4, 4>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(4) + m1.at(2) * m2.at(8) + m1.at(3) * m2.at(12), m1.at(0) * m2.at(1) + m1.at(1) * m2.at(5) + m1.at(2) * m2.at(9) + m1.at(3) * m2.at(13), m1.at(0) * m2.at(2) + m1.at(1) * m2.at(6) + m1.at(2) * m2.at(10) + m1.at(3) * m2.at(14), m1.at(0) * m2.at(3) + m1.at(1) * m2.at(7) + m1.at(2) * m2.at(11) + m1.at(3) * m2.at(15),
		m1.at(4) * m2.at(0) + m1.at(5) * m2.at(4) + m1.at(6) * m2.at(8) + m1.at(7) * m2.at(12), m1.at(4) * m2.at(1) + m1.at(5) * m2.at(5) + m1.at(6) * m2.at(9) + m1.at(7) * m2.at(13), m1.at(4) * m2.at(2) + m1.at(5) * m2.at(6) + m1.at(6) * m2.at(10) + m1.at(7) * m2.at(14), m1.at(4) * m2.at(3) + m1.at(5) * m2.at(7) + m1.at(6) * m2.at(11) + m1.at(7) * m2.at(15),
		m1.at(8) * m2.at(0) + m1.at(9) * m2.at(4) + m1.at(10) * m2.at(8) + m1.at(11) * m2.at(12), m1.at(8) * m2.at(1) + m1.at(9) * m2.at(5) + m1.at(10) * m2.at(9) + m1.at(11) * m2.at(13), m1.at(8) * m2.at(2) + m1.at(9) * m2.at(6) + m1.at(10) * m2.at(10) + m1.at(11) * m2.at(14), m1.at(8) * m2.at(3) + m1.at(9) * m2.at(7) + m1.at(10) * m2.at(11) + m1.at(11) * m2.at(15),
		m1.at(12) * m2.at(0) + m1.at(13) * m2.at(4) + m1.at(14) * m2.at(8) + m1.at(15) * m2.at(12), m1.at(12) * m2.at(1) + m1.at(13) * m2.at(5) + m1.at(14) * m2.at(9) + m1.at(15) * m2.at(13), m1.at(12) * m2.at(2) + m1.at(13) * m2.at(6) + m1.at(14) * m2.at(10) + m1.at(15) * m2.at(14), m1.at(12) * m2.at(3) + m1.at(13) * m2.at(7) + m1.at(14) * m2.at(11) + m1.at(15) * m2.at(15) });
}

// Matrix * Vector / Column Matrix = Column Matrix

sml_export template<arithmetic T, size_t dim, arithmetic T2>
inline Matrix<T, dim, 1>& operator * (const Matrix<T, dim, dim>& m, const Vector<T2, dim>& v) {
	Matrix<T, dim, 1> ret(0);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			ret[i][0] += (m[i][j] * static_cast<T>(v[j]));
		}
	}
	return ret;
}

//Unrolled for small, common combinations

// 2x2 Matrix * 2x1 Column Matrix = 2x1 Matrix
sml_export template<class T>
inline Matrix<T, 2, 1> operator * (const Matrix<T, 2, 2>& m1, const Matrix<T, 2, 1>& m2) {
	return Matrix<T, 2, 1>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(1),
		m1.at(2) * m2.at(0) + m1.at(3) * m2.at(1) });
}
// 2x2 Matrix * 2-Vector = 2x1 Matrix
sml_export template<class T>
inline Matrix<T, 2, 1> operator * (const Matrix<T, 2, 2>& m1, const Vector<T, 2>& v) {
	return Matrix<T, 2, 1>({
		m1.at(0) * v.at(0) + m1.at(1) * v.at(1),
		m1.at(2) * v.at(0) + m1.at(3) * v.at(1) });
}
// 3x3 Matrix * 3x1 Column Matrix = 3x1 Matrix
sml_export template<class T>
inline Matrix<T, 3, 1> operator * (const Matrix<T, 3, 3>& m1, const Matrix<T, 3, 1>& m2) {
	return Matrix<T, 3, 1>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(1) + m1.at(2) * m2.at(2),
		m1.at(3) * m2.at(0) + m1.at(4) * m2.at(1) + m1.at(5) * m2.at(2),
		m1.at(6) * m2.at(0) + m1.at(7) * m2.at(1) + m1.at(8) * m2.at(2) });
}
// 3x3 Matrix * 3-Vector = 3x1 Matrix
sml_export template<class T>
inline Matrix<T, 3, 1> operator * (const Matrix<T, 3, 3>& m, const Vector<T, 3>& v) {
	return Matrix<T, 3, 1>({ 
		m.at(0) * v.at(0) + m.at(1) * v.at(1) + m.at(2) * v.at(2),
		m.at(3) * v.at(0) + m.at(4) * v.at(1) + m.at(5) * v.at(2),
		m.at(6) * v.at(0) + m.at(7) * v.at(1) + m.at(8) * v.at(2) });
}
// 4x4 Matrix * 4x1 Column Matrix = 4x1 Matrix
sml_export template<class T>
inline Matrix<T, 4, 1> operator * (const Matrix<T, 4, 4>& m1, const Matrix<T, 4, 1>& m2) {
	return Matrix<T, 4, 1>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(1) + m1.at(2) * m2.at(2) + m1.at(3) * m2.at(3),
		m1.at(4) * m2.at(0) + m1.at(5) * m2.at(1) + m1.at(6) * m2.at(2) + m1.at(7) * m2.at(3),
		m1.at(8) * m2.at(0) + m1.at(9) * m2.at(1) + m1.at(10) * m2.at(2) + m1.at(11) * m2.at(3),
		m1.at(12) * m2.at(0) + m1.at(13) * m2.at(1) + m1.at(14) * m2.at(2) + m1.at(15) * m2.at(3) });
}
// 4x4 Matrix * 4-Vector = 4x1 Matrix
sml_export template<class T>
inline Matrix<T, 4, 1> operator * (const Matrix<T, 4, 4>& m1, const Vector<T, 4>& v) {
	return Matrix<T, 4, 1>({
		m1.at(0) * v.at(0) + m1.at(1) * v.at(1) + m1.at(2) * v.at(2) + m1.at(3) * v.at(3),
		m1.at(4) * v.at(0) + m1.at(5) * v.at(1) + m1.at(6) * v.at(2) + m1.at(7) * v.at(3),
		m1.at(8) * v.at(0) + m1.at(9) * v.at(1) + m1.at(10) * v.at(2) + m1.at(11) * v.at(3),
		m1.at(12) * v.at(0) + m1.at(13) * v.at(1) + m1.at(14) * v.at(2) + m1.at(15) * v.at(3) });
}

// Vector / Row Matrix * Matrix = Row Matrix

sml_export template<arithmetic T, size_t dim, arithmetic T2>
inline Matrix<T, 1, dim>& operator * (const Vector<T2, dim>& v, const Matrix<T, dim, dim>& m) {
	Matrix<T, 1, dim> ret(0);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			ret[0][i] += (m[j][i] * static_cast<T>(v[j]));
		}
	}
	return ret;
}

// 1x2 Row Matrix * 2x2 Matrix = 1x2 Matrix
sml_export template<class T>
inline Matrix<T, 1, 2> operator * (const Matrix<T, 1, 2>& m1, const Matrix<T, 2, 2>& m2) {
	return Matrix<T, 1, 2>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(2), m1.at(0) * m2.at(1) + m1.at(1) * m2.at(3) });
}
// 2-Vector * 2x2 Matrix = 1x2 Matrix
sml_export template<class T>
inline Matrix<T, 1, 2> operator * (const Vector<T, 2>& v, const Matrix<T, 2, 2>& m) {
	return Matrix<T, 1, 2>({
		v.at(0) * m.at(0) + v.at(1) * m.at(2), v.at(0) * m.at(1) + v.at(1) * m.at(3) });
}
// 1x3 Row Matrix * 3x3 Matrix = 1x3 Matrix
sml_export template<class T>
inline Matrix<T, 1, 3> operator * (const Matrix<T, 1, 3>& m1, const Matrix<T, 3, 3>& m2) {
	return Matrix<T, 1, 3>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(3) + m1.at(2) * m2.at(6), m1.at(0) * m2.at(1) + m1.at(1) * m2.at(4) + m1.at(2) * m2.at(7), m1.at(0) * m2.at(2) + m1.at(1) * m2.at(5) + m1.at(2) * m2.at(8) });
}
// 3-Vector * 3x3 Matrix = 1x3 Matrix
sml_export template<class T>
inline Matrix<T, 1, 3> operator * (const Vector<T, 3>& v, const Matrix<T, 3, 3>& m) {
	return Matrix<T, 1, 3>({ 
		v.at(0) * m.at(0) + v.at(1) * m.at(3) + v.at(2) * m.at(6),
		v.at(0) * m.at(1) + v.at(1) * m.at(4) + v.at(2) * m.at(7),
		v.at(0) * m.at(2) + v.at(1) * m.at(5) + v.at(2) * m.at(8) });
}
// 1x4 Row Matrix * 4x4 Matrix = 1x4 Matrix
sml_export template<class T>
inline Matrix<T, 1, 4> operator * (const Matrix<T, 1, 4>& m1, const Matrix<T, 4, 4>& m2) {
	return Matrix<T, 1, 4>({
		m1.at(0) * m2.at(0) + m1.at(1) * m2.at(4) + m1.at(2) * m2.at(8) + m1.at(3) * m2.at(12), m1.at(0) * m2.at(1) + m1.at(1) * m2.at(5) + m1.at(2) * m2.at(9) + m1.at(3) * m2.at(13), m1.at(0) * m2.at(2) + m1.at(1) * m2.at(6) + m1.at(2) * m2.at(10) + m1.at(3) * m2.at(14), m1.at(0) * m2.at(3) + m1.at(1) * m2.at(7) + m1.at(2) * m2.at(11) + m1.at(3) * m2.at(15) });
}
// 4-Vector * 4x4 Matrix = 1x4 Matrix
sml_export template<class T>
inline Matrix<T, 1, 4> operator * (const Vector<T, 4>& v, const Matrix<T, 4, 4>& m) {
	return Matrix<T, 1, 4>({
		v.at(0) * m.at(0) + v.at(1) * m.at(4) + v.at(2) * m.at(8) + v.at(3) * m.at(12), v.at(0) * m.at(1) + v.at(1) * m.at(5) + v.at(2) * m.at(9) + v.at(3) * m.at(13), v.at(0) * m.at(2) + v.at(1) * m.at(6) + v.at(2) * m.at(10) + v.at(3) * m.at(14), v.at(0) * m.at(3) + v.at(1) * m.at(7) + v.at(2) * m.at(11) + v.at(3) * m.at(15) });
}

// Outer Product - Returns the Matrix multiplication of two Vectors, with the first treated as a 1xm Matrix and the second as an mx1 Matrix.
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
Matrix<T, nrows, ncols> outer_product(const Vector<T, nrows>& v1, const Vector<T2, ncols>& v2) {
	Matrix<T, nrows, ncols> ret(0);
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			ret[i][j] = v1[i] * static_cast<T>(v2[j]);
		}
	}
	return ret;
}

// Outer Product - Returns the Matrix multiplication of a row Matrix and a column Matrix.
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
Matrix<T, nrows, ncols> outer_product(const Matrix<T, nrows, 1>& m1, const Matrix<T2, 1, ncols>& m2) {
	return m1 * m2;
}

// Scalar multiplication
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator *= (const T2& t) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] *= static_cast<T>(t);
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator * (Matrix<T, nrows, ncols> m1, const T2& t) {
	m1 *= t;
	return m1;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator * (const T2& t, Matrix<T, nrows, ncols> m1) {
	m1 *= t;
	return m1;
}

// Scalar division
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator /= (const T2& t) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] /= static_cast<T>(t);
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator / (Matrix<T, nrows, ncols> m1, const T2& t) {
	m1 /= t;
	return m1;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator / (const T2& t, Matrix<T, nrows, ncols> m1) {
	m1 /= t;
	return m1;
}

// Matrix modulus - requires integer operands
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator %= (const Matrix<T2, nrows, ncols>& m2) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] %= static_cast<T>(m2.at(i, j));
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator % (Matrix<T, nrows, ncols> m1, const Matrix<T2, nrows, ncols>& m2) {
	m1 %= m2;
	return m1;
}

// Scalar modulus - requires integer operands
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::operator %= (const T2& t) {
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			data[(i * ncols) + j] %= static_cast<T>(t);
		}
	}
	return *this;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator % (Matrix<T, nrows, ncols> m1, const T2& t) {
	m1 %= t;
	return m1;
}
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
inline Matrix<T, nrows, ncols>& operator % (const T2& t, Matrix<T, nrows, ncols> m1) {
	m1 %= t;
	return m1;
}

// Add a row to each row of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::add_row(const Matrix<T2, 1, ncols>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] += static_cast<T>(m2.at(0, j));
		}
	}
	return *this;
}
// Subtract a row from each row of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::sub_row(const Matrix<T2, 1, ncols>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] -= static_cast<T>(m2.at(0, j));
		}
	}
	return *this;
}
// Multiply each row of a Matrix with a row
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::mul_row(const Matrix<T2, 1, ncols>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] *= static_cast<T>(m2.at(0, j));
		}
	}
	return *this;
}
// Divide a each row of a Matrix by a row
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::div_row(const Matrix<T2, 1, ncols>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] /= static_cast<T>(m2.at(0, j));
		}
	}
	return *this;
}

// Add a column to each column of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::add_col(const Matrix<T2, nrows, 1>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] += static_cast<T>(m2.at(i, 0));
		}
	}
	return *this;
}
// Subtract a column from each column of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::sub_col(const Matrix<T2, nrows, 1>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] -= static_cast<T>(m2.at(i, 0));
		}
	}
	return *this;
}
// Multiply each column of a Matrix with a column
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::mul_col(const Matrix<T2, nrows, 1>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] *= static_cast<T>(m2.at(i, 0));
		}
	}
	return *this;
}
// Divide a each column of a Matrix by a column
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::div_col(const Matrix<T2, nrows, 1>& m2) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] /= static_cast<T>(m2.at(i, 0));
		}
	}
	return *this;
}

// Add a row to each row of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::add_row(const Vector<T2, ncols>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] += static_cast<T>(v.at(j));
		}
	}
	return *this;
}
// Subtract a row from each row of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::sub_row(const Vector<T2, ncols>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] -= static_cast<T>(v.at(j));
		}
	}
	return *this;
}
// Multiply each row of a Matrix with a row
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::mul_row(const Vector<T2, ncols>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] *= static_cast<T>(v.at(j));
		}
	}
	return *this;
}
// Divide a each row of a Matrix by a row
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::div_row(const Vector<T2, ncols>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] /= static_cast<T>(v.at(j));
		}
	}
	return *this;
}

// Add a column to each column of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::add_col(const Vector<T2, nrows>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] += static_cast<T>(v.at(i));
		}
	}
	return *this;
}
// Subtract a column from each column of a Matrix
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::sub_col(const Vector<T2, nrows>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] -= static_cast<T>(v.at(i));
		}
	}
	return *this;
}
// Multiply each column of a Matrix with a column
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::mul_col(const Vector<T2, nrows>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] *= static_cast<T>(v.at(i));
		}
	}
	return *this;
}
// Divide a each column of a Matrix by a column
sml_export template<arithmetic T, size_t nrows, size_t ncols>
template<arithmetic T2>
inline Matrix<T, nrows, ncols>& Matrix<T, nrows, ncols>::div_col(const Vector<T2, nrows>& v) {
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			(*this)[i][j] /= static_cast<T>(v.at(i));
		}
	}
	return *this;
}

// Transpose matrices
sml_export template<arithmetic T, size_t nrows, size_t ncols>
Matrix<T, nrows, ncols> transpose(const Matrix<T, nrows, ncols>& original) {
	Matrix<T, ncols, nrows> temp;
	for (size_t i = 0; i < nrows; i++) {
		for (size_t j = 0; j < ncols; j++) {
			temp[j][i] = original[i][j];
		}
	}
	return temp;
}

// Dot product 
sml_export template<arithmetic T, size_t elements>
double dot(const Matrix<T, 1, elements>& m1, const Matrix<T, elements, 1>& m2) {
	double ret = 0;
	for (size_t i = 0; i < elements; i++) {
		ret += (m1.at(0, i) * m2.at(i, 0));
	}
	return ret;
}
sml_export template<arithmetic T, size_t elements>
double dot(const Matrix<T, elements, 1>& m1, const Matrix<T, 1, elements>& m2) {
	double ret = 0;
	for (size_t i = 0; i < elements; i++) {
		ret += (m1.at(i, 0) * m2.at(0, i));
	}
	return ret;
}
sml_export template<arithmetic T, size_t elements>
double dot(const Matrix<T, elements, 1>& m1, const Matrix<T, elements, 1>& m2) {
	double ret = 0;
	for (size_t i = 0; i < elements; i++) {
		ret += (m1.at(i, 0) * m2.at(i, 0));
	}
	return ret;
}
sml_export template<arithmetic T, size_t elements>
double dot(const Matrix<T, 1, elements>& m1, const Matrix<T, 1, elements>& m2) {
	double ret = 0;
	for (size_t i = 0; i < elements; i++) {
		ret += (m1.at(0, i) * m2.at(0, i));
	}
	return ret;
}

// Element-wise matrix multiplication (Hadamard product)
sml_export template<arithmetic T, size_t nrows, size_t ncols, arithmetic T2>
Matrix<T, nrows, ncols> multiply_elements(Matrix<T, nrows, ncols> m1, const Matrix<T2, nrows, ncols>& m2) {
	std::transform(m1.begin(), m1.end(), m2.begin(), m1.begin(), [](T i, T2 j) -> T {return i * static_cast<T>(j); });
	return m1;
}

// Calculate the trace of a matrix
sml_export template<arithmetic T, size_t dim>
T trace(const Matrix<T, dim, dim>& m) {
	T ret = 0;
	for (size_t i = 0; i < dim; i++) {
		ret += m.at(i, i);
	}
	return ret;
}

sml_export template<arithmetic T, size_t dim>
std::tuple<Matrix<float, dim, dim>, Vector<size_t, dim + 1>> LUPDecomposition(const Matrix<T, dim, dim>& m) {

	Matrix<float, dim, dim> A;
	for (int i = 0; i < m.size(); i++) {
		A.at(i) = static_cast<float>(m.at(i));
	}

	size_t numberOfPivots = 0;
	Vector<size_t, dim + 1> pivot_matrix(0);

	int maxI = 0, j = 0, k = 0, i = 0;
	float maxA, absA;
	float ptr;

	for (i = 0; i <= dim; i++) {
		pivot_matrix.at(i) = i;
	}

	for (i = 0; i < dim; i++) {
		maxA = 0.0;
		maxI = i;

		for (k = i; k < dim; k++) {
			if ((absA = fabs(A[k][i])) > maxA) {
				maxA = absA;
				maxI = k;
			}
		}

		if (maxI != i) {
			// Pivoting P
			j = static_cast<int>(pivot_matrix.at(i));
			pivot_matrix.at(i) = pivot_matrix.at(maxI);
			pivot_matrix.at(maxI) = j;

			// Pivot rows of A
			for (size_t r = 0; r < dim; r++) {
				ptr = A[i][r];
				A[i][r] = A[maxI][r];
				A[maxI][r] = ptr;
			}

			// We store the number of pivots in the final element of the unit permutation vector
			pivot_matrix.at(dim)++;
		}

		for (j = i + 1; j < dim; j++) {
			A[j][i] /= A[i][i];
			for (k = i + 1; k < dim; k++) {
				A[j][k] -= A[j][i] * A[i][k];
			}
		}
	}

	return std::make_tuple(A, pivot_matrix);
}

sml_export template<arithmetic T, size_t dim>
float det(const Matrix<T, dim, dim>& m) {

	// Returns the determinant of matrix m from its LUP decomposition

	Matrix<T, dim, dim> A;
	Vector<size_t, dim + 1> P;
	size_t numberOfPermutations = 0;

	std::tie(A, P) = LUPDecomposition(m);

	float det = static_cast<float>(A[0][0]);
	for (int i = 1; i < dim; i++) {
		det *= static_cast<float>(A[i][i]);
	}

	return (P.at(dim) - dim) % 2 == 0 ? det : -det;
}

sml_export template<arithmetic T>
double det(const Matrix<T, 2, 2>& m) {
	return static_cast<double>((m[0][0] * m[1][1]) - (m[0][1] * m[1][0]));
}

sml_export template<arithmetic T>
double det(const Matrix<T, 3, 3>& m) {
	return static_cast<double>(m[0][0] * ((m[1][1] * m[2][2]) - (m[1][2] * m[2][1]))
		- m[0][1] * ((m[1][0] * m[2][2]) - (m[1][2] * m[2][0]))
		+ m[0][2] * ((m[1][0] * m[2][1]) - (m[1][1] * m[2][0])));
}

sml_export template<arithmetic T, size_t dim>
Matrix<T, dim, dim> identity() {
	Matrix<T, dim, dim> ret(0);
	for (size_t i = 0; i < dim; i++) {
		ret[i][i] = 1;
	}
	return ret;
}

sml_export template<arithmetic T, size_t nrows, size_t ncols>
Matrix<T, nrows, ncols> exchange_columns(Matrix<T, nrows, ncols>& m, const size_t& colA, const size_t& colB) {
	for (size_t i = 0; i < nrows; i++) {
		std::swap(m.at(i, colA), m.at(i, colB));
	}
	return m;
}

sml_export template<arithmetic T, size_t nrows, size_t ncols>
Matrix<T, nrows, ncols> exchange_rows(Matrix<T, nrows, ncols>& m, const size_t& colA, const size_t& colB) {
	for (size_t i = 0; i < ncols; i++) {
		std::swap(m.at(colA, i), m.at(colB, i));
	}
	return m;
}

sml_export template<arithmetic T, size_t dim>
Matrix<T, dim, dim> inverse(const Matrix<T, dim, dim>& m) {

	Matrix<T, dim, dim> I = identity<T, dim>();
	Matrix<T, dim, dim> M = m;

	for (size_t i = 0; i < dim; i++) {
		if (M.at(i, i) == 0 ) {
			// Swap with a row below it
			for (size_t j = i; j < dim; j++) {
				if (M.at(j, j) != 0) {
					exchange_columns(M, i, j);
					exchange_columns(I, i, j);
				}
			}
			if (M.at(i, i) == 0)
				return Matrix<T, dim, dim>(0);
		}

		// Divide row by value
		T temp = 1.0/M.at(i, i);
		for (size_t j = 0; j < dim; j++) {
			M.at(i, j) *= temp;
			I.at(i, j) *= temp;
		}

		// Annihilate entries above and below the pivot
		for (size_t j = 0; j < dim; j++) {
			if (j == i)
				continue;
			// Subtract the current row multiplied by the element being annihilated
			T factor = M.at(j, i) / M.at(i,i);
			for (size_t k = 0; k < dim; k++) {
				M.at(j, k) -= (factor * M.at(i, k));
				I.at(j, k) -= (factor * I.at(i, k));
			}
		}
	}

	return I;
}

//Take the absolute value of each component
using std::abs;
sml_export template<arithmetic T, size_t nrows, size_t ncols >
Matrix<T, nrows, ncols> abs(Matrix<T, nrows, ncols>& m) {
	Matrix<T, nrows, ncols> ret = m;
	for (T& i : ret) {
		i = std::abs(i);
	}
	return ret;
}

sml_export template<arithmetic T, size_t rows, size_t cols, arithmetic T2, arithmetic T3>
Matrix<T, rows, cols> lerp(const Matrix<T, rows, cols>& m1, const Matrix<T2, rows, cols>& m2, const T3 t) {
	return (m1 + ((m2 - m1) * static_cast<T>(t)));
}

// Return the index of the largest element
sml_export template<arithmetic T, size_t nrows, size_t ncols>
size_t max_element(const Matrix<T, nrows, ncols>& m) {
	return std::distance(m.begin(), std::max_element(m.begin(), m.end()));
}

// Return the index of the smallest element
sml_export template<arithmetic T, size_t nrows, size_t ncols>
size_t min_element(const Matrix<T, nrows, ncols>& m) {
	return std::distance(m.begin(), std::min_element(m.begin(), m.end()));
}

// Return the largest element
sml_export template<arithmetic T, size_t nrows, size_t ncols>
T max(const Matrix<T, nrows, ncols>& m) {
	return *std::max_element(m.begin(), m.end());
}

// Return the smallest element
sml_export template<arithmetic T, size_t nrows, size_t ncols>
T min(const Matrix<T, nrows, ncols>& m) {
	return *std::min_element(m.begin(), m.end());
}

// Clamp
sml_export template<arithmetic T, size_t nrows, size_t ncols>
Matrix<T, nrows, ncols> clamp(const Matrix<T, nrows, ncols>& m, const T& t1, const T& t2) {
	Matrix<T, nrows, ncols> ret = m; std::transform(ret.begin(), ret.end(), ret.begin(), [t1, t2](T i) -> T {return std::clamp(i, t1, t2); });
	return ret;
}

// Create a rotation matrix for a rotation about the X-axis
sml_export inline Matrix<float, 4, 4> RotateX(const float radians) {
	return Matrix<float, 4, 4>( 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(radians), -sin(radians), 0.0f,
		0.0f, sin(radians), cos(radians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}
// Create a rotation matrix for a rotation about the Y-axis
sml_export inline Matrix<float, 4, 4> RotateY(const float radians) {
	return Matrix<float, 4, 4>(   cos(radians), 0.0f, sin(radians), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(radians), 0.0f, cos(radians), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}
// Create a rotation matrix for a rotation about the Z-axis
sml_export inline Matrix<float, 4, 4> RotateZ(const float radians) {
	return Matrix<float, 4, 4>(   cos(radians), -sin(radians), 0.0f, 0.0f,
		sin(radians), cos(radians), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f );
}

// Return the square matrix to the top left that is 1 smaller in each dimension eg a 3x3 from a 4x4
sml_export template<arithmetic T, size_t dim>
	requires (dim >= 2)
sml::Matrix<T, dim - 1, dim - 1> top_left(const sml::Matrix<T, dim, dim>& m) {
	sml::Matrix<T, dim - 1, dim - 1> ret(0);
	for (int i = 0; i < (dim - 1); i++) {
		for (int j = 0; j < (dim - 1); j++) {
			ret.at(i, j) = m.at(i, j);
		}
	}
	return ret;
}
sml_export template<arithmetic T, size_t dim>
	requires (dim <= 1)
T top_left(const sml::Matrix<T, dim, dim>& m) {
	return m.at(0, 0);
}

// Return the square matrix to the top right that is 1 smaller in each dimension eg a 3x3 from a 4x4
sml_export template<arithmetic T, size_t dim>
	requires (dim >= 2)
sml::Matrix<T, dim - 1, dim - 1> top_right(const sml::Matrix<T, dim, dim>& m) {
	sml::Matrix<T, dim - 1, dim - 1> ret(0);
	for (int i = 0; i < (dim - 1); i++) {
		for (int j = 1; j < dim; j++) {
			ret.at(i, j - 1) = m.at(i, j);
		}
	}
	return ret;
}
sml_export template<arithmetic T, size_t dim>
	requires (dim <= 1)
T top_right(const sml::Matrix<T, dim, dim>& m) {
	return m.at(0, 1);
}

// Return the square matrix to the bottom left that is 1 smaller in each dimension eg a 3x3 from a 4x4
sml_export template<arithmetic T, size_t dim>
	requires (dim >= 2)
sml::Matrix<T, dim - 1, dim - 1> bottom_left(const sml::Matrix<T, dim, dim>& m) {
	sml::Matrix<T, dim - 1, dim - 1> ret(0);
	for (int i = 1; i < dim; i++) {
		for (int j = 0; j < dim - 1; j++) {
			ret.at(i - 1, j) = m.at(i, j);
		}
	}
	return ret;
}

sml_export template<arithmetic T, size_t dim>
	requires (dim <= 1)
T bottom_left(const sml::Matrix<T, dim, dim>& m) {
	return m.at(1, 0);
}

// Return the square matrix to the bottom right that is 1 smaller in each dimension eg a 3x3 from a 4x4
sml_export template<arithmetic T, size_t dim>
	requires (dim >= 2)
sml::Matrix<T, dim - 1, dim - 1> bottom_right(const sml::Matrix<T, dim, dim>& m) {
	sml::Matrix<T, dim - 1, dim - 1> ret(0);
	for (int i = 1; i < dim; i++) {
		for (int j = 1; j < dim; j++) {
			ret.at(i - 1, j - 1) = m.at(i, j);
		}
	}
	return ret;
}
sml_export template<arithmetic T, size_t dim>
	requires (dim <= 1)
T bottom_right(const sml::Matrix<T, dim, dim>& m) {
	return m.at(1, 1);
}

// Efficiently calculate the inverse transpose of a Mat44f - useful for shaders
sml_export sml::Matrix<float, 4, 4> inverse_transpose(const sml::Matrix<float, 4, 4> m) {
	float subFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float subFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float subFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float subFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float subFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float subFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	float subFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	float subFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	float subFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	float subFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	float subFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	float subFactor11 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	float subFactor12 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	float subFactor13 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	float subFactor14 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	float subFactor15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	float subFactor16 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	float subFactor17 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	sml::Matrix<float, 4, 4> inverse;
	inverse[0][0] = +(m[1][1] * subFactor00 - m[1][2] * subFactor01 + m[1][3] * subFactor02);
	inverse[0][1] = -(m[1][0] * subFactor00 - m[1][2] * subFactor03 + m[1][3] * subFactor04);
	inverse[0][2] = +(m[1][0] * subFactor01 - m[1][1] * subFactor03 + m[1][3] * subFactor05);
	inverse[0][3] = -(m[1][0] * subFactor02 - m[1][1] * subFactor04 + m[1][2] * subFactor05);

	inverse[1][0] = -(m[0][1] * subFactor00 - m[0][2] * subFactor01 + m[0][3] * subFactor02);
	inverse[1][1] = +(m[0][0] * subFactor00 - m[0][2] * subFactor03 + m[0][3] * subFactor04);
	inverse[1][2] = -(m[0][0] * subFactor01 - m[0][1] * subFactor03 + m[0][3] * subFactor05);
	inverse[1][3] = +(m[0][0] * subFactor02 - m[0][1] * subFactor04 + m[0][2] * subFactor05);

	inverse[2][0] = +(m[0][1] * subFactor06 - m[0][2] * subFactor07 + m[0][3] * subFactor08);
	inverse[2][1] = -(m[0][0] * subFactor06 - m[0][2] * subFactor09 + m[0][3] * subFactor10);
	inverse[2][2] = +(m[0][0] * subFactor07 - m[0][1] * subFactor09 + m[0][3] * subFactor11);
	inverse[2][3] = -(m[0][0] * subFactor08 - m[0][1] * subFactor10 + m[0][2] * subFactor11);

	inverse[3][0] = -(m[0][1] * subFactor12 - m[0][2] * subFactor13 + m[0][3] * subFactor14);
	inverse[3][1] = +(m[0][0] * subFactor12 - m[0][2] * subFactor15 + m[0][3] * subFactor16);
	inverse[3][2] = -(m[0][0] * subFactor13 - m[0][1] * subFactor15 + m[0][3] * subFactor17);
	inverse[3][3] = +(m[0][0] * subFactor14 - m[0][1] * subFactor16 + m[0][2] * subFactor17);

	float determinant =
		+m[0][0] * inverse[0][0]
		+ m[0][1] * inverse[0][1]
		+ m[0][2] * inverse[0][2]
		+ m[0][3] * inverse[0][3];

	inverse /= determinant;
	return inverse;
}
	
// Shorthand definitions
sml_export using Mat22d = Matrix<double, 2, 2>;
sml_export using Mat23d = Matrix<double, 2, 3>;
sml_export using Mat24d = Matrix<double, 2, 4>;
sml_export using Mat32d = Matrix<double, 3, 2>;
sml_export using Mat33d = Matrix<double, 3, 3>;
sml_export using Mat34d = Matrix<double, 3, 4>;
sml_export using Mat42d = Matrix<double, 4, 2>;
sml_export using Mat43d = Matrix<double, 4, 3>;
sml_export using Mat44d = Matrix<double, 4, 4>;
sml_export using Mat22f = Matrix<float, 2, 2>;
sml_export using Mat23f = Matrix<float, 2, 3>;
sml_export using Mat24f = Matrix<float, 2, 4>;
sml_export using Mat32f = Matrix<float, 3, 2>;
sml_export using Mat33f = Matrix<float, 3, 3>;
sml_export using Mat34f = Matrix<float, 3, 4>;
sml_export using Mat42f = Matrix<float, 4, 2>;
sml_export using Mat43f = Matrix<float, 4, 3>;
sml_export using Mat44f = Matrix<float, 4, 4>;
sml_export using Mat22i = Matrix<int, 2, 2>;
sml_export using Mat23i = Matrix<int, 2, 3>;
sml_export using Mat24i = Matrix<int, 2, 4>;
sml_export using Mat32i = Matrix<int, 3, 2>;
sml_export using Mat33i = Matrix<int, 3, 3>;
sml_export using Mat34i = Matrix<int, 3, 4>;
sml_export using Mat42i = Matrix<int, 4, 2>;
sml_export using Mat43i = Matrix<int, 4, 3>;
sml_export using Mat44i = Matrix<int, 4, 4>;

}// !namespace sml
#endif // !SML_MATRIX_HPP
