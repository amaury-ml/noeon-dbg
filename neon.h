#include <cstdint>
#include <cstddef>

#include <array>
#include <algorithm>

#include "float16.h"

using float32_t = float;
using float64_t = double;
using float16_t = _Float16; 

static_assert(!std::is_same<float16_t, uint16_t>(), "float16 cannot be a uint16");


namespace neon {

	template <typename T>
	struct ieee754;

	template <>
	struct ieee754<float16_t>
	{
		using itype = uint16_t;
	};

	template <>
	struct ieee754<float32_t>
	{
		using itype = uint32_t;
	};

	template <>
	struct ieee754<float64_t>
	{
		using itype = uint64_t;
	};

	template <typename T>
	struct is_ieee754 : std::false_type
	{};

	template <>
	struct is_ieee754<float16_t> : std::true_type
	{
	};

	template <>
	struct is_ieee754<float32_t> : std::true_type
	{
	};

	template <>
	struct is_ieee754<float64_t> : std::true_type
	{
	};

	template <typename T>
	struct is_integer : std::false_type
	{};


	template <>struct is_integer<int8_t> : std::true_type {};
	template <>struct is_integer<int16_t> : std::true_type {};
	template <>struct is_integer<int32_t> : std::true_type {};
	template <>struct is_integer<int64_t> : std::true_type {};

	template <>struct is_integer<uint8_t> : std::true_type {};
	template <>struct is_integer<uint16_t> : std::true_type {};
	template <>struct is_integer<uint32_t> : std::true_type {};
	template <>struct is_integer<uint64_t> : std::true_type {};




	template <typename T>
	struct neon_type;


	template<> struct neon_type<uint8_t>
	{
		using mask_type = uint8_t;
		using wide_type = uint16_t;
	};

	template<> struct neon_type<int8_t>
	{
		using mask_type = uint8_t;
		using wide_type = int16_t;
	};

	template<> struct neon_type<uint16_t>
	{
		using mask_type = uint16_t;
		using wide_type = uint32_t;
		using narrow_type = uint8_t;
	};

	template<> struct neon_type<int16_t>
	{
		using mask_type = uint16_t;
		using wide_type = int32_t;
		using narrow_type = int8_t;
	};

	template<> struct neon_type<uint32_t>
	{
		using mask_type = uint32_t;
		using wide_type = uint64_t;
		using narrow_type = uint16_t;
	};

	template<> struct neon_type<int32_t>
	{
		using mask_type = uint32_t;
		using wide_type = int64_t;
		using narrow_type = int16_t;
	};

	template<> struct neon_type<uint64_t>
	{
		using mask_type = uint64_t;
		using narrow_type = uint32_t;
	};

	template<> struct neon_type<int64_t>
	{
		using mask_type = uint64_t;
		using narrow_type = int32_t;
	};

	template<> struct neon_type<float16_t>
	{
		using mask_type = uint16_t;
	};

	template<> struct neon_type<float32_t>
	{
		using mask_type = uint32_t;
	};

	template<> struct neon_type<float64_t>
	{
		using mask_type = uint64_t;
	};



	template <typename T>
	struct neon128
	{
		using type = T;
		static constexpr size_t N = 16 / sizeof(T);
		T const& operator[](int n) const { return v_[n]; }
		std::array<T, N> v_;
	};

	template <typename T>
	struct neon64
	{
		using type = T;
		static constexpr size_t N = 8 / sizeof(T);
		T const& operator[](int n) const { return v_[n]; }
		std::array<T, N> v_;
	};


	template <typename T>
	typename std::enable_if <is_integer<T>::value, neon128<T>>::type
		vabd(neon128<T> const& a0, neon128<T> const& a1)
	{
		using signed_type = typename std::make_signed<T>::type;

		neon128<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = std::abs(signed_type(a0.v_[i]) - signed_type(a1.v_[i]));
		return r;
	}

	template <typename T>
	typename std::enable_if <is_ieee754<T>::value, neon128<T>>::type
		vabd(neon128<T> const& a0, neon128<T> const& a1)
	{
		neon128<T> r;
		for (int i = 0; i < neon128<float>::N; i++)
			r.v_[i] = std::abs(a0.v_[i] - a1.v_[i]);
		return r;
	}


