/*********************************************************\
 *  File: ProgramUniformGLSL.cpp                         *
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
#include <PLMath/Vector3i.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Matrix4x4.h>
#include <PLMath/Quaternion.h>
#include <PLGraphics/Color/Color3.h>
#include <PLGraphics/Color/Color4.h>
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/TextureBuffer1D.h"
#include "PLRendererOpenGL/TextureBuffer2D.h"
#include "PLRendererOpenGL/TextureBufferRectangle.h"
#include "PLRendererOpenGL/TextureBuffer3D.h"
#include "PLRendererOpenGL/TextureBufferCube.h"
#include "PLRendererOpenGL/Misc/ExtensionDefinitions.h"
#include "PLRendererOpenGL/ProgramUniformGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
using namespace PLGraphics;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    If this uniform is a texture, this method sets the OpenGL texture assigned to the uniform
*/
int ProgramUniformGLSL::Set(GLenum nOpenGLTextureTarget, GLuint nOpenGLTexture)
{
	// Valid texture unit?
	if (m_nTextureUnit >= 0) {
		// Activate the texture unit the uniform is assigned to
		glActiveTextureARB(GL_TEXTURE0_ARB + m_nTextureUnit);

		// Bind the OpenGL texture
		glBindTexture(nOpenGLTextureTarget, nOpenGLTexture);

		// Assign the current bound texture at the current activated texture unit to the uniform
		glUniform1iARB(m_nOpenGLUniformLocation, m_nTextureUnit);
	}

	// Done, return the texture unit the uniform is assigned to
	return m_nTextureUnit;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProgramUniformGLSL::ProgramUniformGLSL(GLuint nOpenGLProgram, int nOpenGLUniformLocation, int nTextureUnit) :
	m_nOpenGLProgram(nOpenGLProgram),
	m_nOpenGLUniformLocation(nOpenGLUniformLocation),
	m_nTextureUnit(nTextureUnit)
{
}

/**
*  @brief
*    Destructor
*/
ProgramUniformGLSL::~ProgramUniformGLSL()
{
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::ProgramUniform functions   ]
//[-------------------------------------------------------]
// 1 component
void ProgramUniformGLSL::Get(int &nX)
{
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, &nX);
}

void ProgramUniformGLSL::Get(float &fX)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, &fX);
}

void ProgramUniformGLSL::Get(double &fX)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Set(int nX)
{
	glUniform1iARB(m_nOpenGLUniformLocation, nX);
}

void ProgramUniformGLSL::Set(float fX)
{
	glUniform1fARB(m_nOpenGLUniformLocation, fX);
}

void ProgramUniformGLSL::Set(double fX)
{
	// Not supported by GL_ARB_shader_objects
}

// 2 components
void ProgramUniformGLSL::Get(int &nX, int &nY)
{
	int nElements[2];
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, nElements);
	nX = nElements[0];
	nY = nElements[1];
}

void ProgramUniformGLSL::Get(float &fX, float &fY)
{
	float fElements[2];
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, fElements);
	fX = fElements[0];
	fY = fElements[1];
}

void ProgramUniformGLSL::Get(double &fX, double &fY)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Get(Vector2i &vVector)
{
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, vVector.nV);
}

void ProgramUniformGLSL::Get(Vector2 &vVector)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, vVector.fV);
}

void ProgramUniformGLSL::Get2(int *pnComponents)
{
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, pnComponents);
}

void ProgramUniformGLSL::Get2(float *pfComponents)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, pfComponents);
}

void ProgramUniformGLSL::Get2(double *pfComponents)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Set(int nX, int nY)
{
	glUniform2iARB(m_nOpenGLUniformLocation, nX, nY);
}

void ProgramUniformGLSL::Set(float fX, float fY)
{
	glUniform2fARB(m_nOpenGLUniformLocation, fX, fY);
}

void ProgramUniformGLSL::Set(double fX, double fY)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Set(const Vector2i &vVector)
{
	glUniform2ivARB(m_nOpenGLUniformLocation, 1, vVector);
}

void ProgramUniformGLSL::Set(const Vector2 &vVector)
{
	glUniform2fvARB(m_nOpenGLUniformLocation, 1, vVector);
}

void ProgramUniformGLSL::Set2(const int *pnComponents)
{
	glUniform2ivARB(m_nOpenGLUniformLocation, 1, pnComponents);
}

