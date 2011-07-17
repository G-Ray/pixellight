/*********************************************************\
 *  File: MeshCreatorTorus.cpp                           *
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
#include <PLMath/Vector2.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include <PLRenderer/Renderer/VertexBuffer.h>
#include "PLMesh/Geometry.h"
#include "PLMesh/MeshManager.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshMorphTarget.h"
#include "PLMesh/Creator/MeshCreatorTorus.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLMath;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(MeshCreatorTorus)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreatorTorus::MeshCreatorTorus() :
	Radius(this),
	Width(this),
	Sides(this),
	Rings(this),
	Offset(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreatorTorus::~MeshCreatorTorus()
{
}


//[-------------------------------------------------------]
//[ Private virtual MeshCreator functions                 ]
//[-------------------------------------------------------]
Mesh *MeshCreatorTorus::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
{
	// Call base implementation
	MeshCreator::Create(cMesh, nLODLevel, bStatic);

	// Get morph target
	MeshMorphTarget *pMorphTarget = cMesh.GetMorphTarget(0);
	if (pMorphTarget && pMorphTarget->GetVertexBuffer()) {
		// Increment sides and rings to handle texture coordinate wrapping
		uint32 nSides = Sides+1;
		uint32 nRings = Rings+1;

		// Allocate vertex buffer
		VertexBuffer *pVertexBuffer = pMorphTarget->GetVertexBuffer();
		pVertexBuffer->AddVertexAttribute(VertexBuffer::Position, 0, VertexBuffer::Float3);
		if (TexCoords)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::TexCoord, 0, VertexBuffer::Float2);
		if (Normals)
			pVertexBuffer->AddVertexAttribute(VertexBuffer::Normal,   0, VertexBuffer::Float3);
		pVertexBuffer->Allocate(nSides*nRings, bStatic ? Usage::Static : Usage::Dynamic);

		// Setup vertices
		if (pVertexBuffer->Lock(Lock::WriteOnly)) {
			const float fRingDelta = static_cast<float>(Math::Pi2/(nRings-1));
			const float fSideDelta = static_cast<float>(Math::Pi2/(nSides-1));
			float fRadius = Width;
			float fWidth  = Radius;

			// Get offset
			const Vector3 &vOffset = Offset.Get();

			uint32 nVertex = 0;
			for (uint32 i=0; i<nRings; i++) {
				float theta    = i*fRingDelta;
				float cosTheta = Math::Cos(theta);
				float sinTheta = Math::Sin(theta);

				for (uint32 j=0; j<nSides; j++) {
					float phi    = j*fSideDelta;
					float cosPhi = Math::Cos(phi);
					float sinPhi = Math::Sin(phi);
					float dist   = fWidth + fRadius*cosPhi;

					// (x,y,z) below is the parametric equation for a torus
					// when theta and phi both vary from 0 to pi
					float *pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Position));
					// x = cos(theta)*(R + r*cos(phi))
					pfVertex[Vector3::X] = vOffset.x + cosTheta*dist;
					// y = -sin(theta)*(R + r*cos(phi))
					pfVertex[Vector3::Y] = vOffset.y - sinTheta*dist;
					// z = r*sin(phi)
					pfVertex[Vector3::Z] = vOffset.z + fRadius*sinPhi;

					// Texture coordinates for each vertex
					if (TexCoords) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::TexCoord));
						pfVertex[Vector2::X] = static_cast<float>(j)/nSides;
						pfVertex[Vector2::Y] = static_cast<float>(i)/nRings;
					}

					// Normal
					if (Normals) {
						pfVertex = static_cast<float*>(pVertexBuffer->GetData(nVertex, VertexBuffer::Normal));
						pfVertex[Vector3::X] = cosTheta*cosPhi;
						pfVertex[Vector3::Y] = -sinTheta*cosPhi;
						pfVertex[Vector3::Z] = sinPhi;
					}
					nVertex++;
				}
			}

			// Unlock the vertex buffer
			pVertexBuffer->Unlock();
		}

		// Get LOD level
		MeshLODLevel *pLODLevel = cMesh.GetLODLevel(nLODLevel);
		if (pLODLevel && pLODLevel->GetIndexBuffer()) { // Setup geometry
			// Allocate index buffer
			IndexBuffer *pIndexBuffer = pLODLevel->GetIndexBuffer();
			uint32 nIndices = nSides*2*Rings;
			pIndexBuffer->SetElementTypeByMaximumIndex(pVertexBuffer->GetNumOfElements()-1);
			pIndexBuffer->Allocate(nIndices, bStatic ? Usage::Static : Usage::Dynamic);

			// Create and setup geometries
			Array<Geometry> &lstGeometries = *pLODLevel->GetGeometries();
			if (pIndexBuffer->Lock(Lock::WriteOnly)) {
				uint32 nIndex = 0;
				if (Order) {
					for (uint32 i=0; i<Rings; i++) {
						// Create and setup new geometry
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(i*nSides*2);
						cGeometry.SetIndexSize(nSides*2);
						uint32 ii = i+1;
						for (uint32 j=0; j<nSides; j++) {
							pIndexBuffer->SetData(nIndex++, i*nSides+j);
							pIndexBuffer->SetData(nIndex++, ii*nSides+j);
						}
					}
				} else {
					for (uint32 i=0; i<Rings; i++) {
						// Create and setup new geometry
						Geometry &cGeometry = lstGeometries.Add();
						cGeometry.SetPrimitiveType(Primitive::TriangleStrip);
						cGeometry.SetStartIndex(i*nSides*2);
						cGeometry.SetIndexSize(nSides*2);
						uint32 ii = i+1;
						for (uint32 j=0; j<nSides; j++) {
							pIndexBuffer->SetData(nIndex++, ii*nSides+j);
							pIndexBuffer->SetData(nIndex++, i*nSides+j);
						}
					}
				}

				// Unlock the index buffer
				pIndexBuffer->Unlock();
			}
		}
	}

	// Return the created mesh
	return &cMesh;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
