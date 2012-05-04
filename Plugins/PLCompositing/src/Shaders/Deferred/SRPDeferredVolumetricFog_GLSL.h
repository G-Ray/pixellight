/*********************************************************\
 *  File: SRPDeferredVolumetricFog_GLSL.h                *
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


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#define STRINGIFY(ME) #ME


// OpenGL 2.0 ("#version 110") GLSL vertex shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredVolumetricFog_GLSL_VS = STRINGIFY(
// Attributes
attribute vec4 VertexPosition;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)
									// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)
varying   vec2 VertexTexCoordVS;	// Vertex texture coordinate 0 output, lower/left is (0,0) and upper/right is (1,1)

// Programs
void main()
{
	// Set the clip space vertex position
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);

	// Pass through the vertex texture coordinate
	VertexTexCoordVS = VertexPosition.zw;
}
);	// STRINGIFY


// OpenGL 2.0 ("#version 110") GLSL fragment shader source code, "#version" is added by "PLRenderer::ProgramGenerator"
static const PLCore::String sDeferredVolumetricFog_GLSL_FS =
"#extension GL_ARB_texture_rectangle : enable\n"
STRINGIFY(
// Attributes
varying vec2 VertexTexCoordVS;	// Vertex texture coordinate input from vertex shader, lower/left is (0,0) and upper/right is (1,1)

// Uniforms
uniform vec3           FogPosition;		// View space fog position
uniform float          FogInvRadius;	// View space fog inverse radius
uniform vec3           FogColor0;
uniform vec3           FogColor1;
uniform vec2           InvFocalLen;		// 1/FocalLen (for example: 0.57771897 x 0.4142136)
uniform ivec2          Resolution;		// Resolution of the input texture data (for example 636 x 456)
uniform sampler2DRect  NormalDepthMap;	// Normal depth texture

// Programs
// Decodes a 2 component normal vector to a 3 component normal vector
vec3 decodeNormalVector(vec2 normal)
{
	vec2 fenc = normal*4.0 - 2.0;
	float f = dot(fenc, fenc);
	float g = sqrt(1.0 - f/4.0);
	vec3 n;
	n.xy = fenc*g;
	n.z = 1.0 - f/2.0;
	return n;
}

// Reconstructs view-space position (current surface point) using given parameters
vec3 uv_to_eye(vec2 uv, float eye_z, vec2 invFocalLen)
{
	// Convert from texture space [0,1] range into [-1,1] clip space
	uv = uv*2.0 - 1.0;

	// Reconstructs view-space position
	return vec3(uv * invFocalLen * eye_z, eye_z);
}

// Reconstructs view-space position (current surface point) using a given texel
vec3 fetch_eye_pos(sampler2DRect texture, ivec2 textureResolution, vec2 uv, vec2 invFocalLen)
{
	// Fetch the linear view space depth at the given texel position
	float eye_z = texture2DRect(texture, uv*vec2(textureResolution)).b;

	// Reconstructs view-space position using given parameters
	return uv_to_eye(uv, eye_z, invFocalLen);
}

// Program entry point
void main()
{
	// Get the current view space position
	vec3 viewSpacePos = fetch_eye_pos(NormalDepthMap, Resolution, VertexTexCoordVS, InvFocalLen);
	viewSpacePos.z = -viewSpacePos.z;

	// Calculate the fog factor
	vec3 fogPosition = (FogPosition - viewSpacePos)*FogInvRadius;
	viewSpacePos = -viewSpacePos*FogInvRadius;
	float value = dot(viewSpacePos, viewSpacePos);
	if (value == 0.0)
		value = 0.000001;	// Do never ever divide through zero, this may lead to ugly blocks!
	float k  = clamp(dot(fogPosition, viewSpacePos)/value, 0.0, 1.0);
	vec4  pl = vec4(k*viewSpacePos - fogPosition, 1.0);
	value = dot(pl, pl);
	if (value == 0.0)
		value = 0.000001;	// Do never ever divide through zero, this may lead to ugly blocks!
	gl_FragColor.rgb = clamp(FogColor0/value - FogColor1, 0.0, 1.0);
	gl_FragColor.a = 1.0;
}
);	// STRINGIFY


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef STRINGIFY
