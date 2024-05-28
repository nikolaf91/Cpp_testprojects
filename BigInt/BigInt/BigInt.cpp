#include "BigInt.h"
#include <stdexcept>

CBigInt::CBigInt()
{
}

CBigInt::~CBigInt()
{
	Detach();
}

void CBigInt::Detach()
{
	if (!m_ptr)
		return;

	if (m_ptr->aRefs.fetch_sub(1) == 1) {
		delete [] m_ptr->pData;
		delete m_ptr;
	}
	m_ptr = NULL;
}

void CBigInt::Attach(SBigInt* ptr)
{
	Detach();

	if(!ptr)
		return;

	ptr->aRefs.fetch_add(1);

	m_ptr = ptr;
}

size_t CBigInt::count(size_t uSize) {
	size_t uCount = uSize / sizeof(BIGINT_CHUNK);
	if (uSize % sizeof(BIGINT_CHUNK))
		uCount++;
	return uCount;
}

void CBigInt::AlocData(size_t uSize) 
{
	SBigInt* ptr = nullptr;

	if (uSize > 0)
	{
		ptr = new SBigInt();
		size_t uCount = CBigInt::count(uSize);
		ptr->pData = new BIGINT_CHUNK[uCount];
		ptr->uSize = uSize;
		memset(ptr->pData,0,uSize);
		ptr->aRefs.store(0);
	}

	Attach(ptr);
}

std::wstring CBigInt::ToHex(bool bLower) const
{
	std::wstring value;

	// declare the wstring value with the prefix 0x
	value += L"0x";
	for (size_t i = 0; i < m_ptr->uSize / sizeof(BIGINT_CHUNK); i++) {
		// get each chunk as an unsigned long
		unsigned long chunk = m_ptr->pData[i];
		// loop through each byte of the chunk
		for (int j = sizeof(BIGINT_CHUNK) - 1; j >= 0; j--) {
			// get each nibble (4 bits) of the byte
			int nibble = (chunk >> (j * 8 + 4)) & 0xF;
			// append the corresponding hex digit to the wstring
			value += (nibble < 10 ? L'0' + nibble : L'A' + nibble - 10);
			nibble = (chunk >> (j * 8)) & 0xF;
			value += (nibble < 10 ? L'0' + nibble : L'A' + nibble - 10);
		}
	}

	return value;
}

bool CBigInt::FromHex(const std::wstring& value)
{
	Detach(); // detach from any previous data

	try {
		// check if the string has a 0x prefix
		bool has_prefix = (value.size() >= 2 && value[0] == L'0' && (value[1] == L'x' || value[1] == L'X'));
		// get the start position of the hex digits
		size_t start = has_prefix ? 2 : 0;
		// get the number of hex digits
		size_t count = value.size() - start;
		// check if the number of hex digits is valid
		if (count == 0 || count > sizeof(BIGINT_CHUNK) * 2) {
			m_ptr = nullptr; // set m_ptr to null
			throw std::invalid_argument("Invalid number of hex digits"); // throw an exception
		}
		// convert the hex string to an unsigned long
		unsigned long chunk = std::stoul(value.substr(start), nullptr, 16);
		// allocate data for one chunk
		AlocData(sizeof(BIGINT_CHUNK));
		// copy the chunk to the data
		memcpy(m_ptr->pData, &chunk, sizeof(BIGINT_CHUNK));
		return true; // conversion successful
	}
	catch (std::invalid_argument& e) {
		// an invalid argument exception occurred
		m_ptr = nullptr; // set m_ptr to null
		throw std::invalid_argument("Invalid argument"); // throw an exception
	}
	catch (std::out_of_range& e) {
		// an out of range exception occurred
		m_ptr = nullptr; // set m_ptr to null
		throw std::out_of_range("Out of range"); // throw an exception
	} 
}

std::wstring CBigInt::ToBin() const
{
	std::wstring bin;
	size_t uCount = count(m_ptr->uSize);
	for (int i = uCount - 1; i >= 0; i--) {
		for (int j = sizeof(BIGINT_CHUNK) * 8 - 1; j >= 0 ; j--) {
			//
			// 1011101010 >> 2 = 0010111010 // unsigned case
			// 1011101010 >> 2 = 1110111010 // signed case negative number
			// 0011101010 >> 2 = 0010111010 // signed case positive number
			//
			// trinary operator: 
			// condition ? [return when contidiotn true] : [return wne condition false]
			//
			bin.append(((*(m_ptr->pData + i) >> j) & 1) ? L"1" : L"0");
		}
	}
	return bin;
}

