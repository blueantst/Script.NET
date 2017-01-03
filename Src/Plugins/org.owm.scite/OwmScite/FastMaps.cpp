// FastMaps.cpp: implementation of the FastMaps class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FastMaps.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=========================================================================
// Derived tree class: CCStringToString
//=========================================================================

CCStringToString::~CCStringToString()
{
    RemoveAll();
}

void CCStringToString::onDeleteKey(void*& keyPtr)
{
    free(keyPtr);
}

void CCStringToString::onDeleteData(void*& dataPtr)
{
    free(dataPtr);
}

int CCStringToString::onSetKey(void*& keyPtr, void* key)
{
    keyPtr = _tcsdup((const TCHAR*)key);
    return keyPtr ? 1 : 0;
}

int CCStringToString::onSetData(void*& dataPtr, void* data)
{
    dataPtr = _tcsdup((const TCHAR*)data);
    return dataPtr ? 1 : 0;
}

CCStringToString::relativeKeyValue 
    CCStringToString::onCompareKeys(void* key1, void* key2)
{
    return (relativeKeyValue)strcmp((const TCHAR*)key1, (const TCHAR*)key2);
}

const char* CCStringToString::onGetKeyName(void* keyPtr)
{
    return (const char*)keyPtr;
}

//==========================================================================
// Derived tree class: CCStringToULong
//==========================================================================

CCStringToULong::~CCStringToULong()
{
    RemoveAll();
}

void CCStringToULong::onDeleteKey(void*& keyPtr)
{
    free(keyPtr);
}

void CCStringToULong::onDeleteData(void*& /*dataPtr*/)
{
}

int CCStringToULong::onSetKey(void*& keyPtr, void* key)
{
    keyPtr = _tcsdup((const TCHAR*)key);
    return keyPtr ? 1 : 0;
}

int CCStringToULong::onSetData(void*& dataPtr, void* data)
{
    dataPtr = data;
    return 1;
}

CCStringToULong::relativeKeyValue 
    CCStringToULong::onCompareKeys(void* key1, void* key2)
{
    return (relativeKeyValue)strcmp((const TCHAR*)key1, (const TCHAR*)key2);
}

const TCHAR* CCStringToULong::onGetKeyName(void* keyPtr)
{
    return (const TCHAR*)keyPtr;
}

//==========================================================================
// Derived tree class: CCULongToString
//==========================================================================

CCULongToString::~CCULongToString()
{
    RemoveAll();
}

void CCULongToString::onDeleteKey(void*& /*keyPtr*/)
{
}

void CCULongToString::onDeleteData(void*& dataPtr)
{
    free(dataPtr);
}

int CCULongToString::onSetKey(void*& keyPtr, void* key)
{
    keyPtr = key;
    return 1;
}

int CCULongToString::onSetData(void*& dataPtr, void* data)
{
    dataPtr = _tcsdup((const TCHAR*)data);
    return dataPtr ? 1 : 0;
}

CCULongToString::relativeKeyValue 
    CCULongToString::onCompareKeys(void* key1, void* key2)
{
    if ((unsigned long)key1 < (unsigned long)key2)
        return less;
    if ((unsigned long)key1 == (unsigned long)key2)
        return equal;
    return greater;
}

static char keyName_tULongToString[30];
const char* CCULongToString::onGetKeyName(void* keyPtr)
{
    sprintf(keyName_tULongToString, "%lu", (unsigned long)keyPtr);
    return (const char*)keyName_tULongToString;
}

//==========================================================================
// Derived tree class: CCULongToULong
//==========================================================================

CCULongToULong::~CCULongToULong()
{
    RemoveAll();
}

void CCULongToULong::onDeleteKey(void*& /*keyPtr*/)
{
}

void CCULongToULong::onDeleteData(void*& /*dataPtr*/)
{
}

int CCULongToULong::onSetKey(void*& keyPtr, void* key)
{
    keyPtr = key;
    return 1;
}

int CCULongToULong::onSetData(void*& dataPtr, void* data)
{
    dataPtr = data;
    return 1;
}

CCULongToULong::relativeKeyValue 
    CCULongToULong::onCompareKeys(void* key1, void* key2)
{
    if ((unsigned long)key1 < (unsigned long)key2)
        return less;
    if ((unsigned long)key1 == (unsigned long)key2)
        return equal;
    return greater;
}

static char keyName_tULongToULong[30];
const char* CCULongToULong::onGetKeyName(void* keyPtr)
{
    sprintf(keyName_tULongToULong, "%lu", (unsigned long)keyPtr);
    return (const char*)keyName_tULongToULong;
}

//==========================================================================
// Derived tree class: CCLongToString
//==========================================================================

CCLongToString::~CCLongToString()
{
    RemoveAll();
}

void CCLongToString::onDeleteKey(void*& /*keyPtr*/)
{
}

void CCLongToString::onDeleteData(void*& dataPtr)
{
    free(dataPtr);
}

int CCLongToString::onSetKey(void*& keyPtr, void* key)
{
    keyPtr = key;
    return 1;
}

int CCLongToString::onSetData(void*& dataPtr, void* data)
{
    dataPtr = _tcsdup((const TCHAR*)data);
    return dataPtr ? 1 : 0;
}

CCLongToString::relativeKeyValue 
    CCLongToString::onCompareKeys(void* key1, void* key2)
{
    if ((long)key1 < (long)key2)
        return less;
    if ((long)key1 == (long)key2)
        return equal;
    return greater;
}

static char keyName_tLongToString[30];
const char* CCLongToString::onGetKeyName(void* keyPtr)
{
    sprintf(keyName_tLongToString, "%li", (long)keyPtr);
    return (const char*)keyName_tLongToString;
}

//==========================================================================
// Derived tree class: CCLongToULong
//==========================================================================

CCLongToULong::~CCLongToULong()
{
    RemoveAll();
}

void CCLongToULong::onDeleteKey(void*& /*keyPtr*/)
{
}

void CCLongToULong::onDeleteData(void*& /*dataPtr*/)
{
}

int CCLongToULong::onSetKey(void*& keyPtr, void* key)
{
    keyPtr = key;
    return 1;
}

int CCLongToULong::onSetData(void*& dataPtr, void* data)
{
    dataPtr = data;
    return 1;
}

CCLongToULong::relativeKeyValue 
    CCLongToULong::onCompareKeys(void* key1, void* key2)
{
    if ((long)key1 < (long)key2)
        return less;
    if ((long)key1 == (long)key2)
        return equal;
    return greater;
}

static char keyName_tLongToULong[30];
const char* CCLongToULong::onGetKeyName(void* keyPtr)
{
    sprintf(keyName_tLongToULong, "%li", (long)keyPtr);
    return (const char*)keyName_tLongToULong;
}
