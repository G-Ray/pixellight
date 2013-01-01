/*********************************************************\
 *  File: VertexShaderGLSL.cpp                           *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRendererOpenGL/ShaderLanguageGLSL.h"
#include "PLRendererOpenGL/ShaderToolsGLSL.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/VertexShaderGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
VertexShaderGLSL::~VertexShaderGLSL()
{
	// Destroy the OpenGL vertex shader
	glDeleteObjectARB(m_nOpenGLVertexShader);
}

/**
*  @brief
*    Returns the OpenGL vertex shader
*/
GLuint VertexShaderGLSL::GetOpenGLVertexShader() const
{
	return m_nOpenGLVertexShader;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
VertexShaderGLSL::VertexShaderGLSL(PLRenderer::Renderer &cRenderer) : PLRenderer::VertexShader(cRenderer),
	m_nOpenGLVertexShader(glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB))
{
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Shader functions           ]
//[-------------------------------------------------------]
String VertexShaderGLSL::GetShaderLanguage() const
{
	return ShaderLanguageGLSL::GLSL;
}

String VertexShaderGLSL::GetSourceCode() const
{
	// Get the length of the shader source, including the null termination character
	GLint nShaderSourceLength = 0;
	glGetObjectParameterivARB(m_nOpenGLVertexShader, GL_OBJECT_SHADER_SOURCE_LENGTH_ARB, &nShaderSourceLength);
	if (nShaderSourceLength > 1) {
		// The string class takes over the control of the string memory and also deletes it
		char *pszSourceCode = new char[nShaderSourceLength];
		glGetShaderSourceARB(m_nOpenGLVertexShader, nShaderSourceLength, nullptr, pszSourceCode);
		return String(pszSourceCode, false, nShaderSourceLength-1);	// -1 = excluding the null termination character
	}

	// Error!
	return "";
}

String VertexShaderGLSL::GetProfile() const
{
	// GLSL doesn't have profiles
	return "";
}

String VertexShaderGLSL::GetArguments() const
{
	// GLSL doesn't have shader compiler arguments
	return "";
}

String VertexShaderGLSL::GetEntry() const
{
	// GLSL doesn't have an user defined entry point
	return "";
}

bool VertexShaderGLSL::SetSourceCode(const String &sSourceCode, const String &sProfile, const String &sArguments, const String &sEntry)
{
	// GLSL doesn't have profiles, so sProfile is just ignored
	// GLSL doesn't have shader compiler arguments, so sArguments is just ignored
	// GLSL doesn't have an user defined entry point, so sEntry is just ignored
	return ShaderToolsGLSL::SetSourceCode(m_nOpenGLVertexShader, sSourceCode);
}


//[-------------------------------------------------------]
//[ Private virtual PLRenderer::Resource functions        ]
//[-------------------------------------------------------]
void VertexShaderGLSL::BackupDeviceData(uint8 **ppBackup)
{
	// Get the length of the shader source, including the null termination character
	GLint nShaderSourceLength = 0;
	glGetObjectParameterivARB(m_nOpenGLVertexShader, GL_OBJECT_SHADER_SOURCE_LENGTH_ARB, &nShaderSourceLength);
	if (nShaderSourceLength > 1) {
		*ppBackup = new uint8[nShaderSourceLength];
		glGetShaderSourceARB(m_nOpenGLVertexShader, nShaderSourceLength, nullptr, reinterpret_cast<GLcharARB*>(*ppBackup));
		glDeleteObjectARB(m_nOpenGLVertexShader);
		m_nOpenGLVertexShader = 0;
	} else {
		*ppBackup = nullptr;
	}
}

void VertexShaderGLSL::RestoreDeviceData(uint8 **ppBackup)
{
	// Restore data
	if (*ppBackup && !m_nOpenGLVertexShader) {
		m_nOpenGLVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

		// The string class takes over the control of the string memory and also deletes it
		SetSourceCode(String(reinterpret_cast<char*>(*ppBackup), false));
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
