/*********************************************************\
 *  File: StreamWav.cpp                                  *
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
#include <PLGeneral/File/File.h>
#include "PLSoundOpenAL/SoundManager.h"
#include "PLSoundOpenAL/Buffer.h"
#include "PLSoundOpenAL/StreamWav.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
namespace PLSoundOpenAL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
StreamWav::StreamWav(ALuint nSource, const Buffer &cBuffer) : Stream(nSource, cBuffer),
	m_pFile(NULL),
	m_pnData(NULL),
	m_nStreamSize(0),
	m_nStreamPos(0),
	m_nFormat(0),
	m_nFrequency(0),
	m_nSwapSize(0),
	m_pnSwap(NULL)
{
	// Create front and back buffers
	alGenBuffers(2, m_nBuffers);
}

/**
*  @brief
*    Destructor
*/
StreamWav::~StreamWav()
{
	DeInit();
	alDeleteBuffers(2, m_nBuffers);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Opens the stream
*/
bool StreamWav::OpenStream()
{
	// If already opened, restart it
	if (m_pFile) {
		// Restart
		m_pFile->Seek(sizeof(Header));
		m_nStreamPos = 0;

	} else if (m_pnData) {
		// Restart
		if (!GetBuffer().GetData())
			return false; // Error!
		m_pnData  = GetBuffer().GetData();
		m_pnData += sizeof(Header);
		m_nStreamPos = 0;

	} else {
		// Check buffer
		if (!GetBuffer().IsStreamed())
			return false; // Error!

		// Prepare for streaming
		Header sWavHeader;
		if (GetBuffer().GetData()) { // Stream from memory
			// Set data stream
			m_pnData = GetBuffer().GetData();

			// Set stream size
			m_nStreamSize = GetBuffer().GetDataSize() - sizeof(Header);

			// Read wav header
			MemoryManager::Copy(&sWavHeader, m_pnData, sizeof(Header));
			m_pnData += sizeof(Header);
		} else { // Stream from file
			m_pFile = GetBuffer().OpenFile();
			if (!m_pFile) {
				// Error!
				return false;
			}

			// Set stream size
			m_nStreamSize = m_pFile->GetSize() - sizeof(Header);

			// Read wav header
			m_pFile->Read(&sWavHeader, sizeof(Header), 1);
		}

		// Set stream position
		m_nStreamPos = 0;

		// Check the number of channels... and bits per sample
		if (sWavHeader.Channels == 1)
			m_nFormat = (sWavHeader.BitsPerSample == 16) ? AL_FORMAT_MONO16   : AL_FORMAT_MONO8;
		else
			m_nFormat = (sWavHeader.BitsPerSample == 16) ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8;

		// Get frequency
		m_nFrequency = sWavHeader.SamplesPerSec;

		// Setup the swap buffer, we want a buffer size that can hold 2 seconds
		if (m_nSwapSize != sWavHeader.BytesPerSec*2) {
			m_nSwapSize = sWavHeader.BytesPerSec*2;
			if (m_pnSwap)
				delete [] m_pnSwap;
			m_pnSwap = new uint8[m_nSwapSize];
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Fill a buffer
*/
bool StreamWav::FillBuffer(ALuint nBuffer)
{
	if (m_pnSwap && m_nSwapSize) {
		bool bEndOfStream = false;
		int nSizeToRead;

		// Stream finished?
		if (m_nStreamPos+m_nSwapSize >= m_nStreamSize) {
			nSizeToRead  = m_nStreamSize - m_nStreamPos;
			m_nStreamPos = m_nStreamSize;
			bEndOfStream = true;
		} else {
			nSizeToRead   = m_nSwapSize;
			m_nStreamPos += m_nSwapSize;
		}
		if (!nSizeToRead)
			return false; // Error!

		// Read data
		if (m_pFile)
			m_pFile->Read(m_pnSwap, nSizeToRead, 1);
		if (m_pnData) {
			MemoryManager::Copy(m_pnSwap, m_pnData, nSizeToRead);
			m_pnData += nSizeToRead;
		}

		// Fill buffer
		alBufferData(nBuffer, m_nFormat, m_pnSwap, nSizeToRead, m_nFrequency);

		// Done
		return !bEndOfStream;
	} else {
		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Public virtual Stream functions                       ]
//[-------------------------------------------------------]
bool StreamWav::Init()
{
	// First, de-initialize old stream
	DeInit();

	// Open stream
	if (OpenStream()) {
		// Start streaming
		if (FillBuffer(m_nBuffers[0]) && FillBuffer(m_nBuffers[1])) {
			alSourceQueueBuffers(GetSource(), 2, m_nBuffers);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool StreamWav::IsInitialized() const
{
	return (m_pFile || m_pnData);
}

void StreamWav::DeInit()
{
	// Is the stream initialized?
	if (m_pFile || m_pnData) {
		// Unqeue buffers
		int nQueued;
		alGetSourcei(GetSource(), AL_BUFFERS_QUEUED, &nQueued);
		while (nQueued--) {
			ALuint nBuffer;
			alSourceUnqueueBuffers(GetSource(), 1, &nBuffer);
		}

		// Close streaming source
		if (m_pFile) {
			m_pFile->Close();
			delete m_pFile;
			m_pFile = NULL;
		}
		m_pnData	  = NULL; // Don't delete the shared data!
		m_nStreamSize = 0;
		m_nStreamPos  = 0;
		m_nFormat	  = 0;
		m_nFrequency  = 0;
	}
	if (m_pnSwap) {
		delete [] m_pnSwap;
		m_nSwapSize = 0;
		m_pnSwap    = NULL;
	}
}

bool StreamWav::Update()
{
	int nActive = 0;

	// Currently initialized?
	if (IsInitialized()) {
		// Update all processed buffers
		int nProcessed = 0;
		alGetSourcei(GetSource(), AL_BUFFERS_PROCESSED, &nProcessed);
		while (nProcessed--) {
			ALuint nBuffer;
			alSourceUnqueueBuffers(GetSource(), 1, &nBuffer);
			if (!FillBuffer(nBuffer)) {
				bool bKeepPlaying = false;

				// This stream is now finished, perform looping?
				if (IsLooping()) {
					// Open stream
					if (OpenStream()) {
						// Fill buffer
						if (FillBuffer(nBuffer)) {
							// This stream is now active, again
							nActive++;
							alSourceQueueBuffers(GetSource(), 1, &nBuffer);
							bKeepPlaying = true;
						}
					}
				}

				// Deinitialize the stream right now?
				if (!bKeepPlaying)
					DeInit();
			} else {
				// This stream is still in processing
				nActive++;
				alSourceQueueBuffers(GetSource(), 1, &nBuffer);
			}
		}

		// Get source state and start the source if necessary (the buffers may have been completely emptied and the source therefore been stopped)
		if (nActive != 0) {
			ALint nValue = AL_STOPPED;
			alGetSourcei(GetSource(), AL_SOURCE_STATE, &nValue);
			if (nValue == AL_STOPPED)
				alSourcePlay(GetSource());
		}
	}

	// Return active state
	return (nActive != 0);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLSoundOpenAL
