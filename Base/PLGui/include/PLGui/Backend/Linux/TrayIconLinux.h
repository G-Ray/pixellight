/*********************************************************\
 *  File: TrayIconLinux.h                                *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
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


#ifndef __PLGUI_TRAYICONLINUX_H__
#define __PLGUI_TRAYICONLINUX_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include "PLGui/Backend/TrayIconImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Widget;
class TrayIconWidgetLinux;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Tray icon implementation for Linux
*/
class TrayIconLinux : public TrayIconImpl {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GuiLinux;
	friend class TrayIconWidgetLinux;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cTrayIcon
		*    Reference to platform independent tray icon
		*/
		PLGUI_API TrayIconLinux(TrayIcon &cTrayIcon);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~TrayIconLinux();


	//[-------------------------------------------------------]
	//[ Private virtual TrayIconImpl functions                ]
	//[-------------------------------------------------------]
	private:
		PLGUI_API virtual void CreateTrayIcon() override;
		PLGUI_API virtual void DestroyTrayIcon() override;
		PLGUI_API virtual void SetVisible(bool bVisible) override;
		PLGUI_API virtual void Update() override;
		PLGUI_API virtual void ShowNotification(const PLCore::String &sTitle, const PLCore::String &sText) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Display				*m_pDisplay;	/**< X display */
		TrayIconWidgetLinux *m_pWidget;		/**< Tray icon widget */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_TRAYICONLINUX_H__
