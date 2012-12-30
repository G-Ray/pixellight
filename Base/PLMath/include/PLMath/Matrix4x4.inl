/*********************************************************\
 *  File: Matrix4x4.inl                                  *
 *      Matrix4x4 inline implementation
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Constructor                                           ]
//[-------------------------------------------------------]
inline Matrix4x4::Matrix4x4() :
	// Set identity - and respect the correct initialization order!
	xx(1.0f), yx(0.0f), zx(0.0f), wx(0.0f),
	xy(0.0f), yy(1.0f), zy(0.0f), wy(0.0f),
	xz(0.0f), yz(0.0f), zz(1.0f), wz(0.0f),
	xw(0.0f), yw(0.0f), zw(0.0f), ww(1.0f)
{
}

inline Matrix4x4::Matrix4x4(const float fS[])
{
	PLCore::MemoryManager::Copy(this->fM, fS, sizeof(float)*16);
}

inline Matrix4x4::Matrix4x4(const Matrix4x4 &mM)
{
	PLCore::MemoryManager::Copy(this->fM, mM.fM, sizeof(float)*16);
}

inline Matrix4x4::Matrix4x4(float fXX, float fXY, float fXZ, float fXW,
							float fYX, float fYY, float fYZ, float fYW,
							float fZX, float fZY, float fZZ, float fZW,
							float fWX, float fWY, float fWZ, float fWW) :
	// Copy over the values - and respect the correct initialization order!
	xx(fXX), yx(fYX), zx(fZX), wx(fWX),
	xy(fXY), yy(fYY), zy(fZY), wy(fWY),
	xz(fXZ), yz(fYZ), zz(fZZ), wz(fWZ),
	xw(fXW), yw(fYW), zw(fZW), ww(fWW)
{
}


//[-------------------------------------------------------]
//[ Destructor                                            ]
//[-------------------------------------------------------]
inline Matrix4x4::~Matrix4x4()
{
}


//[-------------------------------------------------------]
//[ Operators                                             ]
//[-------------------------------------------------------]
inline Matrix4x4 &Matrix4x4::operator =(const float fS[])
{
	PLCore::MemoryManager::Copy(this->fM, fS, sizeof(float)*16);
	return *this;
}

inline Matrix4x4 &Matrix4x4::operator =(const Matrix4x4 &mM)
{
	PLCore::MemoryManager::Copy(this->fM, mM.fM, sizeof(float)*16);
	return *this;
}

inline Matrix4x4 Matrix4x4::operator +(const Matrix4x4 &mM) const
{
	return Matrix4x4(xx+mM.xx, xy+mM.xy, xz+mM.xz, xw+mM.xw,
					 yx+mM.yx, yy+mM.yy, yz+mM.yz, yw+mM.yw,
					 zx+mM.zx, zy+mM.zy, zz+mM.zz, zw+mM.zw,
					 wx+mM.wx, wy+mM.wy, wz+mM.wz, ww+mM.ww);
}

inline void Matrix4x4::operator +=(const Matrix4x4 &mM)
{
	xx += mM.xx; xy += mM.xy; xz += mM.xz; xw += mM.xw;
	yx += mM.yx; yy += mM.yy; yz += mM.yz; yw += mM.yw;
	zx += mM.zx; zy += mM.zy; zz += mM.zz; zw += mM.zw;
	wx += mM.wx; wy += mM.wy; wz += mM.wz; ww += mM.ww;
}

inline Matrix4x4 Matrix4x4::operator -() const
{
	return GetInverted();
}

inline Matrix4x4 Matrix4x4::operator -(const Matrix4x4 &mM) const
{
	return Matrix4x4(xx-mM.xx, xy-mM.xy, xz-mM.xz, xw-mM.xw,
					 yx-mM.yx, yy-mM.yy, yz-mM.yz, yw-mM.yw,
					 zx-mM.zx, zy-mM.zy, zz-mM.zz, zw-mM.zw,
					 wx-mM.wx, wy-mM.wy, wz-mM.wz, ww-mM.ww);
}

inline void Matrix4x4::operator -=(const Matrix4x4 &mM)
{
	xx -= mM.xx; xy -= mM.xy; xz -= mM.xz; xw -= mM.xw;
	yx -= mM.yx; yy -= mM.yy; yz -= mM.yz; yw -= mM.yw;
	zx -= mM.zx; zy -= mM.zy; zz -= mM.zz; zw -= mM.zw;
	wx -= mM.wx; wy -= mM.wy; wz -= mM.wz; ww -= mM.ww;
}

inline Matrix4x4 Matrix4x4::operator *(float fS) const
{
	return Matrix4x4(xx*fS, xy*fS, xz*fS, xw*fS,
					 yx*fS, yy*fS, yz*fS, yw*fS,
					 zx*fS, zy*fS, zz*fS, zw*fS,
					 wx*fS, wy*fS, wz*fS, ww*fS);
}

inline void Matrix4x4::operator *=(float fS)
{
	xx *= fS; xy *= fS; xz *= fS; xw *= fS;
	yx *= fS; yy *= fS; yz *= fS; yw *= fS;
	zx *= fS; zy *= fS; zz *= fS; zw *= fS;
	wx *= fS; wy *= fS; wz *= fS; ww *= fS;
}

inline Vector2 Matrix4x4::operator *(const Vector2 &vV) const
{
	const float x = vV.x, y = vV.y;
	const float fInvW = 1.0f/(wx*x + wy*y + ww);
	return Vector2((xx*x + xy*y + xw)*fInvW,
				   (yx*x + yy*y + yw)*fInvW);
}

inline Vector3 Matrix4x4::operator *(const Vector3 &vV) const
{
	const float x = vV.x, y = vV.y, z = vV.z;
	const float fInvW = 1.0f/(wx*x + wy*y + wz*z + ww);
	return Vector3((xx*x + xy*y + xz*z + xw)*fInvW,
				   (yx*x + yy*y + yz*z + yw)*fInvW,
				   (zx*x + zy*y + zz*z + zw)*fInvW);
}

inline Vector4 Matrix4x4::operator *(const Vector4 &vV) const
{
	const float x = vV.x, y = vV.y, z = vV.z, w = vV.w;
	return Vector4(xx*x + xy*y + xz*z + xw*w,
				   yx*x + yy*y + yz*z + yw*w,
				   zx*x + zy*y + zz*z + zw*w,
				   wx*x + wy*y + wz*z + ww*w);
}

inline void Matrix4x4::operator *=(const Matrix3x4 &mM)
{
	*this = *this * mM;
}

inline void Matrix4x4::operator *=(const Matrix4x4 &mM)
{
	*this = *this * mM;
}

inline Matrix4x4 Matrix4x4::operator /(float fS) const
{
	return Matrix4x4(xx/fS, xy/fS, xz/fS, xw/fS,
					 yx/fS, yy/fS, yz/fS, yw/fS,
					 zx/fS, zy/fS, zz/fS, zw/fS,
					 wx/fS, wy/fS, wz/fS, ww/fS);
}

inline void Matrix4x4::operator /=(float fS)
{
	xx /= fS; xy /= fS; xz /= fS; xw /= fS;
	yx /= fS; yy /= fS; yz /= fS; yw /= fS;
	zx /= fS; zy /= fS; zz /= fS; zw /= fS;
	wx /= fS; wy /= fS; wz /= fS; ww /= fS;
}

inline float Matrix4x4::operator [](int nIndex) const
{
	return fM[nIndex];
}

inline float &Matrix4x4::operator [](int nIndex)
{
	return fM[nIndex];
}

inline float Matrix4x4::operator () (PLCore::uint32 nRow, PLCore::uint32 nColumn) const
{
	return fM[nRow+4*nColumn];
}

inline float &Matrix4x4::operator () (PLCore::uint32 nRow, PLCore::uint32 nColumn)
{
	return fM[nRow+4*nColumn];
}

inline Matrix4x4::operator float *()
{
	return this->fM;
}

inline Matrix4x4::operator const float *() const
{
	return this->fM;
}


//[-------------------------------------------------------]
//[ Matrix operations                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns whether or not this matrix is the zero matrix using an epsilon environment
*/
inline bool Matrix4x4::IsZero() const
{
	return (Math::AreEqual(xx, 0.0f) && Math::AreEqual(xy, 0.0f) && Math::AreEqual(xz, 0.0f) && Math::AreEqual(xw, 0.0f) &&
			Math::AreEqual(yx, 0.0f) && Math::AreEqual(yy, 0.0f) && Math::AreEqual(yz, 0.0f) && Math::AreEqual(yw, 0.0f) &&
			Math::AreEqual(zx, 0.0f) && Math::AreEqual(zy, 0.0f) && Math::AreEqual(zz, 0.0f) && Math::AreEqual(zw, 0.0f) &&
			Math::AreEqual(wx, 0.0f) && Math::AreEqual(wy, 0.0f) && Math::AreEqual(wz, 0.0f) && Math::AreEqual(ww, 0.0f));
}

