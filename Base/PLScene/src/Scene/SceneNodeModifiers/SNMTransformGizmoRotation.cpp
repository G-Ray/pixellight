/*********************************************************\
 *  File: SNMTransformGizmoRotation.cpp                  *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Tools.h>
#include <PLMath/Ray.h>
#include <PLMath/Intersect.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/MeshManager.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMTransformGizmoRotation.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLMesh;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMTransformGizmoRotation)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMTransformGizmoRotation::SNMTransformGizmoRotation(SceneNode &cSceneNode) : SNMTransformGizmo(cSceneNode),
	LineWidth(this),
	Tolerance(this),
	m_pDiskMeshHandler(new MeshHandler())
{
	// Get/create the 'disk silhouette' mesh
	MeshManager &cMeshManager = cSceneNode.GetSceneContext()->GetMeshManager();
	Mesh        *pMesh		  = cMeshManager.GetByName("SNTransformGizmo_rotate_disk");
	if (!pMesh) {
		pMesh = cMeshManager.CreateMesh("PLMesh::MeshCreatorDisk", true, "DrawStyle=\"2\" InnerRadius=\"0.0\" OuterRadius=\"5.0\" Slices=\"20\" Loops=\"1\"");
		if (pMesh)
			pMesh->SetName("SNTransformGizmo_rotate_disk");
	}
	m_pDiskMeshHandler->SetMesh(pMesh);
}

/**
*  @brief
*    Destructor
*/
SNMTransformGizmoRotation::~SNMTransformGizmoRotation()
{
	delete m_pDiskMeshHandler;
}


//[-------------------------------------------------------]
//[ Protected virtual SNMTransformGizmo functions         ]
//[-------------------------------------------------------]
uint32 SNMTransformGizmoRotation::DetermineSelected(const Ray &cRay) const
{
	const Vector3 &vRayPos = cRay.GetPos();
	const Vector3 &vRayDir = cRay.GetDir();
	float fMinDistance = -1.0f;
	uint32 nSelected = 0;
	Plane cPlane;
	Vector3 vV;

	// Get inner and outer tolerance radius
	const float fInnerRadius = 5.0f - Tolerance.Get();
	const float fOuterRadius = 5.0f + Tolerance.Get();

	// X axis
	cPlane.ComputeND(Vector3::Zero, Vector3::UnitX);
	if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
		const float fDistance = vV.GetLength();
		if (fDistance <= fOuterRadius && fDistance >= fInnerRadius) {
			fMinDistance = fDistance;
			nSelected    = XAxis;
		}
	}

	// Y axis...
	cPlane.ComputeND(Vector3::Zero, Vector3::UnitY);
	if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
		const float fDistance = vV.GetLength();
		if (fDistance <= fOuterRadius && fDistance >= fInnerRadius && (fMinDistance < 0.0f || fMinDistance > fDistance)) {
			fMinDistance = fDistance;
			nSelected    = YAxis;
		}
	}

	// Z axis...
	cPlane.ComputeND(Vector3::Zero, Vector3::UnitZ);
	if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
		const float fDistance = vV.GetLength();
		if (fDistance <= fOuterRadius && fDistance >= fInnerRadius && (fMinDistance < 0.0f || fMinDistance > fDistance))
			nSelected    = ZAxis;
	}

	// No axis selected
	return nSelected;
}

void SNMTransformGizmoRotation::DrawGizmo(Renderer &cRenderer, const VisNode *pVisNode)
{
	// This method does not need to be hightly effective because it's usually only called once per frame, so we prefere the generic way

	// The internal transform gizmo object space coordinate system we draw in is 10x10x10 so we don't have to work with to small numbers

	// [TODO] Remove the direct usage of fixed functions in here
	// Fixed functions support required
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		// Set identitiy projection and view matrix
		const Matrix4x4 mProjectionBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
		const Matrix4x4 mViewBackup       = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, Matrix4x4::Identity);
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       Matrix4x4::Identity);

		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::CullMode,     Cull::None);
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);
		cRenderer.SetRenderState(RenderState::LineWidth,	Tools::FloatToUInt32(LineWidth.Get()));

		// Draw X disk silhouette
		Matrix4x4 mLocal;
		mLocal.FromEulerAngleY(static_cast<float>(90.0f*Math::DegToRad));
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, m_mObjectSpaceToClipSpace*mLocal);
		pFixedFunctions->SetColor((m_nSelected & XAxis) ? Color4::Yellow : Color4::Red);
		m_pDiskMeshHandler->Draw(false, false);

		// Draw Y disk silhouette
		mLocal.FromEulerAngleX(static_cast<float>(90.0f*Math::DegToRad));
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, m_mObjectSpaceToClipSpace*mLocal);
		pFixedFunctions->SetColor((m_nSelected & YAxis) ? Color4::Yellow : Color4::Green);
		m_pDiskMeshHandler->Draw(false, false);

		// Draw Z disk silhouette
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, m_mObjectSpaceToClipSpace);
		pFixedFunctions->SetColor((m_nSelected & ZAxis) ? Color4::Yellow : Color4::Blue);
		m_pDiskMeshHandler->Draw(false, false);

		// Reset identitiy projection and view matrix
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProjectionBackup);
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       mViewBackup);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
