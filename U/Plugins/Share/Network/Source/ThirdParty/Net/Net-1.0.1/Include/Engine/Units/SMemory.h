#pragma once
#include "Engine/HAL/SPlatform.h"
#include "SIsPointer.h"

struct SMemory
{
private:
	template <typename T>
	static inline void Valswap(T& A, T& B)
	{
		// Usually such an implementation would use move semantics, but
		// we're only ever going to call it on fundamental types and MoveTemp
		// is not necessarily in scope here anyway, so we don't want to
		// #include it if we don't need to.
		T Tmp = A;
		A = B;
		B = Tmp;
	}

public:
	static inline void* Memmove(void* Dest, const void* Src, SIZE_T Count)
	{
		return memmove(Dest, Src, Count);
	}

	static inline int32 Memcmp(const void* Buf1, const void* Buf2, SIZE_T Count)
	{
		return memcmp(Buf1, Buf2, Count);
	}

	static inline void* Memset(void* Dest, uint8 Char, SIZE_T Count)
	{
		return memset(Dest, Char, Count);
	}

	template< class T >
	static inline void Memset(T& Src, uint8 ValueToSet)
	{
		static_assert(!SIsPointer<T>::Value, "For pointers use the three parameters function");
		Memset(&Src, ValueToSet, sizeof(T));
	}

	static inline void* Memzero(void* Dest, SIZE_T Count)
	{
		return memset(Dest, 0, Count);
	}

	template< class T >
	static inline void Memzero(T& Src)
	{
		static_assert(!SIsPointer<T>::Value, "For pointers use the two parameters function");
		Memzero(&Src, sizeof(T));
	}

	static inline void* Memcpy(void* Dest, const void* Src, SIZE_T Count)
	{
		return memcpy(Dest, Src, Count);
	}

	template< class T >
	static inline void Memcpy(T& Dest, const T& Src)
	{
		static_assert(!SIsPointer<T>::Value, "For pointers use the three parameters function");
		Memcpy(&Dest, &Src, sizeof(T));
	}

	static inline void Memswap(void* Ptr1, void* Ptr2, SIZE_T Size)
	{
		switch (Size)
		{
		case 0:
			break;

		case 1:
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 2:
			Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
			break;

		case 3:
			Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 4:
			Valswap(*(uint32*)Ptr1, *(uint32*)Ptr2);
			break;

		case 5:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 6:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*(uint16*)Ptr1, *(uint16*)Ptr2);
			break;

		case 7:
			Valswap(*((uint32*&)Ptr1)++, *((uint32*&)Ptr2)++);
			Valswap(*((uint16*&)Ptr1)++, *((uint16*&)Ptr2)++);
			Valswap(*(uint8*)Ptr1, *(uint8*)Ptr2);
			break;

		case 8:
			Valswap(*(uint64*)Ptr1, *(uint64*)Ptr2);
			break;

		case 16:
			Valswap(((uint64*)Ptr1)[0], ((uint64*)Ptr2)[0]);
			Valswap(((uint64*)Ptr1)[1], ((uint64*)Ptr2)[1]);
			break;

		default:
			break;
		}
	}

	static FORCEINLINE void* Malloc(SIZE_T Size)
	{
		return ::malloc(Size);
	}

	static FORCEINLINE void Free(void* Ptr)
	{
		::free(Ptr);
	}
};