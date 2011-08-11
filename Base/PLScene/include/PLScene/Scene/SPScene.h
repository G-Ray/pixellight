/*********************************************************\
 *  File: SPScene.h                                      *
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


#ifndef __PLSCENE_SCENEPAINTER_H__
#define __PLSCENE_SCENEPAINTER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/SurfacePainter.h>
#include "PLScene/PLScene.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
}
namespace PLScene {
	class SQCull;
	class SNCamera;
	class SceneRenderer;
	class SceneContainer;
	class SceneNodeHandler;
	class SceneQueryHandler;
	class SceneRendererHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene graph surface painter class
*
*  @remarks
*    The scene surface painter is responsible for rendering the scene by using a given camera.
*
*    'The scene' is normally subdivided into a 'root container' and a 'scene container' - both
*    normally instances of SceneContainer. The 'root container' is responsible for clearing
*    the frame buffer, drawing the 'scene container' and finally adding some additional
*    things like GUI, engine information, console etc. The 'scene container' itself is
*    the concrete scene - this distinction between 'root' and 'scene' is quite comfortable
*    when it comes to things like rendering 'reflection/refraction' where only the 'scene'
*    should be rendererd without clearing the frame buffer or adding additional information
*    like FPS display. Here's an example how such a root container may look like:
*      SceneContainer  (Root container)
*        SceneContainer  (Scene container with the 'real scene')
*          SNCamera
*          SNSky
*          SNMesh
*          ...
*        SNGui
*        SNEngineInformation
*        SNConsole
*    This make things extreme flexible. :)
*/
class SPScene : public PLRenderer::SurfacePainter {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SPScene, "PLScene", PLRenderer::SurfacePainter, "Scene graph surface painter class")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLRenderer::Renderer&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		PLS_API SPScene(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SPScene();

		/**
		*  @brief
		*    Returns the used scene context
		*
		*  @return
		*    The used scene context, can be a null pointer
		*/
		PLS_API SceneContext *GetSceneContext() const;

		/**
		*  @brief
		*    Returns the scene root container
		*
		*  @return
		*    Scene root container, a null pointer if there's no one
		*/
		PLS_API SceneContainer *GetRootContainer() const;

		/**
		*  @brief
		*    Sets the scene root container
		*
		*  @param[in] pContainer
		*    Scene root container, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool SetRootContainer(SceneContainer *pContainer);

		/**
		*  @brief
		*    Returns the scene container
		*
		*  @return
		*    Scene container, a null pointer if there's no one
		*/
		PLS_API SceneContainer *GetSceneContainer() const;

		/**
		*  @brief
		*    Sets the scene container
		*
		*  @param[in] pContainer
		*    Scene container, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLS_API bool SetSceneContainer(SceneContainer *pContainer);

		/**
		*  @brief
		*    Returns the default camera scene node
		*
		*  @return
		*    Default camera scene node, a null pointer if there's no one
		*/
		PLS_API SNCamera *GetCamera() const;

		/**
		*  @brief
		*    Sets the default camera scene
		*
		*  @param[in] pCamera
		*    New default camera scene node, can be a null pointer
		*/
		PLS_API void SetCamera(SNCamera *pCamera);

		/**
		*  @brief
		*    Returns the cull query
		*
		*  @return
		*    The cull query, a null pointer on error
		*/
		PLS_API SQCull *GetCullQuery() const;

		/**
		*  @brief
		*    Returns the default scene renderer
		*
		*  @return
		*    The default scene renderer, can be a null pointer
		*
		*  @note
		*    - The default scene renderer is used if the used camera has
		*      no scene renderer set or there's no camera at all
		*/
		PLS_API SceneRenderer *GetDefaultSceneRenderer() const;

		/**
		*  @brief
		*    Sets the default scene renderer
		*
		*  @param[in] sSceneRenderer
		*    Filename of the default scene renderer
		*
		*  @see
		*    - GetDefaultSceneRenderer()
		*/
		PLS_API void SetDefaultSceneRenderer(const PLCore::String &sSceneRenderer = "Forward.sr");


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Pre draw all scene nodes recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cContainer
		*    Parent scene container
		*/
		void DrawPre(PLRenderer::Renderer &cRenderer, SceneContainer &cContainer);

		/**
		*  @brief
		*    Solid draw all scene nodes recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cContainer
		*    Parent scene container
		*/
		void DrawSolid(PLRenderer::Renderer &cRenderer, SceneContainer &cContainer);

		/**
		*  @brief
		*    Transparent draw all scene nodes recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cContainer
		*    Parent scene container
		*/
		void DrawTransparent(PLRenderer::Renderer &cRenderer, SceneContainer &cContainer);

		/**
		*  @brief
		*    Debug draw all scene nodes recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cContainer
		*    Parent scene container
		*/
		void DrawDebug(PLRenderer::Renderer &cRenderer, SceneContainer &cContainer);

		/**
		*  @brief
		*    Post draw all scene nodes recursive
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cContainer
		*    Parent scene container
		*/
		void DrawPost(PLRenderer::Renderer &cRenderer, SceneContainer &cContainer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SceneNodeHandler     *m_pRootContainerHandler;			/**< Scene root container handler (always valid!) */
		SceneNodeHandler     *m_pSceneContainerHandler;			/**< Scene container handler (always valid!) */
		SceneQueryHandler    *m_pCullQuery;						/**< Cull query (always valid!) */
		SceneNodeHandler     *m_pCameraNodeHandler;				/**< Camera scene node handler (always valid!) */
		PLCore::String		  m_sDefaultSceneRenderer;			/**< Default scene renderer */
		SceneRendererHandler *m_pDefaultSceneRendererHandler;	/**< Default scene renderer handler, always valid! */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::SurfacePainter functions   ]
	//[-------------------------------------------------------]
	public:
		virtual void OnPaint(PLRenderer::Surface &cSurface) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENEPAINTER_H__
