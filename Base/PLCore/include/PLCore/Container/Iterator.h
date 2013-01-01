/*********************************************************\
 *  File: Iterator.h                                     *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the �Software�), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#ifndef __PLCORE_CONTAINER_ITERATOR_H__
#define __PLCORE_CONTAINER_ITERATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
template <class ValueType> class IteratorImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Iterator class
*
*  @remarks
*    If an iterator is used, the iterable (for instance 'Map', 'Container' or 'Heap') the iterator
*    is operating on should not be manipulated by adding/removing new elements because this may
*    produce undefined behavior! Is highly recommended to keep the iterator in the smallest possible
*    local scope and to never keep an iterator as for instance pointer over a long time.
*
*  @verbatim
*    Usage example:
*    Array<String> lstNames;	// Iterable container
*    lstNames.Add("Lenny");		// Add "Lenny" to the iterable container
*    lstNames.Add("Barny");		// Add "Barny" to the iterable container
*    lstNames.Add("Homer");		// Add "Homer" to the iterable container
*    Iterator<String> cIterator = lstNames.GetIterator();	// Get an iterator instance
*    while (cIterator.HasNext())	// Is there a next element?
*        String sName = cIterator.Next();	// Return the next element, in this case, returns "Lenny", then "Barny" and then "Homer"
*  @endverbatim
*
*  @note
*    - As the class same indicates, this is an implementation of the iterator design pattern
*    - An iterator is similar to a "for each loop"
*/
template <class ValueType>
class Iterator {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cIteratorImpl
		*    Reference to the iterator specific implementation
		*/
		Iterator(IteratorImpl<ValueType> &cIteratorImpl);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cIterator
		*    Source to copy from
		*/
		Iterator(const Iterator<ValueType> &cIterator);

		/**
		*  @brief
		*    Destructor
		*/
		~Iterator<ValueType>();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cIterator
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Iterator<ValueType> &operator =(const Iterator<ValueType> &cIterator);

		/**
		*  @brief
		*    Checks whether the iterator can return a next element
		*
		*  @return
		*    'true' if the iterator can return a next element, else 'false'
		*/
		bool HasNext() const;

		/**
		*  @brief
		*    Returns the next element
		*
		*  @return
		*    Reference to the next element, reference to the 'Null'-object on error
		*/
		ValueType &Next();

		/**
		*  @brief
		*    Returns the next element
		*
		*  @return
		*    Reference to the next element, reference to the 'Null'-object on error
		*/
		ValueType &operator ++();

		/**
		*  @brief
		*    Checks whether the iterator can return a previous element
		*
		*  @return
		*    'true' if the iterator can return a previous element, else 'false'
		*/
		bool HasPrevious() const;

		/**
		*  @brief
		*    Returns the previous element
		*
		*  @return
		*    Reference to the previous element, reference to the 'Null'-object on error
		*/
		ValueType &Previous();

		/**
		*  @brief
		*    Returns the previous element
		*
		*  @return
		*    Reference to the previous element, reference to the 'Null'-object on error
		*/
		ValueType &operator --();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Checks whether the current internal iterator implementation is 'shared' and splits it if so
		*/
		void UniqueImplementation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IteratorImpl<ValueType> *m_pIteratorImpl;	/**< Pointer to the iterator specific implementation, always valid */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Container/Iterator.inl"


#endif // __PLCORE_CONTAINER_ITERATOR_H__
