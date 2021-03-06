/*********************************************************\
 *  File: SNMTransformGizmoScale.cpp                     *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Ray.h>
#include <PLMath/Intersect.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FixedFunctions.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLMesh/MeshManager.h>
#include "PLScene/Visibility/VisNode.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodeModifiers/SNMTransformGizmoScale.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLMesh;
using namespace PLRenderer;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMTransformGizmoScale)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMTransformGizmoScale::SNMTransformGizmoScale(SceneNode &cSceneNode) : SNMTransformGizmo(cSceneNode),
	LineWidth(this),
	Tolerance(this),
	m_pMeshHandler(new MeshHandler())
{
	// Get/create the 'cone' mesh for the axis arrow
	MeshManager &cMeshManager = GetSceneNode().GetSceneContext()->GetMeshManager();
	Mesh		*pMesh		  = cMeshManager.GetByName("SNTransformGizmo_scale_sphere");
	if (!pMesh) {
		// Create mesh
		pMesh = cMeshManager.CreateMesh("PLMesh::MeshCreatorSphere", true, "Radius=\"0.4\"' Detail=\"10\"");
		if (pMesh)
			pMesh->SetName("SNTransformGizmo_scale_sphere");
	}
	m_pMeshHandler->SetMesh(pMesh);
}

/**
*  @brief
*    Destructor
*/
SNMTransformGizmoScale::~SNMTransformGizmoScale()
{
	delete m_pMeshHandler;
}


//[-------------------------------------------------------]
//[ Protected virtual SNMTransformGizmo functions         ]
//[-------------------------------------------------------]
uint32 SNMTransformGizmoScale::DetermineSelected(const Ray &cRay) const
{
	const Vector3 &vRayPos = cRay.GetPos();
	const Vector3 &vRayDir = cRay.GetDir();
	uint32 nSelected = 0;

	// Perform ray/triangle collision detection for multiple axis selection. The distance found collision
	// is used. The XYZ-triangles are topmost.
	// XYZ
	Vector3 vN, vV;
	vN.GetFaceNormal(Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 5.0f));
	if (Intersect::TriangleRayNegative(Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 5.0f), vN,                     cRay, vV) ||
		Intersect::TriangleRayNegative(Vector3::Zero,             Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 5.0f), Vector3::NegativeUnitX, cRay, vV) ||
		Intersect::TriangleRayNegative(Vector3(5.0f, 0.0f, 0.0f), Vector3::Zero,             Vector3(0.0f, 0.0f, 5.0f), Vector3::NegativeUnitY, cRay, vV) ||
		Intersect::TriangleRayNegative(Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3::Zero,             Vector3::NegativeUnitZ, cRay, vV)) {
		nSelected = XAxis | YAxis | ZAxis;
	} else {
		Vector3 vMax = vRayPos;

		// XY
		if (Intersect::TriangleRayNegative(Vector3(0.0f, 8.0f, 0.0f), Vector3(8.0f, 0.0f, 0.0f), Vector3(5.0f, 0.0f, 0.0f), Vector3::UnitZ, cRay, vV) ||
			Intersect::TriangleRayNegative(Vector3(0.0f, 8.0f, 0.0f), Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3::UnitZ, cRay, vV)) {
			vMax = vV;
			nSelected = XAxis | YAxis;
		}

		// XZ
		if ((Intersect::TriangleRayNegative(Vector3(0.0f, 0.0f, 5.0f), Vector3(5.0f, 0.0f, 0.0f), Vector3(8.0f, 0.0f, 0.0f), Vector3::UnitY, cRay, vV) ||
			 Intersect::TriangleRayNegative(Vector3(0.0f, 0.0f, 5.0f), Vector3(8.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 8.0f), Vector3::UnitY, cRay, vV)) &&
			(vRayPos-vV).GetSquaredLength() > (vRayPos-vMax).GetSquaredLength()) {
			vMax = vV;
			nSelected = XAxis | ZAxis;
		}

		// YZ
		if ((Intersect::TriangleRayNegative(Vector3(0.0f, 0.0f, 8.0f), Vector3(0.0f, 8.0f, 0.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3::UnitX, cRay, vV) ||
			 Intersect::TriangleRayNegative(Vector3(0.0f, 0.0f, 8.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 5.0f), Vector3::UnitX, cRay, vV)) &&
			(vRayPos-vV).GetSquaredLength() > (vRayPos-vMax).GetSquaredLength()) {
			vMax = vV;
			nSelected = YAxis | ZAxis;
		}
	}

	// If no multiple axis selection was found, check whether a single axis is selected. This is done
	// by calculating a ray/plane intersection of the plane the axis is on. Then the closest point on the
	// axis line to this ray/plane intersection point is calculated. If this two point are not too far away
	// the given axis is selected.
	if (!nSelected) {
		const float fTolerance = Tolerance.Get();
		Plane cPlane;
		Vector3 vMin;

		// X axis
		cPlane.ComputeND(Vector3::Zero, Vector3::UnitY);
		if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
			vMin = vV.ClosestPointOnLine(Vector3::Zero, Vector3(10.0f, 0.0f, 0.0f));
			if ((vMin-vV).GetSquaredLength() < fTolerance)
				nSelected = XAxis;
		}
		if (nSelected != XAxis) {
			// Try another plane for sure
			cPlane.ComputeND(Vector3::Zero, Vector3::UnitZ);
			if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
				vMin = vV.ClosestPointOnLine(Vector3::Zero, Vector3(10.0f, 0.0f, 0.0f));
				if ((vMin-vV).GetSquaredLength() < fTolerance)
					nSelected = XAxis;
			}
		}

		// Y axis
		cPlane.ComputeND(Vector3::Zero, Vector3::UnitX);
		if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
			vMin = vV.ClosestPointOnLine(Vector3::Zero, Vector3(0.0f, 10.0f, 0.0f));
			if ((vMin-vV).GetSquaredLength() < fTolerance)
				nSelected = YAxis;
		}
		if (nSelected != YAxis) {
			// Try another plane for sure
			cPlane.ComputeND(Vector3::Zero, Vector3::UnitZ);
			if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
				vMin = vV.ClosestPointOnLine(Vector3::Zero, Vector3(0.0f, 10.0f, 0.0f));
				if ((vMin-vV).GetSquaredLength() < fTolerance)
					nSelected = YAxis;
			}
		}

		// Z axis
		cPlane.ComputeND(Vector3::Zero, Vector3::UnitY);
		if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
			vMin = vV.ClosestPointOnLine(Vector3::Zero, Vector3(0.0f, 0.0f, 10.0f));
			if ((vMin-vV).GetSquaredLength() < fTolerance)
				nSelected = ZAxis;
		}
		if (nSelected != ZAxis) {
			// Try another plane for sure
			cPlane.ComputeND(Vector3::Zero, Vector3::UnitX);
			if (Intersect::PlaneRayNegative(cPlane, vRayPos, vRayDir, vV)) {
				vMin = vV.ClosestPointOnLine(Vector3::Zero, Vector3(0.0f, 0.0f, 10.0f));
				if ((vMin-vV).GetSquaredLength() < fTolerance)
					nSelected = ZAxis;
			}
		}
	}

	// Return the selected axis
	return nSelected;
}

