/*********************************************************\
 *  File: Screenshot.cpp                                 *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/File/File.h>
#include <PLCore/File/Directory.h>
#include <PLCore/System/System.h>
#include <PLGraphics/Image/Image.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/SurfacePainter.h>
#include "PLEngine/Tools/Screenshot.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Screenshot)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Screenshot::Screenshot(RendererContext *pRendererContext, SurfacePainter *pSurfacePainter) :
	m_pRendererContext(pRendererContext),
	m_pSurfacePainter(pSurfacePainter)
{
}

/**
*  @brief
*    Destructor
*/
Screenshot::~Screenshot()
{
}

/**
*  @brief
*    Get renderer context
*/
RendererContext *Screenshot::GetRendererContext() const
{
	// Return renderer context
	return m_pRendererContext;
}

/**
*  @brief
*    Set renderer context
*/
void Screenshot::SetRendererContext(RendererContext *pRendererContext)
{
	// Set renderer context
	m_pRendererContext = pRendererContext;
}

/**
*  @brief
*    Get surface painter
*/
SurfacePainter *Screenshot::GetPainter() const
{
	// Return surface painter
	return m_pSurfacePainter;
}

/**
*  @brief
*    Set surface painter
*/
void Screenshot::SetPainter(SurfacePainter *pSurfacePainter)
{
	// Set surface painter
	m_pSurfacePainter = pSurfacePainter;
}

/**
*  @brief
*    Get screenshot directory
*/
String Screenshot::GetScreenshotDirectory() const
{
	// Return directory
	return m_sScreenshotDirectory;
}

/**
*  @brief
*    Set screenshot directory
*/
void Screenshot::SetScreenshotDirectory(const String &sPath)
{
	// Set directory
	m_sScreenshotDirectory = sPath;
}

/**
*  @brief
*    Save screenshot from current render target
*/
bool Screenshot::SaveScreenshot(const String &sFilename) const
{
	// Is there a renderer context?
	if (GetRendererContext()) {
		// Get screenshot
		Image cImage;
		GetRendererContext()->GetRenderer().MakeScreenshot(cImage);

		// Save screenshot
		return cImage.Save((sFilename.GetLength() > 0 ? sFilename : GetScreenshotFilename()));
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Save screenshot by using the surface painter of the current render target surface
*/
bool Screenshot::SaveCustomScreenshot(const String &sFilename, uint16 nWidth, uint16 nHeight, TextureBuffer::EPixelFormat nFormat, uint32 nFlags) const
{
	bool bResult = false; // Error by default

	// Check if width and height are valid and if there is a renderer context
	if (nWidth > 0 && nHeight > 0 && GetRendererContext()) {
		// Get renderer
		Renderer &cRenderer = GetRendererContext()->GetRenderer();

		// Backup current render target
		Surface *pRenderTargetBackup = cRenderer.GetRenderTarget();

		// Get painter of current render target
		if (m_pSurfacePainter) {
			// Create render target
			SurfaceTextureBuffer *pRenderTarget = cRenderer.CreateSurfaceTextureBufferRectangle(Vector2i(nWidth, nHeight), nFormat, nFlags);
			if (pRenderTarget) {
				// Set surface painter of our new render target
				pRenderTarget->SetPainter(m_pSurfacePainter);

				// Draw using the set surface painter
				pRenderTarget->Update();

				// Get screenshot
				Image cImage;
				cRenderer.MakeScreenshot(cImage);

				// Save screenshot
				bResult = cImage.Save((sFilename.GetLength() > 0 ? sFilename : GetScreenshotFilename()));

				// Restore previous render target
				cRenderer.SetRenderTarget(pRenderTargetBackup);

				// Unset surface painter of our new render target - else the shared surface painter is destroyed
				pRenderTarget->SetPainter(nullptr, false);

				// Destroy the render target
				delete pRenderTarget;
			}
		}
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Get a screenshot filename recommendation
*/
String Screenshot::GetScreenshotFilename(const String &sExtension) const
{
	// Open/create screenshots directory
	String sDirectory = m_sScreenshotDirectory;
	if (!sDirectory.GetLength())
		sDirectory = System::GetInstance()->GetCurrentDir();
	Directory cDirectory(sDirectory + "/_Screenshots");
	if (!cDirectory.Exists())
		cDirectory.Create();

	// Is this REALLY a directory?
	if (cDirectory.Exists() && cDirectory.IsDirectory()) {
		// Save screenshot
		uint32 nIndex = 0;
		do {
			// Get a screenshot name
			const String sFilename = String("Screenshot_") + nIndex + '.' + sExtension;

			// Is there already a file with this name?
			File cFile(sDirectory + "/_Screenshots/" + sFilename);
			if (cFile.Exists() && cFile.IsFile()) {
				// Jep, we have to find another filename :(
				nIndex++;

			// Save screenshot
			} else {
				// Save
				return sDirectory + "/_Screenshots/" + sFilename;
			}
		} while (nIndex);
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