bool CBigInt::FromBin(const std::wstring& value)
{
	int bitCount = value.size();
	size_t chunkCount = bitCount / (sizeof(BIGINT_CHUNK)*8);
	if (bitCount % (sizeof(BIGINT_CHUNK) * 8)) {
		chunkCount++;
	}
	AlocData(chunkCount*sizeof(BIGINT_CHUNK));
	for (int i = bitCount - 1; i >= 0; i--) {
		switch (value[(bitCount - 1) - i]) {
		case L'0':
			break;
		case L'1': {
			size_t chunkIndex = i / (sizeof(BIGINT_CHUNK)*8);
			size_t bitIndex = i % (sizeof(BIGINT_CHUNK) * 8);
			m_ptr->pData[chunkIndex] |= 1 << bitIndex;
		}
		    break;
		default:
			return false;
		}
	}
	return true;
}


int CBigInt::Compare(const CBigInt& other) const
{
	//
	// A typical comapre function returns 0 is the 2 objects match
	// 1 if one object here the "this" object is bigger then the other called "other"
	// and -1 when the other object called "other" is bigger the the "this" object
	//

	size_t lSize = m_ptr ? m_ptr->uSize : 0;
	size_t rSize = other.m_ptr ? other.m_ptr->uSize : 0;

	size_t uSize = 0;
	if (lSize < rSize)
		uSize = rSize;
	else
		uSize = lSize;

	size_t uCount = count(uSize);
	for (int i = uCount - 1; i >= 0; i--) 
	{

		BIGINT_CHUNK l = (m_ptr && m_ptr->uSize > i) ? m_ptr->pData[i] : 0;
		BIGINT_CHUNK r = (other.m_ptr && other.m_ptr->uSize > i) ? other.m_ptr->pData[i] : 0;


		if (l < r)
			return -1;
		if (l > r)
			return 1;
	}

	return 0;
}


// Add function for CBigInt class
CBigInt CBigInt::Add(const CBigInt& other) const {

	// Determine the maximum size between this and other
	size_t maxSize = std::max(this->m_ptr->uSize, other.m_ptr->uSize);

	CBigInt result;
	result.AlocData(maxSize);  // Allocate memory for result
	for (size_t i = 0; i < maxSize; i++) {
		BIGINT_CHUNK addend1 = (i < this->m_ptr->uSize) ? this->m_ptr->pData[i] : 0;
		BIGINT_CHUNK addend2 = (i < other.m_ptr->uSize) ? other.m_ptr->pData[i] : 0;
		// Calculate the sum
		BIGINT_CHUNK sum = addend1 + addend2;
		result.m_ptr->pData[i] = sum;
		if(addend1 || addend2 == 0) { 
			break;
		}
	}
	return result;
}

// Subtract function
CBigInt CBigInt::Subtract(const CBigInt& other) const {

	// Check if other is larger than this
	if (this->Compare(other) < 0) {
		throw std::runtime_error("Cannot subtract larger number from smaller one");
	}

	CBigInt result;
	result.AlocData(m_ptr->uSize);  // Allocate memory for result

	for (size_t i = 0; i < m_ptr->uSize; i++) {
		BIGINT_CHUNK subtrahend = (i < other.m_ptr->uSize) ? other.m_ptr->pData[i] : 0;
		result.m_ptr->pData[i] = m_ptr->pData[i] - subtrahend;
		if(subtrahend || other.m_ptr->pData[i] == 0) { 
			break;
		}
	}
	return result;
}

//Multiply function for CBigInt class
CBigInt CBigInt::Multiply(const CBigInt& other) const {

	// Check if either operand is zero
	if (*this == 0 || other == 0) {
		return CBigInt(0);
	}

	CBigInt result(0);  // Initialize result to 0
	CBigInt counter(*this);  // Copy this to counter

	// Repeatedly add other to result until counter is zero
	while (counter > 0) {
		result = result.Add(other);
		counter = counter.Subtract(CBigInt(1));  // Decrement counter by 1
	}

	return result;
}

// Divide function for CBigInt class
CBigInt CBigInt::Divide(const CBigInt& other) const {

	// Check if other is larger than this
	if (this->Compare(other) < 0) {
		return CBigInt(0);
	}

	// Check if other is zero
	if (other == 0) {
		throw std::runtime_error("Division by zero");
	}

	CBigInt result(0);  // Initialize result to 0
	CBigInt remainder(*this);  // Copy this to remainder

	// Repeatedly subtract other from remainder until remainder is less than other
	while (remainder.Compare(other) >= 0) {
		remainder = remainder.Subtract(other);
		result = result.Add(CBigInt(1));  // Increment result by 1
	}

	return result;
}

// Modulo two big integers and return a new CBigInt object
// If other is zero, throw an exception
CBigInt CBigInt::Modulo(const CBigInt& other) const {

	// Check if other is larger than this
	if (this->Compare(other) < 0) {
		return *this;
	}

	// Check if other is zero
	if (other == 0) {
		throw std::runtime_error("Modulo by zero");
	}

	CBigInt result(*this);  // Copy this to result

	// Repeatedly subtract other from result until result is less than other
	while (result.Compare(other) >= 0) {
		result = result.Subtract(other);
	}

	return result;
}



