/*********************************************************\
 *  File: AndroidMouseDevice.cpp                         *
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
#include <android/input.h>
#include <PLMath/Math.h>
#include "PLInput/Input/Devices/Mouse.h"
#include "PLInput/Backend/Android/AndroidSplitTouchPadDevice.h"
#include "PLInput/Backend/Android/AndroidMouseDevice.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
AndroidMouseDevice::AndroidMouseDevice(AndroidSplitTouchPadDevice *pAndroidSplitTouchPadDevice) :
	m_pAndroidSplitTouchPadDevice(pAndroidSplitTouchPadDevice),
	m_bMouseMoved(false),
	m_fPreviousMousePositionX(0.0f),
	m_fPreviousMousePositionY(0.0f),
	m_fPreviousMousePressure(0.0f),
	m_fMousePositionX(0.0f),
	m_fMousePositionY(0.0f),
	m_fMousePressure(0.0f),
	m_bLeftMouseButton(false)
{
	// Destroy device implementation automatically
	m_bDelete = true;
}

/**
*  @brief
*    Destructor
*/
AndroidMouseDevice::~AndroidMouseDevice()
{
}

/**
*  @brief
*    Call this to process the next motion input event
*/
void AndroidMouseDevice::OnMotionInputEvent(const struct AInputEvent &cAMotionInputEvent)
{
	// Get the number of pointers of data contained in this event
	const size_t nAndroidPointerCount = AMotionEvent_getPointerCount(&cAMotionInputEvent);
	if (nAndroidPointerCount) {
		// Get the current X and Y coordinate of this event for the given pointer index
		m_fMousePositionX = AMotionEvent_getX(&cAMotionInputEvent, 0);
		m_fMousePositionY = AMotionEvent_getY(&cAMotionInputEvent, 0);
		m_fMousePressure  = AMotionEvent_getPressure(&cAMotionInputEvent, 0);

		// Get the combined motion event action code and the action code
		const int32_t nAndroidCombinedAction = AMotionEvent_getAction(&cAMotionInputEvent);
		const int32_t nAndroidAction		 = (nAndroidCombinedAction & AMOTION_EVENT_ACTION_MASK);

		// Touch end?
		if (nAndroidAction == AMOTION_EVENT_ACTION_UP) {
			// Jap, touch end, previous mouse position = current mouse position
			m_fPreviousMousePositionX = m_fMousePositionX;
			m_fPreviousMousePositionY = m_fMousePositionY;
			m_fPreviousMousePressure = m_fMousePressure;

			// Mouse moved during the current touch? If no, this is handled as a left mouse button click as well.
			if (!m_bMouseMoved && !m_bLeftMouseButton) {
				// Check if input device is valid
				if (m_pDevice) {
					// Get mouse device
					Mouse *pMouse = static_cast<Mouse*>(m_pDevice);

					// Update button
					if (pMouse->Left.IsPressed() != true)
						pMouse->Left.SetPressed(true);
				}
			}

			// The left mouse button is now no longer down
			m_bLeftMouseButton = false;
		} else {
			// Touch start?
			if (nAndroidAction == AMOTION_EVENT_ACTION_DOWN) {
				// Jap, touch start, previous mouse position = current mouse position
				m_fPreviousMousePositionX = m_fMousePositionX;
				m_fPreviousMousePositionY = m_fMousePositionY;
				m_fPreviousMousePressure = m_fMousePressure;

				// The mouse was not yet moved
				m_bMouseMoved = false;

				// The left mouse button is not pressed
				m_bLeftMouseButton = false;
			}

			// As long as the mouse was not yet moved, a "left mouse button is hold down" can still be generated
			if (!m_bMouseMoved && !m_bLeftMouseButton) {
				// Get the past time since the touch has been started (in nanoseconds)
				const int64_t nPastTime = AMotionEvent_getEventTime(&cAMotionInputEvent) - AMotionEvent_getDownTime(&cAMotionInputEvent);

				// If the mouse has not been moved for half a second, we go into "left mouse button is hold down"-mode
				if (nPastTime > 500*1000*1000) {
					// The left mouse button is now down
					m_bLeftMouseButton = true;
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual UpdateDevice functions                 ]
//[-------------------------------------------------------]
void AndroidMouseDevice::Update()
{
	// Check if input device is valid
	if (m_pDevice) {
		// Get mouse device
		Mouse *pMouse = static_cast<Mouse*>(m_pDevice);

		// Update relative axes?
		// ->  In case there's an active "AndroidSplitTouchPadDevice"-instance we have to deactivate the
		//     "mouse movement"-emulation or this will conflict with "AndroidSplitTouchPadDevice"
		if (!m_pAndroidSplitTouchPadDevice || !m_pAndroidSplitTouchPadDevice->GetDevice() || !m_pAndroidSplitTouchPadDevice->GetDevice()->GetActive()) {
			// Get the mouse movement
			float fDeltaX = m_fMousePositionX - m_fPreviousMousePositionX;
			float fDeltaY = m_fMousePositionY - m_fPreviousMousePositionY;
			float fDeltaPressure = m_fMousePressure - m_fPreviousMousePressure;

			// Was the mouse already moved? (if so, we're in "mouse move"-mode, not in "left mouse button click"-mode)
			if (!m_bMouseMoved) {
				// Check whether or not the mouse was moved - with a little bit of tollerance
				if ((Math::Abs(fDeltaX) > 6 || Math::Abs(fDeltaY) > 6) && Math::Abs(fDeltaPressure) < 0.4f) {
					m_bMouseMoved = true;
				} else {
					fDeltaX = fDeltaY = 0.0f;
					m_fPreviousMousePositionX = m_fMousePositionX;
					m_fPreviousMousePositionY = m_fMousePositionY;
				}
			}

			// Update axes
			if (pMouse->X.GetValue() != fDeltaX)
				pMouse->X.SetValue(fDeltaX, true);
			if (pMouse->Y.GetValue() != fDeltaY)
				pMouse->Y.SetValue(fDeltaY, true);

			// The current mouse position becomes the previous mouse position
			m_fPreviousMousePositionX = m_fMousePositionX;
			m_fPreviousMousePositionY = m_fMousePositionY;
		}

		// Update buttons
		if (pMouse->Left.IsPressed() != m_bLeftMouseButton)
			pMouse->Left.SetPressed(m_bLeftMouseButton);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
