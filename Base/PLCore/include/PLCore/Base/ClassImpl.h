/*********************************************************\
 *  File: ClassImpl.h                                    *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
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


#ifndef __PLCORE_CLASS_IMPL_H__
#define __PLCORE_CLASS_IMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/List.h"
#include "PLCore/Container/HashMap.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Class;
class Object;
class Module;
class VarDesc;
class FuncDesc;
class DynParams;
class EventDesc;
class MemberDesc;
class ConstructorDesc;
class EventHandlerDesc;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract class implementation base class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Class' abstraction
*/
class ClassImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Class;
	friend class ClassReal;
	friend class ClassDummy;
	friend class ClassManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the pointer to the owner class instance wrapping this class implementation
		*
		*  @return
		*    Pointer to the owner class instance (should never be a null pointer, unless something is *terribly* wrong ;-) )
		*/
		inline Class *GetClass() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nModuleID
		*    ID of owner module
		*  @param[in] sName
		*    Name
		*  @param[in] sDescription
		*    Description
		*  @param[in] sNamespace
		*    Namespace
		*  @param[in] sBaseClass
		*    Base class
		*/
		PLCORE_API ClassImpl(uint32 nModuleID, const String &sName, const String &sDescription, const String &sNamespace, const String &sBaseClass);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ClassImpl();

		/**
		*  @brief
		*    Get module the class belongs to
		*
		*  @return
		*    Module (always valid)
		*/
		PLCORE_API const Module *GetModule() const;

		/**
		*  @brief
		*    Get full class name (with namespace)
		*
		*  @return
		*    Name of class and namespace
		*/
		inline String GetClassName() const;

		/**
		*  @brief
		*    Get full name of base class (with namespace)
		*
		*  @return
		*    Name of base class and namespace
		*/
		inline String GetBaseClassName() const;

		/**
		*  @brief
		*    Get class name (without namespace)
		*
		*  @return
		*    Name of class
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Get class description
		*
		*  @return
		*    Description
		*/
		inline String GetDescription() const;

		/**
		*  @brief
		*    Get namespace
		*
		*  @return
		*    Namespace
		*/
		inline String GetNamespace() const;

		/**
		*  @brief
		*    Get base class
		*
		*  @return
		*    Pointer to base class (can be a null pointer)
		*/
		PLCORE_API const Class *GetBaseClass() const;

		/**
		*  @brief
		*    Check if class is derived from another class
		*
		*  @param[in] cBaseClass
		*    Base class
		*
		*  @return
		*    'true' if class is derived from given base class, else 'false'
		*/
		PLCORE_API bool IsDerivedFrom(const Class &cBaseClass) const;

		/**
		*  @brief
		*    Check if class is derived from another class
		*
		*  @param[in] sBaseClass
		*    Base class name (with namespace)
		*
		*  @return
		*    'true' if class is derived from given base class, else 'false'
		*/
		PLCORE_API bool IsDerivedFrom(const String &sBaseClass) const;

		/**
		*  @brief
		*    Get properties
		*
		*  @return
		*    Hash map of properties (name -> value)
		*
		*  @remarks
		*    A property is a name/value pair of strings, that can be assigned to a class. Use this to
		*    transport additional information for your class, e.g.
		*      "PluginType"  -> "Widget"
		*      "FileFormats" -> "avi mpg mp4"
		*/
		inline const HashMap<String, String> &GetProperties() const;

		//[-------------------------------------------------------]
		//[ Class management                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add property
		*
		*  @param[in] sName
		*    Property name
		*  @param[in] sValue
		*    Property value
		*/
		PLCORE_API void AddProperty(const String &sName, const String &sValue);


	//[-------------------------------------------------------]
	//[ Protected virtual ClassImpl functions                 ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Class management                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the class implementation is a dummy used for delayed shared library loading
		*
		*  @return
		*    'true' if the class implementation is a dummy used for delayed shared library loading, else 'false'
		*/
		virtual bool IsDummy() const = 0;

		/**
		*  @brief
		*    Initialize class and class members
		*
		*  @remarks
		*    This function is called automatically when it is necessary, e.g. the first time
		*    any members are being accessed. It will search for the base class of the class
		*    and initialize all members. If later a class is changed (e.g. a new member is
		*    registered at one of the base classes), that class and all derived classes will
		*    destroy their information and must be initialized again.
		*/
		virtual void InitClass() const = 0;

		/**
		*  @brief
		*    De-Initialize class and class members
		*
		*  @remarks
		*    This function destroys all data about the class and it's members. See
		*    InitClass() for more information about why this is necessary and when.
		*/
		virtual void DeInitClass() const = 0;

		//[-------------------------------------------------------]
		//[ Class interface                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get attributes
		*
		*  @return
		*    List of attribute descriptors
		*/
		virtual const List<VarDesc*> &GetAttributes() const = 0;

		/**
		*  @brief
		*    Get attribute
		*
		*  @param[in] sName
		*    Attribute name
		*
		*  @return
		*    Attribute descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const VarDesc *GetAttribute(const String &sName) const = 0;

		/**
		*  @brief
		*    Get methods
		*
		*  @return
		*    List of method descriptors
		*/
		virtual const List<FuncDesc*> &GetMethods() const = 0;

		/**
		*  @brief
		*    Get method
		*
		*  @param[in] sName
		*    Method name
		*
		*  @return
		*    Method descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const FuncDesc *GetMethod(const String &sName) const = 0;

		/**
		*  @brief
		*    Get signals
		*
		*  @return
		*    List of signal descriptors
		*/
		virtual const List<EventDesc*> &GetSignals() const = 0;

		/**
		*  @brief
		*    Get signal
		*
		*  @param[in] sName
		*    Signal name
		*
		*  @return
		*    Signal descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const EventDesc *GetSignal(const String &sName) const = 0;

		/**
		*  @brief
		*    Get slot
		*
		*  @return
		*    List of slot descriptors
		*/
		virtual const List<EventHandlerDesc*> &GetSlots() const = 0;

		/**
		*  @brief
		*    Get slot
		*
		*  @param[in] sName
		*    Slot name
		*
		*  @return
		*    Slot descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const EventHandlerDesc *GetSlot(const String &sName) const = 0;

		/**
		*  @brief
		*    Check if class has any constructors
		*
		*  @return
		*    'true' if class has at least one constructor, else 'false'
		*/
		virtual bool HasConstructor() const = 0;

		/**
		*  @brief
		*    Check if class has a default constructor
		*
		*  @return
		*    'true' if class has a default constructor, else 'false'
		*/
		virtual bool HasDefaultConstructor() const = 0;

		/**
		*  @brief
		*    Get constructors
		*
		*  @return
		*    List of constructor descriptors
		*/
		virtual const List<ConstructorDesc*> &GetConstructors() const = 0;

		/**
		*  @brief
		*    Get constructor
		*
		*  @param[in] sName
		*    Constructor name
		*
		*  @return
		*    Constructor descriptor (can be a null pointer, if no member with that name could be found)
		*/
		virtual const ConstructorDesc *GetConstructor(const String &sName) const = 0;

		/**
		*  @brief
		*    Create object by using the default constructor
		*
		*  @return
		*    Pointer to created object (can be a null pointer)
		*
		*  @remarks
		*    This function will call the default constructor of the class.
		*    If the class has no default constructor, the function will fail and return a null pointer.
		*/
		virtual Object *Create() const = 0;

		/**
		*  @brief
		*    Create object by using typed constructor parameters in order to identity the constructor automatically
		*
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer)
		*
		*  @remarks
		*    This function will search for a constructor that matches the signature of the given parameters.
		*    If no such constructor can be found, the function will fail and return a null pointer.
		*/
		virtual Object *Create(const DynParams &cParams) const = 0;

		/**
		*  @brief
		*    Create object by using a given constructor name and typed constructor parameters
		*
		*  @param[in] sName
		*    Constructor name
		*  @param[in] cParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer)
		*
		*  @remarks
		*    This function will search for a constructor with the specified name. If no such constructor can be found, or
		*    the given parameters do not match the signature of the constructor, the function will fail and return a null pointer.
		*/
		virtual Object *Create(const String &sName, const DynParams &cParams) const = 0;

		/**
		*  @brief
		*    Create object by using a given constructor name and typeless constructor parameters
		*
		*  @param[in] sName
		*    Constructor name
		*  @param[in] sParams
		*    Constructor parameters
		*
		*  @return
		*    Pointer to created object (can be a null pointer, destroy the returned instance when you no longer need it)
		*
		*  @remarks
		*    This function will search for a constructor with the specified name. If no such constructor can be found,
		*    the function will fail and return a null pointer.
		*/
		virtual Object *Create(const String &sName, const String &sParams) const = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Class information
		Class							*m_pClass;				/**< Class instance wrapping this class implementation (can be a null pointer, set and managed by the class manager) */
		String							 m_sName;				/**< Name of class */
		String							 m_sNamespace;			/**< Namespace of class */
		String							 m_sClassName;			/**< Name of class (with namespace) */
		String							 m_sDescription;		/**< Description of class */
		String							 m_sBaseClass;			/**< Name of base class (with namespace) */

		// Own data (does not include data from base classes)
		HashMap<String, String>			 m_mapOwnProperties;	/**< Hash map of properties (name -> value) */

		// Runtime data
		mutable uint32					 m_nModuleID;			/**< ID of owner module */
		mutable bool					 m_bInitialized;		/**< Is the class initialized? */
		mutable const Class			    *m_pBaseClass;			/**< Pointer to base class */

		// Member lists (also including the members from base classes)
		mutable HashMap<String, String>  m_mapProperties;		/**< Hash map of properties (name -> value) */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ClassImpl(const ClassImpl &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		ClassImpl &operator =(const ClassImpl &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Base/ClassImpl.inl"


#endif // __PLCORE_CLASS_IMPL_H__
