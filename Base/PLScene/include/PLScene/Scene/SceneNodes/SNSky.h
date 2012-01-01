/*********************************************************\
 *  File: SNSky.h                                        *
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


#ifndef __PLSCENE_SCENENODE_SKY_H__
#define __PLSCENE_SCENENODE_SKY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Loadable.h>
#include "PLScene/Scene/SceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SNSky;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sky layer
*
*  @note
*  - Each sky mesh geometry has it's own material
*  - A sky plane has one geometry
*  - A sky cube has 6 geometries\n
*    Order: x-positive (0), x-negative (1), y-positive (2),
*    y-negative (3), z-positive (4), z-negative (5)
*
*/
class SkyLayer : public PLCore::Object, public PLCore::Element<SkyLayer> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class SNSky;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sky layer type
		*/
		enum EType {
			Unknown  = 0,	/**< Unknown sky type */
			Plane    = 1,	/**< Sky plane */
			Cube     = 2,	/**< Sky cube */
			Sphere   = 3,	/**< Sky sphere */
			Dome     = 4,	/**< Sky dome */
			Cylinder = 5	/**< Sky cylinder */
		};
		pl_enum(EType)
			pl_enum_value(Unknown,	"Unknown sky type")
			pl_enum_value(Plane,	"Sky plane")
			pl_enum_value(Cube,		"Sky cube")
			pl_enum_value(Sphere,	"Sky sphere")
			pl_enum_value(Dome,		"Sky dome")
			pl_enum_value(Cylinder,	"Sky cylinder")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SkyLayer, "PLScene", PLCore::Object, "Sky layer")
		// Attributes
		pl_attribute(Type,		pl_enum_type(EType),	Unknown,							ReadWrite,	GetSet,	"Sky layer type",															"")
		pl_attribute(Position,	PLMath::Vector3,		PLMath::Vector3(0.0f, 0.0f, 0.0f),	ReadWrite,	GetSet,	"Sky layer position",														"")
		pl_attribute(Rotation,	PLMath::Vector3,		PLMath::Vector3(0.0f, 0.0f, 0.0f),	ReadWrite,	GetSet,	"Sky layer rotation",														"")
		pl_attribute(Scale,		PLMath::Vector3,		PLMath::Vector3(1.0f, 1.0f, 1.0f),	ReadWrite,	GetSet,	"Sky layer scale",															"")
		pl_attribute(Name,		PLCore::String,			"",									ReadWrite,	GetSet,	"Optional sky layer name. If not defined, a name is chosen automatically.",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API EType GetType() const;
		PLS_API void SetType(EType nValue);
		PLS_API const PLMath::Vector3 &GetPosition() const;
		PLS_API void SetPosition(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetRotation() const;
		PLS_API void SetRotation(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetScale() const;
		PLS_API void SetScale(const PLMath::Vector3 &vValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SkyLayer();

		/**
		*  @brief
		*    Returns the sky this sky layer is in
		*
		*  @return
		*    The sky this sky layer is in, NEVER a null pointer
		*/
		PLS_API SNSky *GetSky() const;

		/**
		*  @brief
		*    Returns the sky layer mesh handler
		*
		*  @return
		*    The mesh of the sky layer handler, a null pointer on error
		*/
		PLS_API PLMesh::MeshHandler *GetMeshHandler() const;

		/**
		*  @brief
		*    Loads a material
		*
		*  @param[in] sFilename
		*    Material filename
		*  @param[in] nMaterial
		*    Material ID
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool LoadMaterial(const PLCore::String &sFilename, PLCore::uint32 nMaterial = 0);

		/**
		*  @brief
		*    Returns the current final transform matrix
		*
		*  @return
		*    The current final transform matrix
		*
		*  @note
		*    - If position, rotation or scale was changed, the current transform matrix
		*      is recalculated internally before it is returned
		*/
		PLS_API const PLMath::Matrix3x4 &GetTransformMatrix();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] pManager
		*    Element manager using this element, can be a null pointer
		*/
		SkyLayer(const PLCore::String &sName, PLCore::ElementManager<SkyLayer> *pManager);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EType				 m_nType;					/**< Sky layer type */
		PLMath::Vector3		 m_vPos;					/**< Sky layer position */
		PLMath::Vector3		 m_vRot;					/**< Sky layer rotation */
		PLMath::Vector3		 m_vScale;					/**< Sky layer scale */
		PLMesh::MeshHandler *m_pMeshHandler;			/**< Sky layer mesh (ALWAYS valid!) */
		PLMath::Matrix3x4	 m_mTrans;					/**< Current final transform matrix */
		bool				 m_bRecalculateWorldMatrix;	/**< World matrix recalculation required? */


};

/**
*  @brief
*    Sky scene node
*/
class SNSky : public SceneNode, public PLCore::ElementManager<SkyLayer>, public PLCore::Loadable {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNSky, "PLScene", PLScene::SceneNode, "Sky scene node")
		// Attributes
			// Overwritten SceneNode attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	NoCulling,											ReadWrite,	GetSet,		"Flags",																															"")
		pl_attribute(MaxDrawDistance,	float,					-10000.0f,											ReadWrite,	ModifyAttr,	"Maximum draw distance of the scene node to the camera, if 0 do always draw, if negative, do always draw this node before other",	"")
		pl_attribute(AABBMin,			PLMath::Vector3,		PLMath::Vector3(-10000.0f, -10000.0f, -10000.0f),	ReadWrite,	GetSet,		"Minimum position of the 'scene node space' axis aligned bounding box",																"")
		pl_attribute(AABBMax,			PLMath::Vector3,		PLMath::Vector3( 10000.0f,  10000.0f,  10000.0f),	ReadWrite,	GetSet,		"Maximum position of the 'scene node space' axis aligned bounding box",																"")
			// Overwritten PLCore::Loadable attributes
		pl_attribute(Filename,			PLCore::String,			"",													ReadWrite,	GetSet,		"Sky filename",																														"Type='Sky'")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API void SetFilename(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNSky();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNSky();


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawPre(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Private virtual SceneNode functions                   ]
	//[-------------------------------------------------------]
	private:
		virtual void InitFunction() override;
		virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual SkyLayer *CreateElement(const PLCore::String &sName) override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual bool Unload() override;
		PLS_API virtual PLCore::String GetLoadableTypeName() const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::Loadable functions            ]
	//[-------------------------------------------------------]
	private:
		virtual bool CallLoadable(PLCore::File &cFile, PLCore::Loader &cLoader, const PLCore::String &sMethod, const PLCore::String &sParams) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_SKY_H__
