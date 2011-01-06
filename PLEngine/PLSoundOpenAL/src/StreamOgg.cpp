/*********************************************************\
 *  File: StreamOgg.cpp                                  *
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
#include "PLSoundOpenAL/StreamOgg.h"


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
StreamOgg::StreamOgg(ALuint nSource, const Buffer &cBuffer) : Stream(nSource, cBuffer),
	m_pVorbisInfo(nullptr),
	m_nFormat(0),
	m_nSwapSize(0),
	m_pnSwap(nullptr)
{
	// Create front and back buffers
	alGenBuffers(2, m_nBuffers);
}

/**
*  @brief
*    Destructor
*/
StreamOgg::~StreamOgg()
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
bool StreamOgg::OpenStream()
{
	// Check buffer
	if (GetBuffer().IsStreamed()) {
		// Prepare for streaming
		if (GetBuffer().GetData()) { // Stream from memory
			// Set data pointers
			SoundManager::MemData sMemData;
			sMemData.pnData  = GetBuffer().GetData();
			sMemData.pnDataE = sMemData.pnData + GetBuffer().GetDataSize();

			// Try opening the given file
			ov_callbacks ovc;
			ovc.read_func  = &SoundManager::read_func_mem;
			ovc.seek_func  = &SoundManager::seek_func_mem;
			ovc.close_func = &SoundManager::close_func_mem;
			ovc.tell_func  = &SoundManager::tell_func_mem;
			if (!ov_open_callbacks(&sMemData, &m_cOggStream, nullptr, 0, ovc))
				return true; // Done
		} else { // Stream from file
			File *pFile = GetBuffer().OpenFile();
			if (pFile) {
				// Try opening the given file
				ov_callbacks ovc;
				ovc.read_func  = &SoundManager::read_func;
				ovc.seek_func  = &SoundManager::seek_func;
				ovc.close_func = &SoundManager::close_func;
				ovc.tell_func  = &SoundManager::tell_func;
				if (!ov_open_callbacks(pFile, &m_cOggStream, nullptr, 0, ovc)) {
					// Get information
					m_pVorbisInfo = ov_info(&m_cOggStream, -1);
					m_nFormat     = (m_pVorbisInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

					// Done (the stream takes over the file reference)
					return true;
				}
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Fill a buffer
*/
bool StreamOgg::FillBuffer(ALuint nBuffer)
{
	int nSize = 0;

	// Read data
	while (nSize < (int)m_nSwapSize) {
		int nSection;
		int nResult = ov_read(&m_cOggStream, (char*)&m_pnSwap[nSize], m_nSwapSize - nSize, 0, 2, 1, &nSection);
		if (nResult > 0) nSize += nResult;
		else {
			if (nResult < 0)
				return false; // Error!
			else
				break;
		}
	}
	if (!nSize)
		return false; // Error!

	// Fill buffer
	alBufferData(nBuffer, m_nFormat, m_pnSwap, nSize, m_pVorbisInfo->rate);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual Stream functions                       ]
//[-------------------------------------------------------]
bool StreamOgg::Init()
{
	// First, de-initialize old stream
	DeInit();

	// Open stream
	if (OpenStream() && m_pVorbisInfo) {
		// Get the average bytes per second
		const uint32 nAverageBytesPerSecond = m_pVorbisInfo->rate*m_pVorbisInfo->channels*2;

		// Setup the swap buffer, we want a buffer size that can hold 2 seconds
		if (m_nSwapSize != nAverageBytesPerSecond) {
			m_nSwapSize = nAverageBytesPerSecond;
			if (m_pnSwap)
				delete [] m_pnSwap;
			m_pnSwap = new uint8[m_nSwapSize];
		}

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

bool StreamOgg::IsInitialized() const
{
	return (m_pVorbisInfo != nullptr);
}

void StreamOgg::DeInit()
{
	// Is the stream initialized?
	if (m_pVorbisInfo) {
		// Unqeue buffers
		int nQueued;
		alGetSourcei(GetSource(), AL_BUFFERS_QUEUED, &nQueued);
		while (nQueued--) {
			ALuint nBuffer;
			alSourceUnqueueBuffers(GetSource(), 1, &nBuffer);
		}
		ov_clear(&m_cOggStream);
		m_pVorbisInfo = nullptr;
		m_nFormat	  = 0;
	}
	if (m_pnSwap) {
		delete [] m_pnSwap;
		m_nSwapSize = 0;
		m_pnSwap    = nullptr;
	}
}

bool StreamOgg::Update()
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
			if (FillBuffer(nBuffer)) {
				// This stream is still in processing
				nActive++;
				alSourceQueueBuffers(GetSource(), 1, &nBuffer);
			} else {
				bool bKeepPlaying = false;

				// This stream is now finished, perform looping?
				if (IsLooping()) {
					// Open stream
					ov_clear(&m_cOggStream);
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
