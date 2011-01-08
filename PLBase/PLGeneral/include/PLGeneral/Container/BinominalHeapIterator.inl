/*********************************************************\
 *  File: BinominalHeapIterator.inl                      *
 *      Binominal heap iterator template implementation
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGeneral/Container/BinominalHeap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGeneral {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeapIterator<KeyType, ValueType, Comparer>::BinominalHeapIterator(const BinominalHeap<KeyType, ValueType, Comparer> &cHeapOwner, uint32 nIndex) :
	m_pHeapOwner(&cHeapOwner)
{
	// Which search direction?
	if (nIndex <= m_pHeapOwner->m_nNumOfElements/2) { // 0 -> (NumOfElements-1)
		// Initialize data
		m_pNextTree     = m_pHeapOwner->m_pRoot;
		m_pPreviousTree = nullptr;

		// Go to the correct start index
		uint32 nCurrentIndex = 0;
		while (nCurrentIndex < nIndex && HasNext()) {
			Next();
			nCurrentIndex++;
		}
	} else { // (NumOfElements-1) -> 0
		// Initialize data
		m_pNextTree     = nullptr;
		m_pPreviousTree = m_pHeapOwner->m_pRoot;
		do {
			// Find last sibling
			while (m_pPreviousTree->m_pNextSibling)
				m_pPreviousTree = m_pPreviousTree->m_pNextSibling;

			// Is there a child?
			if (m_pPreviousTree->m_pChild) {
				m_lstParent.Push(m_pPreviousTree);
				m_pPreviousTree = m_pPreviousTree->m_pChild;
			} else {
				break;
			}
		} while (1);

		// Go to the correct start index
		uint32 nCurrentIndex = m_pHeapOwner->m_nNumOfElements-1;
		while (nCurrentIndex > nIndex && HasPrevious()) {
			Previous();
			nCurrentIndex--;
		}
	}
}

/**
*  @brief
*    Constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeapIterator<KeyType, ValueType, Comparer>::BinominalHeapIterator(const BinominalHeap<KeyType, ValueType, Comparer> &cHeapOwner) :
	m_pHeapOwner(&cHeapOwner),
	m_pNextTree(nullptr),
	m_pPreviousTree(m_pHeapOwner->m_pRoot)
{
	// Initialize data
	if (m_pPreviousTree) {
		do {
			// Find last sibling
			while (m_pPreviousTree->m_pNextSibling)
				m_pPreviousTree = m_pPreviousTree->m_pNextSibling;

			// Is there a child?
			if (m_pPreviousTree->m_pChild) {
				m_lstParent.Push(m_pPreviousTree);
				m_pPreviousTree = m_pPreviousTree->m_pChild;
			} else {
				break;
			}
		} while (true);
	}
}

/**
*  @brief
*    Copy constructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeapIterator<KeyType, ValueType, Comparer>::BinominalHeapIterator(const BinominalHeapIterator<KeyType, ValueType, Comparer> &cSource) :
	m_pHeapOwner(cSource.m_pHeapOwner),
	m_lstParent(cSource.m_lstParent),
	m_pNextTree(cSource.m_pNextTree),
	m_pPreviousTree(cSource.m_pPreviousTree)
{
}

/**
*  @brief
*    Destructor
*/
template <class KeyType, class ValueType, class Comparer>
BinominalHeapIterator<KeyType, ValueType, Comparer>::~BinominalHeapIterator()
{
}

/**
*  @brief
*    Returns the previous sibling of the given tree
*/
template <class KeyType, class ValueType, class Comparer>
typename BinominalHeap<KeyType, ValueType, Comparer>::Tree *BinominalHeapIterator<KeyType, ValueType, Comparer>::GetPreviousSibling(typename BinominalHeap<KeyType, ValueType, Comparer>::Tree *pTree) const
{
	typename BinominalHeap<KeyType, ValueType, Comparer>::Tree *pPrevious = nullptr;
	typename BinominalHeap<KeyType, ValueType, Comparer>::Tree *pNext     = nullptr;

	// Get the FIRST tree
	if (m_lstParent.GetNumOfElements()) {
		pNext = m_lstParent.Top();
		pNext = pNext->m_pChild;
	} else {
		pNext = m_pHeapOwner->m_pRoot;
	}

	// Previous and next tree
	while (pNext != pTree) {
		pPrevious = pNext;
		pNext     = pNext->m_pNextSibling;
	}

	// Return the previous sibling
	return pPrevious;
}


