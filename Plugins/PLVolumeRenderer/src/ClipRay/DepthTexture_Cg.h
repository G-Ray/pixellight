/*********************************************************\
 *  File: DepthTexture_Cg.h                              *
 *      Fragment shader source code - Cg
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


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


// Depth buffer information usage for ray clipping as described within the paper
// "GPU-Based High-Quality Volume Rendering For Virtual Environments" by Andrea Kratz,
// Markus Hadwiger, Anton Fuhrmann, Rainer Splechtna, Katja B�hler and the book
// "Real-Time Volume Graphics", section "11.4.1 Opaque Scene Geometry Intersection with a Ray-Cast Volume" (page 286).


// Template version
static const PLCore::String sSourceCode_Fragment_Template = STRINGIFY(
// Uniforms
uniform samplerRECT DepthTexture_x_;			// Depth texture map
uniform float4x4    ClipSpaceToObjectSpace_x_;	// Clip space to object space matrix

// Varying
float2 RayClip_FragmentCoordinate_x_ : WPOS;

/**
*  @brief
*    Performs an clipping operation on the given ray
*
*  @param[in, out] RayOrigin
*    Start position of the ray inside the volume, within the interval [(0, 0, 0) .. (1, 1, 1)]
*  @param[in]      RayDirection
*    Normalized ray direction
*  @param[in, out] MaximumTravelLength
*    Maximum travel length along the ray, within the interval [0 .. 1]
*/
void ClipRay_x_(inout float3 RayOrigin, float3 RayDirection, inout float MaximumTravelLength)
{
	// Compute the homogeneous view-space position
	float4 hviewpos;
	hviewpos.xy = RayClip_FragmentCoordinate_x_.xy/texRECTsize(DepthTexture_x_, 0).xy;					// Window position is in [0,1]^2
	hviewpos.z  = texRECTfetch(DepthTexture_x_, int4(int2(RayClip_FragmentCoordinate_x_.xy), 0, 0)).r;	// Depth in [0,1]
	hviewpos.w  = 1.0;

	// We need this to be in [-1,1]^3 clip space
	hviewpos = hviewpos*2.0 - 1.0;

	// Back-project to homogeneous volume space
	float4 hvolpos = mul(ClipSpaceToObjectSpace_x_, hviewpos);

	// Get normalized volume-space position
	float4 position = hvolpos/hvolpos.w;

	// Update maximum travel length along the ray
	float3 delta = position.xyz - RayOrigin;
	float newLength = length(delta);
	if (MaximumTravelLength > newLength)
		MaximumTravelLength = newLength;

	// In case we're drawing backfaces with disabled depth buffer test, we need
	// to check in here whether or not the ray is completely culled
	if (dot(1.0/delta, RayDirection) < 0.0)
		discard;
}
);	// STRINGIFY

// None-template version
static const PLCore::String sSourceCode_Fragment = sSourceCode_Fragment_Template + '\n' + STRINGIFY(
void ClipRay(inout float3 RayOrigin, float3 RayDirection, inout float MaximumTravelLength)
{
	// Call the function version also used by the template
	ClipRay_x_(RayOrigin, RayDirection, MaximumTravelLength);
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
