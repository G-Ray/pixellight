/*********************************************************\
 *  File: SkinLoaderPL.cpp                               *
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
#include <PLCore/Xml/Xml.h>
#include <PLCore/Log/Log.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLMesh/MeshHandler.h>
#include "PLScene/Scene/SceneContext.h"
#include "PLScene/Scene/SceneNodes/SNMesh.h"
#include "PLScene/Scene/SceneNodes/Loader/SkinLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SkinLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool SkinLoaderPL::Load(SNMesh &cSNMesh, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get skin element
		const XmlElement *pSkinElement = cDocument.GetFirstChildElement("Skin");
		if (pSkinElement) {
			// Get the format version
			const int nVersion = pSkinElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cSNMesh, *pSkinElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cSNMesh, *pSkinElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'Skin' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool SkinLoaderPL::Save(SNMesh &cSNMesh, File &cFile)
{
	// Get the mesh handler
	const PLMesh::MeshHandler *pMeshHandler = cSNMesh.GetMeshHandler();
	if (!pMeshHandler)
		return false; // Error!

	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add skin
	XmlElement *pSkinElement = new XmlElement("Skin");
	pSkinElement->SetAttribute("Version", "1");

	// Loop through all materials
	for (uint32 nMaterial=0; nMaterial<pMeshHandler->GetNumOfMaterials(); nMaterial++) {
		// Get the material
		const PLRenderer::Material *pMaterial = pMeshHandler->GetMaterial(nMaterial);
		if (pMaterial) {
			// Create the material element
			XmlElement *pMaterialElement = new XmlElement("Material");

			// Set the ID attribute
			pMaterialElement->SetAttribute("ID", String::Format("%d", nMaterial));

			// Add value
			XmlText *pValue = new XmlText(pMaterial->GetName());
			pMaterialElement->LinkEndChild(*pValue);

			// Link material element
			pSkinElement->LinkEndChild(*pMaterialElement);
		}
	}

	// Link skin element
	cDocument.LinkEndChild(*pSkinElement);

	// Save settings
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
SkinLoaderPL::SkinLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
SkinLoaderPL::~SkinLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool SkinLoaderPL::LoadV1(SNMesh &cSNMesh, const XmlElement &cSkinElement) const
{
	// Get the mesh handler
	PLMesh::MeshHandler *pMeshHandler = cSNMesh.GetMeshHandler();
	if (pMeshHandler) {
		// Get the used mesh
		PLMesh::Mesh *pMesh = pMeshHandler->GetMesh();
		if (pMesh) {
			// Iterate through all materials
			const XmlElement *pMaterialElement = cSkinElement.GetFirstChildElement("Material");
			while (pMaterialElement) {
				String sName;
				int nID = -1;

				// Get material ID
				if (pMaterialElement->QueryIntAttribute("ID", &nID) != XmlElement::Success) {
					// Get material name
					sName = pMaterialElement->GetAttribute("Name");
					if (sName.GetLength()) {
						// Get the ID of this material
						for (uint32 i=0; i<pMesh->GetNumOfMaterials(); i++) {
							if (pMesh->GetMaterial(i)->GetName() == sName) {
								nID = i;
								break;
							}
						}
					}
				}

				// Check index
				if (nID > -1) {
					// Get value
					const XmlNode *pNode = pMaterialElement->GetFirstChild();
					if (pNode && pNode->GetType() == XmlNode::Text) {
						String sValue = pNode->GetValue();
						if (sValue.GetLength()) {
							// Set the new material
							PLRenderer::Material *pMaterial = cSNMesh.GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(sValue);
							if (!pMaterial || !pMeshHandler->SetMaterial(nID, pMaterial)) {
								if (sName.GetLength())
									PL_LOG(Error, "Can't replace material '" + sName + "' at index " + nID + " with material '" + sValue + "'!")
								else
									PL_LOG(Error, String("Can't replace material at index ") + nID + " with material '" + sValue + "'!")
							}
						}
					}
				} else {
					if (sName.GetLength())
						PL_LOG(Error, "Can't replace material '" + sName + "' at index " + nID + " because it does no exist")
					else
						PL_LOG(Error, "Can't replace material at invalid material index")
				}

				// Next element, please
				pMaterialElement = pMaterialElement->GetNextSiblingElement("Material");
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
