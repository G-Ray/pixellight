/*********************************************************\
 *  File: DebugRayEnd_GLSL.h                             *
 *      Fragment shader source code - GLSL (OpenGL 3.3 ("#version 330")
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


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


static const PLCore::String sSourceCode_Fragment = STRINGIFY(
/**
*  @brief
*    Integrates over the volume
*
*  @param[in] StartPosition
*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] NumberOfSteps
*    Number of steps along the ray (= number of samples to take), must be positive
*  @param[in] StepPositionDelta
*    Position advance per step along the ray, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in] MaximumTravelLength
*    Maximum travel length along the ray, within the interval [0 .. 1]
*
*  @return
*    RGBA result of the ray traversal
*/
vec4 RayTraversal(vec3 StartPosition, int NumberOfSteps, vec3 StepPositionDelta, float MaximumTravelLength)
{
	// Show end position inside the volume, there's no real ray traversal within this debug function
	vec3 endPosition = StartPosition + normalize(StepPositionDelta)*MaximumTravelLength;
	return vec4(endPosition.x, endPosition.y, endPosition.z, 1.0);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
