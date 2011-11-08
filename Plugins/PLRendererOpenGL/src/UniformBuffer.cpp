/*********************************************************\
 *  File: UniformBuffer.cpp                              *
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
#include <PLCore/System/System.h>
#include "PLRendererOpenGL/Context.h"
#include "PLRendererOpenGL/Renderer.h"
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/UniformBuffer.h"


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
UniformBuffer::~UniformBuffer()
{
	Clear();

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferNum--;
}

/**
*  @brief
*    Returns the dynamic data (none UBO)
*/
void *UniformBuffer::GetDynamicData() const
{
	return m_nUniformBuffer ? nullptr : m_pData;
}

/**
*  @brief
*    Returns the OpenGL uniform buffer
*/
uint32 UniformBuffer::GetOpenGLUniformBuffer() const
{
	return m_nUniformBuffer;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
UniformBuffer::UniformBuffer(PLRenderer::Renderer &cRenderer, const String &sShaderLanguage) : PLRenderer::UniformBuffer(cRenderer),
	m_sShaderLanguage(sShaderLanguage),
	m_nUniformBuffer(0),
	m_pData(nullptr),
	m_pLockedData(nullptr),
	m_bLockReadOnly(false),
	m_bUpdateUBO(false),
	m_nUsageAPI(0)
{
	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(cRenderer).GetWritableStatistics().nUniformBufferNum++;
}

/**
*  @brief
*    Makes this uniform buffer to the renderers current uniform buffer
*/
bool UniformBuffer::MakeCurrent()
{
	// Is there an uniform buffer?
	if (!m_nUniformBuffer && !m_pData)
		return false; // Error!

	// Bind the uniform buffer
	if (m_nUniformBuffer) {
		glBindBufferARB(GL_UNIFORM_BUFFER, m_nUniformBuffer);

		// Do we need to update the UBO?
		if (m_bUpdateUBO && m_pData) {
			m_bUpdateUBO = false;

			// Upload new data
			glBufferSubDataARB(GL_UNIFORM_BUFFER, 0, m_nSize, m_pData);
		}
	} else if (m_pData) {
		// Nothing to do
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::UniformBuffer functions    ]
//[-------------------------------------------------------]
String UniformBuffer::GetShaderLanguage() const
{
	return m_sShaderLanguage;
}


//[-------------------------------------------------------]
//[ Public virtual PLRenderer::Buffer functions           ]
//[-------------------------------------------------------]
bool UniformBuffer::IsAllocated() const
{
	return (m_nUniformBuffer != 0 || m_pData != nullptr);
}

bool UniformBuffer::Allocate(uint32 nElements, PLRenderer::Usage::Enum nUsage, bool bManaged, bool bKeepData)
{
	// Check if we have to reallocate the buffer
	if (m_nSize == nElements && nUsage == m_nUsage && m_bManaged == bManaged)
		return true; // Done

	// Get extensions instance
	const Extensions &cExtensions = static_cast<Renderer&>(GetRenderer()).GetContext().GetExtensions();

	// Vertex buffer extension available?
	if (!cExtensions.IsGL_ARB_vertex_buffer_object()) {
		// Fallback to legacy software implementation
		nUsage = PLRenderer::Usage::Software;
		bManaged = false;
	}

	// Get API dependent usage
 	if (nUsage == PLRenderer::Usage::Static)
		m_nUsageAPI = GL_STATIC_DRAW_ARB;
	else if (nUsage == PLRenderer::Usage::Dynamic)
		m_nUsageAPI = GL_DYNAMIC_DRAW_ARB;
	else if (nUsage == PLRenderer::Usage::WriteOnly)
		m_nUsageAPI = GL_STREAM_DRAW_ARB;
	else if (nUsage != PLRenderer::Usage::Software) {
		m_nUsageAPI = 0;

		// Error!
		return false;
	} else {
		m_nUsageAPI = 0;
	}

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferMem -= m_nSize;

	// Init data
	uint8 *pDataBackup = nullptr;
	uint32 nSizeBackup = m_nSize;
	if (m_pData && ((m_nSize != nElements) || (m_bManaged && !bManaged))) {
		// Backup the current data
		if (bKeepData) {
			pDataBackup = new uint8[m_nSize];
			MemoryManager::Copy(pDataBackup, m_pData, m_nSize);
		}

		// Delete data
		delete [] static_cast<uint8*>(m_pData);
		m_pData = nullptr;
	}
	if (m_nUniformBuffer && nUsage == PLRenderer::Usage::Software) {
		if (bKeepData && !m_pData && !pDataBackup && Lock(PLRenderer::Lock::ReadOnly)) {
			// Backup the current data
			pDataBackup = new uint8[m_nSize];
			MemoryManager::Copy(pDataBackup, GetData(), m_nSize);
			Unlock();
		}

		// Delete uniform buffer object
		glDeleteBuffersARB(1, &m_nUniformBuffer);
		m_nUniformBuffer = 0;
	}
	ForceUnlock();
	m_nElements  = nElements;
	m_nSize		 = m_nElements;
	m_nUsage	 = nUsage;
	m_bManaged   = bManaged;

	// Create the uniform buffer
	bool bUBO = false;
	if (nUsage != PLRenderer::Usage::Software && cExtensions.IsGL_ARB_vertex_buffer_object()) {
		// Use UBO
		bUBO = true;
		if (!m_nUniformBuffer)
			glGenBuffersARB(1, &m_nUniformBuffer);
		GLint nElementArrayBufferBackup;
		glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &nElementArrayBufferBackup);
		glBindBufferARB(GL_UNIFORM_BUFFER, m_nUniformBuffer);
		glBufferDataARB(GL_UNIFORM_BUFFER, m_nSize, nullptr, m_nUsageAPI);

		// Error checking
		int nArrayObjectSize = 0;
		glGetBufferParameterivARB(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE_ARB, &nArrayObjectSize);
		glBindBufferARB(GL_UNIFORM_BUFFER, nElementArrayBufferBackup);
		if (nArrayObjectSize <= 0) {
			// Delete uniform buffer object
			glDeleteBuffersARB(1, &m_nUniformBuffer);
			m_nUniformBuffer = 0;

			// Error!
			return false;
		}
	}

	// No UBO or managed?
	if (!bUBO || bManaged) {
		if (!m_pData)
			m_pData = new uint8[m_nSize];
		m_bUpdateUBO = true;
	} else {
		m_bUpdateUBO = false;
	}

	// Restore old data if required
	if (pDataBackup) {
		// We can just copy the old data in...
		if (Lock(PLRenderer::Lock::WriteOnly)) {
			uint32 nSize = nSizeBackup;
			if (nSize > m_nSize)
				nSize = m_nSize;
			MemoryManager::Copy(GetData(), pDataBackup, nSize);
			Unlock();
		}

		// Cleanup
		delete [] pDataBackup;
	}

	// Update renderer statistics
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferMem += m_nSize;

	// Done
	return true;
}

bool UniformBuffer::Clear()
{
	if (IsAllocated()) {
		ForceUnlock();
		if (m_nUniformBuffer) {
			glDeleteBuffersARB(1, &m_nUniformBuffer);
			m_nUniformBuffer = 0;
		}
		if (m_pData) {
			delete [] static_cast<uint8*>(m_pData);
			m_pData = nullptr;
		}

		// Update renderer statistics
		static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferMem -= m_nSize;

		// Init
		m_nElements  = 0;
		m_nSize		 = 0;
		m_nUsage	 = PLRenderer::Usage::Unknown;
		m_nUsageAPI  = 0;
		m_bUpdateUBO = false;
	}

	// Done
	return true;
}

void *UniformBuffer::Lock(uint32 nFlag)
{
	// Check whether there's an uniform buffer
	if (!m_nUniformBuffer && !m_pData)
		return nullptr; // Error!

	// Check whether the uniform buffer is already locked
	m_nLockCount++;
	if (!m_pLockedData) {
		// Get API dependent flag
		uint32 nFlagAPI;
		if (nFlag == PLRenderer::Lock::ReadOnly) {
			nFlagAPI        = GL_READ_ONLY_ARB;
			m_bLockReadOnly = true;
		} else if (nFlag == PLRenderer::Lock::WriteOnly) {
			nFlagAPI        = GL_WRITE_ONLY_ARB;
			m_bLockReadOnly = false;
		} else if (nFlag == PLRenderer::Lock::ReadWrite) {
			nFlagAPI        = GL_READ_WRITE_ARB;
			m_bLockReadOnly = false;
		} else {
			// Error!
			return nullptr;
		}

		// Map the uniform buffer
		static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBufferLocks++;
		m_nLockStartTime = System::GetInstance()->GetMicroseconds();
		if (m_pData)
			m_pLockedData = m_pData;
		else if (m_nUniformBuffer) {
			// Make this uniform buffer to the current one
			MakeCurrent();

			// Map
			m_pLockedData = glMapBufferARB(GL_UNIFORM_BUFFER, nFlagAPI);
		}

		// Lock valid?
		if (!m_pLockedData)
			m_nLockCount--;
	}

	// Return the locked data
	return m_pLockedData;
}

void *UniformBuffer::GetData()
{
	return m_pLockedData;
}

bool UniformBuffer::Unlock()
{
	// Check whether data is locked
	if (!m_pLockedData)
		return false; // Error!

	// Do we have to unlock the buffer now?
	m_nLockCount--;
	if (m_nLockCount)
		return true; // Nope, it's still used somewhere else...

	// Unlock the uniform buffer
	if (m_pData) {
		if (m_nUniformBuffer && !m_bLockReadOnly) {
			m_bUpdateUBO = true;

			// Make this uniform buffer to the current one
			MakeCurrent();
		}
	} else {
		if (m_nUniformBuffer) {
			// Make this uniform buffer to the current one
			MakeCurrent();

			// Unmap
			glUnmapBufferARB(GL_UNIFORM_BUFFER);
		}
	}
	static_cast<PLRenderer::RendererBackend&>(GetRenderer()).GetWritableStatistics().nUniformBuffersSetupTime += System::GetInstance()->GetMicroseconds()-m_nLockStartTime;
	m_pLockedData   = nullptr;
	m_bLockReadOnly = false;

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Protected virtual PLRenderer::Resource functions      ]
//[-------------------------------------------------------]
void UniformBuffer::BackupDeviceData(uint8 **ppBackup)
{
	// Backup data
	if (!m_pData && Lock(PLRenderer::Lock::ReadOnly)) {
		*ppBackup = new uint8[m_nSize];
		MemoryManager::Copy(*ppBackup, GetData(), m_nSize);
		ForceUnlock();
	} else {
		*ppBackup = nullptr;
	}

	// Destroy the uniform buffer
	if (m_nUniformBuffer) {
		glDeleteBuffersARB(1, &m_nUniformBuffer);
		m_nUniformBuffer = 0;
	}
}

void UniformBuffer::RestoreDeviceData(uint8 **ppBackup)
{
	if (*ppBackup) {
		GLint nElementArrayBufferBackup;
		glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &nElementArrayBufferBackup);
		glGenBuffersARB(1, &m_nUniformBuffer);
		glBindBufferARB(GL_UNIFORM_BUFFER, m_nUniformBuffer);
		glBufferDataARB(GL_UNIFORM_BUFFER, m_nSize, nullptr, m_nUsageAPI);
		if (Lock(PLRenderer::Lock::WriteOnly)) {
			MemoryManager::Copy(GetData(), *ppBackup, m_nSize);
			Unlock();
		}
		glBindBufferARB(GL_UNIFORM_BUFFER, nElementArrayBufferBackup);

		// Delete memory
		delete [] *ppBackup;
	} else {
		if (m_bManaged) {
			GLint nElementArrayBufferBackup;
			glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &nElementArrayBufferBackup);
			glGenBuffersARB(1, &m_nUniformBuffer);
			glBindBufferARB(GL_UNIFORM_BUFFER, m_nUniformBuffer);
			glBufferDataARB(GL_UNIFORM_BUFFER, m_nSize, nullptr, m_nUsageAPI);
			glBindBufferARB(GL_UNIFORM_BUFFER, nElementArrayBufferBackup);
			m_bUpdateUBO = true;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
