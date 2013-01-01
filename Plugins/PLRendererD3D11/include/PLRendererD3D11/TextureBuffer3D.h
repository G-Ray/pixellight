/*********************************************************\
 *  File: TextureBuffer3D.h                              *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the �Software�), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#ifndef __PLRENDERERD3D11_TEXTUREBUFFER3D_H__
#define __PLRENDERERD3D11_TEXTUREBUFFER3D_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/TextureBuffer3D.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererD3D11 {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    D3D11 3D texture buffer resource
*/
class TextureBuffer3D : public PLRenderer::TextureBuffer3D {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Renderer;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureBuffer3D();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] cImage
		*    Texture buffer image
		*  @param[in] nInternalFormat
		*    Desired internal texture buffer pixel format, if unknown use the format of the given image
		*  @param[in] nFlags
		*    Texture buffer flags (see EFlags)
		*
		*  @note
		*    - The image is assumed to be valid because it was already checked by the renderer texture buffer
		*      create function
		*/
		TextureBuffer3D(PLRenderer::Renderer &cRenderer, PLGraphics::Image &cImage, EPixelFormat nInternalFormat, PLCore::uint32 nFlags);


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::TextureBuffer functions    ]
	//[-------------------------------------------------------]
	public:
		virtual bool CopyDataFrom(PLCore::uint32 nMipmap, EPixelFormat nFormat, const void *pData, PLCore::uint8 nFace = 0) override;
		virtual bool CopyDataTo(PLCore::uint32 nMipmap, EPixelFormat nFormat, void *pData, PLCore::uint8 nFace = 0) const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::TextureBuffer functions   ]
	//[-------------------------------------------------------]
	private:
		virtual bool MakeCurrent(PLCore::uint32 nStage) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererD3D11


#endif // __PLRENDERERD3D11_TEXTUREBUFFER3D_H__
