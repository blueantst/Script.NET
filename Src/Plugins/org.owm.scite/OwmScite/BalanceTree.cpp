// BalanceTree.cpp: implementation of the CBalanceTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BalanceTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//==========================================================================
// CBalanceTree class: Abstract base class for balanced binary trees.
//==========================================================================

//--------------------------------------------------------------------------
// Construction
CBalanceTree::CBalanceTree()
{
    m_imbalance = 15;                   // empirically determined default
    m_root      = 0;                    // tree initially empty
    ResetStatistics();                  // no-op in debug build
}

//--------------------------------------------------------------------------
// Destruction.  NOTE: All derived classes MUST call RemoveAll() in their
// destructor in order for dynamically allocated storage to be freed.
CBalanceTree::~CBalanceTree()
{
    ASSERT(!m_root);    // derived class did not call RemoveAll() in dtor
}

//--------------------------------------------------------------------------
// Empties the tree.  NOTE: All derived classes MUST call this member in
// their destructor in order for dynamically allocated storage to be freed.
void CBalanceTree::RemoveAll(void)
{
    if (!m_root)
        return;
	CBalanceTreeNode* temp;
	CBalanceTreeNode* cur = m_root;
	m_root = 0;
	while (cur)
	{
		// find a leaf.
		while (cur->m_leftSubtree || cur->m_rightSubtree)
		{
			if (cur->m_leftSubtree)
				cur = cur->m_leftSubtree;
			else
				cur = cur->m_rightSubtree;
		}
		// keep leaf pointer, move to parent, reset parent's leaf-ptr.
		temp = cur;
		cur = cur->m_parent;
		if (cur)
		{
			if (cur->m_leftSubtree == temp)
				cur->m_leftSubtree = 0;
			else
				cur->m_rightSubtree = 0;
		}
		// delete the disconnected leaf.
        if (temp->m_nodeKey)
            onDeleteKey((void*&)temp->m_nodeKey);
        if (temp->m_nodeData)
            onDeleteData((void*&)temp->m_nodeData);
		free(temp);
	}
}

//--------------------------------------------------------------------------
// Sets the maximum level of imbalance allowed before an autobalance
// operation is performed.  Returns the previous setting.  To disable
// autobalance, pass 0.
long CBalanceTree::SetAutoBalance(long maxImbalance)
{
    long oldValue = m_imbalance;
    if (oldValue == LONG_MAX)
        oldValue = 0;
    ASSERT(maxImbalance > -1);
    if (maxImbalance == 0)
        maxImbalance = LONG_MAX;
    if (maxImbalance > -1)
        m_imbalance = maxImbalance;
    return oldValue;
}

//--------------------------------------------------------------------------
// Performs a full tree rebalance.  If the "force" parameter is nonzero,
// the rebalance is performed regardless of whether it is needed.  If the
// "force" parameter is zero, the rebalance is performed only if the top
// level of the tree is imbalanced; note that it is possible for the top
// level to be in balance while some subtrees are extremely imbalanced.
int CBalanceTree::Balance(int force)
{
    int didBalance = 0;
    if (m_root)
    {
        if (force || nodeNeedsBalance(m_root))
        {
            STAT_UPDATE(m_nBalManual);
            nodeBalance(m_root);
            didBalance = 1;
        }
    }
    return didBalance;
}

//--------------------------------------------------------------------------
// Set routine for use by derived classes.  Return 1 if success, or 0 if
// memory failure.
int CBalanceTree::set(void* key, void* data)
{
    STAT_UPDATE(m_nSet);
    if (!m_root)                        // empty tree, create first node
    {
        m_root = nodeCreate(key, data);
        return m_root ? 1 : 0;
    }
    int ok = 1;                         // only out-of-memory causes failure
    CBalanceTreeNode* added = 0;
    CBalanceTreeNode* cur = m_root;
    relativeKeyValue where = undefined;
    
    // Find proper location for node and add/replace.
    while (where != equal)
    {
        where = onCompareKeys(key, cur->m_nodeKey);
        if (where == equal)                     // this item, replace data
        {
            if (cur->m_nodeData)
                onDeleteData((void*&)cur->m_nodeData);
            onSetData((void*&)cur->m_nodeData, data);
        }
        else if (where == less)
        {
            if (!cur->m_leftSubtree)
            {
                added = nodeCreate(key, data);
                if (added)
                    nodeSetLeftChild(cur, added);
                else
                    ok = 0;                     // allocation failure
                where = equal;
            }
            else
                cur = cur->m_leftSubtree;
        }
        else    // where == greater
        {
            if (!cur->m_rightSubtree)
            {
                added = nodeCreate(key, data);
                if (added)
                    nodeSetRightChild(cur, added);
                else
                    ok = 0;                     // allocation failure
                where = equal;
            }
            else
                cur = cur->m_rightSubtree;
        }
    }
    if (added && cur)
        nodeUpdateBalance(cur);
    return ok;
}

