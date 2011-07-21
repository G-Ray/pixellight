/*********************************************************\
 *  File: Picking.h                                      *
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


#ifndef __PLENGINE_PICKING_H__
#define __PLENGINE_PICKING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	template <class ValueType> class Array;
}
namespace PLMath {
	class Vector3;
}
namespace PLScene {
	class SceneNode;
	class SceneQuery;
	class SceneContainer;
}
namespace PLEngine {
	class PickingResult;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class offering scene picking (some also call it 'trace line') functionality
*/
class Picking {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PL_API Picking();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~Picking();

		/**
		*  @brief
		*    Performs picking by using the given line start and end positions
		*
		*  @param[out] cPickingResult
		*    Receives the picking result if all went fine
		*  @param[in]  cContainer
		*    Scene container we're performing the picking in
		*  @param[in]  vLineStartPos
		*    Picking line start position within the given scene container
		*  @param[in]  vLineEndPos
		*    Picking line end position within the given scene container
		*
		*  @return
		*    'true' if anything has been picked, else 'false'
		*/
		PL_API bool PerformPicking(PickingResult &cPickingResult, PLScene::SceneContainer &cContainer, const PLMath::Vector3 &vLineStartPos, const PLMath::Vector3 &vLineEndPos);

		/**
		*  @brief
		*    Performs picking using the mesh of the given scene node by using the given line start and end positions
		*
		*  @param[out] cPickingResult
		*    Receives the picking result if all went fine
		*  @param[in]  cSceneNode
		*    Scene node we're performing the mesh picking in
		*  @param[in]  vLineStartPos
		*    Picking line start position within the given scene node
		*  @param[in]  vLineEndPos
		*    Picking line end position within the given scene node
		*  @param[in]  plstGeometries
		*    List of mesh geometry indices to use, if a null pointer all mesh geometries are used
		*
		*  @return
		*    'true' if anything has been picked, else 'false'
		*/
		PL_API bool PerformPicking(PickingResult &cPickingResult, PLScene::SceneNode &cSceneNode, const PLMath::Vector3 &vLineStartPos, const PLMath::Vector3 &vLineEndPos,
								   PLCore::Array<PLCore::uint32> *plstGeometries = nullptr);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a scene node was found
		*
		*  @param[in] cQuery
		*    Query found the scene node
		*  @param[in] cSceneNode
		*    Found scene node
		*/
		void OnSceneNode(PLScene::SceneQuery &cQuery, PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Perform mesh intersection
		*
		*  @param[in]  cSceneNode
		*    Scene node we're performing the mesh picking in
		*  @param[in]  vLineStartPos
		*    Picking line start position within the given scene node
		*  @param[in]  vLineEndPos
		*    Picking line end position within the given scene node
		*  @param[in]  plstGeometries
		*    List of mesh geometry indices to use, if a null pointer all mesh geometries are used
		*/
		void MeshIntersection(PLScene::SceneNode &cSceneNode, const PLMath::Vector3 &vLineStartPos, const PLMath::Vector3 &vLineEndPos,
							  PLCore::Array<PLCore::uint32> *plstGeometries = nullptr);


	//[-------------------------------------------------------]
	//[ Protected virtual Picking functions                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Function that is called when a picking candidate has been found ("picking filter function")
		*
		*  @param[in] cSceneNode
		*    Scene node candidate
		*
		*  @return
		*    'true' to take this candidate into account, else 'false' to throw it away (no futher processing of the scene node)
		*
		*  @note
		*    - The default implementation is empty
		*/
		PL_API virtual bool OnPickingCandidate(PLScene::SceneNode &cSceneNode);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<PLScene::SceneQuery &, PLScene::SceneNode &> EventHandlerSceneNode;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PickingResult *m_pPickingResult;	/**< Current picking result, always valid! (used inside 'OnSceneNode()') */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_PICKING_H__
