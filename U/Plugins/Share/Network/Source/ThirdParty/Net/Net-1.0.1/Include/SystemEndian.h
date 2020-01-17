#pragma once
#include "Engine/HAL/SPlatform.h"
#include "Networkdef.h"


#define Swap16(l) \
			( ( ((l) & 0xff) << 8 ) | \
			  ( ((l) >> 8) & 0xff) )

#define Swap32(l)                \
            ( ( ((l) >> 24) & 0x000000FFL ) |       \
              ( ((l) >>  8) & 0x0000FF00L ) |       \
              ( ((l) <<  8) & 0x00FF0000L ) |       \
              ( ((l) << 24) & 0xFF000000L ) )

#define Swap64(l)            \
            ( ( ((l) >> 56) & 0x00000000000000FFLL ) |       \
              ( ((l) >> 40) & 0x000000000000FF00LL ) |       \
              ( ((l) >> 24) & 0x0000000000FF0000LL ) |       \
              ( ((l) >>  8) & 0x00000000FF000000LL ) |       \
              ( ((l) <<  8) & 0x000000FF00000000LL ) |       \
              ( ((l) << 24) & 0x0000FF0000000000LL ) |       \
              ( ((l) << 40) & 0x00FF000000000000LL ) |       \
              ( ((l) << 56) & 0xFF00000000000000LL ) )

enum ESystemEndian
{
	SE_BIG_ENDIAN = 0,
	SE_LITTLE_ENDIAN,
};

class FSystemEndian
{
private:
	static ESystemEndian LocalEndian()
	{
		union check
		{
			int    i;
			char ch;
		}m;

		m.i = 1;

		if (m.ch == 0x00)
		{
			return SE_BIG_ENDIAN;
		}

		return SE_LITTLE_ENDIAN;
	}

public:
	static int16 Swap(int16 value)
	{
#if !VERIFYENDIAN
		return value;
#endif

#if SERVER_BIGENDIAN
		if (LocalEndian() == SE_BIG_ENDIAN)
		{
			return value;
		}
#else
		if (LocalEndian() == SE_LITTLE_ENDIAN)
		{
			return value;
		}
#endif

		return Swap16(value);
	}

	static int32 Swap(int32 value)
	{
#if !VERIFYENDIAN
		return value;
#endif

#if SERVER_BIGENDIAN
		if (LocalEndian() == SE_BIG_ENDIAN)
		{
			return value;
		}
#else
		if (LocalEndian() == SE_LITTLE_ENDIAN)
		{
			return value;
		}
#endif

		return Swap32(value);
	}

	static int64 Swap(int64 value)
	{
#if !VERIFYENDIAN
		return value;
#endif

#if SERVER_BIGENDIAN
		if (LocalEndian() == SE_BIG_ENDIAN)
		{
			return value;
		}
#else
		if (LocalEndian() == SE_LITTLE_ENDIAN)
		{
			return value;
		}
#endif

		return Swap64(value);
	}

	static uint16 Swap(uint16 value)
	{
#if !VERIFYENDIAN
		return value;
#endif

#if SERVER_BIGENDIAN
		if (LocalEndian() == SE_BIG_ENDIAN)
		{
			return value;
		}
#else
		if (LocalEndian() == SE_LITTLE_ENDIAN)
		{
			return value;
		}
#endif

		return Swap16(value);
	}

	static uint32 Swap(uint32 value)
	{
#if !VERIFYENDIAN
		return value;
#endif

#if SERVER_BIGENDIAN
		if (LocalEndian() == SE_BIG_ENDIAN)
		{
			return value;
		}
#else
		if (LocalEndian() == SE_LITTLE_ENDIAN)
		{
			return value;
		}
#endif

		return Swap32(value);
	}

	static uint64 Swap(uint64 value)
	{
#if !VERIFYENDIAN
		return value;
#endif

#if SERVER_BIGENDIAN
		if (LocalEndian() == SE_BIG_ENDIAN)
		{
			return value;
		}
#else
		if (LocalEndian() == SE_LITTLE_ENDIAN)
		{
			return value;
		}
#endif

		return Swap64(value);
	}

	static uint32 Swapf(float value)
	{
		uint32 Tempval = *(uint32*)(&value);
		return Swap(Tempval);
	}

	static float Swapf(uint32 value)
	{
		const uint32 Tempval = Swap(value);
		float Retval;
		*((uint32*)& Retval) = Tempval;
		return Retval;
	}

	static uint64 Swapd(double value)
	{
		uint64 Tempval = *(uint64*)(&value);
		return Swap(Tempval);
	}

	static double Swapd(uint64 value)
	{
		const uint64 Tempval = Swap(value);
		double Retval;
		*((uint64*)& Retval) = Tempval;
		return Retval;
	}
};


