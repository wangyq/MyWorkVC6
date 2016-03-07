#include "stdafx.h"
#include "BitArray.h"

#define BITARRAY_BASESIZE  (sizeof(int)*8)    //8-bit
#define BITARRAY_ARRAYSIZE(nSize)  (((nSize) + BITARRAY_BASESIZE -1 )/BITARRAY_BASESIZE)
#define BITARRAY_ARRAYINDEX(index)  ((index)/BITARRAY_BASESIZE)
#define BITARRAY_ARRAYOFFSET(index)  ((index)%BITARRAY_BASESIZE)

//#define GETMASK(offset)  (~(1<<(offset)))
#define BITARRAY_GETVALUE(val,offset)   ( ((val)>>(offset)) & 0x1 )

#define BITARRAY_CLEARMASK(offset)  (~(1<<(offset)))
#define BITARRAY_CLEARVALUE(val,offset)  ((val) = BITARRAY_CLEARMASK(offset) & (val) )
#define BITARRAY_SETVALUE(val,offset,bit)  ((val) = ( ( (bit)<<(offset) ) | BITARRAY_CLEARVALUE(val,offset) ))

#define ALLBITSET (-1)

CBitArray::CBitArray(int nSize):m_nSize(0),m_pData(NULL),m_nCapacity(0)
{
	if (nSize > 0)
	{
		m_nSize = nSize;
	}
	if (m_nSize > 0)
	{
		int nArraySize = BITARRAY_ARRAYSIZE(m_nSize);

		m_pData = new int[nArraySize];
		if (NULL != m_pData)
		{
			for (int i = 0; i < nArraySize; i++) {
				m_pData[i] = 0;  //default is zero!
			}
			m_nCapacity = nArraySize;
		}

	}
}


CBitArray::~CBitArray()
{
	if (NULL != m_pData) {
		delete[] m_pData;
	}
}

int CBitArray::GetSize()
{
	return m_nSize;
}

bool CBitArray::SetSize(int nNewSize)
{
	if (nNewSize < 0) return false;

	//bool bOK = false;
	int oldLen = BITARRAY_ARRAYSIZE(m_nSize);
	int newLen = BITARRAY_ARRAYSIZE(nNewSize);

	if (newLen > m_nCapacity) //need resize capacity
	{
		int* pNewData = new int[newLen];
		if (NULL == pNewData) {
			return false;
		}

		int i = 0;
		while (i < oldLen) {
			pNewData[i] = m_pData[i]; //copy current data!
			i++;
		}
		while (i<newLen)
		{
			pNewData[i] = 0;
			i++;
		}
		//==
		delete[] m_pData;
		m_pData = pNewData;
		m_nCapacity = newLen;  //resize capacity
	}
	else if( nNewSize < m_nSize ){//no need resize
		int i = 0;
		for ( i = newLen; i < oldLen; i++) {
			m_pData[i] = 0; //
		}
		int start = BITARRAY_ARRAYOFFSET(nNewSize);
		for ( i = start; i < BITARRAY_BASESIZE; i++) {
			m_pData[newLen - 1] &= (~(1 << i));
		}
	}
	else { //nNewSize >= m_nSize

	}

	m_nSize = nNewSize;
	//return bOK;
	return true;
}
bool CBitArray::Clear(int index)
{
	return Set(index,false);
}
bool CBitArray::ClearAll()
{
	int nArraySize = m_nCapacity;//BITARRAY_ARRAYSIZE(m_nSize);
	for (int i = 0; i < nArraySize; i++) {
		m_pData[i] = 0;  //default is zero!
	}
	return true;
}

bool CBitArray::Set(int index, bool bit /*= true*/)
{
	if (index < 0 || index >= m_nSize) return false;
	int val = bit ? 1: 0;
	int pos = BITARRAY_ARRAYINDEX(index);
	int offset = BITARRAY_ARRAYOFFSET(index);

	BITARRAY_SETVALUE(m_pData[pos], offset,val);

	return true;
}

bool CBitArray::Get(int index)
{
	if (index < 0 || index >= m_nSize) return false;
	int pos = BITARRAY_ARRAYINDEX(index);
	int offset = BITARRAY_ARRAYOFFSET(index);

	int val = BITARRAY_GETVALUE(m_pData[pos], offset);

	return val==1;
}



bool CBitArray::isEmpty()
{
	int size = BITARRAY_ARRAYSIZE(m_nSize);
	for (int i = 0; i < size; i++)
	{
		if (m_pData[i] != 0) {
			return false;
		}
	}
	return true;
}
bool CBitArray::isFull()
{
	int size = BITARRAY_ARRAYSIZE(m_nSize);
	for (int i = 0; i < size -1; i++)
	{
		if (m_pData[i] != ALLBITSET) {
			return false;
		}
	}
	int offset = BITARRAY_ARRAYOFFSET(m_nSize);// non-exist last bit from 0-31
	if (offset > 0)
	{
		int isSet = (1 << (offset) ) - 1; //make carefully
		if (m_pData[size - 1] != isSet) return false;
	}
	else {
		if (m_pData[size-1] != ALLBITSET) {
			return false;
		}
	}
	return true;
}
bool CBitArray::isOtherClear(int index)
{
	if (index < 0 || index >= m_nSize) return false;

	int pos = BITARRAY_ARRAYINDEX(index);
	int offset = BITARRAY_ARRAYOFFSET(index);
	int size = BITARRAY_ARRAYSIZE(m_nSize);

	for (int i = 0; i < size ; i++)
	{
		if (i == pos) continue;
		if (m_pData[i] != 0) return false;
	}
	int isOtherClear = ~(1 << offset);

	return 0 == (m_pData[pos] & isOtherClear);

}
bool CBitArray::isOtherSet(int index)
{
	if (index < 0 || index >= m_nSize) return false;

	int pos = BITARRAY_ARRAYINDEX(index);
	int offset = BITARRAY_ARRAYOFFSET(index);
	int size = BITARRAY_ARRAYSIZE(m_nSize);
	int offsetLast = BITARRAY_ARRAYOFFSET(m_nSize); // non-exist last bit from 0-31

	for (int i = 0; i < size -1; i++)
	{
		if (i == pos) continue;
		if (m_pData[i] != ALLBITSET)
			return false;
	}
	if (pos == (size - 1) )
	{
		int set = ALLBITSET;

		if (offsetLast > 0)
		{
			set = (1 << (offsetLast )) - 1;
		}
		int mask = (1 << offset);
		if (set != (mask | m_pData[size - 1]))
		{
			return false;
		}

	}
	else {//pos != (size-1)
		int set = ALLBITSET;

		if (offsetLast > 0)
		{
			set = (1 << (offsetLast )) - 1;
		}
		if (set != m_pData[size - 1])
		{
			return false;
		}
		//== pos ==
		int mask = (1 << offset);
		if ((m_pData[pos] | mask) != ALLBITSET) {
			return false;
		}
	}
	return true;
}

