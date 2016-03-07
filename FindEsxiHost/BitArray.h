
#ifndef CBITARRAY_H_12305
#define CBITARRAY_H_12305




//#pragma once

class CBitArray
{
private:
	int m_nCapacity;
	int m_nSize;
	int * m_pData;
public:
	CBitArray(int nSize=0);
	~CBitArray();
	bool Clear(int index);
	bool ClearAll();
	bool Set(int index, bool bit=true);
	bool Get(int index);

	int GetSize();
	bool SetSize(int nNewSize);

	bool isEmpty();
	bool isFull();
	//All clear except this location
	bool isOtherClear(int index);
	//All set except this location
	bool isOtherSet(int index);
};


#endif // !CBITARRAY_H_12305

