#pragma once

#include <vector>

template <typename T>
class CXString
{
public:
	CXString() {}

	CXString(const T* pStr, size_t uLength = -1)
	{
		if (uLength == -1)
			uLength = xstrlen(pStr);

		m = Create(uLength);
		if (pStr) {
			m->uLength = uLength;
			memcpy(m->pData, pStr, uLength * sizeof(T));
			m->pData[uLength] = 0;
		}
	}

	CXString(const CXString& Other)
	{
		*this = Other;
	}


	~CXString()
	{
		Release();
	}

	const char* C_Str() const {
		return m->pData;

	}

	CXString& operator = (const CXString& Other)
	{
		Release();

		if (!Other.m)
			return *this;

		m = Create(Other.m->uLength);
		if (Other.m->pData) {
			m->uLength = Other.m->uLength;
			memcpy(m->pData, Other.m->pData, Other.m->uLength * sizeof(T));
			m->pData[Other.m->uLength] = 0;
		}

		return *this;
	}

	int Compare(const CXString& Other) {

		size_t uLen = (m ? m->uLength : 0);
		size_t uOtherLen = (Other.m ? Other.m->uLength : 0);
		if (uLen != uOtherLen) {
			if(uLen < uOtherLen)
				return 1;
			return -1;
		}

		for (size_t i = 0; i < uLen; i++) {
			if (m->pData[i] != Other.m->pData[i]) {
				if(m->pData[i] < Other.m->pData[i])
					return 1;
				return -1;
			}
		}
		return 0;
	}

	bool operator == (const CXString& Other) {return Compare(Other) == 0;}
	bool operator != (const CXString& Other) {return Compare(Other) != 0;}
	bool operator > (const CXString& Other)  {return Compare(Other) > 0;}
	bool operator >= (const CXString& Other) {return Compare(Other) >= 0;}
	bool operator < (const CXString& Other)  {return Compare(Other) < 0;}
	bool operator <= (const CXString& Other) {return Compare(Other) <= 0;}

	CXString substr(size_t pos, size_t len = -1) const
	{
		if (pos >= m->uLength)
			return CXString();

		if (len == -1 || pos + len > m->uLength)
			len = m->uLength - pos;

		CXString sub_str;
		sub_str.m = Create(len);
		sub_str.m->uLength = len;
		memcpy(sub_str.m->pData, &m->pData[pos], len * sizeof(T));
		sub_str.m->pData[len] = 0;

		return sub_str;
	}

	CXString left(size_t count) const
	{
		return substr(0, count);
	}

	CXString right(size_t count) const
	{
		if(count > m->uLength)
			count = m->uLength;
		return substr(m->uLength - count);
	}

	static bool is_whitespace(const T cur_char) {
		if (cur_char == ' ' || cur_char == '\r' || cur_char == '\n' || cur_char == '\t' || cur_char == '\v' || cur_char == '\f') {
			return true;
		}
		return false;
	}

	CXString trim() const
	{
		if (!m)
			return CXString();

		size_t i = 0;
		while (i < m->uLength) {
			if (!is_whitespace(m->pData[i])) {
				break;
			}
			i++;
		}

		if (i == m->uLength)
			return CXString();

		size_t j = m->uLength - 1;
		while (j >= 0) {
			if (!is_whitespace(m->pData[j])) {
				break;
			}
			j--;
		}

		CXString result = substr(i, j - i + 1);
		return result;
	}

	bool empty() const {
		return !m || m->uLength == 0;
	}

	std::vector<CXString> Split(const CXString& separator, bool bTrim = false, bool bKeepEmpty = true) const
	{
		std::vector<CXString> strings;
		size_t last_offset = 0;
		while (last_offset < m->uLength)
		{
			size_t offset = find(separator, last_offset);
			if (offset == -1)
				offset = m->uLength;

			CXString str = substr(last_offset, offset - last_offset);

			if (bTrim == true)
				str = str.trim();

			if (bKeepEmpty || !str.empty())
				strings.push_back(str);

			last_offset = offset + 1;
		}

		return strings;
	}

	std::pair<CXString, CXString> Split2(const CXString& separator, bool bReverse = false, bool bTrim = false)
	{
		size_t pos = bReverse ? rfind(separator) : find(separator);
		if(pos == -1)
			return std::pair<CXString, CXString>(bTrim ? this->trim() : *this, CXString());

		return std::pair<CXString, CXString>(bTrim ? substr(0, pos).trim() : substr(0, pos), bTrim ? substr(pos+1).trim() : substr(pos+1));
	}


	static CXString Join(const std::vector<CXString>& strings, const CXString& separator)
	{
		CXString result;

		for (size_t i = 0; i < strings.size(); i++) {
			if (i > 0) {
				result.Append(separator);
			}
			result.Append(strings[i]);
		}

		return result;
	}

	void Append(const CXString& other)
	{
		if (!other.m) {
			return;
		}
		if (!m) {
			*this = other;
			return;
		}
		SXString* m_new = Create(m->uLength + other.m->uLength);
		m_new->uLength = m->uLength + other.m->uLength;
		memcpy(m_new->pData, m->pData, m->uLength * sizeof(T));
		memcpy(m_new->pData + m->uLength, other.m->pData, other.m->uLength * sizeof(T));

		Release();

		m = m_new;
		m->pData[m->uLength] = 0;

	}

	bool str_cmp(const char* pStr) const
	{
		size_t i = 0;
		for (; i < m->uLength; i++)
		{
			if (m->pData[i] != pStr[i])
				return false;
		
		}
		return i == m->uLength;
	}


	size_t find(const CXString& toFind, size_t offset = 0) const
	{
		for (size_t i = offset; i < m->uLength; i++)
		{
			if (toFind.str_cmp(&m->pData[i]))
				return i;
		}
		return -1;
	}

	size_t rfind(const CXString& toFind, size_t offset = -1) const
	{
		if(offset == -1)
			offset = m->uLength;
		for (size_t i = offset; i > 0; i--)
		{
			if (toFind.str_cmp(&m->pData[i - 1]))
				return i - 1;
		}
		return -1;
	}


private:
	struct SXString
	{
		size_t uLength; // in charcters
		size_t uSize; // in bytes including terminalting 0
		// todo: add a reference counter for CoW.
		T pData[0];
	};

	SXString* m = NULL;

	static SXString* Create(size_t uCapacity) // in chars
	{
		size_t uSize = (uCapacity + 1) * sizeof(T);
		SXString* p = (SXString*)malloc(sizeof(SXString) + uSize);
		p->uLength = 0;
		p->uSize = uSize;
		p->pData[0] = 0;
#ifdef _DEBUG
		memset(p->pData, 0, p->uSize);
#endif
		return p;
	}

	void Release()
	{
		if (m) {
			free(m);
			m = NULL;
		}
	}

	static size_t xstrlen(const T* ptr)
	{
		if (!ptr)
			return NULL;
		size_t i = 0;
		for (; *ptr++; i++);

		return i;
	}
};