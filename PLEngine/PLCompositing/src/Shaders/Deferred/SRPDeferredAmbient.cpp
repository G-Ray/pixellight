/*********************************************************\
 *  File: SRPDeferredAmbient.cpp                         *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include <PLRenderer/Renderer/ProgramAttribute.h>
#include <PLRenderer/Renderer/TextureBufferRectangle.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLScene/Compositing/FullscreenQuad.h>
#include "PLCompositing/Shaders/Deferred/SRPDeferredGBuffer.h"
#include "PLCompositing/Shaders/Deferred/SRPDeferredAmbient.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLRenderer;
using namespace PLScene;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SRPDeferredAmbient)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRPDeferredAmbient::SRPDeferredAmbient() :
	ShaderLanguage(this),
	AmbientColor(this),
	Flags(this),
	m_pProgramGenerator(NULL)
{
}

/**
*  @brief
*    Destructor
*/
SRPDeferredAmbient::~SRPDeferredAmbient()
{
	// Destroy the program generator
	if (m_pProgramGenerator)
		delete m_pProgramGenerator;
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPDeferredAmbient::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Get the instance of the "PLCompositing::SRPDeferredGBuffer" scene renderer pass
	SRPDeferredGBuffer *pSRPDeferredGBuffer = GetGBuffer();
	if (pSRPDeferredGBuffer) {
		// Get the fullscreen quad instance
		FullscreenQuad *pFullscreenQuad = pSRPDeferredGBuffer->GetFullscreenQuad();
		if (pFullscreenQuad) {
			// Get the vertex buffer of the fullscreen quad
			VertexBuffer *pVertexBuffer = pFullscreenQuad->GetVertexBuffer();
			if (pVertexBuffer) {
				// Get the texture buffer to use
				TextureBufferRectangle *pTextureBuffer = pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(0);
				if (pTextureBuffer) {
					// Get the shader language to use
					String sShaderLanguage = ShaderLanguage;
					if (!sShaderLanguage.GetLength())
						sShaderLanguage = cRenderer.GetDefaultShaderLanguage();

					// Create the program generator if there's currently no instance of it
					if (!m_pProgramGenerator || m_pProgramGenerator->GetShaderLanguage() != sShaderLanguage) {
						// If there's an previous instance of the program generator, destroy it first
						if (m_pProgramGenerator) {
							delete m_pProgramGenerator;
							m_pProgramGenerator = NULL;
						}

						// Choose the shader source codes depending on the requested shader language
						String sDeferredAmbient_VS;
						String sDeferredAmbient_FS;
						if (sShaderLanguage == "GLSL") {
							#include "SRPDeferredAmbient_GLSL.h"
							sDeferredAmbient_VS = sDeferredAmbient_GLSL_VS;
							sDeferredAmbient_FS = sDeferredAmbient_GLSL_FS;
						} else if (sShaderLanguage == "Cg") {
							#include "SRPDeferredAmbient_Cg.h"
							sDeferredAmbient_VS = sDeferredAmbient_Cg_VS;
							sDeferredAmbient_FS = sDeferredAmbient_Cg_FS;
						}

						// Create the program generator
						if (sDeferredAmbient_VS.GetLength() && sDeferredAmbient_FS.GetLength())
							m_pProgramGenerator = new ProgramGenerator(cRenderer, sShaderLanguage, sDeferredAmbient_VS, "arbvp1", sDeferredAmbient_FS, "arbfp1", true);
					}

					// If there's no program generator, we don't need to continue
					if (m_pProgramGenerator) {
						// Reset all render states to default
						cRenderer.GetRendererContext().GetEffectManager().Use();

						// Use stencil buffer?
						if (!(GetFlags() & NoStencil)) {
							// Enable stencil test - ignore pixels tagged with 1 within the stencil buffer
							cRenderer.SetRenderState(RenderState::StencilEnable, true);
							cRenderer.SetRenderState(RenderState::StencilRef,    1);
							cRenderer.SetRenderState(RenderState::StencilFunc,   Compare::NotEqual);
						}

						// Reset the program flags
						m_cProgramFlags.Reset();

						// Albedo used?
						if (!(GetFlags() & NoAlbedo))
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALBEDO)

						// Ambient occlusion used?
						if (!(GetFlags() & NoAmbientOcclusion))
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_AMBIENTOCCLUSION)

						// Self illumination used?
						if (pSRPDeferredGBuffer->IsColorTarget3Used() && !(GetFlags() & NoSelfIllumination))
							PL_ADD_FS_FLAG(m_cProgramFlags, FS_SELFILLUMINATION)

						// Get a program instance from the program generator using the given program flags
						ProgramGenerator::GeneratedProgram *pGeneratedProgram = m_pProgramGenerator->GetProgram(m_cProgramFlags);

						// Make our program to the current one
						if (pGeneratedProgram && cRenderer.SetProgram(pGeneratedProgram->pProgram)) {
							// Set pointers to uniforms & attributes of a generated program if they are not set yet
							GeneratedProgramUserData *pGeneratedProgramUserData = (GeneratedProgramUserData*)pGeneratedProgram->pUserData;
							if (!pGeneratedProgramUserData) {
								pGeneratedProgram->pUserData = pGeneratedProgramUserData = new GeneratedProgramUserData;
								Program *pProgram = pGeneratedProgram->pProgram;
								// Vertex shader attributes
								static const String sVertexPosition = "VertexPosition";
								pGeneratedProgramUserData->pVertexPosition		= pProgram->GetAttribute(sVertexPosition);
								// Vertex shader uniforms
								static const String sTextureSize = "TextureSize";
								pGeneratedProgramUserData->pTextureSize			= pProgram->GetUniform(sTextureSize);
								// Fragment shader uniforms
								static const String sAmbientColor = "AmbientColor";
								pGeneratedProgramUserData->pAmbientColor		= pProgram->GetUniform(sAmbientColor);
								static const String sAlbedoMap = "AlbedoMap";
								pGeneratedProgramUserData->pAlbedoMap			= pProgram->GetUniform(sAlbedoMap);
								static const String sSelfIlluminationMap = "SelfIlluminationMap";
								pGeneratedProgramUserData->pSelfIlluminationMap	= pProgram->GetUniform(sSelfIlluminationMap);
							}

							// Set program vertex attributes, this creates a connection between "Vertex Buffer Attribute" and "Vertex Shader Attribute"
							if (pGeneratedProgramUserData->pVertexPosition)
								pGeneratedProgramUserData->pVertexPosition->Set(pVertexBuffer, PLRenderer::VertexBuffer::Position);

							// Set texture size
							if (pGeneratedProgramUserData->pTextureSize)
								pGeneratedProgramUserData->pTextureSize->Set(pTextureBuffer->GetSize());

							// Ambient color
							if (pGeneratedProgramUserData->pAmbientColor)
								pGeneratedProgramUserData->pAmbientColor->Set(AmbientColor.Get());

							// Albedo map
							if (pGeneratedProgramUserData->pAlbedoMap) {
								const int nTextureUnit = pGeneratedProgramUserData->pAlbedoMap->Set(pTextureBuffer);
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}

							// Self illumination map
							if (pGeneratedProgramUserData->pSelfIlluminationMap) {
								const int nTextureUnit = pGeneratedProgramUserData->pSelfIlluminationMap->Set(pSRPDeferredGBuffer->GetRenderTargetTextureBuffer(3));
								if (nTextureUnit >= 0) {
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV,  TextureAddressing::Clamp);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
									cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);
								}
							}

							// Setup renderer
							const uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
							cRenderer.SetRenderState(RenderState::ScissorTestEnable, false);
							cRenderer.SetRenderState(RenderState::FixedFillMode,	 Fill::Solid);
							cRenderer.SetRenderState(RenderState::CullMode,			 Cull::None);
							cRenderer.SetRenderState(RenderState::ZEnable,			 false);
							cRenderer.SetRenderState(RenderState::ZWriteEnable,		 false);

							// Draw the fullscreen quad
							cRenderer.DrawPrimitives(Primitive::TriangleStrip, 0, 4);

							// Restore fixed fill mode render state
							cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
						}
					}
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing