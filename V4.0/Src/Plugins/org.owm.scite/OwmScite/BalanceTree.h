// BalanceTree.h: interface for the CBalanceTree class.
// Based on codes bearing the copyright:
//       Copyright Dundas Software Ltd. 1998, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALANCETREE_H__6AB2016D_7D34_11D4_8736_00909910D47A__INCLUDED_)
#define AFX_BALANCETREE_H__6AB2016D_7D34_11D4_8736_00909910D47A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Define the STAT_UPDATE macro to update debug-build statistics
#ifdef _DEBUG
    #define STAT_UPDATE(a) a++
#else
    #define STAT_UPDATE(a)
#endif

//==========================================================================
// CBalanceTree class: Abstract base class for balanced binary trees.
//==========================================================================
class CBalanceTree
{
public:
    CBalanceTree();
    virtual ~CBalanceTree();
    
    long SetAutoBalance(long maxImbalance);         // pass 0 to turn off
    int Balance(int force = 0);                     // only cks top unless force
    
    void RemoveAll(void);                           // empties the tree
    
    inline long GetCount(void);                     // returns item count
	inline BOOL IsEmpty(void);						// Is the tree is empty or not?
    inline POSITION GetItem(long item);             // find 0-based item N
    inline long GetItemIndex(POSITION node);        // item's 0-based index
    
    inline POSITION First(void);                    // position at first
    inline POSITION Last(void);                     // position at last
    inline POSITION Prev(POSITION node);            // move to previous
    inline POSITION Next(POSITION node);            // move to next
    
    // debugging support
    inline void DumpStructure(void);                // no-op in release build
    inline void DumpStatistics(void);               // no-op in release build
    inline void ResetStatistics(void);              // no-op in release build
    
    // statistics, available in debug build only:
#ifdef _DEBUG
    long    m_nBalManual;       // manually performed balance operations
    long    m_nBalAuto;         // autobalance operations
    long    m_nGet;             // number of data retrieval operations
    long    m_nSet;             // number of data storage operations
#endif //_DEBUG
    
protected:
    // Relative key value defined same as result from strcmp() except
    // for the "undefined" value.
    enum relativeKeyValue 
	{
		less      = -1,
        equal     = 0,
        greater   = 1,
        undefined = 2
	};
    
    // Support routines for derived classes:
    inline POSITION find(void* key);                // find specific item
    inline void* getKey(POSITION node);             // return key ptr
    inline void* getData(POSITION node);            // return data ptr
    int set(void* key, void* data);                 // 1 if success, else 0
    inline int setData(POSITION node, void* data);  // 1 if success, else 0
    inline void remove(POSITION& node);             // remove specific item
    
    // Overrides required in all derived classes:
    virtual void onDeleteKey(void*& keyPtr) = 0;    // don't zero keyPtr
    virtual void onDeleteData(void*& dataPtr) = 0;  // don't zero dataPtr
    virtual int onSetKey(void*& keyPtr, void* key) = 0;    // don't delete previous
    virtual int onSetData(void*& dataPtr, void* data) = 0; // don't delete previous
    virtual relativeKeyValue onCompareKeys(void* key1, void* key2) = 0;
    
    // Optional overrides for debugging support:
    virtual const char* onGetKeyName(void* keyPtr);
    
    // Member variables:
    typedef struct tStrmHdr // stream header used for serialization
    {
        long segLength;     // total length of data segment incl hdr
        long segBufReqd;    // size to hold largest data item in segment
    } StrmHdr;
    
    long        m_bufSize;                  // max size for input serialization
    void*       m_keyBuf;                   // key buffer during serialization
    void*       m_dataBuf;                  // data buffer during serialization

private:
    // Node structure:
    struct CBalanceTreeNode
    {
        void*           m_nodeKey;
        void*           m_nodeData;
        CBalanceTreeNode*   m_parent;
        CBalanceTreeNode*   m_leftSubtree;
        long                m_leftNodeCount;
        CBalanceTreeNode*   m_rightSubtree;
        long                m_rightNodeCount;
    };
    
    // Other private data:
    CBalanceTreeNode*   m_root;
    long            m_imbalance;

    // Node-handling routines:
    void nodeBalance(CBalanceTreeNode* node);
    void nodeRemove(CBalanceTreeNode* node);
    
    inline long nodeGetCount(CBalanceTreeNode* node);
    inline CBalanceTreeNode* nodeCreate(void* key, void* data);
    inline void nodeSetLeftChild(CBalanceTreeNode* node, CBalanceTreeNode* child);
    inline void nodeSetRightChild(CBalanceTreeNode* node, CBalanceTreeNode* child);
    inline void nodeUpdateBalance(CBalanceTreeNode* node);
    inline int nodeNeedsBalance(CBalanceTreeNode* node);
    inline long nodeGetBalanceItem(long nodes);
    inline CBalanceTreeNode* nodeFindItem(CBalanceTreeNode* node, long item);
    inline CBalanceTreeNode* nodeInsertLeftmost(CBalanceTreeNode* parent,
                                            CBalanceTreeNode* child);
    inline CBalanceTreeNode* nodeInsertRightmost(CBalanceTreeNode* parent,
                                             CBalanceTreeNode* child);
    inline CBalanceTreeNode* nodeMakeBalancePoint(CBalanceTreeNode* oldRoot);
    
    // Debug support routines:
    char* getNodeInfo(CBalanceTreeNode* node);      // no-op in release build
    void dumpStructure(CBalanceTreeNode* node, int level);  // no-op in release build
};

//--------------------------------------------------------------------------
// INLINE members.
//--------------------------------------------------------------------------

// Returns the total number of items in the tree.
inline long CBalanceTree::GetCount(void)
{
    if (m_root)
        return nodeGetCount(m_root);
    return 0;
}

inline BOOL CBalanceTree::IsEmpty(void)
{
	return (GetCount() == 0);
}


// Returns the total number of items in the subtree.
inline long CBalanceTree::nodeGetCount(CBalanceTreeNode* node)
{
    ASSERT(node);
    return node->m_leftNodeCount + node->m_rightNodeCount + 1;
}

// Called to create a new node with the given key/data values.
inline CBalanceTree::CBalanceTreeNode* CBalanceTree::nodeCreate(void* key, void* data)
{
    CBalanceTreeNode* node = (CBalanceTreeNode*)malloc(sizeof(CBalanceTreeNode));
    if (node)
    {
        memset(node, 0, sizeof(CBalanceTreeNode));
        if (!onSetKey((void*&)node->m_nodeKey, key))
        {
            free(node);
            node = 0;
        }
        else
        {
            if (!onSetData((void*&)node->m_nodeData, data))
            {
                onDeleteKey((void*&)node->m_nodeKey);
                free(node);
                node = 0;
            }
        }
    }
    return node;
}

// Sets the specified node's left child, and updates its node counts.
inline void CBalanceTree::nodeSetLeftChild(CBalanceTreeNode* node, CBalanceTreeNode* child)
{
    ASSERT(node);
    node->m_leftSubtree = child;
    if (child)
    {
        node->m_leftNodeCount = child->m_leftNodeCount + child->m_rightNodeCount + 1;
        child->m_parent = node;
    }
    else
        node->m_leftNodeCount = 0;
}

// Sets the specified node's right child, and updates its node counts.
inline void CBalanceTree::nodeSetRightChild(CBalanceTreeNode* node, CBalanceTreeNode* child)
{
    ASSERT(node);
    node->m_rightSubtree = child;
    if (child)
    {
        node->m_rightNodeCount = child->m_leftNodeCount	+ child->m_rightNodeCount + 1;
        child->m_parent = node;
    }
    else
        node->m_rightNodeCount = 0;
}