	template <typename T>
	typename std::enable_if <is_integer<T>::value, neon64<T>>::type
		vabd(neon64<T> const& a0, neon64<T> const& a1)
	{
		using signed_type = typename std::make_signed<T>::type;

		neon64<T> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = std::abs(signed_type(a0.v_[i]) - signed_type(a1.v_[i]));
		return r;
	}

	template <typename T>
	typename std::enable_if <is_ieee754<T>::value, neon64<T>>::type
		vabd(neon64<T> const& a0, neon64<T> const& a1)
	{
		neon64<T> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i] - a1.v_[i]);
		return r;
	}

	template <typename T>
	neon128<T>
		vabs(neon128<T> const& a)
	{
		neon128<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = std::abs(a.v_[i]);
		return r;
	}

	template <typename T>
	neon64<T>
		vabs(neon64<T> const& a)
	{
		neon64<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = std::abs(a.v_[i]);
		return r;
	}

	template <typename T>
	neon128<T>
		vadd(neon128<T> const& a0, neon128<T> const& a1)
	{
		neon128<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a0.v_[i] + a1.v_[i];
		return r;
	}

	template <typename T>
	neon64<T>
		vadd(neon64<T> const& a0, neon64<T> const& a1)
	{
		neon64<T> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a0.v_[i] + a1.v_[i];
		return r;
	}

	template <typename T>
	neon128<typename neon_type<T>::wide_type>
		vaddl(neon64<T> const& a0, neon64<T> const& a1)
	{
		using wide_type = typename neon_type<T>::wide_type;

		neon128<wide_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = wide_type(a0.v_[i]) + wide_type(a1.v_[i]);
		return r;
	}


	template <typename T>
	neon128<typename neon_type<T>::wide_type>
		vaddl_high(neon128<T> const& a0, neon128<T> const& a1)
	{
		using wide_type = typename neon_type<T>::wide_type;

		neon128<wide_type> r;
		for (int i = 0; i < neon128<wide_type>::N; i++)
		{
			auto pos = neon128<wide_type>::N + i;
			r.v_[i] = wide_type(a0.v_[pos]) + wide_type(a1.v_[pos]);
		}
		return r;
	}

	template <typename T>
	neon128<typename neon_type<T>::wide_type>
		vaddw(neon128<typename neon_type<T>::wide_type> const& a0, neon64<T> const& a1)
	{
		using wide_type = typename neon_type<T>::wide_type;

		neon128<wide_type> r;
		for (int i = 0; i < neon128<wide_type>::N; i++)
		{
			r.v_[i] = a0.v_[i] + wide_type(a1.v_[i]);
		}
		return r;
	}

	template <typename T>
	neon128<typename neon_type<T>::wide_type>
		vaddw_high(neon128<typename neon_type<T>::wide_type> const& a0, neon128<T> const& a1)
	{
		using wide_type = typename neon_type<T>::wide_type;

		neon128<wide_type> r;
		for (int i = 0; i < neon128<wide_type>::N; i++)
		{
			auto pos = neon128<wide_type>::N + i;
			r.v_[i] = a0.v_[i] + wide_type(a1.v_[pos]);
		}
		return r;
	}

	template <typename T>
	neon64<typename neon_type<T>::narrow_type>
		vaddhn(neon128<T> const& a0, neon128<T> const& a1)
	{
		using narrow_type = typename neon_type<T>::narrow_type;

		neon64<narrow_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
		{
			auto pos = neon128<narrow_type>::N + i;

			r.v_[i] = narrow_type(a0.v_[i] + a1.v_[i]);
		}
		return r;
	}

	template <typename T>
	neon128<typename neon_type<T>::narrow_type>
		vaddhn_high(neon64<typename neon_type<T>::narrow_type> const& a0, neon128<T> const& a1, neon128<T> const& a2)
	{
		using narrow_type = typename neon_type<T>::narrow_type;
		neon128<narrow_type> r;
		for (int i = 0; i < neon64<narrow_type>::N; i++)
		{
			r.v_[i] = a0.v_[i];
		}
		for (int i = 0; i < neon64<narrow_type>::N; i++)
		{
			auto pos = neon64<narrow_type>::N + i;

			r.v_[pos] = narrow_type(a1.v_[i] + a2.v_[i]);
		}
		return r;
	}

	template <typename T>
	T
		vaddv(neon128<T> const& a)
	{
		T r = a.v_[0];
		for (int i = 1; i < neon128<T>::N; i++)
		{
			r += a.v_[i];
		}
		return r;
	}

	template <typename T>
	T
		vaddv(neon64<T> const& a)
	{
		T r = a.v_[0];
		for (int i = 1; i < neon64<T>::N; i++)
		{
			r += a.v_[i];
		}
		return r;
	}

	template <typename T>
	neon128<T>
		vbic(neon128<T> const& a0, neon128<T> const& a1)
	{
		neon128<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a0.v_[i] & ~a1.v_[i];
		return r;
	}

	template <typename T>
	neon64<T>
		vbic(neon64<T> const& a0, neon64<T> const& a1)
	{
		neon64<T> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a0.v_[i] & ~a1.v_[i];
		return r;
	}


	template <typename T>
	typename std::enable_if < is_integer<T>::value, neon128<T>>::type
		vbsl(neon128<typename std::make_unsigned<T>::type> const& a0, neon128<T> const& a1, neon128<T> const& a2)
	{
		using mask_type = typename std::make_unsigned<T>::type;
		neon128<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = (a0.v_[i] & mask_type(a1.v_[i])) | (~a0.v_[i] & mask_type(a2.v_[i]));
		return r;
	}

	template <typename T>
	typename std::enable_if < is_integer<T>::value, neon64<T>>::type
		vbsl(neon64<typename std::make_unsigned<T>::type> const& a0, neon64<T> const& a1, neon64<T> const& a2)
	{
		using mask_type = typename std::make_unsigned<T>::type;
		neon64<T> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = (a0.v_[i] & mask_type(a1.v_[i])) | (~a0.v_[i] & mask_type(a2.v_[i]));
		return r;
	}

	template <typename T>
	typename std::enable_if <is_ieee754<T>::value, neon128<T>>::type
		vbsl(neon128<typename ieee754<T>::itype> const& a0, neon128<T> const& a1, neon128<T> const& a2)
	{
		using itype = typename ieee754<T>::itype;
		neon128<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
		{
			static_assert(sizeof(T) == sizeof(itype), "Assumption is false");
			const itype um = a0.v_[i];
			const itype ua = *reinterpret_cast<const itype*>(&a1.v_[i]);
			const itype ub = *reinterpret_cast<const itype*>(&a2.v_[i]);
			itype ur = (um & ua) | (~um & ub);

			r.v_[i] = *reinterpret_cast<T*>(&ur);
		}
		return r;
	}

	template <typename T>
	typename std::enable_if <is_ieee754<T>::value, neon64<T>>::type
		vbsl(neon64<typename ieee754<T>::itype> const& a0, neon64<T> const& a1, neon64<T> const& a2)
	{
		using itype = typename ieee754<T>::itype;
		neon64<T> r;
		for (int i = 0; i < neon64<T>::N; i++)
		{
			static_assert(sizeof(T) == sizeof(itype), "Assumption is false");
			const itype um = a0.v_[i];
			const itype ua = *reinterpret_cast<const itype*>(&a1.v_[i]);
			const itype ub = *reinterpret_cast<const itype*>(&a2.v_[i]);
			itype ur = (um & ua) | (~um & ub);

			r.v_[i] = *reinterpret_cast<T*>(&ur);
		}
		return r;
	}

	template <typename T>
	neon128<T>
		vbcax(neon128<T> const& a0, neon128<T> const& a1, neon128<T> const& a2)
	{
		//vn = a0
		//vm = a1
		//va = a2
		neon128<T> r;
		for (int i = 0; i < neon128<T>::N; i++)
		{
			r.v_[i] = a0.v_[i] ^ (a1.v_[i] & ~a2.v_[i]);
		}
		return r;
	}

	template <typename T>
	neon64<T>
		vbcax(neon64<T> const& a0, neon64<T> const& a1, neon64<T> const& a2)
	{
		//vn = a0
		//vm = a1
		//va = a2
		neon64<T> r;
		for (int i = 0; i < neon64<T>::N; i++)
		{
			r.v_[i] = a0.v_[i] ^ (a1.v_[i] & ~a2.v_[i]);
		}
		return r;
	}

	
	template <typename T, typename = void>
	struct cmptype {};

	template <typename T> 
	struct cmptype<T, typename std::enable_if<std::is_integral<T>::value>::type>
	{ using type = typename std::make_unsigned<T>::type; };
	
	template <typename T> 
	struct cmptype<T, typename std::enable_if<is_ieee754<T>::value>::type> 
	{ using type = typename ieee754<T>::itype; };


	template <typename T>
	neon128<typename cmptype<T>::type>
		vceq(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;
	
		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a0.v_[i] == a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}
	

	template <typename T>
	neon64<typename cmptype<T>::type>
		vceq(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a0.v_[i] == a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vceqz(neon128<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a.v_[i] == T(0) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vceqz(neon64<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a.v_[i] == T(0) ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcge(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a0.v_[i] >= a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcge(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a0.v_[i] >= a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon128<typename cmptype<T>::type>
		vcgez(neon128<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a.v_[i] >= T(0) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcgez(neon64<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a.v_[i] >= 0 ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcle(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a0.v_[i] <= a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcle(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a0.v_[i] <= a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vclez(neon128<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a.v_[i] <= T(0) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vclez(neon64<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a.v_[i] <= 0 ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcgt(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a0.v_[i] > a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcgt(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a0.v_[i] > a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon128<typename cmptype<T>::type>
		vcgtz(neon128<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a.v_[i] > T(0) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcgtz(neon64<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a.v_[i] > 0 ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vclt(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a0.v_[i] < a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vclt(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a0.v_[i] < a1.v_[i] ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcltz(neon128<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = a.v_[i] < T(0) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcltz(neon64<T> const& a)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = a.v_[i] < 0 ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcage(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) >= std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcage(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) >= std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcale(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) <= std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcale(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) <= std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcagt(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) > std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcagt(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) > std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename T>
	neon128<typename cmptype<T>::type>
		vcalt(neon128<T> const& a0, neon128<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon128<mask_type> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) < std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}


	template <typename T>
	neon64<typename cmptype<T>::type>
		vcalt(neon64<T> const& a0, neon64<T> const& a1)
	{
		using mask_type = typename cmptype<T>::type;

		neon64<mask_type> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = std::abs(a0.v_[i]) < std::abs(a1.v_[i]) ? mask_type(-1) : mask_type(0);
		return r;
	}

	template <typename OUTT, typename T>
	neon64<OUTT>
		vcvt(neon64<T> const& a)
	{
		neon64<OUTT> r;
		for (int i = 0; i < neon64<T>::N; i++)
			r.v_[i] = OUTT(a.v_[i]);
		return r;
	}

	template <typename OUTT, typename T>
	neon128<OUTT>
		vcvt(neon128<T> const& a)
	{
		neon128<OUTT> r;
		for (int i = 0; i < neon128<T>::N; i++)
			r.v_[i] = OUTT(a.v_[i]);
		return r;
	}

} // namesoace neon


using int8x8_t = neon::neon64<int8_t>;
using uint8x8_t = neon::neon64<uint8_t>;
using int8x16_t = neon::neon128<int8_t>;
using uint8x16_t = neon::neon128<uint8_t>;

using int16x4_t = neon::neon64<int16_t>;
using uint16x4_t = neon::neon64<uint16_t>;
using int16x8_t = neon::neon128<int16_t>;
using uint16x8_t = neon::neon128<uint16_t>;

using  int32x2_t = neon::neon64< int32_t>;
using uint32x2_t = neon::neon64<uint32_t>;
using int32x4_t = neon::neon128<int32_t>;
using uint32x4_t = neon::neon128<uint32_t>;

using  int64x1_t = neon::neon64< int64_t>;
using uint64x1_t = neon::neon64<uint64_t>;
using int64x2_t = neon::neon128<int64_t>;
using uint64x2_t = neon::neon128<uint64_t>;

using float16x4_t = neon::neon64<float16_t>;
using float16x8_t = neon::neon128<float16_t>;

using float32x2_t = neon::neon64<float32_t>;
using float32x4_t = neon::neon128<float32_t>;

using float64x1_t = neon::neon64<float64_t>;
using float64x2_t = neon::neon128<float64_t>;