void ProgramUniformGLSL::Set2(const float *pfComponents)
{
	glUniform2fvARB(m_nOpenGLUniformLocation, 1, pfComponents);
}

void ProgramUniformGLSL::Set2(const double *pfComponents)
{
	// Not supported by GL_ARB_shader_objects
}

// 3 components
void ProgramUniformGLSL::Get(int &nX, int &nY, int &nZ)
{
	int nElements[3];
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, nElements);
	nX = nElements[0];
	nY = nElements[1];
	nZ = nElements[2];
}

void ProgramUniformGLSL::Get(float &fX, float &fY, float &fZ)
{
	float fElements[3];
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, fElements);
	fX = fElements[0];
	fY = fElements[1];
	fZ = fElements[2];
}

void ProgramUniformGLSL::Get(double &fX, double &fY, double &fZ)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Get(Vector3i &vVector)
{
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, vVector.nV);
}

void ProgramUniformGLSL::Get(Vector3 &vVector)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, vVector.fV);
}

void ProgramUniformGLSL::Get(Color3 &cColor)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, cColor.fColor);
}

void ProgramUniformGLSL::Get3(int *pnComponents)
{
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, pnComponents);
}

void ProgramUniformGLSL::Get3(float *pfComponents)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, pfComponents);
}

void ProgramUniformGLSL::Get3(double *pfComponents)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Set(int nX, int nY, int nZ)
{
	glUniform3iARB(m_nOpenGLUniformLocation, nX, nY, nZ);
}

void ProgramUniformGLSL::Set(float fX, float fY, float fZ)
{
	glUniform3fARB(m_nOpenGLUniformLocation, fX, fY, fZ);
}

void ProgramUniformGLSL::Set(double fX, double fY, double fZ)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Set(const Vector3i &vVector)
{
	glUniform3ivARB(m_nOpenGLUniformLocation, 1, vVector);
}

void ProgramUniformGLSL::Set(const Vector3 &vVector)
{
	glUniform3fvARB(m_nOpenGLUniformLocation, 1, vVector);
}

void ProgramUniformGLSL::Set(const Color3 &cColor)
{
	glUniform3fvARB(m_nOpenGLUniformLocation, 1, cColor);
}

void ProgramUniformGLSL::Set3(const int *pnComponents)
{
	glUniform3ivARB(m_nOpenGLUniformLocation, 1, pnComponents);
}

void ProgramUniformGLSL::Set3(const float *pfComponents)
{
	glUniform3fvARB(m_nOpenGLUniformLocation, 1, pfComponents);
}

void ProgramUniformGLSL::Set3(const double *pfComponents)
{
	// Not supported by GL_ARB_shader_objects
}

// 4 components
void ProgramUniformGLSL::Get(int &nX, int &nY, int &nZ, int &nW)
{
	int nElements[4];
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, nElements);
	nX = nElements[0];
	nY = nElements[1];
	nZ = nElements[2];
	nW = nElements[3];
}

void ProgramUniformGLSL::Get(float &fX, float &fY, float &fZ, float &fW)
{
	float fElements[4];
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, fElements);
	fX = fElements[0];
	fY = fElements[1];
	fZ = fElements[2];
	fW = fElements[3];
}

void ProgramUniformGLSL::Get(double &fX, double &fY, double &fZ, double &fW)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Get(Vector4 &vVector)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, vVector.fV);
}

void ProgramUniformGLSL::Get(Color4 &cColor)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, cColor.fColor);
}

void ProgramUniformGLSL::Get(Quaternion &qQuaternion)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, qQuaternion.fQ);
}

void ProgramUniformGLSL::Get4(int *pnComponents)
{
	glGetUniformivARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, pnComponents);
}

void ProgramUniformGLSL::Get4(float *pfComponents)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, pfComponents);
}

void ProgramUniformGLSL::Get4(double *pfComponents)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Set(int nX, int nY, int nZ, int nW)
{
	glUniform4iARB(m_nOpenGLUniformLocation, nX, nY, nZ, nW);
}

void ProgramUniformGLSL::Set(float fX, float fY, float fZ, float fW)
{
	glUniform4fARB(m_nOpenGLUniformLocation, fX, fY, fZ, fW);
}

void ProgramUniformGLSL::Set(double fX, double fY, double fZ, double fW)
{
	// Not supported by GL_ARB_shader_objects
}

void ProgramUniformGLSL::Set(const Vector4 &vVector)
{
	glUniform4fvARB(m_nOpenGLUniformLocation, 1, vVector);
}

