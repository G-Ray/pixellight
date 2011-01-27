/*********************************************************\
 *  File: Plane.inl                                      *
 *      Plane inline implementation
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
#include "PLMath/Math.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor setting all components to 0
*/
inline Plane::Plane() :
	a(0.0f),
	b(0.0f),
	c(0.0f),
	d(0.0f)
{
}

/**
*  @brief
*    Constructor
*/
inline Plane::Plane(float fA, float fB, float fC, float fD) :
	a(fA),
	b(fB),
	c(fC),
	d(fD)
{
	// Normalize normal
	Normalize();
}

/**
*  @brief
*    Constructor
*/
inline Plane::Plane(const Vector3 &vOrigin, const Vector3 &vNormal)
{
	ComputeND(vOrigin, vNormal);
}

/**
*  @brief
*    Constructor
*/
inline Plane::Plane(const Vector3 &vV1, const Vector3 &vV2, const Vector3 &vV3)
{
	ComputeND(vV1, vV2, vV3);
}

/**
*  @brief
*    Destructor
*/
inline Plane::~Plane()
{
}

/**
*  @brief
*    Copy operator
*/
inline Plane &Plane::operator =(const Plane &cSource)
{
	a = cSource.a;
	b = cSource.b;
	c = cSource.c;
	d = cSource.d;

	return *this;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
inline bool Plane::operator ==(const Plane &cPlane) const
{
	return (a == cPlane.a && b == cPlane.b && c == cPlane.c && d == cPlane.d);
}

inline bool Plane::operator !=(const Plane &cPlane) const
{
	return (a != cPlane.a || b != cPlane.b || c != cPlane.c || d != cPlane.d);
}


/**
*  @brief
*    Calculates the plane
*/
inline Plane &Plane::ComputeND(const Vector3 &vOrigin, const Vector3 &vNormal)
{
	const Vector3 vN = vNormal.GetNormalized();
	a = vN.x;
	b = vN.y;
	c = vN.z;
	d = -vOrigin.DotProduct(vNormal);

	return *this;
}

/**
*  @brief
*    Normalizes the plane
*/
inline Plane &Plane::Normalize()
{
	// Avoid division through zero...
	// Normalize normal
	float fU = a*a + b*b + c*c;
	if (fU) {
		fU = Math::Sqrt(fU);
		if (fU) {
			// Scale
			fU = 1.0f/fU;
			a *= fU;
			b *= fU;
			c *= fU;
		}
	}

	return *this;
}

/**
*  @brief
*    Returns the side of the plane the given point is on
*/
inline Plane::ESide Plane::GetSide(const Vector3 &vPoint) const
{
	const float fDistance = a*vPoint.x + b*vPoint.y + c*vPoint.z + d;
	if (fDistance < -Math::Epsilon)
		return Behind;
	else if (fDistance >  Math::Epsilon)
		return InFront;
	else
		return Coinciding;
}

/**
*  @brief
*    Calculates the distance from a point to the plane
*/
inline float Plane::GetDistance(const Vector3 &vPoint) const
{
	return a*vPoint.x + b*vPoint.y + c*vPoint.z + d;
}

/**
*  @brief
*    Calculates the distance from a point to the plane
*/
inline float Plane::GetDistance(const Vector4 &vPoint) const
{
	return a*vPoint.x + b*vPoint.y + c*vPoint.z + d*vPoint.w;
}

/**
*  @brief
*    Calculates the distance to the ray/intersection point
*/
inline float Plane::GetDistance(const Vector3 &vRayPos, const Vector3 &vRayDir) const
{
	const float fA = a*vRayDir.x + b*vRayDir.y + c*vRayDir.z;

	// Ray parallel to plane?
	return fA ? GetDistance(vRayPos)/fA : 0.0f;
}

/**
*  @brief
*    Returns a point on the plane
*/
inline Vector3 Plane::GetPointOnPlane() const
{
	// -D*N
	return Vector3(-fD*a, -fD*b, -fD*c);
}

/**
*  @brief
*    Clips an edge by this plane
*/
inline Vector3 Plane::ClipEdge(const Vector3 &vV1, const Vector3 &vV2) const
{
	const Vector3 vDir = vV2 - vV1;
	return vV1 + vDir*(-GetDistance(vV1)/(a*vDir.x + b*vDir.y + c*vDir.z));
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
