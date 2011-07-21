/*********************************************************\
 *  File: TextureBuffer1D.cpp                            *
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
#include <PLCore/Log/Log.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImagePart.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/TextureBuffer1D.h"


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
TextureBuffer1D::~TextureBuffer1D()
{
	if (m_nOpenGLTexture)
		glDeleteTextures(1, &m_nOpenGLTexture);

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetStatisticsT().nTextureBuffersNum--;
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetStatisticsT().nTextureBuffersMem -= GetTotalNumOfBytes();
}

/**
*  @brief
*    Returns the OpenGL resource ID of the texture buffer
*/
uint32 TextureBuffer1D::GetOpenGLTexture() const
{
	return m_nOpenGLTexture;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
TextureBuffer1D::TextureBuffer1D(PLRenderer::Renderer &cRenderer, Image &cImage, EPixelFormat nInternalFormat, uint32 nFlags) :
	PLRenderer::TextureBuffer1D(cRenderer, nFlags),
	m_nOpenGLTexture(0)
{
	// Get the OpenGL renderer instance
	Renderer &cRendererOpenGL = static_cast<Renderer&>(cRenderer);

	// Update renderer statistics
	cRendererOpenGL.GetStatisticsT().nTextureBuffersNum++;

	// Initialize sampler states
	MemoryManager::Set(m_nSamplerState, PLRenderer::Sampler::Unknown, sizeof(uint32)*PLRenderer::Sampler::Number);

	// Choose the texture buffer pixel formats which should be used
	EPixelFormat nImageFormat;
	bool bUsePreCompressedData;
	m_nFormat = cRendererOpenGL.ChooseFormats(cImage, nInternalFormat, nFlags, nImageFormat, bUsePreCompressedData);


	// [HACK] Uploading is working, but when using 'TextureBuffer1D::BackupDeviceData()' and restoring the data
	// we get trash when using 1D textures with compression - maybe a driver bug or I'am missing anything...
	bUsePreCompressedData = false;
	if (m_nFormat == DXT1)
		m_nFormat = R8G8B8;
	if (m_nFormat == DXT3 || m_nFormat == DXT5)
		m_nFormat = R8G8B8A8;


	// Get the first image part
	const ImagePart *pImagePart = cImage.GetPart(0);
	if (pImagePart) {
		// Get the first image buffer
		const ImageBuffer *pImageBuffer = pImagePart->GetMipmap(0);
		if (pImageBuffer) {
			// Get API pixel format
			uint32 *pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
			if (pAPIPixelFormat) {
				// Is this a compressed texture buffer pixel format?
				const bool bCompressedFormat = IsCompressedFormat();

				// Get uncompressed image format information
				EPixelFormat nImageFormatUncompressed     = GetFormatFromImage(cImage, true);
				uint32		 nAPIImageFormatUncompressed  = cRendererOpenGL.GetOpenGLPixelFormat(nImageFormatUncompressed);
				uint32		 nImageDataFormatUncompressed = cRendererOpenGL.GetOpenGLDataFormat(nImageFormatUncompressed);

				// Set the size
				m_nSize = pImageBuffer->GetSize().x;
				if (m_nSize == 1)
					m_nSize = pImageBuffer->GetSize().y;

				// Create OpenGL texture buffer
				glGenTextures(1, &m_nOpenGLTexture);
				glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

				// Upload the texture buffer
				m_nNumOfMipmaps = pImagePart->GetNumOfMipmaps() - 1;
				const bool bMipmaps = nFlags & Mipmaps;
				if (!m_nNumOfMipmaps && bMipmaps) {
					// Calculate the number of mipmaps
					m_nNumOfMipmaps = static_cast<uint32>(Math::Log2(static_cast<float>(m_nSize)));

					// Try to build mipmaps automatically on the GPU
					if (cRendererOpenGL.IsGL_SGIS_generate_mipmap()) {
						glTexParameteri(GL_TEXTURE_1D, GL_GENERATE_MIPMAP_SGIS, true);

						// Upload the texture buffer
						if (bUsePreCompressedData)
							glCompressedTexImage1DARB(GL_TEXTURE_1D, 0, *pAPIPixelFormat, m_nSize, 0, pImageBuffer->GetCompressedDataSize(), pImageBuffer->GetCompressedData());
						else
							glTexImage1D(GL_TEXTURE_1D, 0, *pAPIPixelFormat, m_nSize, 0, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pImageBuffer->HasData() ? pImageBuffer->GetData() : nullptr);

						// If compressed internal format, check whether all went fine
						if (bCompressedFormat) {
							GLint nCompressed;
							glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_COMPRESSED_ARB, &nCompressed);
							if (!nCompressed) {
								// There was an error, use no compression
								m_nFormat = nImageFormat;
								pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
								if (pAPIPixelFormat)
									glTexImage1D(GL_TEXTURE_1D, 0, *pAPIPixelFormat, m_nSize, 0, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pImageBuffer->HasData() ? pImageBuffer->GetData() : nullptr);
							}
						}
					} else {
						// No pre compressed image data can be used
						gluBuild1DMipmaps(GL_TEXTURE_1D, *pAPIPixelFormat, m_nSize, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pImageBuffer->HasData() ? pImageBuffer->GetData() : nullptr);

						// If compressed internal format, check whether all went fine
						if (bCompressedFormat) {
							GLint nCompressed;
							glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_COMPRESSED_ARB, &nCompressed);
							if (!nCompressed) { // There was an error, use no compression
								m_nFormat = nImageFormat;
								pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
								if (pAPIPixelFormat)
									gluBuild1DMipmaps(GL_TEXTURE_1D, *pAPIPixelFormat, m_nSize, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pImageBuffer->HasData() ? pImageBuffer->GetData() : nullptr);
							}
						}
					}

					// Calculate the total number of bytes this texture buffer requires
					for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++)
						m_nTotalNumOfBytes += GetNumOfBytes(nLevel);
				} else {
					// Ignore mipmaps?
					if (!bMipmaps)
						m_nNumOfMipmaps = 0; // Jep, we do not want to have any mipmaps for this texture buffer

					// Use given mipmaps
					uint32 nSize = 0, nLevel = 0;
					for (; nLevel<=m_nNumOfMipmaps; nLevel++) {
						// Get the mipmap image buffer
						const ImageBuffer *pMipmapImageBuffer = pImagePart->GetMipmap(nLevel);
						if (pMipmapImageBuffer) {
							// Get the size of this mipmap level
							nSize = pMipmapImageBuffer->GetSize().x;
							if (nSize == 1)
								nSize = pMipmapImageBuffer->GetSize().y;

							// Upload the texture buffer
							if (bUsePreCompressedData && pMipmapImageBuffer->HasCompressedData())
								glCompressedTexImage1DARB(GL_TEXTURE_1D, nLevel, *pAPIPixelFormat, nSize, 0, pMipmapImageBuffer->GetCompressedDataSize(), pMipmapImageBuffer->GetCompressedData());
							else
								glTexImage1D(GL_TEXTURE_1D, nLevel, *pAPIPixelFormat, nSize, 0, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pMipmapImageBuffer->HasData() ? pMipmapImageBuffer->GetData() : nullptr);

							// If compressed internal format, check whether all went fine
							if (bCompressedFormat) {
								GLint nCompressed;
								glGetTexLevelParameteriv(GL_TEXTURE_1D, nLevel, GL_TEXTURE_COMPRESSED_ARB, &nCompressed);
								if (!nCompressed) {
									// There was an error, use no compression as fallback
									m_nFormat = nImageFormat;
									pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
									if (pAPIPixelFormat)
										glTexImage1D(GL_TEXTURE_1D, nLevel, *pAPIPixelFormat, nSize, 0, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pMipmapImageBuffer->HasData() ? pMipmapImageBuffer->GetData() : nullptr);
								}
							}
						}

						// Update the total number of bytes this texture buffer requires
						m_nTotalNumOfBytes += GetNumOfBytes(nLevel);
					}

					// We have to define all mipmap levels down to 1 otherwise the texture buffer is invalid when we try to use any min
					// filter that uses mipmaps. OpenGL "normally" uses white color when invalid/incomplete texture buffer is enabled.
					if (bMipmaps && nSize != 1) {
						// Write a warning into the log
						PL_LOG(Warning, String::Format("Lowest mipmap is %d, but should be 1! Missing mipmap levels will be white!", nSize))

						// If we don't define all mipmap levels down to 1 'mysterious' graphics bugs may occure were it is not
						// always easy to pinpoint the problem directly to the mipmaps. So, to avoid frustration during bug finding,
						// we just create the missing mipmap levels down to 1 with a white color - so it's possible to 'see' which texture
						// isn't correct without reading the log message from above. (for some people it appears to be hard to read logs *g*)
						char *pszBuffer = nullptr;
						do {
							// First at all, 'half' the current dimension
							if (nSize > 1)
								nSize /= 2;

							// Update the total number of mipmap levels
							m_nNumOfMipmaps++;

							// Update the total number of bytes this texture buffer requires
							const uint32 nNumOfBytes = GetNumOfBytes(nLevel);
							m_nTotalNumOfBytes += nNumOfBytes;

							// Allocate memory for your white buffer and set it to "white" (we only do this once for the larges one)
							if (!pszBuffer) {
								pszBuffer = new char[nNumOfBytes];
								MemoryManager::Set(pszBuffer, 255, nNumOfBytes);
							}

							// Upload the texture buffer
							glTexImage1D(GL_TEXTURE_1D, nLevel, *pAPIPixelFormat, nSize, 0, nAPIImageFormatUncompressed, nImageDataFormatUncompressed, pszBuffer);

							// Update the mipmap level counter
							nLevel++;
						} while (nSize != 1);

						// Cleanup your white buffer
						if (pszBuffer)
							delete [] pszBuffer;
					}
				}

				// Update renderer statistics
				cRendererOpenGL.GetStatisticsT().nTextureBuffersMem += GetTotalNumOfBytes();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::TextureBuffer functions    ]
//[-------------------------------------------------------]
bool TextureBuffer1D::Upload(uint32 nMipmap, EPixelFormat nFormat, const void *pData, uint8 nFace)
{
	// Check parameters
	if (nMipmap <= m_nNumOfMipmaps && nFormat != Unknown && pData && !nFace) {
		// Get the OpenGL renderer instance
		const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

		// Get API pixel format
		const uint32 *pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
		if (pAPIPixelFormat) {
			// Get the size of this mipmap level
			const uint32 nSize = GetSize(nMipmap);

			// Compressed format?
			if (IsCompressedFormat(nFormat)) {
				// Internal format MUST be the same as the given format
				if (m_nFormat != nFormat)
					return false; // Error!

				// Bind this texture buffer
				glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

				// Get the number of bytes
				const uint32 nNumOfBytes = GetNumOfBytes(nMipmap);

				// Upload
				glCompressedTexImage1DARB(GL_TEXTURE_1D, nMipmap, *pAPIPixelFormat, nSize, 0, nNumOfBytes, pData);
			} else {
				// Bind this texture buffer
				glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

				// Upload
				const uint32 nPixelFormat = cRendererOpenGL.GetOpenGLPixelFormat(nFormat);
				const uint32 nDataFormat  = cRendererOpenGL.GetOpenGLDataFormat(nFormat);
				glTexImage1D(GL_TEXTURE_1D, nMipmap, *pAPIPixelFormat, nSize, 0, nPixelFormat, nDataFormat, pData);
			}

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool TextureBuffer1D::Download(uint32 nMipmap, EPixelFormat nFormat, void *pData, uint8 nFace)
{
	// Check parameters
	if (nMipmap > m_nNumOfMipmaps || nFormat == Unknown || !pData || nFace)
		return false; // Error!

	// Compressed format?
	if (IsCompressedFormat(nFormat)) {
		// Internal format MUST be the same as the given format
		if (m_nFormat != nFormat)
			return false; // Error!

		// Bind this texture buffer
		glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

		// Download
		glGetCompressedTexImageARB(GL_TEXTURE_1D, nMipmap, pData);
	} else {
		// Bind this texture buffer
		glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

		// Get the OpenGL renderer instance
		const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

		// Download
		const uint32 nPixelFormat = cRendererOpenGL.GetOpenGLPixelFormat(nFormat);
		const uint32 nDataFormat  = cRendererOpenGL.GetOpenGLDataFormat(nFormat);
		glGetTexImage(GL_TEXTURE_1D, nMipmap, nPixelFormat, nDataFormat, pData);
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::TextureBuffer functions   ]
//[-------------------------------------------------------]
bool TextureBuffer1D::MakeCurrent(uint32 nStage)
{
	// Check if there are renderer information
	if (GetRenderer().GetTextureBuffer(nStage) != this) {
		if (!GetRenderer().SetTextureBuffer(nStage, this))
			return true; // Done
	}

	// Make this texture buffer to the renderers current one
	glEnable(GL_TEXTURE_1D);
	glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void TextureBuffer1D::BackupDeviceData(uint8 **ppBackup)
{
	// Allocate memory
	const uint32 nTotalNumOfBytes = GetTotalNumOfBytes();
	if (nTotalNumOfBytes) {
		*ppBackup = new uint8[nTotalNumOfBytes];
		if (*ppBackup) {
			// Prepare for backup
			glEnable(GL_TEXTURE_1D);
			glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

			// Backup
			uint8 *pData = *ppBackup;
			if (IsCompressedFormat()) {
				// Loop through all mipmaps
				for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++) {
					// Get the data from the GPU
					glGetCompressedTexImageARB(GL_TEXTURE_1D, nLevel, pData);

					// Next level, please
					pData += GetNumOfBytes(nLevel);
				}
			} else {
				// Get the OpenGL renderer instance
				const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

				// Get format information
				const uint32 nPixelFormat = cRendererOpenGL.GetOpenGLPixelFormat(m_nFormat);
				const uint32 nDataFormat  = cRendererOpenGL.GetOpenGLDataFormat(m_nFormat);

				// Loop through all mipmaps
				for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++) {
					// Get the data from the GPU
					glGetTexImage(GL_TEXTURE_1D, nLevel, nPixelFormat, nDataFormat, pData);

					// Next level, please
					pData += GetNumOfBytes(nLevel);
				}
			}

			// Destroy this OpenGL texture buffer
			if (m_nOpenGLTexture) {
				glDeleteTextures(1, &m_nOpenGLTexture);
				m_nOpenGLTexture = 0;
			}
		}
	} else {
		*ppBackup = nullptr;
	}
}

void TextureBuffer1D::RestoreDeviceData(uint8 **ppBackup)
{
	// Restore data
	if (*ppBackup) {
		glGenTextures(1, &m_nOpenGLTexture);
		glBindTexture(GL_TEXTURE_1D, m_nOpenGLTexture);

		// Get the OpenGL renderer instance
		const Renderer &cRendererOpenGL = static_cast<Renderer&>(GetRenderer());

		// Get API pixel format
		const uint32  nPixelFormat    = cRendererOpenGL.GetOpenGLPixelFormat(m_nFormat);
		const uint32  nDataFormat     = cRendererOpenGL.GetOpenGLDataFormat(m_nFormat);
		const uint32 *pAPIPixelFormat = cRendererOpenGL.GetAPIPixelFormat(m_nFormat);
		if (pAPIPixelFormat) {
			// Is this a compressed texture buffer pixel format?
			const bool bCompressedFormat = IsCompressedFormat();

			// Data pointer
			const uint8 *pData = *ppBackup;

			// Loop through all mipmaps
			for (uint32 nLevel=0; nLevel<=m_nNumOfMipmaps; nLevel++) {
				// Get the size of this mipmap level
				const uint32 nSize = GetSize(nLevel);

				// Get the number of bytes
				const uint32 nNumOfBytes = GetNumOfBytes(nLevel);

				// Upload the texture buffer
				if (bCompressedFormat)
					glCompressedTexImage1DARB(GL_TEXTURE_1D, nLevel, *pAPIPixelFormat, nSize, 0, nNumOfBytes, pData);
				else
					glTexImage1D(GL_TEXTURE_1D, nLevel, *pAPIPixelFormat, nSize, 0, nPixelFormat, nDataFormat, pData);

				// Next level, please
				pData += nNumOfBytes;
			}
		}

		// Delete memory
		delete [] *ppBackup;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
