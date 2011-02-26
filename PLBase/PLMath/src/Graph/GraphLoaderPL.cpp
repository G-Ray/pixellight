/*********************************************************\
 *  File: GraphLoaderPL.cpp                              *
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
#include <PLGeneral/Xml/Xml.h>
#include <PLGeneral/String/ParseTools.h>
#include <PLGeneral/Log/Log.h>
#include "PLMath/Graph/Graph.h"
#include "PLMath/Graph/GraphNode.h"
#include "PLMath/Graph/GraphLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
namespace PLMath {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(GraphLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool GraphLoaderPL::Load(Graph &cGraph, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get graph element
		const XmlElement *pGraphElement = cDocument.GetFirstChildElement("Graph");
		if (pGraphElement) {
			// Get the format version
			int nVersion = pGraphElement->GetAttribute("Version").GetInt();

			// Unkown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cGraph, *pGraphElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cGraph, *pGraphElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else PL_LOG(Error, "Can't find 'Graph' element")
	} else PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())

	// Error!
	return false;
}

bool GraphLoaderPL::Save(const Graph &cGraph, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add graph
	XmlElement *pGraphElement = new XmlElement("Graph");
	pGraphElement->SetAttribute("Version", "1");

	// Loop through all nodes
	for (uint32 nNode=0; nNode<cGraph.GetNumOfElements(); nNode++) {
		// Get the node
		const GraphNode *pNode = cGraph.Get(nNode);
		if (pNode) {
			// Add node
			XmlElement *pNodeElement = new XmlElement("Node");

			// Set node attributes
			pNodeElement->SetAttribute("Name",     pNode->GetName());
			pNodeElement->SetAttribute("Position", pNode->GetPos().ToString());

			// Loop through all node neighbours
			for (uint32 nNeighbour=0; nNeighbour<pNode->GetNumOfNeighbours(); nNeighbour++) {
				// Get the neighbour node
				const GraphNode *pNeighbour = pNode->GetNeighbour(nNeighbour);
				if (pNeighbour) {
					// Add this neighbour
					XmlElement *pNeighbourElement = new XmlElement("Neighbour");
					pNeighbourElement->SetAttribute("Name", pNeighbour->GetName());
					float fDistance = pNode->GetNeighbourDistance(nNeighbour);
					if (fDistance >= 0.0f)
						pNeighbourElement->SetAttribute("Distance", String(fDistance));
					pNodeElement->LinkEndChild(*pNeighbourElement);
				}
			}

			// Link node element
			pGraphElement->LinkEndChild(*pNodeElement);
		}
	}

	// Link graph element
	cDocument.LinkEndChild(*pGraphElement);

	// Save graph
	cDocument.Save(cFile);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
GraphLoaderPL::GraphLoaderPL() :
	MethodLoad(this),
	MethodSave(this)
{
}

/**
*  @brief
*    Destructor
*/
GraphLoaderPL::~GraphLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool GraphLoaderPL::LoadV1(Graph &cGraph, const XmlElement &cGraphElement) const
{
	// Backup data for neighbour connection post processing
	List<GraphNode*>		lstGraphNodes;
	List<const XmlElement*> lstNodeElements;

	// Iterate through all nodes and add them to the graph
	const XmlElement *pXmlElement = cGraphElement.GetFirstChildElement("Node");
	while (pXmlElement) {
		// Get the name of the node
		String sName = pXmlElement->GetAttribute("Name");

		// Is there already a node with this name?
		if (cGraph.Get(sName)) {
			// Write a warning into the log
			PL_LOG(Warning, "Name '" + sName + "' of graph node at row " + pXmlElement->GetRow() + ", column " + pXmlElement->GetColumn() + " is already used! (this node is ignored)")

		// Create the node
		} else {
			GraphNode *pNode = cGraph.Create(sName);
			if (pNode) {
				lstGraphNodes.Add(pNode);
				lstNodeElements.Add(pXmlElement);

				// Set the position of the node
				String sPos = pXmlElement->GetAttribute("Position");
				if (sPos.GetLength()) {
					float fPos[3];
					ParseTools::ParseFloatArray(sPos, fPos, 3);
					pNode->SetPos(fPos[Vector3::X], fPos[Vector3::Y], fPos[Vector3::Z]);
				}
			}
		}

		// Next element, please
		pXmlElement = pXmlElement->GetNextSiblingElement("Node");
	}

	// Iterate through all nodes again setup the neighbour connection
	Iterator<GraphNode*>		cGraphNodeIterator   = lstGraphNodes.GetIterator();
	Iterator<const XmlElement*>	cNodeElementIterator = lstNodeElements.GetIterator();
	while (cGraphNodeIterator.HasNext() && cNodeElementIterator.HasNext()) {
		// Get the pointers
		GraphNode *pNode	   = cGraphNodeIterator.Next();
				   pXmlElement = cNodeElementIterator.Next();

		// Iterate through all neighbours
		const XmlElement *pNeighbourElement = pXmlElement->GetFirstChildElement("Neighbour");
		while (pNeighbourElement) {
			// Get the name of the neighbour
			String sNeighbourName = pNeighbourElement->GetAttribute("Name");
			if (sNeighbourName.GetLength()) {
				// Get the neighbour node
				GraphNode *pNeighbour = cGraph.Get(sNeighbourName);
				if (pNeighbour && pNeighbour != pNode) {
					// Try to get neighbour distance
					float fDistance = -1.0f; // By default, calculate the distance automatically
					pNeighbourElement->QueryFloatAttribute("Distance", &fDistance);

					// And finally add this neighbour
					pNode->AddNeighbour(*pNeighbour, fDistance);

				// Write a warning into the log
				} else PL_LOG(Warning, "Neighbour node '" + sNeighbourName + "' of graph node '" + pNode->GetName() + "' at row " + pXmlElement->GetRow() + ", column " + pXmlElement->GetColumn() + " is invalid! (this neighbour node is ignored)")
			}

			// Next element, please
			pNeighbourElement = pNeighbourElement->GetNextSiblingElement("Neighbour");
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