/**
*  @brief
*    Returns whether or not this matrix is truly the zero matrix
*/
inline bool Matrix4x4::IsTrueZero() const
{
	return (PLCore::MemoryManager::Compare(this->fM, Zero.fM, sizeof(float)*16) == 0);
}

/**
*  @brief
*    Sets a zero matrix
*/
inline void Matrix4x4::SetZero()
{
	PLCore::MemoryManager::Set(this->fM, 0, sizeof(float)*16);
}

/**
*  @brief
*    Returns whether or not this matrix is the identity matrix using an epsilon environment
*/
inline bool Matrix4x4::IsIdentity() const
{
	return (Math::AreEqual(xx, 1.0f) && Math::AreEqual(xy, 0.0f) && Math::AreEqual(xz, 0.0f) && Math::AreEqual(xw, 0.0f) &&
			Math::AreEqual(yx, 0.0f) && Math::AreEqual(yy, 1.0f) && Math::AreEqual(yz, 0.0f) && Math::AreEqual(yw, 0.0f) &&
			Math::AreEqual(zx, 0.0f) && Math::AreEqual(zy, 0.0f) && Math::AreEqual(zz, 1.0f) && Math::AreEqual(zw, 0.0f) &&
			Math::AreEqual(wx, 0.0f) && Math::AreEqual(wy, 0.0f) && Math::AreEqual(wz, 0.0f) && Math::AreEqual(ww, 1.0f));
}

