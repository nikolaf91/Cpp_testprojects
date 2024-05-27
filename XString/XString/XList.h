#pragma once

template <typename T>
class CXList {
public:
	CXList() {};
	~CXList() { Release(); };

	void Append(const T& Item) {
		Expand(GetSize() + 1);
		if (sizeof(T) <= sizeof(T*))
			new (&m->aData[m->uSize++]) T(Item);
		else
			m->aData[m->uSize++] = new T(Item);
	}

	void Prepend(const T& Item) {
		Insert(Item, 0);
	}

	void Insert(const T& Item, size_t Index) {
		Expand(GetSize() + 1);
		memmove(&m->aData[Index + 1], &m->aData[Index], sizeof(T) * (m->uSize++ - Index));
		if (sizeof(T) <= sizeof(T*))
			new (&m->aData[Index]) T(Item);
		else
			m->aData[Index] = new T(Item);
	}

	void Remove(size_t Index, size_t Count = 1) {
		if(Index >= m->uSize)
			return;
		if(Index + Count > m->uSize)
			Count = m->uSize - Index;

		for (size_t i = Index; i < Index + Count; i++)
		{
			if (sizeof(T) <= sizeof(T*))
				((T*)&m->aData[i])->~T();
			else
				delete m->aData[i];
		}

		memmove(&m->aData[Index], &m->aData[Index + Count], sizeof(T) * (m->uSize - (Index + Count)));

		m->uSize -= Count;
	}


	const T& At(size_t Index) const {
		if (!TestIndex(Index))
			throw "Invalid Index";
		if (sizeof(T) <= sizeof(T*))
			return *(T*)&m->aData[Index];
		else
			return *m->aData[Index];
	}
	T& At(size_t Index) {
		if (!TestIndex(Index))
			throw "Invalid Index";
		if (sizeof(T) <= sizeof(T*))
			return *(T*)&m->aData[Index];
		else
			return *m->aData[Index];
	}
	size_t GetSize() const{ return m ? m->uSize : 0; }

	CXList(const CXList& other) {

		m = Create(other.GetSize());
		for (size_t i = 0; i < other.GetSize(); ++i) {
			Append(other.At(i));
		}
	}


	CXList& operator=(const CXList& other) {

		if (this == &other) {
			return;
		}
		Release();

		m = Create(other.GetSize());
		for (size_t i = 0; i < other.GetSize(); ++i) {
			Append(other.At(i));
		}

		return *this;
	}

	T& First() {
		if(!m || m->uSize == 0)
			throw "error empty list";
		return m->aData[0];
	}

	T& Last() {
		if(!m || m->uSize == 0)
			throw "error empty list";
		return m->aData[m->uSize-1];
	}

	T TakeFirst()
	{
		T = First();
		Remove(0);
		return T;
	}

	T TakeLast()
	{
		T = Last();
		Remove(m->uSize - 1); // if size would ben 0 Last would have faild and thrown an exception
		return T;
	}

	CXList<T> mid(size_t pos, size_t count) const
	{
		CXList<T> list;

		if (pos >= m->uSize)
			return list;

		if (count == -1 || pos + count > m->uSize)
			count = m->uSize - pos;

		for (size_t i = pos; i < pos + count; i++)
			list.Append(this->At(i));

		return list;
	}

	CXList<T> left(size_t count) const
	{
		return mid(0, count);
	}

	CXList<T> right(size_t count) const
	{
		if(count > m->uLength)
			count = m->uLength;
		return mid(m->uLength - count);
	}

	size_t find(bool(*match)(const T& Item, void* param), size_t Index = 0, void* param = NULL)
	{
		return -1;
	}
	size_t rfind(bool(*match)(const T& Item, void* param), size_t Index = -1, void* param = NULL)
	{
		return -1;
	}

protected:
	struct SXList {
		size_t uSize;
		size_t uCapacity;
		T* aData[0];
	}* m = NULL;

	SXList* Create(size_t NewCapacity) {
		SXList* new_m = (SXList*)malloc(sizeof(SXList) + sizeof(T*) * NewCapacity);
		new_m->uSize = 0;
		new_m->uCapacity = NewCapacity;
		return new_m;
	}

	void Expand(size_t NewCapacity)
	{
		if (!m) {
			m = Create(NewCapacity);
			return;
		}
		if(m->uCapacity >= NewCapacity)
			return;
		m = (SXList*)realloc(m, sizeof(SXList) + sizeof(T*) * NewCapacity);
		m->uCapacity = NewCapacity;
	}

	void Release()
	{
		if (!m)
			return;

		for (size_t i = 0; i < m->uSize; i++)
		{
			if (sizeof(T) <= sizeof(T*))
				((T*)&m->aData[i])->~T();
			else
				delete m->aData[i];
		}

		free(m);
		m = NULL;
	}

	bool TestIndex(size_t Index) const
	{
		if(!m || m->uSize <= Index)
			return false;
		return true;
	}
};