// Called when a node has been added or deleted; works back to the root
// updating balance counts, then rebalances the topmost subtree that is
// sufficiently imbalanced to require a rebalance (if any).
inline void CBalanceTree::nodeUpdateBalance(CBalanceTreeNode* node)
{
    ASSERT(node);
    CBalanceTreeNode* topImbal = 0;
    CBalanceTreeNode* child;
    while (node != m_root)
    {
        if (nodeNeedsBalance(node))
            topImbal = node;
        child = node;
        node = node->m_parent;
        if (node)
        {
            if (node->m_leftSubtree == child)
                nodeSetLeftChild(node, child);      // to update node counts
            else
                nodeSetRightChild(node, child);
        }
    }
    // Perform autobalance if needed.
    if (nodeNeedsBalance(m_root))
        topImbal = m_root;
    if (topImbal)
    {
        STAT_UPDATE(m_nBalAuto);
        nodeBalance(topImbal);
    }
}

// Checks the specified node's balance.
inline int CBalanceTree::nodeNeedsBalance(CBalanceTreeNode* node)
{
    ASSERT(node);
    long diff = node->m_leftNodeCount - node->m_rightNodeCount;
    if (diff > m_imbalance)
        return 1;
    if (diff < -m_imbalance)
        return 1;
    return 0;
}

// Compute the item number of the balance point; localized to ensure
// consistent computation.
inline long CBalanceTree::nodeGetBalanceItem(long nodes)
{
    return nodes / 2;
}

// Given any node, find 0-based item "item" within the subtree it owns.
inline CBalanceTree::CBalanceTreeNode* CBalanceTree::nodeFindItem(CBalanceTreeNode* node,
                                                      long item)
{
    ASSERT(node);
    ASSERT(item >= 0);
    ASSERT(item < nodeGetCount(node));
    CBalanceTreeNode* found = 0;
    while (!found)
    {
        if (node->m_leftNodeCount > item)   // node is left of this node
            node = node->m_leftSubtree;
        else if (node->m_leftNodeCount == item) // found the node requested
            found = node;
        else                                // node is right of this node
        {
            item -= node->m_leftNodeCount + 1;
            ASSERT(item >= 0);
            node = node->m_rightSubtree;
        }
    }
    return found;
}

// Insert the "child" subtree as the leftmost node of the "parent" subtree,
// and return the topmost node that is out of balance.
inline CBalanceTree::CBalanceTreeNode* 
    CBalanceTree::nodeInsertLeftmost(CBalanceTreeNode* parent, CBalanceTreeNode* child)
{
    ASSERT(parent);
    ASSERT(child);
    CBalanceTreeNode* topImbal = 0;
    long childCount = nodeGetCount(child);
    // Move to leftmost node, updating balance on the way.
    while (parent->m_leftSubtree)
    {
        parent->m_leftNodeCount += childCount;
        if (!topImbal && nodeNeedsBalance(parent))
            topImbal = parent;
        parent = parent->m_leftSubtree;
    }
    // Insert the subtree supplied.
    nodeSetLeftChild(parent, child);
    return topImbal;
}

// Insert the "child" subtree as the rightmost node of the "parent" subtree,
// and return the topmost node that is out of balance.
inline CBalanceTree::CBalanceTreeNode* 
    CBalanceTree::nodeInsertRightmost(CBalanceTreeNode* parent, CBalanceTreeNode* child)
{
    ASSERT(parent);
    ASSERT(child);
    CBalanceTreeNode* topImbal = 0;
    long childCount = nodeGetCount(child);
    // Move to rightmost node, updating balance on the way.
    while (parent->m_rightSubtree)
    {
        parent->m_rightNodeCount += childCount;
        if (!topImbal && nodeNeedsBalance(parent))
            topImbal = parent;
        parent = parent->m_rightSubtree;
    }
    // Insert the subtree supplied.
    nodeSetRightChild(parent, child);
    return topImbal;
}

