/*********************************************************\
 *  File: Geometry.h                                     *
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


#ifndef __PLMESH_GEOMETRY_H__
#define __PLMESH_GEOMETRY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/Types.h>
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    A geometry defines a part of the index buffer to describe a geometry
*/
class Geometry {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API Geometry();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API ~Geometry();

		/**
		*  @brief
		*    Returns the geometrie's name
		*
		*  @return
		*    Name of the geometry
		*/
		PLMESH_API PLCore::String GetName() const;

		/**
		*  @brief
		*    Sets the geometry name
		*
		*  @param[in] sName
		*    Name of the geometry
		*/
		PLMESH_API void SetName(const PLCore::String &sName = "");

		/**
		*  @brief
		*    Returns the geometry flags
		*
		*  @return
		*    Geometry flags
		*/
		PLMESH_API PLCore::uint32 GetFlags() const;

		/**
		*  @brief
		*    Sets the geometry flags
		*
		*  @param[in] nFlags
		*    Geometry flags
		*/
		PLMESH_API void SetFlags(PLCore::uint32 nFlags = 0);

		/**
		*  @brief
		*    Returns the geometry active flag
		*
		*  @return
		*    'true' if the geometry is active, else 'false'
		*/
		PLMESH_API bool IsActive() const;

		/**
		*  @brief
		*    Sets the geometry active flag
		*
		*  @param[in] bActive
		*    'true' if the geometry is active, else 'false'
		*/
		PLMESH_API void SetActive(bool bActive = true);

		/**
		*  @brief
		*    Returns the type of the primitives
		*
		*  @return
		*    Primitive type (e.g. triangles, triangle strips etc.)
		*/
		PLMESH_API PLRenderer::Primitive::Enum GetPrimitiveType() const;

		/**
		*  @brief
		*    Sets the type of the primitives
		*
		*  @param[in] nPrimitiveType
		*    Primitive type (e.g. triangles, triangle strips etc.)
		*/
		PLMESH_API void SetPrimitiveType(PLRenderer::Primitive::Enum nPrimitiveType = PLRenderer::Primitive::Unknown);

		/**
		*  @brief
		*    Returns the material used by this geometry
		*
		*  @return
		*    Number of the material that is used
		*/
		PLMESH_API PLCore::uint32 GetMaterial() const;

		/**
		*  @brief
		*    Sets the material used by this geometry
		*
		*  @param[in] nMaterial
		*    Number of the material that is used
		*/
		PLMESH_API void SetMaterial(PLCore::uint32 nMaterial = 0);

		/**
		*  @brief
		*    Returns the start index
		*
		*  @return
		*    Start index
		*/
		PLMESH_API PLCore::uint32 GetStartIndex() const;

		/**
		*  @brief
		*    Sets the start index
		*
		*  @param[in] nStartIndex
		*    Start index
		*/
		PLMESH_API void SetStartIndex(PLCore::uint32 nStartIndex = 0);

		/**
		*  @brief
		*    Returns the index size
		*
		*  @return
		*    Index size
		*/
		PLMESH_API PLCore::uint32 GetIndexSize() const;

		/**
		*  @brief
		*    Sets the index size
		*
		*  @param[in] nIndexSize
		*    Index size
		*/
		PLMESH_API void SetIndexSize(PLCore::uint32 nIndexSize = 0);

		/**
		*  @brief
		*    Returns the number of triangles the geometry consists of
		*
		*  @return
		*    Number of triangles (always 0 if less than 3 indices!)
		*/
		PLMESH_API PLCore::uint32 GetNumOfTriangles() const;

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this object
		*/
		PLMESH_API Geometry &operator =(const Geometry &cSource);

		/**
		*  @brief
		*    Compares two geometries
		*
		*  @param[in] cGeometry
		*    Geometry to compare with
		*
		*  @return
		*    'true' if both geometries are equal, else 'false'
		*/
		PLMESH_API bool operator ==(const Geometry &cGeometry) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Geometry description
		PLCore::String m_sName;		/**< Name of the geometry */
		PLCore::uint32 m_nFlags;	/**< Flags */
		bool		   m_bActive;	/**< Is this geometry active? */
		// Geometry data
		PLRenderer::Primitive::Enum	 m_nPrimitiveType;	/**< Primitive type */
		PLCore::uint32				 m_nMaterial;		/**< Material number */
		PLCore::uint32				 m_nStartIndex;		/**< Start index of index buffer */
		PLCore::uint32				 m_nIndexSize;		/**< Number of indices */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_GEOMETRY_H__
