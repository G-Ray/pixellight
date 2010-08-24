/*********************************************************\
 *  File: DrawHelpersBackendFixedFunctions.cpp           *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
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
#include <PLGeneral/Tools/Tools.h>
#include <PLMath/Rectangle.h>
#include "PLRenderer/Renderer/Renderer.h"
#include "PLRenderer/Renderer/VertexBuffer.h"
#include "PLRenderer/Renderer/SamplerStates.h"
#include "PLRenderer/Renderer/FixedFunctions.h"
#include "PLRenderer/Renderer/TextureBuffer2D.h"
#include "PLRenderer/Renderer/TextureBufferRectangle.h"
#include "PLRenderer/Renderer/Backend/DrawHelpersBackendFixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DrawHelpersBackendFixedFunctions::DrawHelpersBackendFixedFunctions(Renderer &cRenderer) : DrawHelpersBackend(cRenderer)
{
}

/**
*  @brief
*    Destructor
*/
DrawHelpersBackendFixedFunctions::~DrawHelpersBackendFixedFunctions()
{
}


//[-------------------------------------------------------]
//[ Public virtual DrawHelpers functions                  ]
//[-------------------------------------------------------]
void DrawHelpersBackendFixedFunctions::Begin2DMode(float fX1, float fY1, float fX2, float fY2)
{
	// Is the 2D mode already set?
	if (m_b2DMode)
		End2DMode();

	// Set to viewport values?
	if (!fX1 && !fY1 && !fX2 && !fY2) {
		const Rectangle &cViewportRect = m_pRenderer->GetViewport();
		fX1 = cViewportRect.vMin.x;
		fY1 = cViewportRect.vMin.y;
		fX2 = cViewportRect.vMax.x;
		fY2 = cViewportRect.vMax.y;
	}

	// Backup settings
	m_fVirtualScreen[0] = fX1;
	m_fVirtualScreen[1] = fY1;
	m_fVirtualScreen[2] = fX2;
	m_fVirtualScreen[3] = fY2;

	// Fixed functions
	FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
	if (pFixedFunctions) {
		// Backup current projection and view matrix
		m_m2DModeProjBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
		m_m2DModeViewBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);

		// Set orthogonal projection matrix
		Matrix4x4 mProj;
		mProj.OrthoOffCenter(fX1, fX2, fY1, fY2, -1.0f, 1.0f);

		// Set identity view matrix
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProj);
		pFixedFunctions->SetTransformState(FixedFunctions::Transform::View, Matrix4x4::Identity);
	}

	// Set 2D mode status
	m_b2DMode = true;
}

void DrawHelpersBackendFixedFunctions::End2DMode()
{
	// Is the 2D mode set?
	if (m_b2DMode) {
		// Fixed functions
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Reset projection and view matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, m_m2DModeProjBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,		  m_m2DModeViewBackup);
		}

		// Set 2D mode status
		m_b2DMode = false;
	}
}

