/*********************************************************\
 *  File: ConnectionDevice.cpp                           *
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
#include <PLCore/System/Thread.h>
#include <PLCore/System/CriticalSection.h>
#include "PLInput/Backend/ConnectionDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ConnectionDevice::ConnectionDevice() :
	m_nDeviceType(DeviceTypeUnknown),
	m_pInputBuffer(nullptr),
	m_pOutputBuffer(nullptr),
	m_nInputReportSize(0),
	m_nOutputReportSize(0),
	m_pThread(nullptr),
	m_pCriticalSection(nullptr),
	m_bThreadExit(false)
{
	// Set backend type
	m_nBackendType = BackendConnectionDevice;
}

/**
*  @brief
*    Destructor
*/
ConnectionDevice::~ConnectionDevice()
{
	// Stop thread
	StopThread();
}

/**
*  @brief
*    Get device type
*/
ConnectionDevice::EDeviceType ConnectionDevice::GetDeviceType() const
{
	// Return device type
	return m_nDeviceType;
}

/**
*  @brief
*    Get input report size
*/
uint32 ConnectionDevice::GetInputReportSize() const
{
	return m_nInputReportSize;
}

/**
*  @brief
*    Set input report size
*/
void ConnectionDevice::SetInputReportSize(uint32 nSize)
{
	m_nInputReportSize = static_cast<uint16>(nSize);
}

/**
*  @brief
*    Get output report size
*/
uint32 ConnectionDevice::GetOutputReportSize() const
{
	return m_nOutputReportSize;
}

/**
*  @brief
*    Set output report size
*/
void ConnectionDevice::SetOutputReportSize(uint32 nSize)
{
	m_nOutputReportSize = static_cast<uint16>(nSize);
}

/**
*  @brief
*    Get input buffer
*/
uint8 *ConnectionDevice::GetInputBuffer() const
{
	return m_pInputBuffer;
}

/**
*  @brief
*    Get output buffer
*/
uint8 *ConnectionDevice::GetOutputBuffer() const
{
	return m_pOutputBuffer;
}


//[-------------------------------------------------------]
//[ Public virtual ConnectionDevice functions             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Open device connection
*/
bool ConnectionDevice::Open(uint16 nOutputPort, uint16 nInputPort)
{
	// To be implemented in derived classes
	// Please use InitThread() to start the read thread after successful connection
	// And don't forget to call EventOnConnect please

	// Not implemented here...
	return false;
}

/**
*  @brief
*    Close device connection
*/
bool ConnectionDevice::Close()
{
	// To be implemented in derived classes
	// Please use StopThread() to stop the read thread after closing the connection
	// And don't forget to call EventOnDisconnect please

	// Not implemented here...
	return false;
}

/**
*  @brief
*    Check if the device is open
*/
bool ConnectionDevice::IsOpen() const
{
	// To be implemented in derived classes

	// Not implemented here...
	return false;
}

/**
*  @brief
*    Read from device
*/
bool ConnectionDevice::Read(uint8 *pBuffer, uint32 nSize)
{
	// To be implemented in derived classes
	// Please call LockCriticalSection() before and UnlockCriticalSection() after the read operation
	// And don't forget to call EventOnRead please

	// Not implemented here...
	return false;
}

/**
*  @brief
*    Write to device
*/
bool ConnectionDevice::Write(const uint8 *pBuffer, uint32 nSize)
{
	// To be implemented in derived classes
	// Please call LockCriticalSection() before and UnlockCriticalSection() after the write operation

	// Not implemented here...
	return false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initialize and start thread for read/write operations
*/
void ConnectionDevice::InitThread()
{
	// Create input buffer
	if (!m_pInputBuffer && m_nInputReportSize > 0)
		m_pInputBuffer  = new uint8[m_nInputReportSize];

	// Create output buffer
	if (!m_pOutputBuffer && m_nOutputReportSize > 0)
		m_pOutputBuffer = new uint8[m_nOutputReportSize];

	// Create critical section
	if (!m_pCriticalSection)
		m_pCriticalSection = new CriticalSection();

	// Start thread
	if (!m_pThread) {
		m_pThread = new Thread(ConnectionDevice::ReadThread, this);
		m_pThread->Start();
	}
}

/**
*  @brief
*    Stop thread for read/write operations
*/
void ConnectionDevice::StopThread()
{
	// Stop thread
	if (m_pThread) {
		// Exit thread function and wait until it has ended
		m_bThreadExit = true;
		m_pThread->Join();

		// Delete thread
		delete m_pThread;
		m_pThread = nullptr;
	}

	// Delete critical section
	if (m_pCriticalSection) {
		delete m_pCriticalSection;
		m_pCriticalSection = nullptr;
	}

	// Destroy input buffer
	if (m_pInputBuffer) {
		delete [] m_pInputBuffer;
		m_pInputBuffer = nullptr;
	}

	// Destroy output buffer
	if (m_pOutputBuffer) {
		delete [] m_pOutputBuffer;
		m_pOutputBuffer = nullptr;
	}
}

/**
*  @brief
*    Lock read/write critical section
*/
void ConnectionDevice::LockCriticalSection()
{
	// Check critical section
	if (m_pCriticalSection) {
		// Lock critical section (no read/write allowed)
		m_pCriticalSection->Lock();
	}
}

/**
*  @brief
*    Unlock read/write critical section
*/
void ConnectionDevice::UnlockCriticalSection()
{
	// Check critical section
	if (m_pCriticalSection) {
		// Unlock critical section
		m_pCriticalSection->Unlock();
	}
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Device update thread function
*/
int ConnectionDevice::ReadThread(void *pData)
{
	// Get handler
	ConnectionDevice *pDevice = static_cast<ConnectionDevice*>(pData);
	while (!pDevice->m_bThreadExit) {
		// Read data from device
		pDevice->Read(pDevice->m_pInputBuffer, pDevice->m_nInputReportSize);
	}

	// Done
	return 0;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
