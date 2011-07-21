/*********************************************************\
 *  File: PGMagic1.cpp                                   *
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
#include <PLCore/Tools/Timing.h>
#include <PLCore/System/System.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLParticleGroups/PGMagic1.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLScene;
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(PGMagic1)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGMagic1::PGMagic1() :
	Size(this),
	BuildPerSec(this),
	Material(this),
	Particles(this),
	EventHandlerUpdate(&PGMagic1::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial  = "Data/Textures/PGMagic1.dds";
	m_nParticles = 200;
}

/**
*  @brief
*    Destructor
*/
PGMagic1::~PGMagic1()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGMagic1::OnActivate(bool bActivate)
{
	// Call the base implementation
	PGPhysics::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}

void PGMagic1::OnAddedToVisibilityTree(VisNode &cVisNode)
{
	m_bUpdate = true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void PGMagic1::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		// If there are free particles, create new particles
		if (!(System::GetInstance()->GetMicroseconds() % BuildPerSec) && !(Math::GetRand())) {
			Particle *pParticle = AddParticle();
			if (pParticle) {
				pParticle->fSize		= Math::GetRandFloat()*10*Size;
				pParticle->fRot			= Math::GetRandNegFloat()*180;
				pParticle->vPos			= GetTransform().GetPosition();
				pParticle->fEnergy		= Math::GetRandFloat()*2;
				pParticle->vVelocity.x	= Math::GetRandNegFloat()*2;
				pParticle->vVelocity.y	= Math::GetRandNegFloat();
				pParticle->vVelocity.z	= Math::GetRandNegFloat()*2;
				pParticle->vColor.r		= 0.2f+Math::GetRandFloat()*0.8f;
				pParticle->vColor.g		= 0.2f+Math::GetRandFloat()*0.8f;
				pParticle->vColor.a		= 0.2f+Math::GetRandFloat()*0.8f;
				pParticle->vColor.a		= Math::Max(0.6f, Math::GetRandFloat());
				pParticle->nCustom1		= 0;
				pParticle->fCustom1		= 0.5f+Math::GetRandFloat()*2.0f;
				if (Math::GetRandFloat() > 0.5f)
					pParticle->fCustom1 = -pParticle->fCustom1;

				// Create second particle
				Particle *pParticleT = AddParticle();
				if (pParticleT) {
					pParticleT->vColor.r = pParticleT->vColor.g = pParticleT->vColor.b = 0.5f+Math::GetRandFloat()*0.5f;
					pParticleT->vColor.a = Math::Max(0.6f, Math::GetRandFloat());
					pParticleT->fCustom1  = -pParticleT->fCustom1;
				}
			}
		}

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				cParticle.fEnergy -= fTimeDiff/10;
				if (cParticle.fCustom1 > 0.0f)
					cParticle.vColor.a -= cParticle.fCustom1*fTimeDiff/5;
				else
					cParticle.vColor.a += cParticle.fCustom1*fTimeDiff/5;
				if (cParticle.vColor.a < 0.0f) {
					RemoveParticle(cParticle);
				} else {
					Vector3 vGravity = GetGravity();
					cParticle.fSize += fTimeDiff*Size*cParticle.fCustom1*2;
					if (cParticle.vColor.a > 0.2f)
						cParticle.vVelocity += vGravity*fTimeDiff/10-vGravity*(GetTransform().GetPosition()-cParticle.vPos).GetLength()*fTimeDiff/10;
					else
						cParticle.vVelocity -= vGravity*fTimeDiff;
					cParticle.vPos += cParticle.vVelocity*fTimeDiff;
					cParticle.fRot += fTimeDiff*cParticle.fCustom1;

					// Texture animation
					cParticle.fAnimationTimer += fTimeDiff;
					if (cParticle.fAnimationTimer > 0.2f) {
						cParticle.fAnimationTimer = 0.0f;
						cParticle.nAnimationStep++;
						if (cParticle.nAnimationStep >= GetTextureAnimationSteps())
							cParticle.nAnimationStep = 0;
					}
				}
			}
		}

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
