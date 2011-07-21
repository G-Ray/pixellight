/*********************************************************\
 *  File: SNSound.h                                      *
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


#ifndef __PLSOUND_SCENENODES_SNSOUND_H__
#define __PLSOUND_SCENENODES_SNSOUND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNode.h>
#include "PLSound/PLSound.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLSound {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Source;
class SCSound;
class ResourceHandler;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene node which is able to playback a sound
*/
class SNSound : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			Stream          = 1<<10,	/**< Stream the file (recommended for large files!) */
			No3D            = 1<<11,	/**< No 3D sound */
			NoLoop          = 1<<12,	/**< Do not loop the sound playback */
			NoStartPlayback = 1<<13		/**< Do not start sound playback after scene node initialization */
		};
		pl_enum(EFlags)
			pl_enum_base(PLScene::SceneNode::EFlags)
			pl_enum_value(Stream,			"Stream the file (recommended for large files!)")
			pl_enum_value(No3D,				"No 3D sound")
			pl_enum_value(NoLoop,			"Do not loop the sound playback")
			pl_enum_value(NoStartPlayback,	"Do not start sound playback after scene node initialization")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLSOUND_RTTI_EXPORT, SNSound, "PLSound", PLScene::SceneNode, "Scene node which is able to playback a sound")
		// Properties
		pl_properties
			pl_property("Icon",	"Data/Textures/IconSound.dds")
		pl_properties_end
		// Attributes
		pl_attribute(Sound,				PLCore::String,			"",			ReadWrite,	GetSet,		"Filename of the sound which should be played (full path, supported file formats are API dependent)",			"Ext='mp3 ogg wav mid midi it mod s3m xm'")
		pl_attribute(Volume,			float,					1.0f,		ReadWrite,	GetSet,		"Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)",												"Min='0.0' Max='1.0'")
		pl_attribute(Pitch,				float,					1.0f,		ReadWrite,	GetSet,		"Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster)",										"Min='0.0'")
		pl_attribute(ReferenceDistance,	float,					1.0f,		ReadWrite,	GetSet,		"Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively",	"Min='0.0'")
		pl_attribute(MaxDistance,		float,					10000.0f,	ReadWrite,	GetSet,		"Defines a distance beyond which the source will not be further attenuated by distance",						"Min='0.0'")
		pl_attribute(RolloffFactor,		float,					1.0f,		ReadWrite,	GetSet,		"This will scale the distance attenuation over the applicable range",											"Min='0.0'")
			// Overwritten PLScene::SceneNode attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,			ReadWrite,	GetSet,		"Flags",																										"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Slots
		pl_slot_0(OnPosition,	"Called when the scene node position changed",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLSOUND_API PLCore::String GetSound() const;
		PLSOUND_API void SetSound(const PLCore::String &sValue);
		PLSOUND_API float GetVolume() const;
		PLSOUND_API void SetVolume(float fValue);
		PLSOUND_API float GetPitch() const;
		PLSOUND_API void SetPitch(float fValue);
		PLSOUND_API float GetReferenceDistance() const;
		PLSOUND_API void SetReferenceDistance(float fValue);
		PLSOUND_API float GetMaxDistance() const;
		PLSOUND_API void SetMaxDistance(float fValue);
		PLSOUND_API float GetRolloffFactor() const;
		PLSOUND_API void SetRolloffFactor(float fValue);
		PLSOUND_API virtual void SetFlags(PLCore::uint32 nValue);	// From PLScene::SceneNode


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLSOUND_API SNSound();

		/**
		*  @brief
		*    Destructor
		*/
		PLSOUND_API virtual ~SNSound();

		/**
		*  @brief
		*    Returns the sound source
		*
		*  @return
		*    The sound source, can be a null pointer
		*/
		PLSOUND_API Source *GetSoundSource() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String m_sSound;				/**< Filename of the sound which should be played (full path, supported file formats are API dependent) */
		float		   m_fVolume;				/**< Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume) */
		float		   m_fPitch;				/**< Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster) */
		float		   m_fReferenceDistance;	/**< Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively */
		float		   m_fMaxDistance;			/**< Defines a distance beyond which the source will not be further attenuated by distance */
		float		   m_fRolloffFactor;		/**< This will scale the distance attenuation over the applicable range */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Returns the sound container this sound is in
		*
		*  @return
		*    The sound container this sound is in, a null pointer on error
		*/
		SCSound *GetSoundContainer() const;

		/**
		*  @brief
		*    Loads/reloads the sound
		*/
		void Load();

		/**
		*  @brief
		*    Called when the scene node position changed
		*/
		void OnPosition();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ResourceHandler *m_pSoundSourceHandler;	/**< Sound source handler (always valid!) */


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNode functions        ]
	//[-------------------------------------------------------]
	protected:
		PLSOUND_API virtual void InitFunction();

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSound


#endif // __PLSOUND_SCENENODES_SNSOUND_H__