//[-------------------------------------------------------]
//[ Private virtual IteratorImpl functions                ]
//[-------------------------------------------------------]
template <class KeyType, class ValueType, class Comparer>
IteratorImpl<ValueType> *BinominalHeapIterator<KeyType, ValueType, Comparer>::Clone() const
{
	return new BinominalHeapIterator<KeyType, ValueType, Comparer>(*this);
}

template <class KeyType, class ValueType, class Comparer>
bool BinominalHeapIterator<KeyType, ValueType, Comparer>::HasNext() const
{
	return (m_pNextTree != nullptr);
}

template <class KeyType, class ValueType, class Comparer>
ValueType &BinominalHeapIterator<KeyType, ValueType, Comparer>::Next()
{
	// Is there a next tree?
	if (!m_pNextTree)
		return BinominalHeap<KeyType, ValueType, Comparer>::Null;

	// Backup value
	ValueType *pValue = &m_pNextTree->m_Value;

	// Set previous tree
	m_pPreviousTree = m_pNextTree;

	// Get next tree, first check whether there's a child
	if (m_pNextTree->m_pChild) {
		m_lstParent.Push(m_pNextTree);
		m_pNextTree = m_pNextTree->m_pChild;
	} else {
		// Get next sibling
		m_pNextTree = m_pNextTree->m_pNextSibling;
		while (!m_pNextTree) {
			// Go to parent
			if (!m_lstParent.GetNumOfElements())
				break; // Ok, no parent, no next sibling - this must be the end :)
			m_lstParent.Pop(&m_pNextTree);

			// Get next sibling
			m_pNextTree = m_pNextTree->m_pNextSibling;
		}
	}

	// Return value
	return pValue ? *pValue : BinominalHeap<KeyType, ValueType, Comparer>::Null;
}

template <class KeyType, class ValueType, class Comparer>
bool BinominalHeapIterator<KeyType, ValueType, Comparer>::HasPrevious() const
{
	return (m_pPreviousTree != nullptr);
}

template <class KeyType, class ValueType, class Comparer>
ValueType &BinominalHeapIterator<KeyType, ValueType, Comparer>::Previous()
{
	// Is there a previous tree?
	if (!m_pPreviousTree)
		return BinominalHeap<KeyType, ValueType, Comparer>::Null;

	// Backup value
	ValueType *pValue = &m_pPreviousTree->m_Value;

	// Set next tree
	m_pNextTree = m_pPreviousTree;

	// Get previous sibling
	m_pPreviousTree = GetPreviousSibling(m_pPreviousTree);
	if (!m_pPreviousTree) {
		// Go to parent...
		if (!m_lstParent.GetNumOfElements()) {
			// Ok, no parent, no previous sibling - this must be the beginning :)
			m_pPreviousTree = nullptr;

			// Return the value
			return pValue ? *pValue : BinominalHeap<KeyType, ValueType, Comparer>::Null;
		}
		m_lstParent.Pop(&m_pPreviousTree);

		// Return the value
		return pValue ? *pValue : BinominalHeap<KeyType, ValueType, Comparer>::Null;
	}

	// Check whether there's a child
	while (m_pPreviousTree && m_pPreviousTree->m_pChild) {
		// Push current tree onto the stack
		m_lstParent.Push(m_pPreviousTree);

		// Change into child...
		m_pPreviousTree = m_pPreviousTree->m_pChild;

		// ...and take the last tree
		while (m_pPreviousTree->m_pNextSibling)
			m_pPreviousTree = m_pPreviousTree->m_pNextSibling;
	}

	// Return the value
	return pValue ? *pValue : BinominalHeap<KeyType, ValueType, Comparer>::Null;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGeneral
