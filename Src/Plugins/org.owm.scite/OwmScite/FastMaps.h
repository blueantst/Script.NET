// FastMaps.h: interface for the FastMaps class.
// Based on codes bearing the copyright:
//       Copyright Dundas Software Ltd. 1998, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FASTMAPS_H__6AB2016E_7D34_11D4_8736_00909910D47A__INCLUDED_)
#define AFX_FASTMAPS_H__6AB2016E_7D34_11D4_8736_00909910D47A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BalanceTree.h"

//==========================================================================
// Derived tree class: string key, string data
//==========================================================================
class CCStringToString : public CBalanceTree
{
public:
    virtual ~CCStringToString();
    inline int Set(const TCHAR* key, const TCHAR* data);
    inline const TCHAR* Get(const TCHAR* key);
    inline const TCHAR* GetKey(POSITION pos);
    inline const TCHAR* GetData(POSITION pos);
    inline int SetData(POSITION pos, const TCHAR* data);
    inline POSITION Find(const TCHAR* key);
    inline int Remove(const TCHAR* key);
    inline int Remove(POSITION& pos);
    
protected:
    // These members required in all derived classes:
    virtual void onDeleteKey(void*& keyPtr);
    virtual void onDeleteData(void*& dataPtr);
    virtual int onSetKey(void*& keyPtr, void* key);
    virtual int onSetData(void*& dataPtr, void* data);
    virtual relativeKeyValue onCompareKeys(void* key1, void* key2);
    
    // This member provided for debugging support:
    virtual const char* onGetKeyName(void* keyPtr);
};

//==========================================================================
// Derived tree class: string key, unsigned long data
//==========================================================================
class CCStringToULong : public CBalanceTree
{
public:
    virtual ~CCStringToULong();
    inline int Set(const TCHAR* key, unsigned long data);
    inline unsigned long Get(const TCHAR* key);
    inline const TCHAR* GetKey(POSITION pos);
    inline unsigned long GetData(POSITION pos);
    inline int SetData(POSITION pos, unsigned long data);
    inline POSITION Find(const TCHAR* key);
    inline int Remove(const TCHAR* key);
    inline int Remove(POSITION& pos);
    
protected:
    // These members required in all derived classes:
    virtual void onDeleteKey(void*& keyPtr);
    virtual void onDeleteData(void*& dataPtr);
    virtual int onSetKey(void*& keyPtr, void* key);
    virtual int onSetData(void*& dataPtr, void* data);
    virtual relativeKeyValue onCompareKeys(void* key1, void* key2);
    
    // This member provided for debugging support:
    virtual const char* onGetKeyName(void* keyPtr);
};

//==========================================================================
// Derived tree class: unsigned long key, string data
//==========================================================================
class CCULongToString : public CBalanceTree
{
public:
    virtual ~CCULongToString();
    inline int Set(unsigned long key, const TCHAR* data);
    inline const TCHAR* Get(unsigned long key);
    inline unsigned long GetKey(POSITION pos);
    inline const TCHAR* GetData(POSITION pos);
    inline int SetData(POSITION pos, const TCHAR* data);
    inline POSITION Find(unsigned long key);
    inline int Remove(unsigned long key);
    inline int Remove(POSITION& pos);
    
protected:
    // These members required in all derived classes:
    virtual void onDeleteKey(void*& keyPtr);
    virtual void onDeleteData(void*& dataPtr);
    virtual int onSetKey(void*& keyPtr, void* key);
    virtual int onSetData(void*& dataPtr, void* data);
    virtual relativeKeyValue onCompareKeys(void* key1, void* key2);
    
    // This member provided for debugging support:
    virtual const char* onGetKeyName(void* keyPtr);
};

//==========================================================================
// Derived tree class: unsigned long key, unsigned long data
//==========================================================================
class CCULongToULong : public CBalanceTree
{
public:
    virtual ~CCULongToULong();
    inline int Set(unsigned long key, unsigned long data);
    inline unsigned long Get(unsigned long key);
    inline unsigned long GetKey(POSITION pos);
    inline unsigned long GetData(POSITION pos);
    inline int SetData(POSITION pos, unsigned long data);
    inline POSITION Find(unsigned long key);
    inline int Remove(unsigned long key);
    inline int Remove(POSITION& pos);
    
protected:
    // These members required in all derived classes:
    virtual void onDeleteKey(void*& keyPtr);
    virtual void onDeleteData(void*& dataPtr);
    virtual int onSetKey(void*& keyPtr, void* key);
    virtual int onSetData(void*& dataPtr, void* data);
    virtual relativeKeyValue onCompareKeys(void* key1, void* key2);
    
    // This member provided for debugging support:
    virtual const char* onGetKeyName(void* keyPtr);
};

//==========================================================================
// Derived tree class: long key, string data
//==========================================================================
class CCLongToString : public CBalanceTree
{
public:
    virtual ~CCLongToString();
    inline int Set(long key, const TCHAR* data);
    inline const TCHAR* Get(long key);
    inline long GetKey(POSITION pos);
    inline const TCHAR* GetData(POSITION pos);
    inline int SetData(POSITION pos, const TCHAR* data);
    inline POSITION Find(long key);
    inline int Remove(long key);
    inline int Remove(POSITION& pos);
    
protected:
    // These members required in all derived classes:
    virtual void onDeleteKey(void*& keyPtr);
    virtual void onDeleteData(void*& dataPtr);
    virtual int onSetKey(void*& keyPtr, void* key);
    virtual int onSetData(void*& dataPtr, void* data);
    virtual relativeKeyValue onCompareKeys(void* key1, void* key2);
    
    // This member provided for debugging support:
    virtual const char* onGetKeyName(void* keyPtr);
};

//==========================================================================
// Derived tree class: long key, unsigned long data
//==========================================================================
class CCLongToULong : public CBalanceTree
{
public:
    virtual ~CCLongToULong();
    inline int Set(long key, unsigned long data);
    inline unsigned long Get(long key);
    inline long GetKey(POSITION pos);
    inline unsigned long GetData(POSITION pos);
    inline int SetData(POSITION pos, unsigned long data);
    inline POSITION Find(long key);
    inline int Remove(long key);
    inline int Remove(POSITION& pos);
    
protected:
    // These members required in all derived classes:
    virtual void onDeleteKey(void*& keyPtr);
    virtual void onDeleteData(void*& dataPtr);
    virtual int onSetKey(void*& keyPtr, void* key);
    virtual int onSetData(void*& dataPtr, void* data);
    virtual relativeKeyValue onCompareKeys(void* key1, void* key2);
    
    // This member provided for debugging support:
    virtual const char* onGetKeyName(void* keyPtr);
};

//--------------------------------------------------------------------------
// INLINE members for all classes defined here.
//--------------------------------------------------------------------------

// CCStringToString
inline int CCStringToString::Set(const TCHAR* key, const TCHAR* data)
{
    return set((void*)key, (void*)data);
}

inline const TCHAR* CCStringToString::Get(const TCHAR* key)
{
    return (const TCHAR*)getData(find((void*)key));
}

inline POSITION CCStringToString::Find(const TCHAR* key)
{
    return (POSITION)find((void*)key);
}

inline int CCStringToString::SetData(POSITION node, const TCHAR* data)
{
    return setData(node, (void*)data);
}

