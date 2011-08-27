/*********************************************************\
 *  File: RendererContext.h                              *
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


#ifndef __PLRENDERER_RENDERERCONTEXT_H__
#define __PLRENDERER_RENDERERCONTEXT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include <PLCore/Core/AbstractContext.h>
#include "PLRenderer/Renderer/Renderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class String;
}
namespace PLRenderer {
	class TextureManager;
	class EffectManager;
	class MaterialManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Renderer context
*
*  @remarks
*    The renderer context stores and manages all data related to the renderer like e.g. the used PLRenderer implementation,
*    textures and so on.
*
*    Please note that the renderer context is by design window independent, it doesn't even know anything about one or multiple
*    windows (renderer targets). The renderer instance is connected with the renderer context. So, the renderer doesn't depend
*    on an OS window as well, but the internal renderer implementation may need an OS window. If you have an OS main window which is
*    valid as long as the renderer context instance exists, it's highly recommended to tell the renderer context of this main window
*    during creation, else the internal renderer may create it's own invisible dummy window. Most times, there's no problem with an
*    invisible dummy window holding a renderer implementation together. Sadly, there are e.g. some OpenGL ES 2.0 implementations which
*    just fail when using multiple windows (the internal invisible dummy window and the real visible one => two windows).
*
*    In a nutshell:
*      - If you've already got an OS main window which is valid as long as the renderer context instance exists, just tell the renderer
*        context about it
*      - If you don't have such an OS main window, don't create one just for the renderer context and pass in "NULL_HANDLE", the renderer
*        implementation must be able to deal with this situation on it's own
*
*  @note
*    - There should be only one renderer context instance per application
*/
class RendererContext : public PLCore::AbstractContext {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class RendererBackend;


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<>	EventUpdate;	/**< Update event */


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Creates a renderer context instance
		*
		*  @param[in] sBackend
		*    Name of the renderer backend to use (for example 'PLRendererOpenGL::Renderer')
		*  @param[in] nNativeWindowHandle
		*    Handle of a native OS window which is valid as long as the renderer instance exists, "NULL_HANDLE" if there's no such window (see class remarks for more information)
		*  @param[in] nMode
		*    Mode hint the renderer should run in, the renderer is not enforced to use this requested mode
		*  @param[in] nZBufferBits
		*     Z buffer bits (just a hint, for example 24)
		*  @param[in] nStencilBits
		*    Stencil buffer bits (just a hint, for example 8)
		*  @param[in] nMultisampleAntialiasingSamples
		*    Multisample antialiasing samples per pixel, <=1 means no antialiasing (just a hint)
		*  @param[in] sDefaultShaderLanguage
		*    The name of the default shader language of the renderer (for example "GLSL" or "Cg"), if the string
		*    is empty, the default is chosen by the renderer implementation, this information is just a hint
		*
		*  @return
		*    Creates a renderer context instance, a null pointer on error
		*/
		PLRENDERER_API static RendererContext *CreateInstance(const PLCore::String &sBackend, PLCore::handle nNativeWindowHandle, Renderer::EMode nMode = Renderer::ModeBoth, PLCore::uint32 nZBufferBits = 24, PLCore::uint32 nStencilBits = 8, PLCore::uint32 nMultisampleAntialiasingSamples = 0, const PLCore::String &sDefaultShaderLanguage = "");


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API ~RendererContext();

		/**
		*  @brief
		*    Returns the used renderer
		*
		*  @return
		*    Reference to the used renderer
		*/
		PLRENDERER_API Renderer &GetRenderer() const;

		/**
		*  @brief
		*    Returns the texture manager of this renderer context
		*
		*  @return
		*    Reference to the texture manager of this renderer context
		*/
		PLRENDERER_API TextureManager &GetTextureManager();

		/**
		*  @brief
		*    Returns the effect manager of this renderer context
		*
		*  @return
		*    Reference to the effect manager of this renderer context
		*/
		PLRENDERER_API EffectManager &GetEffectManager();

		/**
		*  @brief
		*    Returns the material manager of this renderer context
		*
		*  @return
		*    Reference to the material manager of this renderer context
		*/
		PLRENDERER_API MaterialManager &GetMaterialManager();

		/**
		*  @brief
		*    Updates the render context
		*
		*  @note
		*    - Should be called once per frame
		*    - Updates the effect manager
		*    - Emits the update event
		*    - Updates the renderer ("redraw")
		*    - Collects renderer context profiling information
		*/
		PLRENDERER_API void Update();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		PLRENDERER_API RendererContext(Renderer &cRenderer);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		RendererContext &operator =(const RendererContext &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Renderer		*m_pRenderer;			/**< Used renderer, always valid! */
		TextureManager  *m_pTextureManager;		/**< The texture manager of this renderer context, a null pointer if not yet initialized */
		EffectManager   *m_pEffectManager;		/**< The effect manager of this renderer context, a null pointer if not yet initialized */
		MaterialManager *m_pMaterialManager;	/**< The material manager of this renderer context, a null pointer if not yet initialized */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_RENDERERCONTEXT_H__
