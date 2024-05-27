#pragma once

enum class EMapInsertMode
{
	eNormal = 0,
	eMulti,
	eNoReplace
};

template <typename K, typename V, class H>
class CXMap {
protected:

	struct SEntry {
		unsigned long	Hash = 0;
		SEntry*		Next = NULL;
		K			Key;
		V			Value;
	};

	struct SXMap {
		size_t		uSize;
		size_t		uBucketCount;
		SEntry*		pBuckets[0];
	}* m = NULL;

public:

	struct SIterator {
		int			BucketIndex = 0;
		SEntry*		BucketItem = NULL;
		SXMap*		m = NULL;
		K*			Key = NULL;
		SIterator() {}
		SIterator(SIterator& Other) {
			*this = Other;
		}
		~SIterator() {
			if(Key) delete Key;
		}

		SIterator& operator = (SIterator& Other) {
			BucketIndex = Other.BucketIndex;
			BucketItem = Other.BucketItem;
			m = Other.m;
			if (Key) {
				delete Key;
				Key = NULL;
			}
			if (Other.Key) 
				Key = new K(*Other.Key);
			return *this;
		}

		SIterator& operator++() {
			if(BucketItem == NULL)
				return *this;
			SetNext(BucketItem->Next);
			return *this;
		}

		SIterator operator++(int i) {
			if(BucketItem == NULL)
				return *this;
			SIterator Old = *this;
			SetNext(BucketItem->Next);
			return Old;
		}

		bool operator != (const SIterator& other) {
			return this->m != other.m || this->BucketIndex != other.BucketIndex || this->BucketItem != other.BucketItem;
		}

		bool AtEnd() {
			return BucketItem == NULL;
		}

		SEntry* operator->() {
			return BucketItem;
		}

	protected: 
		friend class CXMap;
		void SetNext(SEntry* pNext) 
		{
			BucketItem = pNext;

			if (Key) {
				while(BucketItem && BucketItem->Key != *Key)
					BucketItem = BucketItem->Next;
				return;
			}

			while (BucketItem == NULL) {
				if(++BucketIndex >= m->uBucketCount)
					break;
				BucketItem = m->pBuckets[BucketIndex];
			}
		}
	};

	SIterator Begin()
	{
		SIterator begin;
		begin.m = m;
		begin.SetNext(m->pBuckets[0]);
		return begin;
	}

	SIterator End()
	{
		SIterator begin;
		begin.BucketIndex = m->uBucketCount;
		begin.m = m;
		return begin;
	}

public:
	CXMap() {};
	~CXMap() { Release(); };

	CXMap(const CXMap& other) {
		if (other.m) {
			m = Create(other.m->uBucketCount);
			for (size_t i = 0; i < other.m->uBucketCount; ++i) {
				SEntry* currentEntry = other.m->pBuckets[i];
				while (currentEntry != nullptr) {
					Insert(currentEntry->Key, currentEntry->Value);
					currentEntry = currentEntry->Next;
				}
			}
		}
	}

	CXMap& operator=(const CXMap& other){
		if (this != &other) {
			Release();

			if (other.m) {
				m = Create(other.m->uBucketCount);
				for (size_t i = 0; i < other.m->uBucketCount; ++i) {
					SEntry* currentEntry = other.m->pBuckets[i];
					while (currentEntry != nullptr) {
						Insert(currentEntry->Key, currentEntry->Value);
						currentEntry = currentEntry->Next;
					}
				}
			}
		}
		return*this;
	}

	bool Insert(const K& Key, const V& Value, EMapInsertMode Mode = EMapInsertMode::eMulti)
	{
		if (m && Mode != EMapInsertMode::eMulti)
		{
			SEntry* pEntry = GetPtr2(Key);
			if (pEntry)
			{
				if(Mode == EMapInsertMode::eNormal)
					pEntry->Value = Value;
				return false;
			}
		}

		InsertPtr(Key, Value);
		return true;
	}


	int Remove(const K& Key, bool bAll = true)
	{
		return RemoveImpl([](SEntry* pEntry, void* Param) {
			const K &Key = *(const K*)(Param);
			return pEntry->Key == Key;
		}, (void*)&Key, Key, bAll);
	}

	int RemoveValue(const K& Key, const V& Value, bool bAll = true)
	{
		struct SParams {
			SParams(const K& Key, const V& Value) : Key(Key), Value(Value) {}
			const K& Key;
			const V& Value;
		};
		SParams Params = SParams(Key, Value);

		return RemoveImpl([](SEntry* pEntry, void* Param) {
			SParams &Params = *(SParams*)(Param);
			return pEntry->Key == Params.Key && pEntry->Value == Params.Value;
		}, (void*)&Params, Key, bAll);
	}

	V GetValue(const K& Key) const
	{
		V* Value = GetPtr(Key);
		if (!Value)
			return V();
		return *Value;
	}

	CXList<K> Keys() const
	{
		CXList<K> keyList;
		for (size_t i = 0; i < m->uBucketCount; ++i) {
			SEntry* currentEntry =m->pBuckets[i];
			while (currentEntry != nullptr) {
				keyList.Append(currentEntry->Key);
				currentEntry = currentEntry ->Next;
			}
		}
		return keyList;
	}