inline int CCStringToString::Remove(const TCHAR* key)
{
    POSITION pos = find((void*)key);
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline int CCStringToString::Remove(POSITION& pos)
{
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline const TCHAR* CCStringToString::GetKey(POSITION pos)
{
    return (const TCHAR*)getKey(pos);
}

inline const TCHAR* CCStringToString::GetData(POSITION pos)
{
    return (const TCHAR*)getData(pos);
}

// CCStringToULong
inline int CCStringToULong::Set(const TCHAR* key, unsigned long data)
{
    return set((void*)key, (void*)data);
}

inline unsigned long CCStringToULong::Get(const TCHAR* key)
{
    return (unsigned long)getData(find((void*)key));
}

inline int CCStringToULong::SetData(POSITION node, unsigned long data)
{
    return setData(node, (void*)data);
}

inline POSITION CCStringToULong::Find(const TCHAR* key)
{
    return (POSITION)find((void*)key);
}

inline int CCStringToULong::Remove(const TCHAR* key)
{
    POSITION pos = find((void*)key);
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline int CCStringToULong::Remove(POSITION& pos)
{
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline const TCHAR* CCStringToULong::GetKey(POSITION pos)
{
    return (const TCHAR*)getKey(pos);
}

inline unsigned long CCStringToULong::GetData(POSITION pos)
{
    return (unsigned long)getData(pos);
}

// CCULongToString
inline int CCULongToString::Set(unsigned long key, const TCHAR* data)
{
    return set((void*)key, (void*)data);
}

inline const TCHAR* CCULongToString::Get(unsigned long key)
{
    return (const TCHAR*)getData(find((void*)key));
}

inline int CCULongToString::SetData(POSITION node, const TCHAR* data)
{
    return setData(node, (void*)data);
}

inline POSITION CCULongToString::Find(unsigned long key)
{
    return (POSITION)find((void*)key);
}

inline int CCULongToString::Remove(unsigned long key)
{
    POSITION pos = find((void*)key);
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline int CCULongToString::Remove(POSITION& pos)
{
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline unsigned long CCULongToString::GetKey(POSITION pos)
{
    return (unsigned long)getKey(pos);
}

inline const TCHAR* CCULongToString::GetData(POSITION pos)
{
    return (const TCHAR*)getData(pos);
}

// CCULongToULong
inline int CCULongToULong::Set(unsigned long key, unsigned long data)
{
    return set((void*)key, (void*)data);
}

inline unsigned long CCULongToULong::Get(unsigned long key)
{
    return (unsigned long)getData(find((void*)key));
}

inline int CCULongToULong::SetData(POSITION node, unsigned long data)
{
    return setData(node, (void*)data);
}

inline POSITION CCULongToULong::Find(unsigned long key)
{
    return (POSITION)find((void*)key);
}

inline int CCULongToULong::Remove(unsigned long key)
{
    POSITION pos = find((void*)key);
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline int CCULongToULong::Remove(POSITION& pos)
{
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline unsigned long CCULongToULong::GetKey(POSITION pos)
{
    return (unsigned long)getKey(pos);
}

inline unsigned long CCULongToULong::GetData(POSITION pos)
{
    return (unsigned long)getData(pos);
}

// CCLongToString
inline int CCLongToString::Set(long key, const TCHAR* data)
{
    return set((void*)key, (void*)data);
}

inline const TCHAR* CCLongToString::Get(long key)
{
    return (const TCHAR*)getData(find((void*)key));
}

inline int CCLongToString::SetData(POSITION node, const TCHAR* data)
{
    return setData(node, (void*)data);
}

inline POSITION CCLongToString::Find(long key)
{
    return (POSITION)find((void*)key);
}

inline int CCLongToString::Remove(long key)
{
    POSITION pos = find((void*)key);
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline int CCLongToString::Remove(POSITION& pos)
{
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline long CCLongToString::GetKey(POSITION pos)
{
    return (long)getKey(pos);
}

inline const TCHAR* CCLongToString::GetData(POSITION pos)
{
    return (const TCHAR*)getData(pos);
}

// CCLongToULong
inline int CCLongToULong::Set(long key, unsigned long data)
{
    return set((void*)key, (void*)data);
}

inline unsigned long CCLongToULong::Get(long key)
{
    return (unsigned long)getData(find((void*)key));
}

inline int CCLongToULong::SetData(POSITION node, unsigned long data)
{
    return setData(node, (void*)data);
}

inline POSITION CCLongToULong::Find(long key)
{
    return (POSITION)find((void*)key);
}

inline int CCLongToULong::Remove(long key)
{
    POSITION pos = find((void*)key);
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline int CCLongToULong::Remove(POSITION& pos)
{
    int ok = pos ? 1 : 0;
    remove(pos);
    return ok;
}

inline long CCLongToULong::GetKey(POSITION pos)
{
    return (long)getKey(pos);
}

inline unsigned long CCLongToULong::GetData(POSITION pos)
{
    return (unsigned long)getData(pos);
}

#endif // !defined(AFX_FASTMAPS_H__6AB2016E_7D34_11D4_8736_00909910D47A__INCLUDED_)
