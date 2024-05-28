#pragma once

#include <atomic>
#include <string>

//#define BIGINT_CHUNK	unsigned long
#define BIGINT_CHUNK	unsigned short
//#define BIGINT_CHUNK	unsigned char

class CBigInt
{
public:
	CBigInt();
	template <typename T>
	CBigInt(T value) {
		AlocData(sizeof(T));
		memcpy(m_ptr->pData, &value, sizeof(T));
	}
	CBigInt(const CBigInt& other) {
		Attach(other.m_ptr);
	}
	virtual ~CBigInt();

	CBigInt operator= (const CBigInt& other) {
		Attach(other.m_ptr);
		return *this;
	}

	CBigInt			Add(const CBigInt& other) const; //{ return CBigInt(0); }
	CBigInt			Subtract(const CBigInt& other) const; //{ return CBigInt(0); }

	CBigInt operator+ (const CBigInt& other) const { return Add(other); }
	CBigInt& operator+= (const CBigInt& other) { *this = Add(other); return *this; }
	CBigInt operator- (const CBigInt& other) const { return Subtract(other); }
	CBigInt& operator-= (const CBigInt& other) { *this = Subtract(other); return *this; }

	CBigInt			Multiply(const CBigInt& other) const; //{ return CBigInt(0); }
	CBigInt			Divide(const CBigInt& other) const; //{ return CBigInt(0); }
	CBigInt			Modulo(const CBigInt& other) const; //{ return CBigInt(0); }

	CBigInt operator* (const CBigInt& other) const { return Multiply(other); }
	CBigInt& operator*= (const CBigInt& other) { *this = Multiply(other); return *this; }
	CBigInt operator/ (const CBigInt& other) const { return Divide(other); }
	CBigInt& operator/= (const CBigInt& other) { *this = Divide(other); return *this; }
	CBigInt operator% (const CBigInt& other) const { return Modulo(other); }
	CBigInt& operator%= (const CBigInt& other) { *this = Modulo(other); return *this; }

	int Compare(const CBigInt& other) const;

	bool operator <		(const CBigInt& other) const {return Compare(other) < 0;}
	bool operator >		(const CBigInt& other) const {return Compare(other) > 0;}
	bool operator ==	(const CBigInt& other) const {return Compare(other) == 0;}
	bool operator <=	(const CBigInt& other) const {return Compare(other) <= 0;}
	bool operator >=	(const CBigInt& other) const {return Compare(other) >= 0;}
	bool operator !=	(const CBigInt& other) const {return Compare(other) != 0;}

	/*
	I added a public member function that returns the pointer to the data because otherwise
	I get an error where access to f.e. the potected hex1.m_ptr->pData[0] in main.cpp is not granted
	*/
	const BIGINT_CHUNK* GetData() const {
		return m_ptr->pData;
	}

	std::wstring ToHex(bool bLower = false) const;
	bool FromHex(const std::wstring& value);

	std::wstring ToBin() const;
	bool FromBin(const std::wstring& value);

protected:

	static size_t count(size_t uSize);

	struct SBigInt
	{
		BIGINT_CHUNK* pData;
		size_t uSize; // in bytes
		mutable std::atomic<int> aRefs;
	};

	SBigInt* m_ptr = nullptr;

	void Detach();
	void Attach(SBigInt* ptr);
	void AlocData(size_t uSize);
};

