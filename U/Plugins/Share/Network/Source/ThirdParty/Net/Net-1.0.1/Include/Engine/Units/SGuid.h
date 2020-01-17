#pragma once
#include "Engine/HAL/SPlatform.h"

/**
* Enumerates known GUID formats.
*/
enum class EGuidFormat
{
	/**
	* 32 digits.
	*
	* For example: "00000000000000000000000000000000"
	*/
	Digits,

	/**
	* 32 digits separated by hyphens.
	*
	* For example: 00000000-0000-0000-0000-000000000000
	*/
	DigitsWithHyphens,

	/**
	* 32 digits separated by hyphens and enclosed in braces.
	*
	* For example: {00000000-0000-0000-0000-000000000000}
	*/
	DigitsWithHyphensInBraces,

	/**
	* 32 digits separated by hyphens and enclosed in parentheses.
	*
	* For example: (00000000-0000-0000-0000-000000000000)
	*/
	DigitsWithHyphensInParentheses,

	/**
	* Comma-separated hexadecimal values enclosed in braces.
	*
	* For example: {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
	*/
	HexValuesInBraces,

	/**
	* This format is currently used by the FUniqueObjectGuid class.
	*
	* For example: 00000000-00000000-00000000-00000000
	*/
	UniqueObjectGuid,
};

struct SGuid
{
public:

	/** Default constructor. */
	SGuid()
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{ }

	/**
	* Creates and initializes a new GUID from the specified components.
	*
	* @param InA The first component.
	* @param InB The second component.
	* @param InC The third component.
	* @param InD The fourth component.
	*/
	SGuid(uint32 InA, uint32 InB, uint32 InC, uint32 InD)
		: A(InA), B(InB), C(InC), D(InD)
	{ }

public:

	/**
	* Compares two GUIDs for equality.
	*
	* @param X The first GUID to compare.
	* @param Y The second GUID to compare.
	* @return true if the GUIDs are equal, false otherwise.
	*/
	friend bool operator==(const SGuid& X, const SGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}

	/**
	* Compares two GUIDs for inequality.
	*
	* @param X The first GUID to compare.
	* @param Y The second GUID to compare.
	* @return true if the GUIDs are not equal, false otherwise.
	*/
	friend bool operator!=(const SGuid& X, const SGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}

	/**
	* Compares two GUIDs.
	*
	* @param X The first GUID to compare.
	* @param Y The second GUID to compare.
	* @return true if the first GUID is less than the second one.
	*/
	friend bool operator<(const SGuid& X, const SGuid& Y)
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
			((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
				((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false)))))))); //-V583
	}

	/**
	* Provides access to the GUIDs components.
	*
	* @param Index The index of the component to return (0...3).
	* @return The component.
	*/
	uint32& operator[](int32 Index)
	{
		if (Index < 0 || Index >= 4)
		{
			return A;
		}

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

	/**
	* Provides read-only access to the GUIDs components.
	*
	* @param Index The index of the component to return (0...3).
	* @return The component.
	*/
	const uint32& operator[](int32 Index) const
	{
		if (Index < 0 || Index >= 4)
		{
			return A;
		}

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

public:
	/**
	* Invalidates the GUID.
	*
	* @see IsValid
	*/
	void Invalidate()
	{
		A = B = C = D = 0;
	}

	/**
	* Checks whether this GUID is valid or not.
	*
	* A GUID that has all its components set to zero is considered invalid.
	*
	* @return true if valid, false otherwise.
	* @see Invalidate
	*/
	bool IsValid() const
	{
		return ((A | B | C | D) != 0);
	}

	/**
	* Converts this GUID to its string representation.
	*
	* @return The string representation.
	*/
	char* ToString() const
	{
		return ToString(EGuidFormat::Digits);
	}

	/**
	* Converts this GUID to its string representation using the specified format.
	*
	* @param Format The string format to use.
	* @return The string representation.
	*/
	char* ToString(EGuidFormat Format) const;


public:

	/**
	* Returns a new GUID.
	*
	* @return A new GUID.
	*/
	static SGuid NewGuid();


	//private:
public:

	/** Holds the first component. */
	uint32 A;

	/** Holds the second component. */
	uint32 B;

	/** Holds the third component. */
	uint32 C;

	/** Holds the fourth component. */
	uint32 D;
};