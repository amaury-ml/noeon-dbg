#include "neon_api.h"

template <typename T>
bool operator==(neon::neon128<T> const& a0, neon::neon128<T> const& a1)
{
	for (int i = 0; i < neon::neon128<T>::N; i++)
		if (a0[i] != a1[i]) return false;
	return true;
}

template <typename T>
bool operator!=(neon::neon128<T> const& a0, neon::neon128<T> const& a1)
{
	return !(a0 == a1);
}

template <typename T>
bool operator==(neon::neon64<T> const& a0, neon::neon64<T> const& a1)
{
	for (int i = 0; i < neon::neon64<T>::N; i++)
		if (a0[i] != a1[i]) return false;
	return true;
}

template <typename T>
bool operator!=(neon::neon64<T> const& a0, neon::neon64<T> const& a1)
{
	return !(a0 == a1);
}



int main()
{

	_Float16 a(3.0);
	if (float(a) != 3.0f) __debugbreak();


	// vabd
	{
		uint16x4_t v0{ 1,2,3,4 };
		uint16x4_t v1{ 3,4,5,6 };
		uint16x4_t r = vabd_u16(v0, v1);

		if (r != uint16x4_t{ 2, 2, 2, 2 })
		{
			__debugbreak();
		}
	}
	{
		int16x4_t v0{ 1,2,3,4 };
		int16x4_t v1{ -1,-2,-3,-4 };
		int16x4_t r = vabd_s16(v0, v1);

		if (r != int16x4_t{ 2, 4, 6, 8 })
		{
			__debugbreak();
		}
	}
	{
		float32x2_t v0{ 1,2 };
		float32x2_t v1{ -1,-2 };
		float32x2_t r = vabd_f32(v0, v1);

		if (r != float32x2_t{ 2, 4})
		{
			__debugbreak();
		}
	}
	{
		uint32x4_t v0{ 1,2,3,4 };
		uint32x4_t v1{ 3,4,5,6 };
		uint32x4_t r = vabdq_u32(v0, v1);

		if (r != uint32x4_t{ 2, 2, 2, 2 })
		{
			__debugbreak();
		}
	}
	{
		float32x4_t v0{ 1,2,3,4 };
		float32x4_t v1{ -1,-2,-3,-4 };
		float32x4_t r = vabdq_f32(v0, v1);

		if (r != float32x4_t{ 2, 4, 6, 8 })
		{
			__debugbreak();
		}
	}

	// vadd
	{
		uint16x4_t v0{ 1,2,3,4 };
		uint16x4_t v1{ 3,4,5,6 };
		uint16x4_t r = vadd_u16(v0, v1);

		if (r != uint16x4_t{ 4, 6, 8, 10 })
		{
			__debugbreak();
		}
	}
	{
		int16x4_t v0{ 1,2,3,4 };
		int16x4_t v1{ -1,-2,-3,-4 };
		int16x4_t r = vadd_s16(v0, v1);

		if (r != int16x4_t{ 0, 0, 0, 0 })
		{
			__debugbreak();
		}
	}
	{
		float32x2_t v0{ 1,2 };
		float32x2_t v1{ -1,-2 };
		float32x2_t r = vadd_f32(v0, v1);

		if (r != float32x2_t{ 0, 0 })
		{
			__debugbreak();
		}
	}
	{
		uint32x4_t v0{ 1,2,3,4 };
		uint32x4_t v1{ 3,4,5,6 };
		uint32x4_t r = vaddq_u32(v0, v1);

		if (r != uint32x4_t{ 4, 6, 8, 10 })
		{
			__debugbreak();
		}
	}
	{
		float32x4_t v0{ 1,2,3,4 };
		float32x4_t v1{ -1,-2,-3,-4 };
		float32x4_t r = vaddq_f32(v0, v1);

		if (r != float32x4_t{ 0, 0, 0, 0 })
		{
			__debugbreak();
		}
	}


	// vaddl
	{
		uint16x4_t v0{ uint16_t(-1), 2, 3, 4 };
		uint16x4_t v1{ 3, 4, 5, 6 };
		uint32x4_t r = vaddl_u16(v0, v1);
		if (r != uint32x4_t{ 2 + (1 << 16), 6, 8, 10 })
		{
			__debugbreak();
		}
	}
	{
		uint16x8_t v0{ uint16_t(-1), 2, 3, 4, 5, 6, 7, uint16_t(-1) };
		uint16x8_t v1{ 3, 4, 5, 6, 7, 8, 9, 10 };

		uint32x4_t r = vaddl_high_u16(v0, v1);
		if (r != uint32x4_t{ 12, 14, 16, 9 + (1 << 16) })
		{
			__debugbreak();
		}

	}
	// vaddw
	{
		uint64x2_t v0{ 1, 2 };
		uint32x2_t v1{ 3, 4 };
		auto r = vaddw_u32(v0, v1);
		if (r != uint64x2_t{ 4, 6 })
		{
			__debugbreak();
		}
	}

	{
		int32x4_t v0{ 1, 2, 3, 4 };
		int16x4_t v1{ 5, 6, 7, 8 };
		auto r = vaddw_s16(v0, v1);
		if (r != int32x4_t{ 6, 8, 10, 12 })
		{
			__debugbreak();
		}
	}

	{
		uint64x2_t v0{ 1, 2 };
		uint32x4_t v1{ 3, 4, 5, 6 };
		auto r = vaddw_high_u32(v0, v1);
		if (r != uint64x2_t{ 6, 8 })
		{
			__debugbreak();
		}
	}

	// vaddhn
	{
		int32x4_t v0{ 1, 2, 3, 4 };
		int32x4_t v1{ 5, 6, 7, 8 };
		auto r = vaddhn_s32(v0, v1);
		if (r != int16x4_t{ 6, 8, 10, 12 })
		{
			__debugbreak();
		}
	}

	{
		int32x4_t v0{ 1, 2, 3, 4 };
		int32x4_t v1{ 5, 6, 7, 8 };
		int16x4_t rl{ -1, -2, -3, -4 };
		auto r = vaddhn_high_s32(rl, v0, v1);
		if (r != int16x8_t{ -1, -2, -3, -4, 6, 8, 10, 12 })
		{
			__debugbreak();
		}
	}

	// vaddv
	{
		int32x4_t v0{ 1, 2, 3, 4 };
		auto r = vaddvq_s32(v0);
		if (r != 10)
		{
			__debugbreak();
		}
	}


	// vbic
	{
		int32x4_t v0{ 0b0100, 0b1000, 0b0001, 0b0101 };
		int32x4_t v1{ 0b0100, 0b1000, 0b1001, 0b0100 };

		auto r = vbicq_s32(v0, v1);
		if (r != int32x4_t{ 0, 0, 0, 0b0001 })
		{
			__debugbreak();
		}
	}

	// vbsl
	{
		uint32x4_t mask{ 0b0100, 0b1000, 0b0001, 0b0101 };
		int32x4_t v0  { 0b0100, 0b1000, 0b0001, 0b0101 };
		int32x4_t v1  { 0b1000, 0b0100, 0b1001, 0b0100 };

		auto r = vbslq_s32(mask, v0, v1);
		if (r != int32x4_t{ 0, 0, 0, 0b0001 })
		{
			__debugbreak();
		}
	}

	// vbcax
	{
		int32x4_t mask{ 0b0100, 0b1000, 0b0001, 0b0101 };
		int32x4_t v0{ 0b0100, 0b1000, 0b0001, 0b0101 };
		int32x4_t v1{ 0b1000, 0b0100, 0b1001, 0b0100 };

		auto r = vbcaxq_s32(mask, v0, v1);
		if (r != int32x4_t{ 0, 0, 0, 0b0001 })
		{
			__debugbreak();
		}
	}


}