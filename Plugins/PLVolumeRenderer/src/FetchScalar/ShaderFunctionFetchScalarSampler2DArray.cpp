/*********************************************************\
 *  File: ShaderFunctionFetchScalarSampler2DArray.cpp    *
 *
 *  Master thesis
 *    "Scalable Realtime Volume Rendering"
 *
 *  At
 *    Fachhochschule W�rzburg-Schweinfurt
 *    Fakult�t Informatik, Wirtschaftsinformatik (FIW)
 *    http://www.fh-wuerzburg.de/
 *
 *  Author
 *    Christian Ofenberg (c.ofenberg@pixellight.org or cofenberg@googlemail.com)
 *    Copyright (C) 2011-2012
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLRenderer/Renderer/TextureBuffer2DArray.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Volume.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/FetchScalar/ShaderFunctionFetchScalarSampler2DArray.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionFetchScalarSampler2DArray)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionFetchScalarSampler2DArray::ShaderFunctionFetchScalarSampler2DArray()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionFetchScalarSampler2DArray::~ShaderFunctionFetchScalarSampler2DArray()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionFetchScalarSampler2DArray::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "Sampler2DArray_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderHeader)
			return sSourceCode_Fragment_Header;
	} else if (sShaderLanguage == Cg) {
		#include "Sampler2DArray_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderHeader)
			return sSourceCode_Fragment_Header;
	}

	// Error!
	return "";
}

void ShaderFunctionFetchScalarSampler2DArray::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get used renderer instance
	Renderer &cRenderer = cProgram.GetRenderer();

	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return;	// Early escape, there's no sense in continuing

	// Get the volume resource
	PLVolume::Volume *pVolume = pSNVolume->GetVolume();
	if (!pVolume)
		return;	// Early escape, there's no sense in continuing

	// Get the renderer texture buffer holding the 3D voxel data
	PLRenderer::TextureBuffer *pVolumeTextureBuffer = pVolume->GetVolumeTextureBuffer(cRenderer);
	if (!pVolumeTextureBuffer || pVolumeTextureBuffer->GetType() != PLRenderer::Resource::TypeTextureBuffer2DArray)
		return;	// Early escape, there's no sense in continuing

	// Get the number of available mipmaps within the volume texture
	const uint32 nNumOfMipmaps = pVolumeTextureBuffer->GetNumOfMipmaps();

	{ // Volume texture
		const int nTextureUnit = cProgramWrapper.Set("VolumeTexture", pVolumeTextureBuffer);
		if (nTextureUnit >= 0) {
			// Setup texture filtering by using clamp to border color
			// -> By default, the border color is black -> out-of-bound access will return black
			// -> Do not use "TextureAddressing::Clamp" in here which would reuse the last valid color
			//    which might result in funny looking infinity stretched borders
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressU, TextureAddressing::Border);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressV, TextureAddressing::Border);
			cRenderer.SetSamplerState(nTextureUnit, Sampler::AddressW, TextureAddressing::Border);

			// Setup texture filtering
			switch (pSNVolume->Reconstruction.Get()) {
				// Nearest neighbour filtering
				case PLVolume::SNVolume::NearestNeighbourFiltering:
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::None);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::Linear);	// We still want to have mipmapping
					break;

				// Trilinear filtering
				case PLVolume::SNVolume::TrilinearFiltering:
				// Tri-cubic filtering (third-order texture filtering, has same basic setup as trilinear filtering)
				case PLVolume::SNVolume::TriCubicFiltering:
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MagFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MinFilter, TextureFiltering::Linear);
					cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::Linear);
					break;
			}

			// In case there are no mipmaps available within the volume texture: Disable mipmapping
			if (!nNumOfMipmaps)
				cRenderer.SetSamplerState(nTextureUnit, Sampler::MipFilter, TextureFiltering::None);

			// Set the depth of the volume texture (z-component)
			cProgramWrapper.Set("VolumeTextureDepth", static_cast<float>(static_cast<TextureBuffer2DArray*>(pVolumeTextureBuffer)->GetSize().z));
		}
	}

	{ // Set volume texture level of detail (0...<number of mipmaps>-1)
	  // -> Ensure that we don't exceed the number of mipmaps available within the volume texture
		float fVolumeTextureLOD = pSNVolume->VolumeTextureLOD.Get() + cSRPVolume.GetGlobalVolumeTextureLOD();
		if (fVolumeTextureLOD < 0.0f)
			fVolumeTextureLOD = 0.0f;
		else if (fVolumeTextureLOD > nNumOfMipmaps)
			fVolumeTextureLOD = static_cast<float>(nNumOfMipmaps);
		cProgramWrapper.Set("VolumeTextureLOD", fVolumeTextureLOD);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