void DrawHelpersBackendFixedFunctions::DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const Vector2 &vPos, const Vector2 &vSize, const Color4 &cColor, float fAlphaReference,
												 const Vector2 &vTextureCoordinate, const Vector2 &vTextureCoordinateSize, const Matrix4x4 &mTexture)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Get the image size
		Vector2 vImageSize = vSize;
		if (vImageSize.IsNull()) {
			switch (cTextureBuffer.GetType()) {
				case Resource::TypeTextureBuffer2D:
					vImageSize.x = (float)((TextureBuffer2D&)cTextureBuffer).GetSize().x;
					vImageSize.y = (float)((TextureBuffer2D&)cTextureBuffer).GetSize().y;
					break;

				case Resource::TypeTextureBufferRectangle:
					vImageSize.x = (float)((TextureBufferRectangle&)cTextureBuffer).GetSize().x;
					vImageSize.y = (float)((TextureBufferRectangle&)cTextureBuffer).GetSize().y;
					break;

				default:
					return;	// Error, must be 2D or rectangle!
			}
		}

		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y + vImageSize.y;
			pfVertex[2] = m_fZValue2D;
			m_pTempVertexBuffer->SetColor(0, cColor);
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::TexCoord);
			pfVertex[0] = vTextureCoordinate.x;
			pfVertex[1] = vTextureCoordinate.y + vTextureCoordinateSize.y;

			// Vertex 1
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(1, VertexBuffer::Position);
			pfVertex[0] = vPos.x + vImageSize.x;
			pfVertex[1] = vPos.y + vImageSize.y;
			pfVertex[2] = m_fZValue2D;
			m_pTempVertexBuffer->SetColor(1, cColor);
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(1, VertexBuffer::TexCoord);
			pfVertex[0] = vTextureCoordinate.x + vTextureCoordinateSize.x;
			pfVertex[1] = vTextureCoordinate.y + vTextureCoordinateSize.y;

			// Vertex 2
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(2, VertexBuffer::Position);
			pfVertex[0] = vPos.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = m_fZValue2D;
			m_pTempVertexBuffer->SetColor(2, cColor);
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(2, VertexBuffer::TexCoord);
			pfVertex[0] = vTextureCoordinate.x;
			pfVertex[1] = vTextureCoordinate.y;

			// Vertex 3
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(3, VertexBuffer::Position);
			pfVertex[0] = vPos.x + vImageSize.x;
			pfVertex[1] = vPos.y;
			pfVertex[2] = m_fZValue2D;
			m_pTempVertexBuffer->SetColor(3, cColor);
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(3, VertexBuffer::TexCoord);
			pfVertex[0] = vTextureCoordinate.x + vTextureCoordinateSize.x;
			pfVertex[1] = vTextureCoordinate.y;

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Fixed functions
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Backup world matrix
			const Matrix4x4 mWorldBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

			// Set world matrix to identity
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);

			// Setup alpha test
			if (fAlphaReference < 1.0f) {
				// Alpha test is enabled
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable,    true);
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestFunction,  Compare::GreaterEqual);
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestReference, Tools::FloatToUInt32(fAlphaReference));
			} else {
				// Alpha test is disabled
				pFixedFunctions->SetRenderState(FixedFunctions::RenderState::AlphaTestEnable, false);
			}

			// Set vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

			// Set the texture buffer
			static const int TextureUnit = 0;
			m_pRenderer->SetTextureBuffer(TextureUnit, &cTextureBuffer);

			// Set texture matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Texture0, mTexture);

			// Set sampler states
			for (uint32 nState=0; nState<Sampler::Number; nState++)
				m_pRenderer->SetSamplerState(TextureUnit, (Sampler::Enum)nState, cSamplerStates.Get((Sampler::Enum)nState));

			// Draw image
			m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);

			// Restore world matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorldBackup);
		}
	}
}

void DrawHelpersBackendFixedFunctions::DrawPoint(const Color4 &cColor, const Vector2 &vPosition, float fSize)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
			pfVertex[0] = vPosition.x;
			pfVertex[1] = vPosition.y;
			pfVertex[2] = m_fZValue2D;
			m_pTempVertexBuffer->SetColor(0, cColor);

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Fixed functions
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Backup world matrix
			const Matrix4x4 mWorldBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

			// Set world matrix to identity
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);

			// Set vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

			// Set the point size
			m_pRenderer->SetRenderState(RenderState::PointSize, Tools::FloatToUInt32(fSize));

			// Draw point
			m_pRenderer->DrawPrimitives(Primitive::PointList, 0, 1);

			// Restore world matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorldBackup);
		}
	}
}

void DrawHelpersBackendFixedFunctions::DrawPoint(const Color4 &cColor, const Vector3 &vPosition, const Matrix4x4 &mObjectSpaceToClipSpace, float fSize)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
			pfVertex[0] = vPosition.x;
			pfVertex[1] = vPosition.y;
			pfVertex[2] = vPosition.z;
			m_pTempVertexBuffer->SetColor(0, cColor);

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Fixed functions
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Backup matrices
			const Matrix4x4 mProjectionBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
			const Matrix4x4 mViewBackup       = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
			const Matrix4x4 mWorldBackup      = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

			// Set transform - we just receive one final matrix from the user, so set projection and view to identity, and world to the given matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, Matrix4x4::Identity);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       Matrix4x4::Identity);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mObjectSpaceToClipSpace);

			// Set vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

			// Set the point size
			m_pRenderer->SetRenderState(RenderState::PointSize, Tools::FloatToUInt32(fSize));

			// Draw point
			m_pRenderer->DrawPrimitives(Primitive::PointList, 0, 1);

			// Restore matrices
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProjectionBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       mViewBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mWorldBackup);
		}
	}
}

void DrawHelpersBackendFixedFunctions::DrawLine(const Color4 &cColor, const Vector2 &vStartPosition, const Vector2 &vEndPosition, float fWidth)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
			pfVertex[0] = vStartPosition.x;
			pfVertex[1] = vStartPosition.y;
			pfVertex[2] = m_fZValue2D;
			m_pTempVertexBuffer->SetColor(0, cColor);

			// Vertex 1
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(1, VertexBuffer::Position);
			pfVertex[0] = vEndPosition.x;
			pfVertex[1] = vEndPosition.y;
			pfVertex[2] = m_fZValue2D;
			m_pTempVertexBuffer->SetColor(1, cColor);

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Fixed functions
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Backup world matrix
			const Matrix4x4 mWorldBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

			// Set world matrix to identity
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);

			// Set vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

			// Set the line width
			m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

			// Draw line
			m_pRenderer->DrawPrimitives(Primitive::LineList, 0, 2);

			// Restore world matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorldBackup);
		}
	}
}

void DrawHelpersBackendFixedFunctions::DrawLine(const Color4 &cColor, const Vector3 &vStartPosition, const Vector3 &vEndPosition, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
			pfVertex[0] = vStartPosition.x;
			pfVertex[1] = vStartPosition.y;
			pfVertex[2] = vStartPosition.z;
			m_pTempVertexBuffer->SetColor(0, cColor);

			// Vertex 1
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(1, VertexBuffer::Position);
			pfVertex[0] = vEndPosition.x;
			pfVertex[1] = vEndPosition.y;
			pfVertex[2] = vEndPosition.z;
			m_pTempVertexBuffer->SetColor(1, cColor);

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Fixed functions
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Backup matrices
			const Matrix4x4 mProjectionBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
			const Matrix4x4 mViewBackup       = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
			const Matrix4x4 mWorldBackup      = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

			// Set transform - we just receive one final matrix from the user, so set projection and view to identity, and world to the given matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, Matrix4x4::Identity);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       Matrix4x4::Identity);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mObjectSpaceToClipSpace);

			// Set vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

			// Set the line width
			m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

			// Draw line
			m_pRenderer->DrawPrimitives(Primitive::LineList, 0, 2);

			// Restore matrices
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProjectionBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       mViewBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mWorldBackup);
		}
	}
}

void DrawHelpersBackendFixedFunctions::DrawTriangle(const Color4 &cColor, const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	// Create vertex buffer
	if (CreateTempBuffes()) {
		// Setup the vertex buffer
		if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
			// Vertex 0
			float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
			pfVertex[0] = vV1.x;
			pfVertex[1] = vV1.y;
			pfVertex[2] = vV1.z;
			m_pTempVertexBuffer->SetColor(0, cColor);

			// Vertex 1
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(1, VertexBuffer::Position);
			pfVertex[0] = vV2.x;
			pfVertex[1] = vV2.y;
			pfVertex[2] = vV2.z;
			m_pTempVertexBuffer->SetColor(1, cColor);

			// Vertex 2
			pfVertex	= (float*)m_pTempVertexBuffer->GetData(2, VertexBuffer::Position);
			pfVertex[0] = vV3.x;
			pfVertex[1] = vV3.y;
			pfVertex[2] = vV3.z;
			m_pTempVertexBuffer->SetColor(2, cColor);

			// Unlock the vertex buffer
			m_pTempVertexBuffer->Unlock();
		}

		// Fixed functions
		FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
		if (pFixedFunctions) {
			// Backup matrices
			const Matrix4x4 mProjectionBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
			const Matrix4x4 mViewBackup       = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
			const Matrix4x4 mWorldBackup      = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

			// Set transform - we just receive one final matrix from the user, so set projection and view to identity, and world to the given matrix
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, Matrix4x4::Identity);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       Matrix4x4::Identity);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mObjectSpaceToClipSpace);

			// Set vertex buffer
			pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

			// Set the line width
			if (fWidth)
				m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

			// Draw triangle
			m_pRenderer->DrawPrimitives(fWidth ? Primitive::LineList : Primitive::TriangleList, 0, 3);

			// Restore matrices
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProjectionBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       mViewBackup);
			pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mWorldBackup);
		}
	}
}

void DrawHelpersBackendFixedFunctions::DrawQuad(const Color4 &cColor, const Vector2 &vPos, const Vector2 &vSize, float fWidth)
{
	// Draw lines?
	if (fWidth) {
		// Set the line width
		m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

		// Draw single lines
		const Vector3 &vV1 = vPos;
		const Vector3 vV2(vPos.x + vSize.x, vPos.y);
		const Vector3 vV3(vPos.x + vSize.x, vPos.y + vSize.y);
		const Vector3 vV4(vPos.x, vPos.y + vSize.y);
		DrawLine(cColor, vV1, vV2, fWidth);
		DrawLine(cColor, vV2, vV3, fWidth);
		DrawLine(cColor, vV3, vV4, fWidth);
		DrawLine(cColor, vV4, vV1, fWidth);
	} else {
		// Create vertex buffer
		if (CreateTempBuffes()) {
			// Setup the vertex buffer
			if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
				// Vertex 0
				float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
				pfVertex[0] = vPos.x;
				pfVertex[1] = vPos.y + vSize.y;
				pfVertex[2] = m_fZValue2D;
				m_pTempVertexBuffer->SetColor(0, cColor);

				// Vertex 1
				pfVertex	= (float*)m_pTempVertexBuffer->GetData(1, VertexBuffer::Position);
				pfVertex[0] = vPos.x + vSize.x;
				pfVertex[1] = vPos.y + vSize.y;
				pfVertex[2] = m_fZValue2D;
				m_pTempVertexBuffer->SetColor(1, cColor);

				// Vertex 2
				pfVertex	= (float*)m_pTempVertexBuffer->GetData(2, VertexBuffer::Position);
				pfVertex[0] = vPos.x;
				pfVertex[1] = vPos.y;
				pfVertex[2] = m_fZValue2D;
				m_pTempVertexBuffer->SetColor(2, cColor);

				// Vertex 3
				pfVertex	= (float*)m_pTempVertexBuffer->GetData(3, VertexBuffer::Position);
				pfVertex[0] = vPos.x + vSize.x;
				pfVertex[1] = vPos.y;
				pfVertex[2] = m_fZValue2D;
				m_pTempVertexBuffer->SetColor(3, cColor);

				// Unlock the vertex buffer
				m_pTempVertexBuffer->Unlock();
			}

			// Fixed functions
			FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
			if (pFixedFunctions) {
				// Backup world matrix
				const Matrix4x4 mWorldBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

				// Set world matrix to identity
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, Matrix4x4::Identity);

				// Set vertex buffer
				pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

				// Draw quad
				m_pRenderer->DrawPrimitives(Primitive::TriangleStrip, 0, 4);

				// Restore world matrix
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::World, mWorldBackup);
			}
		}
	}
}

