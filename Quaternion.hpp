#ifndef SML_QUATERNION_HPP
#define SML_QUATERNION_HPP

namespace sml {


	sml_export template<arithmetic T>
	class Quaternion {
	public:
		Quaternion() {
			scalar = { static_cast<T>(0) };
			vector = { static_cast<T>(0) };
		}
		template<arithmetic T2>
		Quaternion(const Quaternion<T2>& q2) : scalar{ static_cast<T>(q2.scalar[0])}, vector{q2.vector} {};
		template<arithmetic T2>
		Quaternion(const T2 c) : scalar{ static_cast<T>(c) }, vector{ Vector<T, 3>(static_cast<T>(c),static_cast<T>(c),static_cast<T>(c)) } {};
		template<arithmetic T2, arithmetic T3>
		Quaternion(const T2 s, const Vector<T3, 3> v) : scalar{ static_cast<T>(s) }, vector{ v } {};
		template<arithmetic T2, arithmetic T3, arithmetic T4, arithmetic T5>
		Quaternion(const T2 s, const T3 v0, const T4 v1, const T5 v2) : scalar{ static_cast<T>(s) }, vector{ Vector<T, 3>(static_cast<T>(v0),static_cast<T>(v1),static_cast<T>(v2)) } {};
		template<arithmetic T2>
		Quaternion(const T2 arr[4]) {
			scalar[0] = { static_cast<T>(arr[0]) };
			vector[0] = static_cast<T>(arr[1]);
			vector[1] = static_cast<T>(arr[2]);
			vector[2] = static_cast<T>(arr[3]);
		}
		inline T operator[] (size_t i) const {
			switch (i)
			{
			case 0:
				return scalar[0];
				break;
			case 1:
				return vector[0];
				break;
			case 2:
				return vector[1];
				break;
			case 3:
				return vector[2];
				break;
			default:
				std::cerr << "Out of range error: " << i << "\n";
				break;
			}
		}
		inline T& operator[] (size_t i) {
			switch (i)
			{
			case 0:
				return scalar[0];
				break;
			case 1:
				return vector[0];
				break;
			case 2:
				return vector[1];
				break;
			case 3:
				return vector[2];
				break;
			default:
				std::cerr << "Out of range error: " << i << "\n";
				break;
			}
		}

		inline T& at(size_t i) {
			if (i == 0) {
				return scalar.at(0);
			}
			else {
				return vector.at(i - 1);
			}
		}
		inline T at(size_t i) const {
			if (i == 0) {
				return scalar.at(0);
			}
			else {
				return vector.at(i - 1);
			}
		}

		inline T& q0() { return scalar[0]; }
		inline T& q1() { return vector[0]; }
		inline T& q2() { return vector[1]; }
		inline T& q3() { return vector[2]; }
		inline T& s() { return scalar[0]; }
		inline T& i() { return vector[0]; }
		inline T& j() { return vector[1]; }
		inline T& k() { return vector[2]; }
		inline T q0() const { return scalar[0]; }
		inline T q1() const { return vector[0]; }
		inline T q2() const { return vector[1]; }
		inline T q3() const { return vector[2]; }
		inline T s() const { return scalar[0]; }
		inline T i() const { return vector[0]; }
		inline T j() const { return vector[1]; }
		inline T k() const { return vector[2]; }

		// Unary operators
		inline const Quaternion& operator + () const { return *this; }
		inline Quaternion operator - () const {
			Quaternion<T> ret = *this;
			ret.scalar[0] = -scalar[0];
			ret.vector = -vector;
			return ret;
		}

		// Comparison operators
		inline bool operator == (const Quaternion<T>& q2) const {
			return ((scalar.at(0) == q2.scalar.at(0)) && (vector == q2.vector));
		}
		template<arithmetic T2>
		inline bool operator == (const Quaternion<T2>& q2) const {
			return ((scalar.at(0) == q2.scalar.at(0)) && (vector == q2.vector));
		}

		// Assignment operator
		template<arithmetic T2>
		inline Quaternion<T>& operator = (const Quaternion<T2>& q2) {
			scalar[0] = static_cast<T>(q2.scalar[0]);
			vector = q2.vector;
			return *this;
		}

		template<arithmetic T2>
		inline Quaternion<T>& operator+= (const Quaternion<T2>& q2);

		template<arithmetic T2>
		inline Quaternion<T>& operator-= (const Quaternion<T2>& q2);

		template<arithmetic T2>
		inline Quaternion<T>& operator*= (const Quaternion<T2>& q2);

		template<arithmetic T2>
		inline Quaternion<T>& operator*= (const T2& t);

		template<arithmetic T2>
		inline Quaternion<T>& operator/= (const T2& t);
		template<arithmetic T2>
		inline Quaternion<T>& operator/= (const Quaternion<T2>& q2);

