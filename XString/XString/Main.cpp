#include <iostream>
#include "XString.h"
#include "XList.h"
#include "XMap.h"
#include <string>
#include <map>

class CIntHasher
{
public:
	static unsigned long Hash(int Key)
	{
		return (0xAAAAAAAA ^ Key);
	}
};


template <typename K, typename V, typename H>bool Equalmaps(const CXMap<K, V, H>&map1, const CXMap < K, V, H > &map2) {
	CXList<K> keys1 = map1.Keys();
	CXList<K> keys2 = map2.Keys();
	if (keys1.GetSize() != keys2.GetSize()) {
		return false;
	}
	for (size_t i = 0; i < keys1.GetSize(); ++i) {
		K Key = keys1.At(i);

		V value1 = map1.GetValue(Key);
		V value2 = map2.GetValue(Key);

		if (value1 != value2) {

			return false;
		}
	}
	return true;
}

int main()
{
	/*CXString<char> testNULL;
	CXString<char> testEmpty("");
	CXString<char> testA("a");
	CXString<char> testB("b");

	int testa = testNULL.Compare(testNULL);
	int testb = testNULL.Compare(testEmpty);
	int testc = testEmpty.Compare(testEmpty);
	int testd = testEmpty.Compare(testNULL);
	int teste = testNULL.Compare(testA);
	int testf = testEmpty.Compare(testA);
	int testg = testA.Compare(testB);
	int testh = testA.Compare(testA);*/

	CXString<char> test("test1 hello world");

	CXString<char> hello;
	hello = test.substr(6, 5);
	printf("%s", hello.C_Str());

	std::wstring wtest = L"test hello world";
	size_t pos = wtest.find(L"hel");
	if (pos != -1) {
		std::wstring whello = wtest.substr(pos, 5);
		printf("%S", whello.c_str());
	}

	CXString<char> trim_test = " \tbla blup\t1234\r\n";
	CXString<char> test2 = trim_test.trim();
	printf("%s", test2.C_Str());

	size_t post = test.find("hex");

	size_t rpos = std::wstring(L"test 1234 test 1234 test").rfind(L"1234");

	CXString<char> testS("test1 , hello,, world ");
	auto strings = testS.Split(",", true, false);

	CXString<char> testAppend1("test1");
	CXString<char> testAppend2("test2");

	testAppend1.Append(testAppend2);

	{
		CXString<char> test = "C:\\Path\\SubPath\\FileName.ext";
		auto A = test.Split2("\\");
		auto B = test.Split2("\\", true);
		
	}


	std::vector<CXString<char>> testVector;
	testVector.push_back("test1");
	testVector.push_back("test2");
	testVector.push_back("test3");

	CXString<char> joinTest = CXString<char>::Join(testVector, ",");

	printf("\n\n\n");

	{
		CXList<CXString<char>> MyList;
		
		MyList.Append("test3");
		MyList.Prepend("test1");
		MyList.Insert("test2", 1);

		auto XX = MyList.mid(1,1);
		printf("%s\n", XX.At(0).C_Str());

		MyList.Remove(1, 1);

		for (size_t i = 0; i < MyList.GetSize(); i++)
			printf("%s\n", MyList.At(i).C_Str());
	}

	{
		CXList<int> MyListI;
		MyListI.Append(1);
		MyListI.Append(2);
		MyListI.Append(3);

		for (size_t i = 0; i < MyListI.GetSize(); i++)
			printf("%d\n", MyListI.At(i));
	}

	CXMap<int, CXString<char>, CIntHasher> Map;
	Map.Insert(0, "Zero");
	Map.Insert(1, "One");
	Map.Insert(2, "Two");
	Map.Insert(3, "Three");
	Map.Insert(4, "x");
	Map.Insert(5, "x");
	Map.Insert(6, "x");
	Map.Insert(7, "x");
	Map.Insert(8, "x");
	Map.Insert(9, "x");
	Map.Insert(0, "x");
	Map.Insert(0, "a");
	Map.Insert(0, "b", EMapInsertMode::eNoReplace);
	Map.Insert(0, "c", EMapInsertMode::eNormal);
	Map.Insert(0, "d");
	Map.Insert(0, "e");
	Map.Insert(0, "f");

	printf("\n\nbefore remove:\n");
	Map.DumpTable([](const int& key) {printf("%d", key);}, [](const CXString<char>& value) {printf("%s", value.C_Str());});

	//Map.Remove(0);
	//Map.Remove(0, false);
	//Map.RemoveValue(0, "d");


	printf("\n\nSIterator test:\n");
	//for (CXMap<int, CXString<char>, CIntHasher>::SIterator I = Map.Begin(); I != Map.End();) 
	for (CXMap<int, CXString<char>, CIntHasher>::SIterator I = Map.Begin(); !I.AtEnd();) 
	//for (CXMap<int, CXString<char>, CIntHasher>::SIterator I = Map.Find(0); !I.AtEnd(); ) 
	{
		//printf("%d: %s\n", I->Key, I->Value.C_Str());

		CXMap<int, CXString<char>, CIntHasher>::SIterator O = I;
		++I;
		if (O->Value == "x") 
		{
			Map.RemoveValue(O->Key, O->Value);	
		}
	}

	printf("\n\nafter remove:\n");
	Map.DumpTable([](const int& key) {printf("%d", key);}, [](const CXString<char>& value) {printf("%s", value.C_Str());});
	 

	std::map<int, std::string> std_map;
	std_map[1] = "One";
	std_map[2] = "Two";
	std_map[3] = "Three";

	for (std::map<int, std::string>::iterator I = std_map.begin(); I != std_map.end(); ) {
		printf("%d: %s\n", (*I).first, (*I).second.c_str());
		bool ShouldRemove = false;
		// sShouldRemove in some cases executed
		if(ShouldRemove)
			I = std_map.erase(I);
		else
			I++;
	}


	auto testM = Map[2];

	CXMap<int, std::string, CIntHasher> myMap;
	
	myMap.Insert(1, "One");
	myMap.Insert(2, "Two");
	myMap.Insert(3, "Three");

	std::cout << "testkey2" << myMap.GetValue(2) << std::endl;
	CXMap<int, std::string, CIntHasher> copiedMap = myMap;
	CXMap<int, std::string, CIntHasher> assignedMap;
	assignedMap = myMap;

	std::cout << "test done"<< std::endl;

	CXList<int> keys = myMap.Keys();
	printf("keys of myMap:\n");
	for (size_t i = 0; i < keys.GetSize(); ++i) {
		printf("%i\n", keys.At(i));
	}

	bool equalmaps = Equalmaps(myMap, copiedMap);
	if (equalmaps) {
		printf("theyare.\n");
	}
	else {
		printf("theyarenot.\n");
		return 0;
	}


}
