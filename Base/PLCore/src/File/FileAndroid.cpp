/*********************************************************\
 *  File: FileAndroid.cpp                                *
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
#include <stdio.h>	// For e.g. "off_t"
#include <stdint.h>	// For e.g. "size_t"
#include <android/asset_manager.h>
#include "PLCore/System/SystemAndroid.h"
#include "PLCore/File/FileSearchAndroid.h"
#include "PLCore/File/FileAndroid.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FileAndroid::FileAndroid(const Url &cUrl, const FileAccess *pAccess) : FileImpl(cUrl, pAccess),
	m_sFilename(cUrl.GetUnixPath()),
	m_nAccess(0),
	m_pAAsset(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FileAndroid::~FileAndroid()
{
	// Close file
	Close();
}


//[-------------------------------------------------------]
//[ Private virtual FileImpl functions                    ]
//[-------------------------------------------------------]
bool FileAndroid::Exists() const
{
	// Just reuse "IsFile()" and "IsDirectory()"
	return (IsFile() || IsDirectory());
}

bool FileAndroid::IsFile() const
{
	// Get the Android asset manager instance
	AAssetManager *pAAssetManager = SystemAndroid::GetAssetManager();
	if (pAAssetManager) {
		// Open the asset
		AAsset *pAAsset = AAssetManager_open(pAAssetManager, (m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), AASSET_MODE_RANDOM);
		if (pAAsset) {
			// Close the asset
			AAsset_close(pAAsset);

			// Done - it exists
			return true;
		}
	}

	// Done - it's no file
	return false;
}

bool FileAndroid::IsDirectory() const
{
	// Get the Android asset manager instance
	AAssetManager *pAAssetManager = SystemAndroid::GetAssetManager();
	if (pAAssetManager) {
		// Open the asset directory
		AAssetDir *pAAssetDir = AAssetManager_openDir(pAAssetManager, (m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8());
		if (pAAssetDir) {
			// Close the asset directory
			AAssetDir_close(pAAssetDir);

			// Done - it exists
			return true;
		}
	}

	// Done - it's no directory
	return false;
}

bool FileAndroid::CopyTo(const String &sDest, bool bOverwrite) const
{
	// Error - Copy to not supported
	return false;
}

bool FileAndroid::MoveTo(const String &sDest)
{
	// Close file before moving it
	Close();

	// Error - Move to not supported
	return false;
}

bool FileAndroid::Rename(const String &sName)
{
	// Close file before renaming it
	Close();

	// Check parameter: No path is allowed ('/' or '\')
	if (sName.IsSubstring('\\') || sName.IsSubstring('/'))
		return false; // Error!

	// Error - Rename not supported
	return false;
}

bool FileAndroid::CreateNewFile(bool bAlways)
{
	// Close file if open
	Close();

	// Error - Create file not supported
	return false;
}

bool FileAndroid::CreateNewDirectory()
{
	// Close file if open
	Close();

	// Error - Create directory not supported
	return false;
}

bool FileAndroid::Delete()
{
	// Close file before deleting it
	Close();

	// Error - Delete file not supported
	return false;
}

bool FileAndroid::DeleteDirectory()
{
	// Close if open
	Close();

	// Error - Delete directory not supported
	return false;
}

void FileAndroid::Close()
{
	// Check file pointer
	if (m_pAAsset) {
		// Close file
		AAsset_close(m_pAAsset);
		m_pAAsset = nullptr;
	}
}

bool FileAndroid::Open(uint32 nAccess)
{
	// Close file first
	Close();

	// Check the access flags, only binary reading is supported
	// -> Don't check whether or not "File::FileText" is set, just ignore it and hope everything works... doing a check is causing more applications to fail...
	if (!(nAccess & File::FileWrite) && !(nAccess & File::FileAppend) && !(nAccess & File::FileCreate)) {
		// Get the Android asset manager instance
		AAssetManager *pAAssetManager = SystemAndroid::GetAssetManager();
		if (pAAssetManager) {
			// Save access modes
			m_nAccess = nAccess;

			// Open file, ehm, sorry, "asset" *g*
			m_pAAsset = AAssetManager_open(pAAssetManager, (m_sFilename.GetFormat() == String::ASCII) ? m_sFilename.GetASCII() : m_sFilename.GetUTF8(), AASSET_MODE_RANDOM);
		}
	}

	// Done
	return (m_pAAsset != nullptr);
}

bool FileAndroid::IsOpen() const
{
	// Check whether the file is open
	return (m_pAAsset != nullptr);
}

bool FileAndroid::IsReadable() const
{
	// Check whether the file is readable
	return (m_pAAsset && (m_nAccess & File::FileRead));
}

bool FileAndroid::IsWritable() const
{
	// Check whether the file is writable
	return (m_pAAsset && (m_nAccess & File::FileWrite));
}

bool FileAndroid::IsEof() const
{
	// Check end of file
	return (!m_pAAsset || !AAsset_getRemainingLength(m_pAAsset));
}

int FileAndroid::GetC()
{
	// Error - Read character not supported
	return -1;
}

bool FileAndroid::PutC(int nChar)
{
	// Error - Write character not supported
	return false;
}

String FileAndroid::GetS()
{
	// Error - Read string not supported
	return "";
}

int FileAndroid::PutS(const String &sString)
{
	// Error - Write string not supported
	return -1;
}

uint32 FileAndroid::Read(void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Is the file readable?
	if (IsReadable()) {
		// Read from file
		const int nNumOfReadBytes = AAsset_read(m_pAAsset, pBuffer, nSize*nCount);
		if (nNumOfReadBytes > 0) {
			// Success
			// "AAsset_read()" returns the number of bytes read, zero on EOF, or < 0 on error
			// "FileAndroid::Read()" returns the number of fully read items, if != 'nCount' an error occurred
			// Return the number of fully read items
			return nNumOfReadBytes/nSize;
		}
	}

	// Error!
	return 0;
}

uint32 FileAndroid::Write(const void *pBuffer, uint32 nSize, uint32 nCount)
{
	// Error - Write buffer not supported
	return 0;
}

bool FileAndroid::Flush()
{
	// Error - Flush buffer not supported
	return false;
}

bool FileAndroid::Seek(int32 nOffset, File::ESeek nLocation)
{
	// Check file pointer
	if (m_pAAsset) {
		// Convert seek position to system value
		int nSeek;
		switch (nLocation) {
			case File::SeekCurrent:
				nSeek = SEEK_CUR;
				break;

			case File::SeekEnd:
				nSeek = SEEK_END;
				break;

			case File::SeekSet:
				nSeek = SEEK_SET;
				break;

			default:
				return false; // Error!
		}

		// Seek file ("AAsset_seek()" returns -1 on error)
		return (AAsset_seek(m_pAAsset, nOffset, nSeek) != -1);
	} else {
		// Error!
		return false;
	}
}

int32 FileAndroid::Tell() const
{
	// Get file position
	return m_pAAsset ? (AAsset_getLength(m_pAAsset) - AAsset_getRemainingLength(m_pAAsset)) : -1;
}

uint32 FileAndroid::GetSize() const
{
	// Check file pointer
	if (m_pAAsset) {
		// Get file size
		return AAsset_getLength(m_pAAsset);
	} else {
		// Error!
		return 0;
	}
}

FileSearchImpl *FileAndroid::CreateSearch()
{
	// Create a file searcher
	return new FileSearchAndroid(m_cUrl.GetUnixPath(), m_pAccess);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
