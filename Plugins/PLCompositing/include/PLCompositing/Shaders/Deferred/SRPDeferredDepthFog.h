/*********************************************************\
 *  File: SRPDeferredDepthFog.h                          *
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


#ifndef __PLCOMPOSITING_DEFERRED_DEPTHFOG_H__
#define __PLCOMPOSITING_DEFERRED_DEPTHFOG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferred.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class ProgramUniform;
	class ProgramAttribute;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene renderer pass for deferred rendering classic depth fog
*
*  @note
*    - The technique described in http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter09.html is used
*/
class SRPDeferredDepthFog : public SRPDeferred {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Fog mode
		*/
		enum EFogMode {
			LinearMode       = 0,	/**< Fog effect intensifies linearly between the start and end points (f=(end-d)/(end-start)) */
			ExponentialMode  = 1,	/**< Fog effect intensifies exponentially (f=1/((e^(d*density)))) */
			Exponential2Mode = 2,	/**< Fog effect intensifies exponentially with the square of the distance (f=1/((e^((d*density)^2)))) */
			NumberOfModes    = 3	/**< Number of fog modes */
		};
		pl_enum(EFogMode)
			pl_enum_value(LinearMode,		"Fog effect intensifies linearly between the start and end points (f=(end-d)/(end-start))")
			pl_enum_value(ExponentialMode,	"Fog effect intensifies exponentially (f=1/((e^(d*density))))")
			pl_enum_value(Exponential2Mode,	"Fog effect intensifies exponentially with the square of the distance (f=1/((e^((d*density)^2))))")
		pl_enum_end

		/**
		*  @brief
		*    Scene renderer pass flags (SceneRendererPass flags extension)
		*/
		enum EFlags {
			DisableBlending = 1<<1	/**< Disable blending (for debugging) */
		};
		pl_enum(EFlags)
			pl_enum_base(SRPDeferred::EFlags)
			pl_enum_value(DisableBlending, "Disable blending (for debugging)")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDeferredDepthFog, "PLCompositing", PLCompositing::SRPDeferred, "Scene renderer pass for deferred rendering classic depth fog")
		// Attributes
		pl_attribute(ShaderLanguage,	PLCore::String,			"",											ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
		pl_attribute(FogColor,			PLGraphics::Color4,		PLGraphics::Color4(0.5f, 0.5f, 0.5f, 1.0f),	ReadWrite,	DirectValue,	"Fog color",																															"")
		pl_attribute(FogMode,			pl_enum_type(EFogMode),	ExponentialMode,							ReadWrite,	DirectValue,	"Fog mode",																																"")
		pl_attribute(FogStart,			float,					0.0f,										ReadWrite,	DirectValue,	"Fog start, only for LinearMode",																										"")
		pl_attribute(FogEnd,			float,					1.0f,										ReadWrite,	DirectValue,	"Fog end, only for LinearMode",																											"")
		pl_attribute(FogDensity,		float,					1.0f,										ReadWrite,	DirectValue,	"Fog density, only for ExponentialMode and Exponential2Mode",																			"")
			// Overwritten PLScene::SceneRendererPass attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,											ReadWrite,	GetSet,			"Flags",																																"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCOM_API SRPDeferredDepthFog();

		/**
		*  @brief
		*    Destructur
		*/
		PLCOM_API virtual ~SRPDeferredDepthFog();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_LINEAR_MODE		 = 1<<0,	/**< Fog effect intensifies linearly between the start and end points (f=(end-d)/(end-start)) */
			FS_EXPONENTIAL_MODE	 = 1<<1,	/**< Fog effect intensifies exponentially (f=1/((e^(d*density)))) */
			FS_EXPONENTIAL2_MODE = 1<<2		/**< Fog effect intensifies exponentially with the square of the distance (f=1/((e^((d*density)^2)))) */
		};

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			// Vertex shader uniforms
			PLRenderer::ProgramUniform *pTextureSize;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pFarPlane;
			PLRenderer::ProgramUniform *pFogColor;
			PLRenderer::ProgramUniform *pFogEnd;
			PLRenderer::ProgramUniform *pFogRange;
			PLRenderer::ProgramUniform *pFogDensity;
			PLRenderer::ProgramUniform *pNormalDepthMap;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_DEFERRED_DEPTHFOG_H__
