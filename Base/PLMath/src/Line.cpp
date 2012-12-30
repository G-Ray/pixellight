/*********************************************************\
 *  File: Line.cpp                                       *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/Matrix3x3.h"
#include "PLMath/Matrix3x4.h"
#include "PLMath/Matrix4x4.h"
#include "PLMath/Line.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Transformation                                        ]
//[-------------------------------------------------------]
Line Line::operator *(const Matrix3x3 &mRot) const
{
	Line cLine;
	cLine.vStart = mRot*vStart;
	cLine.vEnd   = mRot*vEnd;
	return cLine;
}

Line Line::operator *(const Matrix3x4 &mTrans) const
{
	Line cLine;
	cLine.vStart = mTrans*vStart;
	cLine.vEnd   = mTrans*vEnd;
	return cLine;
}

Line Line::operator *(const Matrix4x4 &mTrans) const
{
	Line cLine;
	cLine.vStart = mTrans*vStart;
	cLine.vEnd   = mTrans*vEnd;
	return cLine;
}

Line &Line::operator *=(const Matrix3x3 &mRot)
{
	vStart *= mRot;
	vEnd   *= mRot;
	return *this;
}

Line &Line::operator *=(const Matrix3x4 &mTrans)
{
	vStart *= mTrans;
	vEnd   *= mTrans;
	return *this;
}

Line &Line::operator *=(const Matrix4x4 &mTrans)
{
	vStart *= mTrans;
	vEnd   *= mTrans;
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