		std::array<T, 1> scalar;
		Vector<T, 3> vector;

	};

	// Write vector to ostream
	sml_export template<arithmetic T>
	inline std::ostream& operator << (std::ostream& os, const Quaternion<T>& t) {
		os << t[0] << " " << t[1] << " " << t[2] << " " << t[3];
		return os;
	}

	// Write vector to wostream
	sml_export template<arithmetic T>
	inline std::wostream& operator << (std::wostream& os, const Quaternion<T>& t) {
		os << t[0] << " " << t[1] << " " << t[2] << " " << t[3];
		return os;
	}

	sml_export template<arithmetic T>
	inline Quaternion<T> Conjugate(const Quaternion<T>& q) {
		return Quaternion<T>(q.q0(), -q.q1(), -q.q2(), -q.q3());
	}

	using std::abs;
	sml_export template<arithmetic T>
		inline Quaternion<T> abs(const Quaternion<T>& q) {
		return Quaternion<T>(abs(q.scalar), abs(q.vector));
	}

	sml_export template<arithmetic T>
		template<arithmetic T2>
		inline Quaternion<T>& Quaternion<T>::operator+= (const Quaternion<T2>& q2) {
			q0() += static_cast<T>(q2.q0());
			vector += q2.vector;
			return *this;
	}
	sml_export template<arithmetic T, arithmetic T2>
		inline Quaternion<T> operator+ (Quaternion<T> q1, const Quaternion<T2>& q2) {
		return q1 += q2;
	}

	sml_export template<arithmetic T>
		template<arithmetic T2>
	inline Quaternion<T>& Quaternion<T>::operator-= (const Quaternion<T2>& q2) {
		q0() -= static_cast<T>(q2.q0());
		vector -= q2.vector;
		return *this;
	}
	sml_export template<arithmetic T, arithmetic T2>
		inline Quaternion<T> operator- (Quaternion<T> q1, const Quaternion<T2>& q2) {
		return q1 -= q2;
	}

	sml_export template<arithmetic T>
		template<arithmetic T2>
	inline Quaternion<T>& Quaternion<T>::operator*= (const Quaternion<T2>& q) {

		//(a, b, c, d) * (e, f, g, h);
		// q0 q1 q2 q3    q0 q1 q2 q3

		Quaternion<T> ret(
			q0() * static_cast<T>(q.q0()) - q1() * static_cast<T>(q.q1()) - q2() * static_cast<T>(q.q2()) - q3() * static_cast<T>(q.q3()),
			q0() * static_cast<T>(q.q1()) + q1() * static_cast<T>(q.q0()) + q2() * static_cast<T>(q.q3()) - q3() * static_cast<T>(q.q2()),
			q0() * static_cast<T>(q.q2()) - q1() * static_cast<T>(q.q3()) + q2() * static_cast<T>(q.q0()) + q3() * static_cast<T>(q.q1()),
			q0() * static_cast<T>(q.q3()) + q1() * static_cast<T>(q.q2()) - q2() * static_cast<T>(q.q1()) + q3() * static_cast<T>(q.q0())
		);

		q0() = ret.q0();
		q1() = ret.q1();
		q2() = ret.q2();
		q3() = ret.q3();

		return *this;
	}
	sml_export template<arithmetic T, arithmetic T2>
		inline Quaternion<T> operator* (Quaternion<T> q1, const Quaternion<T2>& q2) {
		return q1 *= q2;
	}

	sml_export template<arithmetic T>
		template<arithmetic T2>
	inline Quaternion<T>& Quaternion<T>::operator*= (const T2& t) {

		scalar[0] *= static_cast<T>(t);
		vector *= t;
		return *this;
	}
	sml_export template<arithmetic T, arithmetic T2>
		inline Quaternion<T> operator* (Quaternion<T> q, const T2& t) {
		return q *= t;
	}

	sml_export template<arithmetic T>
		template<arithmetic T2>
	inline Quaternion<T>& Quaternion<T>::operator/=(const T2& t) {
		scalar[0] /= static_cast<T>(t);
		vector /= t;
		return *this;
	}
	sml_export template<arithmetic T, arithmetic T2>
		inline Quaternion<T> operator/ (Quaternion<T> q, const T2& t) {
		return q /= t;
	}

	sml_export template<arithmetic T>
		template<arithmetic T2>
	inline Quaternion<T>& Quaternion<T>::operator/=(const Quaternion<T2>& q2) {
		return *this *= inverse(q2);
	}
	sml_export template<arithmetic T, arithmetic T2>
		inline Quaternion<T> operator/ (Quaternion<T> q, const Quaternion<T2>& q2) {
		return q /= q2;
	}

	sml_export template<arithmetic T>
		inline double SquaredLength(const Quaternion<T>& q) {
		return (q.q0() * q.q0()) + (q.q1() * q.q1()) + (q.q2() * q.q2()) + (q.q3() * q.q3());
	}

