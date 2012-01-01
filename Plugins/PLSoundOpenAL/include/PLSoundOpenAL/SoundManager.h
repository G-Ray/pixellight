/*********************************************************\
 *  File: SoundManager.h                                 *
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


#ifndef __PLSOUNDOPENAL_SOUNDMANAGER_H__
#define __PLSOUNDOPENAL_SOUNDMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <ogg/os_types.h>
#include <PLSound/SoundManager.h>
#include "PLSoundOpenAL/PLSoundOpenAL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenAL sound backend
*
*  @note
*    - It looks like that it's not possible to playback ogg files with 2 channels in 3D (no attenuation and so on)
*/
class SoundManager : public PLSound::SoundManager {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SoundManager, "PLSoundOpenAL", PLSound::SoundManager, "OpenAL sound backend")
		// Attributes
		pl_attribute(DeviceName,	PLCore::String,	"",	ReadWrite,	DirectValue,	"Device name. If not defined the 'preferred device' is selected.",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Read from memory structure for the custom OpenAL file callbacks
		*/
		struct MemData {
			const PLCore::uint8 *pnData;	/**< Current data position (always valid!) */
			const PLCore::uint8 *pnDataE;	/**< Data end position (always valid!) */
		};


	//[-------------------------------------------------------]
	//[ Custom static OpenAL file callbacks                   ]
	//[-------------------------------------------------------]
	public:
		// From PLCore::File
		static size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
		static int seek_func(void *datasource, ogg_int64_t offset, int whence);
		static int close_func(void *datasource);
		static long tell_func(void *datasource);
		// From memory
		static size_t read_func_mem(void *ptr, size_t size, size_t nmemb, void *datasource);
		static int seek_func_mem(void *datasource, ogg_int64_t offset, int whence);
		static int close_func_mem(void *datasource);
		static long tell_func_mem(void *datasource);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SoundManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SoundManager();

		/**
		*  @brief
		*    Adds an active sound source to the sound manager
		*
		*  @param[in] cSource
		*    Sound source to add
		*
		*  @return
		*    'true' if all went fine and the sound source was added to the sound manager,
		*    else 'false'
		*
		*  @note
		*    - The sound source itself is only added to the sound manager's list of sources!
		*/
		bool AddActiveSource(PLSound::Source &cSource);

		/**
		*  @brief
		*    Removes an active sound source from the sound manager
		*
		*  @param[in] cSource
		*    Sound source to remove
		*
		*  @return
		*    'true' if all went fine and the sound source was removed from the
		*    sound manager, else 'false' (maybe the source isn't in the sound manager)
		*
		*  @note
		*    - The sound source itself isn't destroyed, it is just removed
		*      from the sound manager's list of sources! 
		*/
		bool RemoveActiveSource(PLSound::Source &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ALCdevice					    *m_pDevice;								/**< Device */
		ALCcontext					    *m_pContext;							/**< Context */
		PLCore::uint32					 m_nEAXLevel;							/**< EAX level */
		float							 m_fPitch;								/**< Master pitch */
		PLMath::Vector3					 m_vListenerAttributes[ListenerNumber];	/**< Listener attributes */
		PLCore::Array<PLSound::Source*>	 m_lstActiveSources;					/**< Current active sound sources of this sound manager */


	//[-------------------------------------------------------]
	//[ Public virtual PLSound::SoundManager functions        ]
	//[-------------------------------------------------------]
	public:
		virtual PLCore::String GetDescription() const override;
		virtual bool GetFormatList(PLCore::List<Format> &lstList) const override;
		virtual float GetVolume() const override;
		virtual void SetVolume(float fVolume = 1.0f) override;
		virtual float GetPitch() const override;
		virtual void SetPitch(float fPitch = 1.0f) override;
		virtual float GetDopplerFactor() const override;
		virtual void SetDopplerFactor(float fFactor = 1.0f) override;

		//[-------------------------------------------------------]
		//[ Create sound buffer/source                            ]
		//[-------------------------------------------------------]
		virtual PLSound::Buffer *CreateSoundBuffer(const PLCore::String &sFilename = "", bool bStream = false) override;
		virtual PLSound::Source *CreateSoundSource(PLSound::Buffer *pSoundBuffer = nullptr) override;

		//[-------------------------------------------------------]
		//[ Listener                                              ]
		//[-------------------------------------------------------]
		virtual PLMath::Vector3 GetListenerAttribute(EListener nAttribute) const override;
		virtual bool SetListenerAttribute(EListener nAttribute, const PLMath::Vector3 &vV) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::ResourceManager functions      ]
	//[-------------------------------------------------------]
	public:
		virtual bool Init() override;
		virtual bool DeInit() override;
		virtual bool Update() override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ResourceManager functions     ]
	//[-------------------------------------------------------]
	private:
		virtual PLSound::Buffer *CreateResource(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL


#endif // __PLSOUNDOPENAL_SOUNDMANAGER_H__