	V* GetValuePtr(const K& Key, size_t Index) const
	{
		return GetPtr(Key, Index);
	}

	V& operator[](const K& Key)
	{
		V* Value = GetPtr(Key);
		if (!Value)
			Value = InsertPtr(Key, V());
		return *Value;
	}

	SIterator Find(const K& Key)
	{
		SIterator it = End();
		it.Key = new K(Key);
		if(!m)
			return it;

		unsigned long Hash = H::Hash(Key);

		size_t Index = (Hash % m->uBucketCount);

		SEntry* pEntry  = m->pBuckets[Index];
		while (pEntry != NULL) {

			if (pEntry->Key == Key)
				break;
			pEntry = pEntry->Next;
		}

		it.BucketIndex = Index;
		it.BucketItem = m->pBuckets[Index];

		return it;
	}

	void DumpTable(void(*printKey)(const K& Key), void(*printValue)(const V& Value))
	{
		printf("Table dump:\n");

		if (!m) {
			printf("[EMPTY]");
			return;
		}

		for (size_t i = 0; i < m->uBucketCount; i++) {
			printf("bucket: %d\n", i);

			SEntry* pEntry = m->pBuckets[i];
			while (pEntry != NULL) {
				printf("  ");
				printKey(pEntry->Key);
				printf(" (0x%08X): ", pEntry->Hash);
				printValue(pEntry->Value);
				pEntry = pEntry->Next;
				printf("\n");
			}
		}

		printf("+++\n");
	}

protected:

	V* InsertPtr(const K& Key, const V& Value)
	{
		SEntry* pEntry = new SEntry();
		pEntry->Hash = H::Hash(Key);
		pEntry->Key = Key;
		pEntry->Value = Value;

		if (!m || (m->uBucketCount < (m->uSize/3) + 1))
		{
			size_t newBucketCount = m ? m->uBucketCount * 2 : 1;
			ResizeTable(newBucketCount);
		}

		InsertEntry(pEntry);
		m->uSize++;

		return &pEntry->Value;
	}

	V* GetPtr(const K& Key, size_t ToSkip = 0) const
	{
		SEntry* pEntry = GetPtr2(Key, ToSkip);
		return pEntry ? &pEntry->Value : NULL;
	}

	SEntry* GetPtr2(const K& Key, size_t ToSkip = 0) const
	{
		if(!m)
			return NULL;

		unsigned long Hash = H::Hash(Key);

		size_t Index = (Hash % m->uBucketCount);

		SEntry* pEntry  = m->pBuckets[Index];
		while (pEntry != NULL) {

			if (pEntry->Key == Key) {
				if(ToSkip == 0)
					return pEntry;
				else
					--ToSkip;
			}

			pEntry = pEntry->Next;
		}

		return NULL;
	}

	int RemoveImpl(bool(*Match)(SEntry* pEntry, void* Param), void* Param, const K &Key, bool bAll = true)
	{
		int RemoveCount = 0;
		if(!m)
			return RemoveCount;

		unsigned long Hash = H::Hash(Key);

		size_t Index = (Hash % m->uBucketCount);

		SEntry** pEntry = &m->pBuckets[Index];
		while (*pEntry != NULL) {
			if (!Match(*pEntry, Param)) {
				pEntry = &(*pEntry)->Next;
				continue;
			}
			SEntry* pOld = *pEntry;
			*pEntry = (*pEntry)->Next;
			delete pOld;
			RemoveCount++;
			if(!bAll)
				break;
		}

		return RemoveCount;
	}

	void ResizeTable(size_t newBucketCount)
	{
		SXMap* new_m = Create(newBucketCount);
		
		SEntry* pHead = NULL;
		if (m) {
			new_m->uSize = m->uSize;

			for (size_t i = m->uBucketCount; i--; ) {
				SEntry* &pEntry = m->pBuckets[i];
				while (pEntry != NULL) {
					SEntry* pNext = pEntry->Next;
					pEntry->Next = pHead;
					pHead = pEntry;
					pEntry = pNext;
				}
			}
			m->uBucketCount = 0;

			Release();
		}

		m = new_m;

		while (pHead != NULL) {
			SEntry* pNext = pHead->Next;
			InsertEntry(pHead);
			pHead = pNext;
		}
	}

	void InsertEntry(SEntry* pEntry)
	{
		size_t Index = (pEntry->Hash % m->uBucketCount);

		SEntry* &pBucket = m->pBuckets[Index];
		pEntry->Next = pBucket;
		pBucket = pEntry;
	}

	SXMap* Create(size_t NewCount) {
		SXMap* new_m = (SXMap*)malloc(sizeof(SXMap) + sizeof(SEntry*) * NewCount);
		new_m->uSize = 0;
		new_m->uBucketCount = NewCount;
		memset(new_m->pBuckets, 0, sizeof(SEntry*) * NewCount);
		return new_m;
	}

	void Release()
	{
		if(!m)
			return;

		for (size_t i = m->uBucketCount; i--; ) {
			SEntry* &pEntry = m->pBuckets[i];
			while (pEntry != NULL) {
				SEntry* pNext = pEntry->Next;
				delete pEntry;
				pEntry = pNext;
			}
		}

		free(m);
		m = NULL;
	}
};