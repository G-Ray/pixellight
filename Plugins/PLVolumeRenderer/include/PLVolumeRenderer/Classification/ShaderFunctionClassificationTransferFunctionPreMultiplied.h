/*********************************************************\
 *  File: ShaderFunctionClassificationTransferFunctionPreMultiplied.h *
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


#ifndef __PLVOLUMERENDERER_SHADERFUNCTION_CLASSIFICATION_TRANSFERFUNCTIONPREMULTIPLIED_H__
#define __PLVOLUMERENDERER_SHADERFUNCTION_CLASSIFICATION_TRANSFERFUNCTIONPREMULTIPLIED_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/Classification/ShaderFunctionClassification.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Pre-multiplied (no color bleeding) post-interpolative transfer function shader function implementation class for classification
*
*  @remarks
*    The given scalar value is used as index for a transfer function.
*
*    Is using pre-multiplied colors by their corresponding opacity value as described within the book "Real-Time Volume Graphics", section "3.2.3 Compositing" (page 54)
*/
class ShaderFunctionClassificationTransferFunctionPreMultiplied : public ShaderFunctionClassification {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderFunctionClassificationTransferFunctionPreMultiplied, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunctionClassification, "Pre-multiplied (no color bleeding) post-interpolative function shader function implementation class for classification")
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
		PLVOLUMERENDERER_API ShaderFunctionClassificationTransferFunctionPreMultiplied();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderFunctionClassificationTransferFunctionPreMultiplied();


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunction functions               ]
	//[-------------------------------------------------------]
	public:
		PLVOLUMERENDERER_API virtual PLCore::String GetSourceCode(const PLCore::String &sShaderLanguage, ESourceCodeType nSourceCodeType) override;
		PLVOLUMERENDERER_API virtual void SetProgram(PLRenderer::Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERFUNCTION_CLASSIFICATION_TRANSFERFUNCTIONPREMULTIPLIED_H__