void SNMTransformGizmoScale::DrawGizmo(Renderer &cRenderer, const VisNode *pVisNode)
{
	// This method does not need to be highly effective because it's usually only called once per frame, so we prefer the generic way

	// The internal transform gizmo object space coordinate system we draw in is 10x10x10 so we don't have to work with to small numbers

	// Get primary axis color (changed when axis is selected)
	const Color4 &cPrimaryXAxisColor = (m_nSelected & XAxis) ? Color4::Yellow : Color4::Red;
	const Color4 &cPrimaryYAxisColor = (m_nSelected & YAxis) ? Color4::Yellow : Color4::Green;
	const Color4 &cPrimaryZAxisColor = (m_nSelected & ZAxis) ? Color4::Yellow : Color4::Blue;

	// Setup render states
	cRenderer.GetRendererContext().GetEffectManager().Use();
	cRenderer.SetRenderState(RenderState::CullMode,     Cull::None);
	cRenderer.SetRenderState(RenderState::ZEnable,      false);
	cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

	// [TODO] Remove the direct usage of fixed functions in here
	// Fixed functions support required
	FixedFunctions *pFixedFunctions = cRenderer.GetFixedFunctions();
	if (pFixedFunctions) {
		{ // Draw sphere meshes
			Matrix4x4 mLocal;

			// Set identity projection and view matrix
			const Matrix4x4 mProjectionBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
			const Matrix4x4 mViewBackup       = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, Matrix4x4::Identity);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       Matrix4x4::Identity);

			// Draw X sphere mesh
			mLocal.FromEulerAngleZ(static_cast<float>(-90.0f*Math::DegToRad));
			mLocal.SetTranslation(10.0f, 0.0f, 0.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, m_mObjectSpaceToClipSpace*mLocal);
			pFixedFunctions->SetColor(cPrimaryXAxisColor);
			m_pMeshHandler->Draw(false, false);

			// Draw Y sphere mesh
			mLocal.SetTranslationMatrix(0.0f, 10.0f, 0.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, m_mObjectSpaceToClipSpace*mLocal);
			pFixedFunctions->SetColor(cPrimaryYAxisColor);
			m_pMeshHandler->Draw(false, false);

			// Draw Z sphere mesh
			mLocal.FromEulerAngleX(static_cast<float>(90.0f*Math::DegToRad));
			mLocal.SetTranslation(0.0f, 0.0f, 10.0f);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, m_mObjectSpaceToClipSpace*mLocal);
			pFixedFunctions->SetColor(cPrimaryZAxisColor);
			m_pMeshHandler->Draw(false, false);

			// Reset identity projection and view matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProjectionBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       mViewBackup);
		}
	}

	// Get draw helpers instance
	DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
	const float fLineWidth = LineWidth.Get();

	{ // Draw axis lines
		// X axis
		if (!(m_nSelected & XAxis) || !(m_nSelected & YAxis) || !(m_nSelected & ZAxis))
			cDrawHelpers.DrawLine(cPrimaryXAxisColor, Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 0.0f, 0.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		{ // XY
			const Color4 &cColor = (m_nSelected & XAxis) && (m_nSelected & YAxis) ? Color4::Yellow : Color4::Red;
			cDrawHelpers.DrawLine(cColor, Vector3(5.0f, 0.0f, 0.0f), Vector3(2.5f, 2.5f, 0.0f), m_mObjectSpaceToClipSpace, fLineWidth);
			cDrawHelpers.DrawLine(cColor, Vector3(8.0f, 0.0f, 0.0f), Vector3(4.0f, 4.0f, 0.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		}
		{ // XZ
			const Color4 &cColor = ((m_nSelected & XAxis) && (m_nSelected & ZAxis)) ? Color4::Yellow : Color4::Red;
			cDrawHelpers.DrawLine(cColor, Vector3(5.0f, 0.0f, 0.0f), Vector3(2.5f, 0.0f, 2.5f), m_mObjectSpaceToClipSpace, fLineWidth);
			cDrawHelpers.DrawLine(cColor, Vector3(8.0f, 0.0f, 0.0f), Vector3(4.0f, 0.0f, 4.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		}

		// Y axis
		if (!(m_nSelected & XAxis) || !(m_nSelected & YAxis) || !(m_nSelected & ZAxis))
			cDrawHelpers.DrawLine(cPrimaryYAxisColor, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 10.0f, 0.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		{ // XY
			const Color4 &cColor = ((m_nSelected & XAxis) && (m_nSelected & YAxis)) ? Color4::Yellow : Color4::Green;
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 5.0f, 0.0f), Vector3(2.5f, 2.5f, 0.0f), m_mObjectSpaceToClipSpace, fLineWidth);
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 8.0f, 0.0f), Vector3(4.0f, 4.0f, 0.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		}
		{ // YZ
			const Color4 &cColor = ((m_nSelected & YAxis) && (m_nSelected & ZAxis)) ? Color4::Yellow : Color4::Green;
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 2.5f, 2.5f), m_mObjectSpaceToClipSpace, fLineWidth);
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 8.0f, 0.0f), Vector3(0.0f, 4.0f, 4.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		}

		// Z axis
		if (!(m_nSelected & XAxis) || !(m_nSelected & YAxis) || !(m_nSelected & ZAxis))
			cDrawHelpers.DrawLine(cPrimaryZAxisColor, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 10.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		{ // XZ
			const Color4 &cColor = ((m_nSelected & XAxis) && (m_nSelected & ZAxis)) ? Color4::Yellow : Color4::Blue;
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 0.0f, 5.0f), Vector3(2.5f, 0.0f, 2.5f), m_mObjectSpaceToClipSpace, fLineWidth);
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 0.0f, 8.0f), Vector3(4.0f, 0.0f, 4.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		}
		{ // YZ
			const Color4 &cColor = ((m_nSelected & YAxis) && (m_nSelected & ZAxis)) ? Color4::Yellow : Color4::Blue;
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 0.0f, 5.0f), Vector3(0.0f, 2.5f, 2.5f), m_mObjectSpaceToClipSpace, fLineWidth);
			cDrawHelpers.DrawLine(cColor, Vector3(0.0f, 0.0f, 8.0f), Vector3(0.0f, 4.0f, 4.0f), m_mObjectSpaceToClipSpace, fLineWidth);
		}
	}
	{ // Draw selected axis combination
		// XY
		if ((m_nSelected & XAxis) && (m_nSelected & YAxis) && !(m_nSelected & ZAxis)) {
			cDrawHelpers.DrawQuad(Color4::Yellow, Vector3(0.0f, 8.0f, 0.0f), Vector3(8.0f, 0.0f, 0.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3(5.0f, 0.0f, 0.0f), m_mObjectSpaceToClipSpace);

		// XZ
		} else if ((m_nSelected & XAxis) && !(m_nSelected & YAxis) && (m_nSelected & ZAxis)) {
			cDrawHelpers.DrawQuad(Color4::Yellow, Vector3(0.0f, 0.0f, 5.0f), Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 8.0f), Vector3(8.0f, 0.0f, 0.0f), m_mObjectSpaceToClipSpace);

		// YZ
		} else if (!(m_nSelected & XAxis) && (m_nSelected & YAxis) && (m_nSelected & ZAxis)) {
			cDrawHelpers.DrawQuad(Color4::Yellow, Vector3(0.0f, 0.0f, 8.0f), Vector3(0.0f, 8.0f, 0.0f), Vector3(0.0f, 0.0f, 5.0f), Vector3(0.0f, 5.0f, 0.0f), m_mObjectSpaceToClipSpace);

		// XYZ
		} else if ((m_nSelected & XAxis) && (m_nSelected & YAxis) && (m_nSelected & ZAxis)) {
			cDrawHelpers.DrawTriangle(Color4::Yellow, Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 5.0f), m_mObjectSpaceToClipSpace);
			cDrawHelpers.DrawTriangle(Color4::Yellow, Vector3::Zero,             Vector3(0.0f, 5.0f, 0.0f), Vector3(0.0f, 0.0f, 5.0f), m_mObjectSpaceToClipSpace);
			cDrawHelpers.DrawTriangle(Color4::Yellow, Vector3(5.0f, 0.0f, 0.0f), Vector3::Zero,             Vector3(0.0f, 0.0f, 5.0f), m_mObjectSpaceToClipSpace);

			// Draw axis in selected color
			// X axis
			cDrawHelpers.DrawLine(Color4::Yellow, Vector3(5.0f, 0.0f, 0.0f), Vector3(10.0f,  0.0f,  0.0f), m_mObjectSpaceToClipSpace, fLineWidth*2);
			// Y axis
			cDrawHelpers.DrawLine(Color4::Yellow, Vector3(0.0f, 5.0f, 0.0f), Vector3( 0.0f, 10.0f,  0.0f), m_mObjectSpaceToClipSpace, fLineWidth*2);
			// Z axis
			cDrawHelpers.DrawLine(Color4::Yellow, Vector3(0.0f, 0.0f, 5.0f), Vector3( 0.0f,  0.0f, 10.0f), m_mObjectSpaceToClipSpace, fLineWidth*2);
		}
	}

	// Draw texts
	Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
	if (pFont) {
		// X axis
		cDrawHelpers.DrawText(*pFont, 'X', cPrimaryXAxisColor, Vector3(11.0f, 0.0f, 0.0f), m_mObjectSpaceToClipSpace, Font::CenterText);

		// Y axis
		cDrawHelpers.DrawText(*pFont, 'Y', cPrimaryYAxisColor, Vector3(0.0f, 11.0f, 0.0f), m_mObjectSpaceToClipSpace, Font::CenterText);

		// Z axis
		cDrawHelpers.DrawText(*pFont, 'Z', cPrimaryZAxisColor, Vector3(0.0f, 0.0f, 11.0f), m_mObjectSpaceToClipSpace, Font::CenterText);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
