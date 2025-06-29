#ifndef SML_QUATERNION_HPP
#define SML_QUATERNION_HPP

namespace sml {


	sml_export template<typename T>
	class Quaternion {
	public:
		Quaternion() {
			scalar = { static_cast<T>(0) };
			vector = { static_cast<T>(0) };
		}
		template<typename T2>
		Quaternion(const T2 c) : scalar{ static_cast<T>(c) }, vector{ Vector<T, 3>(static_cast<T>(c),static_cast<T>(c),static_cast<T>(c)) } {};
		template<typename T2, typename T3>
		Quaternion(const T2 s, const Vector<T3, 3> v) : scalar{ static_cast<T>(s) }, vector{ v } {};
		template<typename T2, typename T3, typename T4, typename T5>
		Quaternion(const T2 s, const T3 v0, const T4 v1, const T5 v2) : scalar{ static_cast<T>(s) }, vector{ Vector<T, 3>(static_cast<T>(v0),static_cast<T>(v1),static_cast<T>(v2)) } {};
		template<typename T2>
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
		inline T& i() { return vector[0]; }
		inline T& j() { return vector[1]; }
		inline T& k() { return vector[2]; }
		inline T q0() const { return scalar[0]; }
		inline T q1() const { return vector[0]; }
		inline T q2() const { return vector[1]; }
		inline T q3() const { return vector[2]; }
		inline T i() const { return vector[0]; }
		inline T j() const { return vector[1]; }
		inline T k() const { return vector[2]; }

		template<typename T2>
		inline Quaternion<T>& operator+= (const Quaternion<T2>& q2);

		template<typename T2>
		inline Quaternion<T>& operator-= (const Quaternion<T2>& q2);

		template<typename T2>
		inline Quaternion<T>& operator*= (const Quaternion<T2>& q2);

		template<typename T2>
		inline Quaternion<T>& operator*= (const T2& t);

		std::array<T, 1> scalar;
		Vector<T, 3> vector;

	};

	// Write vector to ostream
	sml_export template<typename T>
	inline std::ostream& operator << (std::ostream& os, const Quaternion<T>& t) {
		os << t[0] << " " << t[1] << " " << t[2] << " " << t[3];
		return os;
	}

	// Write vector to wostream
	sml_export template<typename T>
	inline std::wostream& operator << (std::wostream& os, const Quaternion<T>& t) {
		os << t[0] << " " << t[1] << " " << t[2] << " " << t[3];
		return os;
	}

	sml_export template<typename T>
	inline Quaternion<T> Conjugate(const Quaternion<T>& q) {
		return Quaternion(q.q0(), -q.q1(), -q.q2(), -q.q3());
	}

	using std::abs;
	sml_export template<typename T>
		inline Quaternion<T> abs(const Quaternion<T>& q) {
		return Quaternion(abs(q.scalar), abs(q.vector));
	}

	sml_export template<typename T>
		template<typename T2>
		inline Quaternion<T>& Quaternion<T>::operator+= (const Quaternion<T2>& q2) {
			q0() += static_cast<T>(q2.q0());
			vector += q2.vector;
			return *this;
	}
	sml_export template<typename T, typename T2>
		inline Quaternion<T> operator+ (Quaternion<T> q1, const Quaternion<T2>& q2) {
		return q1 += q2;
	}

	sml_export template<typename T>
		template<typename T2>
	inline Quaternion<T>& Quaternion<T>::operator-= (const Quaternion<T2>& q2) {
		q0() -= static_cast<T>(q2.q0());
		vector -= q2.vector;
		return *this;
	}
	sml_export template<typename T, typename T2>
		inline Quaternion<T> operator- (Quaternion<T> q1, const Quaternion<T2>& q2) {
		return q1 -= q2;
	}

	sml_export template<typename T>
		template<typename T2>
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
	sml_export template<typename T, typename T2>
		inline Quaternion<T> operator* (Quaternion<T> q1, const Quaternion<T2>& q2) {
		return q1 *= q2;
	}

	sml_export template<typename T>
		template<typename T2>
	inline Quaternion<T>& Quaternion<T>::operator*= (const T2& t) {

		scalar[0] *= t;
		vector *= t;
		return *this;
	}
	sml_export template<typename T, typename T2>
		inline Quaternion<T> operator* (Quaternion<T> q, const T2& t) {
		return q *= t;
	}
}

#endif // SML_QUATERNION_HPP