/**
*  @brief
*    Returns whether or not this matrix is truly the identity matrix
*/
inline bool Matrix4x4::IsTrueIdentity() const
{
	return (PLCore::MemoryManager::Compare(this->fM, Identity.fM, sizeof(float)*16) == 0);
}

/**
*  @brief
*    Sets an identity matrix
*/
inline void Matrix4x4::SetIdentity()
{
	PLCore::MemoryManager::Copy(this->fM, Identity.fM, sizeof(float)*16);
}

/**
*  @brief
*    Returns a requested row
*/
inline Vector4 Matrix4x4::GetRow(PLCore::uint8 nRow) const
{
	switch (nRow) {
		case 0:  return Vector4(  xx,   xy,   xz,   xw);
		case 1:  return Vector4(  yx,   yy,   yz,   yw);
		case 2:  return Vector4(  zx,   zy,   zz,   zw);
		case 3:  return Vector4(  wx,   wy,   wz,   ww);
		default: return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

/**
*  @brief
*    Sets a row
*/
inline void Matrix4x4::SetRow(PLCore::uint8 nRow, const Vector4 &vRow)
{
	switch (nRow) {
		case 0: xx = vRow.x; xy = vRow.y; xz = vRow.z; xw = vRow.w; break;
		case 1: yx = vRow.x; yy = vRow.y; yz = vRow.z; yw = vRow.w; break;
		case 2: zx = vRow.x; zy = vRow.y; zz = vRow.z; zw = vRow.w; break;
		case 3: wx = vRow.x; wy = vRow.y; wz = vRow.z; ww = vRow.w; break;
	}
}

/**
*  @brief
*    Returns a requested column
*/
inline Vector4 Matrix4x4::GetColumn(PLCore::uint8 nColumn) const
{
	switch (nColumn) {
		case 0:  return Vector4(  xx,   yx,   zx,   wx);
		case 1:  return Vector4(  xy,   yy,   zy,   wy);
		case 2:  return Vector4(  xz,   yz,   zz,   wz);
		case 3:  return Vector4(  xw,   yw,   zw,   ww);
		default: return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

/**
*  @brief
*    Sets a column
*/
inline void Matrix4x4::SetColumn(PLCore::uint8 nColumn, const Vector4 &vColumn)
{
	switch (nColumn) {
		case 0: xx = vColumn.x; yx = vColumn.y; zx = vColumn.z; wx = vColumn.w; break;
		case 1: xy = vColumn.x; yy = vColumn.y; zy = vColumn.z; wy = vColumn.w; break;
		case 2: xz = vColumn.x; yz = vColumn.y; zz = vColumn.z; wz = vColumn.w; break;
		case 3: xw = vColumn.x; yw = vColumn.y; zw = vColumn.z; ww = vColumn.w; break;
	}
}

/**
*  @brief
*    Returns true if this matrix is symmetric
*/
inline bool Matrix4x4::IsSymmetric() const
{
	return (Math::AreEqual(xy, yx) && Math::AreEqual(xz, zx) && Math::AreEqual(xw, wx) &&
			Math::AreEqual(yz, zy) && Math::AreEqual(yw, wy) && Math::AreEqual(zw, wz));
}

/**
*  @brief
*    Returns true if this matrix is orthogonal
*/
inline bool Matrix4x4::IsOrthogonal() const
{
	return (GetTransposed() == GetInverted());
}

/**
*  @brief
*    Returns true if this matrix is a rotation matrix
*/
inline bool Matrix4x4::IsRotationMatrix() const
{
	return (GetTransposed() == GetInverted() && Math::AreEqual(GetDeterminant(), 1.0f));
}

/**
*  @brief
*    Returns true if this matrix is a rotation and translation matrix
*/
inline bool Matrix4x4::IsRotationTranslationMatrix() const
{
	return (IsRotationMatrix() && !Math::AreEqual(xw, 0.0f) && !Math::AreEqual(yw, 0.0f) && !Math::AreEqual(zw, 0.0f));
}

/**
*  @brief
*    Returns the trace of the matrix
*/
inline float Matrix4x4::GetTrace() const
{
	return xx+yy+zz+ww;
}


//[-------------------------------------------------------]
//[ Scale                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a scale matrix
*/
inline void Matrix4x4::SetScaleMatrix(float fX, float fY, float fZ)
{
	xx =   fX; xy = 0.0f; xz = 0.0f; xw = 0.0f;
	yx = 0.0f; yy =   fY; yz = 0.0f; yw = 0.0f;
	zx = 0.0f; zy = 0.0f; zz =   fZ; zw = 0.0f;
	wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

inline void Matrix4x4::SetScaleMatrix(const Vector3 &vV)
{
	xx = vV.x; xy = 0.0f; xz = 0.0f; xw = 0.0f;
	yx = 0.0f; yy = vV.y; yz = 0.0f; yw = 0.0f;
	zx = 0.0f; zy = 0.0f; zz = vV.z; zw = 0.0f;
	wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

inline Vector3 Matrix4x4::GetScale() const
{
	Vector3 vScale;
	GetScale(vScale.x, vScale.y, vScale.z);
	return vScale;
}

inline void Matrix4x4::GetScale(float fV[]) const
{
	GetScale(fV[0], fV[1], fV[2]);
}


//[-------------------------------------------------------]
//[ Translation                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a translation matrix
*/
inline void Matrix4x4::SetTranslationMatrix(float fX, float fY, float fZ)
{
	xx = 1.0f; xy = 0.0f; xz = 0.0f; xw =   fX;
	yx = 0.0f; yy = 1.0f; yz = 0.0f; yw =   fY;
	zx = 0.0f; zy = 0.0f; zz = 1.0f; zw =   fZ;
	wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

inline void Matrix4x4::SetTranslationMatrix(const Vector3 &vV)
{
	xx = 1.0f; xy = 0.0f; xz = 0.0f; xw = vV.x;
	yx = 0.0f; yy = 1.0f; yz = 0.0f; yw = vV.y;
	zx = 0.0f; zy = 0.0f; zz = 1.0f; zw = vV.z;
	wx = 0.0f; wy = 0.0f; wz = 0.0f; ww = 1.0f;
}

inline void Matrix4x4::GetTranslation(float &fX, float &fY, float &fZ) const
{
	fX = xw;
	fY = yw;
	fZ = zw;
}

inline Vector3 Matrix4x4::GetTranslation() const
{
	return Vector3(xw, yw, zw);
}

inline void Matrix4x4::GetTranslation(float fV[]) const
{
	fV[0] = xw;
	fV[1] = yw;
	fV[2] = zw;
}

inline void Matrix4x4::SetTranslation(float fX, float fY, float fZ)
{
	xw = fX;
	yw = fY;
	zw = fZ;
}

inline void Matrix4x4::SetTranslation(const Vector3 &vV)
{
	SetTranslation(vV.x, vV.y, vV.z);
}

inline void Matrix4x4::SetTranslation(const float fV[])
{
	SetTranslation(fV[0], fV[1], fV[2]);
}


//[-------------------------------------------------------]
//[ Rotation                                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the x (left) axis
*/
inline Vector3 Matrix4x4::GetXAxis() const
{
	return Vector3(xx, yx, zx);
}

/**
*  @brief
*    Returns the y (up) axis
*/
inline Vector3 Matrix4x4::GetYAxis() const
{
	return Vector3(xy, yy, zy);
}

/**
*  @brief
*    Returns the z (forward) axis
*/
inline Vector3 Matrix4x4::GetZAxis() const
{
	return Vector3(xz, yz, zz);
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets a shearing matrix
*/
inline Matrix4x4 &Matrix4x4::SetShearing(float fShearXY, float fShearXZ, float fShearYX, float fShearYZ,
										 float fShearZX, float fShearZY)
{
	xx = 1.0f;     xy = fShearYX; xz = fShearZX; xw = 0.0f;
	yx = fShearXY; yy = 1.0f;     yz = fShearZY; yw = 0.0f;
	zx = fShearXZ; zy = fShearYZ; zz = 1.0f;     zw = 0.0f;
	wx = 0.0f;     wy = 0.0f;     wz = 0.0f;     ww = 1.0f;
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