// Given the root of a subtree, determine the subtree's correct balance point
// and make it the new subtree root; return the new subtree root.  Tracking
// imbalance is unnecessary here since this routine is only called during
// a rebalance operation.
inline CBalanceTree::CBalanceTreeNode* 
    CBalanceTree::nodeMakeBalancePoint(CBalanceTreeNode* oldRoot)
{
    ASSERT(oldRoot);
    CBalanceTreeNode* root = oldRoot;
    long nodes = nodeGetCount(oldRoot);
    long pos = nodeGetBalanceItem(nodes);
    if (nodes > 2 && root->m_leftNodeCount != pos)  // root shift required
    {
        root = nodeFindItem(oldRoot, pos);
        // Cut the link between the new root and its parent, noting 
        // whether the new root's parent is on its left or right.
        CBalanceTreeNode* parent = root->m_parent;
        ASSERT(parent);             // should always be nonzero
        int leftOfRoot;             // new root's PARENT is on its left
        leftOfRoot = parent->m_rightSubtree == root ? 1 : 0;
        if (leftOfRoot)
            nodeSetRightChild(parent, 0);   // note: does not affect old child
        else
            nodeSetLeftChild(parent, 0);
        // Update node counts back to the old root, moving any
        // subtrees that are out of place.
        CBalanceTreeNode* prev;
        CBalanceTreeNode* cur = root;
        while (cur != oldRoot)
        {
            prev = cur;
            cur = parent;
            ASSERT(cur);
            parent = cur->m_parent;
            if (cur->m_leftSubtree == prev)     // cur is right of root
            {
                if (leftOfRoot)     // prev left of root, cur right of root
                {
                    nodeInsertLeftmost(root, cur->m_leftSubtree);
                    nodeSetLeftChild(cur, 0);
                    leftOfRoot = 0;
                }
                else
                    nodeSetLeftChild(cur, cur->m_leftSubtree);  // update counts
            }
            else if (cur->m_rightSubtree == prev)   // cur is left of root
            {
                if (!leftOfRoot)    // prev right of root, cur left of root
                {
                    nodeInsertRightmost(root, cur->m_rightSubtree);
                    nodeSetRightChild(cur, 0);
                    leftOfRoot = 1;
                }
                else
                    nodeSetRightChild(cur, cur->m_rightSubtree);    // counts
            }
        }
        // Insert the remaining old root subtree under the new root.
        if (root != oldRoot)
        {
            if (leftOfRoot)
                nodeInsertLeftmost(root, oldRoot);
            else
                nodeInsertRightmost(root, oldRoot);
        }
    }
    root->m_parent = 0;
    return root;
}

// Removes the specified node from the tree, invalidates the
// POSITION passed.
inline void CBalanceTree::remove(POSITION& node)
{
    if (node)
    {
        nodeRemove((CBalanceTreeNode*)node);
        node = 0;
    }
}

// Returns a POSITION value for the specified 0-based item.  The result is
// 0 if the item is not found.
inline POSITION CBalanceTree::GetItem(long item)
{
    POSITION found = 0;
    if (m_root && item >= 0 && item < nodeGetCount(m_root))
        found = (POSITION)nodeFindItem(m_root, item);
    return found;
}

// Returns a POSITION value for the specified item.  The result is
// 0 if the item is not found.
inline POSITION CBalanceTree::find(void* key)
{
    POSITION found = 0;
    relativeKeyValue where;
    CBalanceTreeNode* cur = m_root;
    while (cur && !found)
    {
        where = onCompareKeys(key, cur->m_nodeKey);
        if (where == less)
            cur = cur->m_leftSubtree;
        else if (where == greater)
            cur = cur->m_rightSubtree;
        else
            found = (POSITION)cur;
    }
    return found;
}

// Returns the specified item's key pointer.
inline void* CBalanceTree::getKey(POSITION node)
{
    return node ? ((CBalanceTreeNode*)node)->m_nodeKey : 0;
}

// Returns the specified item's data pointer.
inline void* CBalanceTree::getData(POSITION node)
{
    STAT_UPDATE(m_nGet);
    return node ? ((CBalanceTreeNode*)node)->m_nodeData : 0;
}

// Sets the specified item's data pointer.
inline int CBalanceTree::setData(POSITION node, void* data)
{
    ASSERT(node);
    if (!node)
        return 0;
    STAT_UPDATE(m_nSet);
    if (((CBalanceTreeNode*)node)->m_nodeData)
        onDeleteData(((CBalanceTreeNode*)node)->m_nodeData);
    return onSetData(((CBalanceTreeNode*)node)->m_nodeData, data);
}

// Move to the first (leftmost) node in the tree.
inline POSITION CBalanceTree::First(void)
{
    CBalanceTreeNode* cur = m_root;
    while (cur && cur->m_leftSubtree)
        cur = cur->m_leftSubtree;
    return (POSITION)cur;
}

// Move to the last (rightmost) node in the tree.
inline POSITION CBalanceTree::Last(void)
{
    CBalanceTreeNode* cur = m_root;
    while (cur && cur->m_rightSubtree)
        cur = cur->m_rightSubtree;
    return (POSITION)cur;
}

// Move to the previous position in the tree.
inline POSITION CBalanceTree::Prev(POSITION node)
{
    CBalanceTreeNode* cur = (CBalanceTreeNode*)node;
    if (cur)
    {
        if (cur->m_leftSubtree)
        {
            cur = cur->m_leftSubtree;
            while (cur && cur->m_rightSubtree)
                cur = cur->m_rightSubtree;
        }
        else
        {
            CBalanceTreeNode* prev = cur;
            cur = cur->m_parent;
            while (cur && cur->m_leftSubtree == prev)
            {
                prev = cur;
                cur = cur->m_parent;
            }
        }
    }
    return (POSITION)cur;
}

// Move to the next position in the tree.
inline POSITION CBalanceTree::Next(POSITION node)
{
    CBalanceTreeNode* cur = (CBalanceTreeNode*)node;
    if (cur)
    {
        if (cur->m_rightSubtree)
        {
            cur = cur->m_rightSubtree;
            while (cur && cur->m_leftSubtree)
                cur = cur->m_leftSubtree;
        }
        else
        {
            CBalanceTreeNode* prev = cur;
            cur = cur->m_parent;
            while (cur && cur->m_rightSubtree == prev)
            {
                prev = cur;
                cur = cur->m_parent;
            }
        }
    }
    return (POSITION)cur;
}

// Get the specified node's 0-based item index.
inline long CBalanceTree::GetItemIndex(POSITION node)
{
    ASSERT(node);
    if (!node)
        return 0;
    CBalanceTreeNode* cur  = (CBalanceTreeNode*)node;
    CBalanceTreeNode* prev = 0;
    long pos = cur->m_leftNodeCount;
    while (cur->m_parent)
    {
        prev = cur;
        cur = cur->m_parent;
        if (cur->m_rightSubtree == prev)
            pos += cur->m_leftNodeCount + 1;
    }
    return pos;
}

// Output tree structure as debug info.
inline void CBalanceTree::DumpStructure(void)
{
#ifdef _DEBUG
	if (m_root)
		dumpStructure(m_root, 0);
	else
		TRACE(".....empty.....\n");
#endif //_DEBUG
}

// Output tree statistics as debug info.
inline void CBalanceTree::DumpStatistics(void)
{
#ifdef _DEBUG
	TRACE("Set operations   = %li\n", m_nSet);
	TRACE("Get operations   = %li\n", m_nGet);
	TRACE("Balance (auto)   = %li\n", m_nBalAuto);
	TRACE("Balance (manual) = %li\n", m_nBalManual);
#endif //_DEBUG
}

// Reset tree statistics.
inline void CBalanceTree::ResetStatistics(void)
{
#ifdef _DEBUG
	m_nBalManual = 0;           // manually performed balance operations
	m_nBalAuto   = 0;           // autobalance operations
	m_nGet       = 0;           // number of data retrieval operations
	m_nSet       = 0;           // number of data storage operations
#endif //_DEBUG
}

#endif // !defined(AFX_BALANCETREE_H__6AB2016D_7D34_11D4_8736_00909910D47A__INCLUDED_)
