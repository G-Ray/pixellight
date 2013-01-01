/*********************************************************\
 *  File: BufferedReaderString.h                         *
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


#ifndef __PLCORE_BUFFEREDREADER_STRING_H__
#define __PLCORE_BUFFEREDREADER_STRING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/BufferedReader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Buffered reader that reads from a string or character buffer
*/
class BufferedReaderString : public BufferedReader {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sBuffer
		*    String to read from
		*/
		BufferedReaderString(const String &sBuffer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BufferedReaderString();


	//[-------------------------------------------------------]
	//[ Public virtual BufferedReader functions               ]
	//[-------------------------------------------------------]
	public:
		virtual void Close() override;
		virtual bool IsEof() const override;
		virtual String::EFormat GetStringFormat() const override;
		virtual int GetChar() override;
		virtual String GetString(uint32 nSize) override;
		virtual int ReadChar() override;
		virtual String ReadString(uint32 nSize) override;
		virtual bool IsString(const String &sString) override;
		virtual bool IsStringNoCase(const String &sString) override;
		virtual uint32 Tell() const override;
		virtual bool Seek(uint32 nPos) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String m_sBuffer;	/**< String that holds the character buffer */
		uint32 m_nCurrent;	/**< The current character index */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_BUFFEREDREADER_STRING_H__
