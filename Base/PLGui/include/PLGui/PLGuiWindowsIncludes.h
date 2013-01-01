/*********************************************************\
 *  File: PLGuiWindowsIncludes.h                         *
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


#ifndef __PLGUI_WINDOWSINCLUDES_H__
#define __PLGUI_WINDOWSINCLUDES_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
// Set windows version to Windows XP
#ifdef WINVER
	#undef WINVER
#endif
#define WINVER			0x0501
#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT	0x0501

// Exclude some stuff from windows.h to speed up compilation a bit
//#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOMENUS
//#define NOICONS
#define NOKEYSTATES
//#define NOSYSCOMMANDS
//#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOMEMMGR
//#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

// Include headers
#include <windows.h>

// We undef these to avoid name conflicts
#undef DrawText
#undef LoadImage
#undef MessageBox
#undef GetClassName
#undef CreateDirectory
#undef SetCurrentDirectory
#undef GetCurrentDirectory
#undef GetEnvironmentVariable
#undef SetEnvironmentVariable
#undef GetComputerName
#undef GetUserName
#undef CreateFont
#undef Yield
#undef SendMessage
#undef PostMessage


#endif // __PLGUI_WINDOWSINCLUDES_H__
