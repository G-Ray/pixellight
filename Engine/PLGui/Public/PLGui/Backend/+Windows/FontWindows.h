/*********************************************************\
 *  File: FontWindows.h                                  *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#ifndef __PLGUI_FONTWINDOWS_H__
#define __PLGUI_FONTWINDOWS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#include "PLGui/Backend/FontImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Windows font implementation
*/
class FontWindows : public FontImpl {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFont
		*    Reference to platform independent font
		*/
		PLGUI_API FontWindows(Font &cFont);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~FontWindows();

		/**
		*  @brief
		*    Returns the font handle
		*
		*  @return
		*    Font handle, can be a null pointer
		*/
		PLGUI_API HFONT GetHandle() const;


	//[-------------------------------------------------------]
	//[ Public virtual FontImpl functions                     ]
	//[-------------------------------------------------------]
	public:
		virtual bool LoadFont(const PLCore::String &sFamily, PLCore::uint32 nHeight, PLCore::uint32 nWeight, EFontStyle nStyle) override;
		virtual bool LoadFont(const PLCore::String &sFilename, PLCore::uint32 nHeight) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HFONT m_hFont;	/**< Font handle, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_FONTWINDOWS_H__