void DrawHelpersBackendFixedFunctions::DrawQuad(const Color4 &cColor, const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3, const Vector3 &vV4, const Matrix4x4 &mObjectSpaceToClipSpace, float fWidth)
{
	// Draw lines?
	if (fWidth) {
		// Set the line width
		m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

		// Draw single lines
		DrawLine(cColor, vV1, vV2, fWidth);
		DrawLine(cColor, vV2, vV3, fWidth);
		DrawLine(cColor, vV3, vV4, fWidth);
		DrawLine(cColor, vV4, vV1, fWidth);
	} else {
		// Create vertex buffer
		if (CreateTempBuffes()) {
			// Setup the vertex buffer
			if (m_pTempVertexBuffer->Lock(Lock::WriteOnly)) {
				// Vertex 0
				float *pfVertex = (float*)m_pTempVertexBuffer->GetData(0, VertexBuffer::Position);
				pfVertex[0] = vV1.x;
				pfVertex[1] = vV1.y;
				pfVertex[2] = vV1.z;
				m_pTempVertexBuffer->SetColor(0, cColor);

				// Vertex 1
				pfVertex	= (float*)m_pTempVertexBuffer->GetData(1, VertexBuffer::Position);
				pfVertex[0] = vV2.x;
				pfVertex[1] = vV2.y;
				pfVertex[2] = vV2.z;
				m_pTempVertexBuffer->SetColor(1, cColor);

				// Vertex 2
				pfVertex	= (float*)m_pTempVertexBuffer->GetData(2, VertexBuffer::Position);
				pfVertex[0] = vV3.x;
				pfVertex[1] = vV3.y;
				pfVertex[2] = vV3.z;
				m_pTempVertexBuffer->SetColor(2, cColor);

				// Vertex 3
				pfVertex	= (float*)m_pTempVertexBuffer->GetData(3, VertexBuffer::Position);
				pfVertex[0] = vV4.x;
				pfVertex[1] = vV4.y;
				pfVertex[2] = vV4.z;
				m_pTempVertexBuffer->SetColor(3, cColor);

				// Unlock the vertex buffer
				m_pTempVertexBuffer->Unlock();
			}

			// Fixed functions
			FixedFunctions *pFixedFunctions = m_pRenderer->GetFixedFunctions();
			if (pFixedFunctions) {
				// Backup matrices
				const Matrix4x4 mProjectionBackup = pFixedFunctions->GetTransformState(FixedFunctions::Transform::Projection);
				const Matrix4x4 mViewBackup       = pFixedFunctions->GetTransformState(FixedFunctions::Transform::View);
				const Matrix4x4 mWorldBackup      = pFixedFunctions->GetTransformState(FixedFunctions::Transform::World);

				// Set transform - we just receive one final matrix from the user, so set projection and view to identity, and world to the given matrix
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, Matrix4x4::Identity);
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       Matrix4x4::Identity);
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mObjectSpaceToClipSpace);

				// Set vertex buffer
				pFixedFunctions->SetVertexBuffer(m_pTempVertexBuffer);

				// Set the line width
				if (fWidth)
					m_pRenderer->SetRenderState(RenderState::LineWidth, Tools::FloatToUInt32(fWidth));

				// Draw quad
				m_pRenderer->DrawPrimitives(fWidth ? Primitive::LineStrip : Primitive::TriangleStrip, 0, 4);

				// Restore matrices
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::Projection, mProjectionBackup);
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::View,       mViewBackup);
				pFixedFunctions->SetTransformState(FixedFunctions::Transform::World,      mWorldBackup);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