void ProgramUniformGLSL::Set(const Color4 &cColor)
{
	glUniform4fvARB(m_nOpenGLUniformLocation, 1, cColor);
}

void ProgramUniformGLSL::Set(const Quaternion &qQuaternion)
{
	glUniform4fvARB(m_nOpenGLUniformLocation, 1, qQuaternion);
}

void ProgramUniformGLSL::Set4(const int *pnComponents)
{
	glUniform4ivARB(m_nOpenGLUniformLocation, 1, pnComponents);
}

void ProgramUniformGLSL::Set4(const float *pfComponents)
{
	glUniform4fvARB(m_nOpenGLUniformLocation, 1, pfComponents);
}

void ProgramUniformGLSL::Set4(const double *pfComponents)
{
	// Not supported by GL_ARB_shader_objects
}

// Matrix
void ProgramUniformGLSL::Get(Matrix3x3 &mMatrix)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, mMatrix.fM);
}

void ProgramUniformGLSL::Get(Matrix4x4 &mMatrix)
{
	glGetUniformfvARB(m_nOpenGLProgram, m_nOpenGLUniformLocation, mMatrix.fM);
}

void ProgramUniformGLSL::Set(const Matrix3x3 &mMatrix, bool bTranspose)
{
	glUniformMatrix3fvARB(m_nOpenGLUniformLocation, 1, bTranspose, mMatrix);
}

void ProgramUniformGLSL::Set(const Matrix4x4 &mMatrix, bool bTranspose)
{
	glUniformMatrix4fvARB(m_nOpenGLUniformLocation, 1, bTranspose, mMatrix);
}

// Texture
int ProgramUniformGLSL::GetTextureUnit() const
{
	return m_nTextureUnit;
}

int ProgramUniformGLSL::Set(PLRenderer::TextureBuffer *pTextureBuffer)
{
	// Valid texture unit?
	if (m_nTextureUnit >= 0) {
		// Get the OpenGL texture
		GLuint nOpenGLTexture       = 0;
		GLenum nOpenGLTextureTarget = GL_TEXTURE_2D;
		if (pTextureBuffer) {
			// Check resource type
			switch (pTextureBuffer->GetType()) {
				case PLRenderer::Resource::TypeTextureBuffer1D:
					nOpenGLTexture = ((TextureBuffer1D*)pTextureBuffer)->GetOpenGLTexture();
					nOpenGLTextureTarget = GL_TEXTURE_1D;
					break;

				case PLRenderer::Resource::TypeTextureBuffer2D:
					nOpenGLTexture = ((TextureBuffer2D*)pTextureBuffer)->GetOpenGLTexture();
					nOpenGLTextureTarget = GL_TEXTURE_2D;
					break;

				case PLRenderer::Resource::TypeTextureBufferRectangle:
					nOpenGLTexture = ((TextureBufferRectangle*)pTextureBuffer)->GetOpenGLTexture();
					nOpenGLTextureTarget = GL_TEXTURE_RECTANGLE_EXT;
					break;

				case PLRenderer::Resource::TypeTextureBuffer3D:
					nOpenGLTexture = ((TextureBuffer3D*)pTextureBuffer)->GetOpenGLTexture();
					nOpenGLTextureTarget = GL_TEXTURE_3D_EXT;
					break;

				case PLRenderer::Resource::TypeTextureBufferCube:
					nOpenGLTexture = ((TextureBufferCube*)pTextureBuffer)->GetOpenGLTexture();
					nOpenGLTextureTarget = GL_TEXTURE_CUBE_MAP_ARB;
					break;
			}
		}

		// Activate the texture unit the uniform is assigned to
		glActiveTextureARB(GL_TEXTURE0_ARB + m_nTextureUnit);

		// Bind the OpenGL texture
		glBindTexture(nOpenGLTextureTarget, nOpenGLTexture);

		// Assign the current bound texture at the current activated texture unit to the uniform
		glUniform1iARB(m_nOpenGLUniformLocation, m_nTextureUnit);

		// Inform the renderer
		if (pTextureBuffer)
			((Renderer&)pTextureBuffer->GetRenderer()).SetShaderProgramTextureBuffer(m_nTextureUnit, pTextureBuffer);

		// Done, return the texture unit the uniform is assigned to
		return m_nTextureUnit;
	}

	// Error!
	return -1;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
