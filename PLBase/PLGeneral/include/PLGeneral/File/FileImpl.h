/*********************************************************\
 *  File: FileImpl.h                                     *
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


#ifndef __PLGENERAL_FILE_IMPL_H__
#define __PLGENERAL_FILE_IMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGeneral/File/File.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGeneral {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class FileAccess;
class FileSearchImpl;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract base class for platform specific file object implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'FileObject' abstraction
*/
class FileImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FileObject;
	friend class File;
	friend class Directory;
	friend class FileSearch;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cUrl
		*    URL of the file or directory
		*  @param[in] pAccess
		*    Additional file access information (can be a null pointer)
		*/
		PLGENERAL_API FileImpl(const Url &cUrl, const FileAccess *pAccess);

		/**
		*  @brief
		*    Destructor
		*/
		PLGENERAL_API virtual ~FileImpl();


	//[-------------------------------------------------------]
	//[ Protected virtual FileImpl functions                  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Check if the file or directory exists
		*
		*  @return
		*    'true', if the object is existing, else 'false'
		*/
		virtual bool Exists() const = 0;

		/**
		*  @brief
		*    Check if the object is a regular file
		*
		*  @return
		*    'true', if the object is a file
		*/
		virtual bool IsFile() const = 0;

		/**
		*  @brief
		*    Check if the object is a directory
		*
		*  @return
		*    'true', if the object is a directory
		*/
		virtual bool IsDirectory() const = 0;

		/**
		*  @brief
		*    Copy the file or directory to a new location
		*
		*  @param[in] sDest
		*    URL of the destination
		*  @param[in] bOverwrite
		*    Shall the file be overwritten if it already exists?
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool CopyTo(const String &sDest, bool bOverwrite) const = 0;

		/**
		*  @brief
		*    Move the file or directory to a new location
		*
		*  @param[in] sDest
		*    URL of the destination
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool MoveTo(const String &sDest) = 0;

		/**
		*  @brief
		*    Rename the file or directory
		*
		*  @param[in] sName
		*    New filename (filename only, no path)
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool Rename(const String &sName) = 0;

		/**
		*  @brief
		*    Create a file
		*
		*  @param[in] bAlways
		*    If 'true', the file is created (and therefore overwritten) if it already exists
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool CreateNewFile(bool bAlways) = 0;

		/**
		*  @brief
		*    Create a directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool CreateNewDirectory() = 0;

		/**
		*  @brief
		*    Delete a file
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool Delete() = 0;

		/**
		*  @brief
		*    Delete a directory
		*
		*  @return
		*    'true', if all went fine, else 'false'
		*/
		virtual bool DeleteDirectory() = 0;

		/**
		*  @brief
		*    Close the file or firectory
		*/
		virtual void Close() = 0;

		/**
		*  @brief
		*    Open the file for reading and/or writing
		*
		*  @param[in] nAccess
		*    Access mode (see EAccess)
		*
		*  @return
		*    'false' on error, else 'true'
		*/
		virtual bool Open(uint32 nAccess) = 0;

		/**
		*  @brief
		*    Returns whether the file is open
		*
		*  @return
		*    'true', if the file is open, else 'false'
		*/
		virtual bool IsOpen() const = 0;

		/**
		*  @brief
		*    Returns whether the file is readable
		*
		*  @return
		*    'true', if the file can be read, else 'false'
		*/
		virtual bool IsReadable() const = 0;

		/**
		*  @brief
		*    Returns whether the file is writable
		*
		*  @return
		*    'true', if the file can be written, else 'false'
		*/
		virtual bool IsWritable() const = 0;

		/**
		*  @brief
		*    Returns whether end of file has been reached
		*
		*  @return
		*    'true', if the end of the file has been reached, else 'false'
		*/
		virtual bool IsEof() const = 0;

		/**
		*  @brief
		*    Reads a character
		*
		*  @return
		*    A character from file, < 0 if there was an error
		*/
		virtual int GetC() = 0;

		/**
		*  @brief
		*    Writes a character
		*
		*  @param[in] nChar
		*    Character to write
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe file is read only)
		*/
		virtual bool PutC(int nChar) = 0;

		/**
		*  @brief
		*    Reads a string
		*
		*  @return
		*    Read string
		*/
		virtual String GetS() = 0;

		/**
		*  @brief
		*    Writes a string
		*
		*  @param[in] sString
		*    String which should be written into the file
		*
		*  @return
		*    Number of bytes written, 0 if nothing was written, < 0 if there was an error
		*/
		virtual int PutS(const String &sString) = 0;

		/**
		*  @brief
		*    Reads the given number of bytes
		*
		*  @param[out] pBuffer
		*    Buffer were the data should be copied in (MUST valid and large enough!)
		*  @param[in]  nSize
		*    Item size in bytes
		*  @param[in]  nCount
		*    Maximum number of items to be read
		*
		*  @return
		*    Number of fully read items, if != 'nCount' an error occured
		*/
		virtual uint32 Read(void *pBuffer, uint32 nSize, uint32 nCount) = 0;

		/**
		*  @brief
		*    Writes the given number of bytes
		*
		*  @param[in] pBuffer
		*    Buffer with the data which should be written into the file (MUST valid and large enough!)
		*  @param[in] nSize
		*    Item size in bytes
		*  @param[in] nCount
		*    Maximum number of items to be written
		*
		*  @return
		*    Number of fully written items, if != 'nCount' an error occured
		*/
		virtual uint32 Write(const void *pBuffer, uint32 nSize, uint32 nCount) = 0;

		/**
		*  @brief
		*    Flushs the file buffer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Flush() = 0;

		/**
		*  @brief
		*    Sets the starting position
		*
		*  @param[in] nOffset
		*    File offset in bytes relative to the given location
		*  @param[in] nLocation
		*    Location
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Seek(int32 nOffset, File::ESeek nLocation) = 0;

		/**
		*  @brief
		*    Gets the current position of the file pointer
		*
		*  @return
		*    The current byte position of the file pointer, < 0 if there was an error
		*/
		virtual int32 Tell() const = 0;

		/**
		*  @brief
		*    Returns the file size
		*
		*  @return
		*    File size in bytes
		*/
		virtual uint32 GetSize() const = 0;

		/**
		*  @brief
		*    Create a file searcher for the directory
		*
		*  @return
		*    Pointer to a file searcher, or a null pointer on error
		*/
		virtual FileSearchImpl *CreateSearch() = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Url				  m_cUrl;		/**< URL of the file or directory */
		const FileAccess *m_pAccess;	/**< File access information, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGeneral


#endif // __PLGENERAL_FILE_IMPL_H__
