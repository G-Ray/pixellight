/*********************************************************\
 *  File: PGMagic2.h                                     *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
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


#ifndef __PLPG_PGMAGIC2_H__
#define __PLPG_PGMAGIC2_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLParticleGroups/SNParticleGroup.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Magic particle group 2
*/
class PGMagic2 : public SNParticleGroup {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPG_RTTI_EXPORT, PGMagic2, "PLParticleGroups", PLParticleGroups::SNParticleGroup, "Magic particle group 2")
		// Attributes
		pl_attribute(SmallParticles,			bool,			true,							ReadWrite,	DirectValue,	"Small particles?",									"")
		pl_attribute(TexAni,					bool,			false,							ReadWrite,	DirectValue,	"Texture animation?",								"")
		pl_attribute(Size,						float,			1.0f,							ReadWrite,	DirectValue,	"Size",												"Min='0.0001'")
			// Overwritten SNParticleGroup attributes
		pl_attribute(Material,					PLCore::String,	"Data/Textures/PGMagic2.dds",	ReadWrite,	GetSet,			"Particle group material",							"Type='Material Effect Image TextureAni'")
		pl_attribute(Particles,					PLCore::uint32,	100,							ReadWrite,	GetSet,			"Number of particles",								"Min=1")
		pl_attribute(TextureAnimationColumns,	PLCore::uint32,	2,								ReadWrite,	GetSet,			"Number of animation frame columns in the texture",	"")
		pl_attribute(TextureAnimationRows,		PLCore::uint32,	1,								ReadWrite,	GetSet,			"Number of animation frame rows in the texture",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPG_API PGMagic2();

		/**
		*  @brief
		*    Destructor
		*/
		PLPG_API virtual ~PGMagic2();


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNode functions          ]
	//[-------------------------------------------------------]
	private:
		virtual void InitFunction() override;
		virtual void OnActivate(bool bActivate) override;
		virtual void OnAddedToVisibilityTree(PLScene::VisNode &cVisNode) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bUpdate;	/**< Perform an update the next time? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups


#endif // __PLPG_PGMAGIC2_H__
