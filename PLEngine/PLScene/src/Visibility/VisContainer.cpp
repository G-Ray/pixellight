/*********************************************************\
 *  File: VisContainer.cpp                               *
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
#include "PLScene/Scene/SNCellPortal.h"
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneNodeHandler.h"
#include "PLScene/Scene/SceneQueryHandler.h"
#include "PLScene/Visibility/SQCull.h"
#include "PLScene/Visibility/VisPortal.h"
#include "PLScene/Visibility/VisManager.h"
#include "PLScene/Visibility/VisContainer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
namespace PLScene {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the cull scene query of this visibility container
*/
const SQCull *VisContainer::GetCullQuery() const
{
	return static_cast<SQCull*>(m_pQueryHandler->GetElement());
}

/**
*  @brief
*    Returns the list of visibililty nodes
*/
const Pool<VisNode*> &VisContainer::GetVisNodes() const
{
	return m_lstNodes;
}

/**
*  @brief
*    Returns projection information
*/
const VisContainer::Projection &VisContainer::GetProjection() const
{
	return m_sProjection;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VisContainer::VisContainer(VisNode *pParent) : VisNode(pParent),
	EventHandlerDestroy(&VisContainer::OnDestroy, this),
	m_pQueryHandler(new SceneQueryHandler())
{
	m_sProjection.fZNear = m_sProjection.fZFar = 0.0f;
}

/**
*  @brief
*    Destructor
*/
VisContainer::~VisContainer()
{
	// Free all nodes
	FreeNodes();

	// Destroy the query
	SceneQuery *pSceneQuery = m_pQueryHandler->GetElement();
	if (pSceneQuery)
		pSceneQuery->GetSceneContainer().DestroyQuery(*pSceneQuery);
	delete m_pQueryHandler;

	{ // Destroy all containers
		Iterator<VisContainer*> cIterator = m_mapContainers.GetIterator();
		while (cIterator.HasNext())
			delete cIterator.Next();
	}

	{ // Destroy all portals
		Iterator<VisPortal*> cIterator = m_mapPortals.GetIterator();
		while (cIterator.HasNext())
			delete cIterator.Next();
	}
}

/**
*  @brief
*    Adds a scene node to this visibility container
*/
VisNode *VisContainer::AddSceneNode(SceneNode &cSceneNode, float fSquaredDistanceToCamera)
{
	// Is this a scene container?
	VisNode *pNode;
	if (cSceneNode.IsContainer()) {
		VisContainer *pContainer = m_mapContainers.Get(cSceneNode.GetName());
		if (pContainer)
			pNode = static_cast<VisNode*>(pContainer);
		else {
			VisContainer *pNewContainer = new VisContainer(this);
			m_mapContainers.Add(cSceneNode.GetName(), pNewContainer);

			// Connect event handler so we get informed if this visibility container is loosing it's scene node
			cSceneNode.SignalDestroy.Connect(&pNewContainer->EventHandlerDestroy);

			SQCull *pCullQuery = static_cast<SQCull*>(static_cast<SceneContainer&>(cSceneNode).CreateQuery("PLScene::SQCull"));
			if (pCullQuery) {
				pNewContainer->m_pQueryHandler->SetElement(pCullQuery);
				pCullQuery->m_pVisRootContainer = GetCullQuery() ? GetCullQuery()->m_pVisRootContainer : pNewContainer;
				pCullQuery->m_pVisContainer = pNewContainer;
			}
			pNode = pNewContainer;
		}

	// Is this a cell-portal?
	} else if (cSceneNode.IsPortal() && cSceneNode.IsInstanceOf("PLScene::SNCellPortal")) {
		VisPortal *pPortal = m_mapPortals.Get(cSceneNode.GetName());
		if (pPortal) {
			if (!pPortal->m_pTargetCell) {
				// Get the target cell
				SceneNode *pCell = reinterpret_cast<SceneNode*>(static_cast<SNCellPortal&>(cSceneNode).GetTargetCellInstance());
				if (pCell) {
					// Is this REALLY a cell or is someone fooling us?
					if (pCell->IsCell()) {
						pPortal->m_pTargetCell = new VisContainer(pPortal);
						SQCull *pCullQuery = static_cast<SQCull*>(static_cast<SceneContainer*>(pCell)->CreateQuery("PLScene::SQCull"));
						if (pCullQuery) {
							pPortal->m_pTargetCell->m_pQueryHandler->SetElement(pCullQuery);
							pCullQuery->m_pVisRootContainer = GetCullQuery() ? GetCullQuery()->m_pVisRootContainer : pPortal->m_pTargetCell;
							pCullQuery->m_pVisContainer     = pPortal->m_pTargetCell;
						}
						pPortal->m_pTargetCell->m_pSceneNodeHandler->SetElement(pCell);
						pPortal->m_pTargetCell->m_fSquaredDistanceToCamera = fSquaredDistanceToCamera;
					} // else: // HA, good try, but not good enough! ;-)
				} // else: Target cell not found, THIS should NEVER happen!
			}
			if (pPortal->m_pTargetCell)
				pPortal->m_pTargetCell->m_fSquaredDistanceToCamera = fSquaredDistanceToCamera;
		} else {
			pPortal = new VisPortal();
			m_mapPortals.Add(cSceneNode.GetName(), pPortal);

			// Connect event handler so we get informed if this visibility portal is loosing it's scene node
			cSceneNode.SignalDestroy.Connect(&pPortal->EventHandlerDestroy);

			// Get the target cell
			SceneNode *pCell = reinterpret_cast<SceneNode*>(static_cast<SNCellPortal&>(cSceneNode).GetTargetCellInstance());
			if (pCell) {
				// Is this REALLY a cell or is someone fooling us?
				if (pCell->IsCell()) {
					pPortal->m_pTargetCell = new VisContainer(pPortal);
					SQCull *pCullQuery = static_cast<SQCull*>(static_cast<SceneContainer*>(pCell)->CreateQuery("PLScene::SQCull"));
					if (pCullQuery) {
						pPortal->m_pTargetCell->m_pQueryHandler->SetElement(pCullQuery);
						pCullQuery->m_pVisRootContainer = GetCullQuery() ? GetCullQuery()->m_pVisRootContainer : pPortal->m_pTargetCell;
						pCullQuery->m_pVisContainer     = pPortal->m_pTargetCell;
					}
					pPortal->m_pTargetCell->m_pSceneNodeHandler->SetElement(pCell);
					pPortal->m_pTargetCell->m_fSquaredDistanceToCamera = fSquaredDistanceToCamera;
				} // else: // HA, good try, but not good enough! ;-)
			} // else: Target cell not found, THIS should NEVER happen!
		}
		pNode = pPortal;

	// Is just a simple scene node :)
	} else {
		// Get the visibility manager
		VisManager *pVisManager = GetVisManager();
		if (pVisManager) {
			pNode = &pVisManager->GetFreeNode();
			pNode->m_pParent = this;
		} else {
			// Error!
			return nullptr;
		}
	}

	// Setup node
	pNode->m_pSceneNodeHandler->SetElement(&cSceneNode);
	pNode->m_fSquaredDistanceToCamera = fSquaredDistanceToCamera;
	m_lstNodes.Add(pNode);

	// Return this node
	return pNode;
}

/**
*  @brief
*    Frees all visibility nodes
*/
void VisContainer::FreeNodes()
{
	while (!m_lstNodes.IsEmpty()) {
		VisNode &cNode = *m_lstNodes[0];
		if (!cNode.IsContainer() && !cNode.IsPortal()) {
			// Get the visibility manager
			VisManager *pVisManager = GetVisManager();
			if (pVisManager)
				pVisManager->FreeNode(cNode);
		}
		m_lstNodes.RemoveAtIndex(0);
	}
}

/**
*  @brief
*    Called when the scene node assigned with this visibililty container was destroyed
*/
void VisContainer::OnDestroy()
{
	SceneNode *pSceneNode = GetSceneNode();
	if (pSceneNode && pSceneNode->IsContainer()) {
		// Get the parent visibility container
		const VisNode *pParent = GetParent();
		if (pParent && pParent->IsContainer()) {
			VisContainer *pParentVisContainer = const_cast<VisContainer*>(static_cast<const VisContainer*>(pParent));

			// Unregister within the parent container
			VisContainer *pContainer = pParentVisContainer->m_mapContainers.Get(pSceneNode->GetName());
			if (pContainer) {
				pParentVisContainer->m_lstNodes.Remove(pContainer);
				delete pContainer;
				pParentVisContainer->m_mapContainers.Remove(pSceneNode->GetName());
			}
		}
	}
}

/**
*  @brief
*    Returns the visibility manager
*/
VisManager *VisContainer::GetVisManager() const
{
	// Get the cull query
	const SQCull *pSQCull = GetCullQuery();
	if (pSQCull) {
		// Get the scene context
		SceneContext *pSceneContext = pSQCull->GetSceneContext();
		if (pSceneContext)
			return &pSceneContext->GetVisManager();
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public virtual VisNode functions                      ]
//[-------------------------------------------------------]
bool VisContainer::IsContainer() const
{
	return true;
}

bool VisContainer::IsCell() const
{
	return (m_pSceneNodeHandler->GetElement() && m_pSceneNodeHandler->GetElement()->IsCell());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
