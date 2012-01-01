/*********************************************************\
 *  File: FontTextureShaders.cpp                         *
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
#include <PLRenderer/Renderer/Program.h>
#include <PLRenderer/Renderer/ProgramUniform.h>
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/FontManager.h"
#include "PLRendererOpenGL/ProgramUniform.h"
#include "PLRendererOpenGL/FontGlyphTexture.h"
#include "PLRendererOpenGL/FontTextureShaders.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
FontTextureShaders::~FontTextureShaders()
{
}


//[-------------------------------------------------------]
//[ Public virtual Font functions                         ]
//[-------------------------------------------------------]
void FontTextureShaders::Draw(const String &sText, const Color4 &cColor, const Matrix4x4 &mObjectSpaceToClipSpace, const Vector2 &vScale, const Vector2 &vBias, uint32 nFlags)
{
	// [TODO] Do no longer set this inside the font method, should be set from outside!
	GetRenderer().SetRenderState(PLRenderer::RenderState::BlendEnable, true);

	// Create glyph texture atlas if required
	if (!m_nOpenGLGlyphTextureAtlas)
		CreateGlyphTextureAtlas();
	if (m_nOpenGLGlyphTextureAtlas) {
		// Get and set the program for font rendering
		FontManager::GeneratedProgramUserData *pGeneratedProgramUserData = nullptr;
		PLRenderer::Program *pProgram = static_cast<FontManager*>(m_pFontManager)->GetProgram(&pGeneratedProgramUserData);
		if (pProgram && pGeneratedProgramUserData && pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix &&
			pGeneratedProgramUserData->pGlyphMap && pGeneratedProgramUserData->pColor && pGeneratedProgramUserData->pGlyphSizePenPosition &&
			pGeneratedProgramUserData->pTextureCoordinateMinMax && m_pFontManager->GetRenderer().SetProgram(pProgram)) {
			// Set object space to clip space matrix
			pGeneratedProgramUserData->pObjectSpaceToClipSpaceMatrix->Set(mObjectSpaceToClipSpace);

			// Set glyph texture atlas
			static_cast<ProgramUniform*>(pGeneratedProgramUserData->pGlyphMap)->Set(GL_TEXTURE_2D, m_nOpenGLGlyphTextureAtlas);

			// Enable/disable mipmapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (nFlags & Mipmapping) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

			// Set color
			pGeneratedProgramUserData->pColor->Set(cColor);

			// Get the text to draw - we only support ASCII
			const char *pszText = sText.GetASCII();

			// The current object space pen position
			Vector2 vPenPosition = vBias;

			// Center the text?
			if (nFlags & CenterText)
				vPenPosition.x -= GetTextWidth(sText)/2;

			// Get the renderer instance
			Renderer &cRenderer = static_cast<Renderer&>(GetFontManager().GetRenderer());

			// Iterate through all characters of the text to draw
			for (uint32 i=0; i<sText.GetLength(); i++, pszText++) {
				// Get the character code
				const unsigned char nCharacterCode = static_cast<unsigned char>(*pszText);

				// Get the glyph instance of the character to draw
				FontGlyphTexture *pFontGlyphTexture = static_cast<FontGlyphTexture*>(m_lstGlyphs[nCharacterCode]);
				if (pFontGlyphTexture) {
					// Set glyph size and pen position
					pGeneratedProgramUserData->pGlyphSizePenPosition->Set(static_cast<float>(pFontGlyphTexture->GetSize().x)*vScale.x,		// Object space glyph x size in points   (x) => x scale
																		  static_cast<float>(pFontGlyphTexture->GetSize().y)*vScale.y,		// Object space glyph y size in points   (y) => y scale
																		  (vPenPosition.x + pFontGlyphTexture->GetCorner().x)*vScale.x,		// Object space pen x position in points (z) => x bias
																		  (vPenPosition.y + pFontGlyphTexture->GetCorner().y)*vScale.y);	// Object space pen y position in points (w) => y bias

					// Set the normalized minimum and maximum glyph texture coordinate inside the glyph texture atlas
					pGeneratedProgramUserData->pTextureCoordinateMinMax->Set(pFontGlyphTexture->GetTexCoordMin().x, pFontGlyphTexture->GetTexCoordMin().y,
																			 pFontGlyphTexture->GetTexCoordMax().x, pFontGlyphTexture->GetTexCoordMax().y);

					// Draw the glyph quad
					cRenderer.DrawPrimitives(PLRenderer::Primitive::TriangleStrip, 0, 4);

					// Draw the glyph and let the pen advance to the object space position of the next character
					vPenPosition.x += pFontGlyphTexture->GetPenAdvance().x;
				}
			}

			// [TODO] UnderlineText & CrossoutText => Draw lines
		}
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FontTextureShaders::FontTextureShaders(FontManager &cFontManager, File &cFile) : FontTexture(cFontManager, cFile)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
