/*********************************************************\
 *  File: GraphicsImpl.h                                 *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
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


#ifndef __PLGUI_GRAPHICSIMPL_H__
#define __PLGUI_GRAPHICSIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include <PLMath/Vector2i.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class String;
}
namespace PLMath {
	class Vector2i;
}
namespace PLGraphics {
	class Color4;
}
namespace PLGui {
	class Graphics;
	class Hint;
	class Image;
	class Font;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Graphics implementation base class
*
*  @remarks
*    This class is the base class for platform specific graphics implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Graphics' abstraction
*/
class GraphicsImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Graphics;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @remarks
		*    Create a graphics implementation without a reference to the owner GUI.
		*    This is necessary to create graphics implementations directly, the GUI
		*    reference should be set immediately after creating the instance!
		*/
		PLGUI_API GraphicsImpl();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGraphics
		*    Reference to platform independent graphics
		*/
		PLGUI_API GraphicsImpl(Graphics &cGraphics);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~GraphicsImpl();


	//[-------------------------------------------------------]
	//[ Public virtual GraphicsImpl functions                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Draws a line
		*
		*  @param[in] cColor
		*    Color of the line
		*  @param[in] vPos1
		*    Start position
		*  @param[in] vPos2
		*    End position
		*  @param[in] nWidth
		*    Line width
		*/
		virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nWidth = 1) = 0;

		/**
		*  @brief
		*    Draws a rectangle
		*
		*  @param[in] cColor
		*    Color of the line
		*  @param[in] vPos1
		*    First corner
		*  @param[in] vPos2
		*    Second corner
		*  @param[in] nRoundX
		*    Rounded border in X direction
		*  @param[in] nRoundY
		*    Rounded border in Y direction
		*  @param[in] nWidth
		*    Line width
		*/
		virtual void DrawRect(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0, PLCore::uint32 nWidth = 1) = 0;

		/**
		*  @brief
		*    Draws a filled box
		*
		*  @param[in] cColor
		*    Color of the box
		*  @param[in] vPos1
		*    First corner
		*  @param[in] vPos2
		*    Second corner
		*  @param[in] nRoundX
		*    Rounded border in X direction
		*  @param[in] nRoundY
		*    Rounded border in Y direction
		*/
		virtual void DrawBox(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0) = 0;

		/**
		*  @brief
		*    Draws a filled box with a color gradient
		*
		*  @param[in] cColor1
		*    First color
		*  @param[in] cColor2
		*    Second color
		*  @param[in] fAngle
		*    Clockwise angle of the gradient color (in degrees). 0� means from left to right.
		*  @param[in] vPos1
		*    First corner
		*  @param[in] vPos2
		*    Second corner
		*/
		virtual void DrawGradientBox(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2) = 0;

		/**
		*  @brief
		*    Draws an image
		*
		*  @param[in] cImage
		*    Image to draw
		*  @param[in] vPos
		*    Image position
		*  @param[in] vSize
		*    Image size
		*
		*  @note
		*    - If vSize = (0 0), the image size is used
		*/
		virtual void DrawImage(const Image &cImage, const PLMath::Vector2i &vPos, const PLMath::Vector2i &vSize) = 0;

		/**
		*  @brief
		*    Draws a text
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] cTextColor
		*    Text color
		*  @param[in] cBkColor
		*    Background color
		*  @param[in] vPosition
		*    Text position
		*  @param[in] sText
		*    Text
		*/
		virtual void DrawText(const Font &cFont, const PLGraphics::Color4 &cTextColor, const PLGraphics::Color4 &cBkColor, const PLMath::Vector2i &vPos, const PLCore::String &sText) = 0;

		/**
		*  @brief
		*    Gets the width of a given text in the current font
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] sText
		*    Text to get the width of
		*
		*  @return
		*    The width of the given text using the given font
		*/
		virtual PLCore::uint32 GetTextWidth(const Font &cFont, const PLCore::String &sText) = 0;

		/**
		*  @brief
		*    Gets the height of a given text in the current font
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] sText
		*    Text to get the height of
		*
		*  @return
		*    The height of the given text using the given font
		*/
		virtual PLCore::uint32 GetTextHeight(const Font &cFont, const PLCore::String &sText) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Graphics *m_pGraphics;	/**< Pointer to the platform independent graphics object */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GRAPHICSIMPL_H__
