/*********************************************************\
 *  File: Provider.h                                     *
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


#ifndef __PLINPUT_PROVIDER_H__
#define __PLINPUT_PROVIDER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLInput/PLInput.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Device;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Input provider
*
*  @remarks
*    An input provider is responsible for detecting and managing a specific set of input devices,
*    e.g. the core provider will look after keyboard and mouse. Other types of input providers can
*    be written to support new input devices, typically implemented as plugins.
*/
class Provider : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class InputManager;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLINPUT_RTTI_EXPORT, Provider, "PLInput", PLCore::Object, "Input provider")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLINPUT_API Provider();

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API virtual ~Provider();

		/**
		*  @brief
		*    Detect devices
		*
		*  @param[in] bReset
		*    If 'true', delete all input devices and re-detect them all. Otherwise,
		*    only new and removed input devices will be detected.
		*
		*  @remarks
		*    bReset = true should only be used if really necessary, because existing
		*    input handlers will most certainly loose their connection to the device.
		*/
		PLINPUT_API void DetectDevices(bool bReset = false);

		/**
		*  @brief
		*    Get list of devices
		*
		*  @return
		*    Device list, do not destroy the returned instances!
		*/
		PLINPUT_API const PLCore::List<Device*> &GetDevices() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Destroy all devices
		*/
		PLINPUT_API void Clear();

		/**
		*  @brief
		*    Check if a device is already known and update it
		*
		*  @param[in] sName
		*    Name of the input device (e.g. "Mouse0")
		*
		*  @return
		*    'true', if a device with this name is known, else 'false'
		*
		*  @remarks
		*    If the device is already present, it's "confirmed"-flag is set to 'true', so
		*    it will not get deleted by DetectDevices(). Therefore, an input provider must
		*    call this function every time in it's QueryDevices()-function.
		*/
		PLINPUT_API bool CheckDevice(const PLCore::String &sName);

		/**
		*  @brief
		*    Add a new input device
		*
		*  @param[in] sName
		*    Name of the input device (e.g. "Mouse0")
		*  @param[in] pDevice
		*    Pointer to the device (derived from Device), shouldn't be a null pointer (but a null pointer is caught internally)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    The function will fail, if the device is already present. Otherwise,
		*    the new device will be registered in the input system. On success, the input provider
		*    takes control over the device instance and will destroy it if it's no longer required.
		*/
		PLINPUT_API bool AddDevice(const PLCore::String &sName, Device *pDevice);


	//[-------------------------------------------------------]
	//[ Protected virtual Provider functions                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Query devices
		*
		*  @remarks
		*    The provider is supposed to detect all of it's devices in this function and then call
		*    AddDevice() with the specific name for each device. AddDevice() will fail if the device
		*    is already there, which can be checked before by calling CheckDevice(). The naming scheme
		*    is up to the provider itself, but something like "DeviceType0" .. "DeviceTypeN" is recommended.
		*/
		virtual void QueryDevices() = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::List<Device*>  m_lstDevices;	/**< List of devices */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_PROVIDER_H__