//--------------------------------------------------------------------------
// Balance the subtree below the specified node.  Note that this is a full
// rebalance of the subtree whether it needs it or not.
void CBalanceTree::nodeBalance(CBalanceTreeNode* node)
{
    ASSERT(node);
    long nodes = nodeGetCount(node);
    if (nodes > 2)
    {
        // Get the new balance point of the subtree, and set it
        // as the result.
        CBalanceTreeNode* owner = node->m_parent;
        CBalanceTreeNode* root = nodeMakeBalancePoint(node);
        if (!owner)
            m_root = root;
        else
        {
            if (owner->m_leftSubtree == node)
                nodeSetLeftChild(owner, root);
            else
                nodeSetRightChild(owner, root);
        }
        // Put any qualifying left or right subtree pointers on the
        // stack.
        CBalanceTreeNode* stack[sizeof(long) * 8];  // enough for LONG_MAX nodes
        int stackIx = 0;
        if (root->m_leftSubtree && nodeGetCount(root->m_leftSubtree) > 2)
            stack[stackIx++] = root->m_leftSubtree;
        if (root->m_rightSubtree && nodeGetCount(root->m_rightSubtree) > 2)
            stack[stackIx++] = root->m_rightSubtree;
        // Balance from the stack until it becomes empty.
        CBalanceTreeNode* cur;
        CBalanceTreeNode* parent;
        CBalanceTreeNode* temp;
        while (stackIx)
        {
            cur = stack[--stackIx];
            parent = cur->m_parent;
            temp = nodeMakeBalancePoint(cur);
            if (parent->m_leftSubtree == cur)
                nodeSetLeftChild(parent, temp);
            else
                nodeSetRightChild(parent, temp);
            if (temp->m_leftSubtree && nodeGetCount(temp->m_leftSubtree) > 2)
                stack[stackIx++] = temp->m_leftSubtree;
            if (temp->m_rightSubtree && nodeGetCount(temp->m_rightSubtree) > 2)
                stack[stackIx++] = temp->m_rightSubtree;
        }
    }
}

//--------------------------------------------------------------------------
// Remove the specified node from the tree, reorganizing and rebalancing
// the tree as necessary.
void CBalanceTree::nodeRemove(CBalanceTreeNode* node)
{
    ASSERT(node);
    if (node)
    {
        CBalanceTreeNode* parent = node->m_parent;
        CBalanceTreeNode* child = 0;
        CBalanceTreeNode* topImbal = 0;
        // Combine the left/right subtrees of item being removed.
        if (node->m_leftSubtree || node->m_rightSubtree)  // not a leaf
        {
            // Heavier subtree becomes parent of lighter subtree.
            if (node->m_leftNodeCount > node->m_rightNodeCount)
            {
                if (node->m_rightSubtree)
                    topImbal = nodeInsertRightmost(node->m_leftSubtree, node->m_rightSubtree);
                child = node->m_leftSubtree;
            }
            else
            {
                if (node->m_leftSubtree)
                    topImbal = nodeInsertLeftmost(node->m_rightSubtree, node->m_leftSubtree);
                child = node->m_rightSubtree;
            }
            if (child)
                child->m_parent = 0;    // no parent link to deleted
        }
        else
            child = 0;                  // removing a leaf
        // "child" now contains combined subtree, insert it in parent.
        if (parent)
        {
            if (parent->m_leftSubtree == node)
                nodeSetLeftChild(parent, child);
            else
                nodeSetRightChild(parent, child);
        }
        else
            m_root = child;     // removing root node, child becomes new root
        // Node removed from tree, now delete it.
        if (node->m_nodeKey)
            onDeleteKey((void*&)node->m_nodeKey);
        if (node->m_nodeData)
            onDeleteData((void*&)node->m_nodeData);
        free(node);
        // Correct any imbalance introduced by removal.
        if (!topImbal)
            topImbal = parent;
        if (topImbal)
            nodeUpdateBalance(topImbal);
    }
}

//--------------------------------------------------------------------------
// Output structure recursively; called by DumpStructure().
void CBalanceTree::dumpStructure(CBalanceTreeNode* node, int level)
{
	UNREFERENCED_PARAMETER(level);
	UNREFERENCED_PARAMETER(node);
#ifdef _DEBUG
	if (!level)
		TRACE("..........right..........\n");
	if (node->m_rightSubtree)
		dumpStructure(node->m_rightSubtree, level+1);
	char temp[100];
	for (int i=0; i<__min(level,99); i++)
		temp[i] = ' ';
	temp[__min(level,99)] = 0;
	char* info = getNodeInfo(node);
	TRACE("%06li %s%s\n", GetItemIndex((POSITION)node), temp, info);
	free(info);
	if (node->m_leftSubtree)
		dumpStructure(node->m_leftSubtree, level+1);
	if (!level)
		TRACE("..........left..........\n");
#endif //_DEBUG
}

//--------------------------------------------------------------------------
// Debug support routine called by DumpStructure().
// Note: caller must free() the result when it is no longer needed.
char* CBalanceTree::getNodeInfo(CBalanceTreeNode* node)
{
    ASSERT(node);
	UNREFERENCED_PARAMETER(node);
    char* temp = 0;
#ifdef _DEBUG
	const char* keyName = 0;
	if (node->m_nodeKey)
		keyName = strdup(onGetKeyName(node->m_nodeKey));
	int len = keyName ? strlen(keyName) : 0;
	const char* parent = "NOPARENT";
	if (node->m_parent)
		if (node->m_parent->m_nodeKey)
			parent = onGetKeyName(node->m_parent->m_nodeKey);
        len += strlen(parent);
        temp = (char*)malloc(len + 30);
        sprintf(temp, "%li,%li=%s (parent=%s)", node->m_leftNodeCount, 
			node->m_rightNodeCount, keyName, parent);
        free((void*)keyName);
#endif //_DEBUG
	return temp;
}

//--------------------------------------------------------------------------
// Return a pointer to ASCII representation of the key's name.  The pointer
// returned may refer to a static buffer.  Used by the DumpStructure() member.
static char nameBuf[12];
const char* CBalanceTree::onGetKeyName(void* keyPtr)
{
    sprintf(nameBuf, "%p", keyPtr);
    return nameBuf;
}
