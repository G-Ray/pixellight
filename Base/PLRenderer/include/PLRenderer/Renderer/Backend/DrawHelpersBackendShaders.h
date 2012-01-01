/*********************************************************\
 *  File: DrawHelpersBackendShaders.h                    *
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


#ifndef __PLRENDERERBACKEND_DRAWHELPERSBACKENDSHADERS_H__
#define __PLRENDERERBACKEND_DRAWHELPERSBACKENDSHADERS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Matrix4x4.h>
#include "PLRenderer/Renderer/ProgramGenerator.h"
#include "PLRenderer/Renderer/Backend/DrawHelpersBackend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Program;
class VertexShader;
class FragmentShader;
class ProgramUniform;
class ProgramAttribute;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Shaders based draw helpers interface implementation
*/
class DrawHelpersBackendShaders : public DrawHelpersBackend {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer instance to use
		*/
		PLRENDERER_API DrawHelpersBackendShaders(Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~DrawHelpersBackendShaders();


	//[-------------------------------------------------------]
	//[ Public virtual DrawHelpers functions                  ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual void Begin2DMode(float fX1 = 0.0f, float fY1 = 0.0f, float fX2 = 1.0f, float fY2 = 1.0f) override;
		PLRENDERER_API virtual void End2DMode() override;
		PLRENDERER_API virtual void DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize = PLMath::Vector2::Zero, const PLGraphics::Color4 &cColor = PLGraphics::Color4::White,
											  float fAlphaReference = 1.0f, const PLMath::Vector2 &vTextureCoordinate = PLMath::Vector2::Zero, const PLMath::Vector2 &vTextureCoordinateSize = PLMath::Vector2::One, const PLMath::Matrix4x4 &mTexture = PLMath::Matrix4x4::Identity) override;
		PLRENDERER_API virtual void DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const PLMath::Vector3 &vPos, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, const PLMath::Vector2 &vSize = PLMath::Vector2::Zero, const PLGraphics::Color4 &cColor = PLGraphics::Color4::White,
											  float fAlphaReference = 1.0f, const PLMath::Vector2 &vTextureCoordinate = PLMath::Vector2::Zero, const PLMath::Vector2 &vTextureCoordinateSize = PLMath::Vector2::One, const PLMath::Matrix4x4 &mTexture = PLMath::Matrix4x4::Identity) override;
		PLRENDERER_API virtual void DrawPoint(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vPosition, float fSize = 1.0f) override;
		PLRENDERER_API virtual void DrawPoint(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vPosition, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fSize = 1.0f) override;
		PLRENDERER_API virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vStartPosition, const PLMath::Vector2 &vEndPosition, float fWidth = 1.0f) override;
		PLRENDERER_API virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vStartPosition, const PLMath::Vector3 &vEndPosition, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 1.0f) override;
		PLRENDERER_API virtual void DrawTriangle(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 0.0f) override;
		PLRENDERER_API virtual void DrawQuad(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize, float fWidth = 0.0f) override;
		PLRENDERER_API virtual void DrawQuad(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Vector3 &vV4, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 0.0f) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Returns the program generator
		*
		*  @return
		*    The program generator, a null pointer on error
		*/
		ProgramGenerator *GetProgramGenerator();

		/**
		*  @brief
		*    Returns the generated program using the current flags
		*
		*  @return
		*    The generated program, a null pointer on error
		*
		*  @note
		*    - Makes this generated program also the the current renderer program
		*/
		ProgramGenerator::GeneratedProgram *GetAndSetGeneratedProgram();

		/**
		*  @brief
		*    Uses the image GPU program
		*
		*  @param[in] cVertexBuffer
		*    Vertex buffer to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] cTextureBuffer
		*    Texture buffer holding the image to draw
		*  @param[in] cSamplerStates
		*    Sampler states
		*  @param[in] fAlphaReference
		*    Alpha test reference value (0-1), all texels below the value will be discarded, if >= 1, no alpha test will be performed
		*  @param[in] mTexture
		*    Texture matrix
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool UseImageProgram(VertexBuffer &cVertexBuffer, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, float fAlphaReference, const PLMath::Matrix4x4 &mTexture);

		/**
		*  @brief
		*    Uses the primitive GPU program
		*
		*  @param[in] cVertexBuffer
		*    Vertex buffer to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] fPointSize
		*    Point size
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool UsePrimitiveProgram(VertexBuffer &cVertexBuffer, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fPointSize = 0.0f);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Vertex shader flags, flag names become to source code definitions
		*/
		enum EVertexShaderFlags {
			VS_TEXCOORD0 = 1<<0,	/**< Use texture coordinate 0 */
			VS_POINTSIZE = 1<<1		/**< Use point size */
		};

		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_DIFFUSEMAP		= 1<<0,	/**< Take diffuse map into account */
				FS_DIFFUSEMAP2D = 1<<1,	/**< If this is set, the diffuse map is a 2D texture, else it's a rectangle texture */
				FS_ALPHATEST	= 1<<2	/**< Use alpha test to discard fragments (FS_DIFFUSEMAP should be defined!) */
		};

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			ProgramAttribute *pVertexPosition;
			ProgramAttribute *pVertexTextureCoordinate;
			// Vertex shader uniforms
			ProgramUniform *pObjectSpaceToClipSpaceMatrix;
			ProgramUniform *pTextureMatrix;
			ProgramUniform *pPointSize;
			// Fragment shader uniforms
			ProgramUniform *pColor;
			ProgramUniform *pDiffuseMap;
			ProgramUniform *pAlphaReference;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Matrix4x4					 m_m2DModeProjBackup;	/**< Projection matrix backup (fixed functions backwards support) */
		PLMath::Matrix4x4					 m_m2DModeViewBackup;	/**< View matrix backup (fixed functions backwards support) */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERERBACKEND_DRAWHELPERSBACKENDSHADERS_H__