	sml_export template<arithmetic T>
		inline double Length(const Quaternion<T>& q) {
		return std::sqrt(SquaredLength(q));
	}

	sml_export template<arithmetic T>
		inline Quaternion<T> Inverse(const Quaternion<T>& q) {
		return Conjugate(q) / SquaredLength(q);
	}

	sml_export template<arithmetic T>
		inline Quaternion<T> Normalise(const Quaternion<T>& q) {
		return (q / Length(q));
	}

	sml_export template<arithmetic T>
		inline bool IsNormal(const Quaternion<T>& q) {
		return (q == Normalise(q));
	}

	sml_export template<arithmetic T>
		inline Quaternion<T> RotationMatrixToQuaternion(sml::Matrix<T, 4, 4> mat) {

		return RotationMatrixToQuaternion(top_left(mat));
	}
	sml_export template<arithmetic T>
		inline Quaternion<T> RotationMatrixToQuaternion(sml::Matrix<T, 3, 3> mat) {

		Quaternion<T> ret(0);

		ret.q0() = std::sqrt(std::max(0.0, 1.0 + mat.at(0, 0) + mat.at(1, 1) + mat.at(2, 2))) / 2;
		ret.q1() = std::sqrt(std::max(0.0, 1.0 + mat.at(0, 0) - mat.at(1, 1) - mat.at(2, 2))) / 2;
		ret.q2() = std::sqrt(std::max(0.0, 1.0 - mat.at(0, 0) + mat.at(1, 1) - mat.at(2, 2))) / 2;
		ret.q3() = std::sqrt(std::max(0.0, 1.0 - mat.at(0, 0) - mat.at(1, 1) + mat.at(2, 2))) / 2;

		ret.i() = std::copysign(ret.i(), mat.at(2, 1) - mat.at(1, 2));
		ret.j() = std::copysign(ret.j(), mat.at(0, 2) - mat.at(2, 0));
		ret.k() = std::copysign(ret.k(), mat.at(1, 0) - mat.at(0, 1));
		
		return ret;
	}

	sml_export template<arithmetic T>
		inline Matrix<T, 4, 4> QuaternionTo44RotationMatrix(const Quaternion<T>& q) {

		assert(IsNormal(q));

		T isq = 2.0 * q.i() * q.i();
		T jsq = 2.0 * q.j() * q.j();
		T ksq = 2.0 * q.k() * q.k();

		T si = 2.0 * q.s() * q.i();
		T ij = 2.0 * q.i() * q.j();
		T jk = 2.0 * q.j() * q.k();
		T sk = 2.0 * q.s() * q.k();
		T ik = 2.0 * q.i() * q.k();
		T sj = 2.0 * q.s() * q.j();

		return Matrix<T,4,4>(1.0 - jsq - ksq, ij - sk, ik + sj, 0.0,
							 ij + sk, 1.0 - isq - ksq, jk - si, 0.0,
							 ik - sj, jk + si, 1.0 - isq - jsq, 0.0, 
							 0.0, 0.0, 0.0, 1.0);
	}

	sml_export template<arithmetic T>
		inline Matrix<T, 3, 3> QuaternionTo33RotationMatrix(const Quaternion<T>& q) {

		assert(IsNormal(q));

		T isq = 2.0 * q.i() * q.i();
		T jsq = 2.0 * q.j() * q.j();
		T ksq = 2.0 * q.k() * q.k();

		T si = 2.0 * q.s() * q.i();
		T ij = 2.0 * q.i() * q.j();
		T jk = 2.0 * q.j() * q.k();
		T sk = 2.0 * q.s() * q.k();
		T ik = 2.0 * q.i() * q.k();
		T sj = 2.0 * q.s() * q.j();

		return Matrix<T, 3, 3>(1.0 - jsq - ksq, ij - sk, ik + sj,
								ij + sk, 1.0 - isq - ksq, jk - si,
								ik - sj, jk + si, 1.0 - isq - jsq);
	}

	sml_export template<arithmetic T, arithmetic T2>
	inline Vector<T2, 3> RotateActive(const Vector<T2, 3> pos, const Quaternion<T> rot) {
		Quaternion<T> temp(0, pos);
		temp = Inverse(rot) * temp * rot;
		return temp.vector;
	}

	sml_export template<arithmetic T, arithmetic T2>
	inline Vector<T2, 3> RotatePassive(const Vector<T2, 3> pos, const Quaternion<T> rot) {
		Quaternion<T> temp(0, pos);
		temp = rot * temp * Inverse(rot);
		return temp.vector;
	}

	sml_export using Quatf = Quaternion<float>;
	sml_export using Quatd = Quaternion<double>;
}

#endif // SML_QUATERNION_HPP