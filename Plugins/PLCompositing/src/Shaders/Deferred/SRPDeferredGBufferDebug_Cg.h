/*********************************************************\
 *  File: SRPDeferredGBufferDebug_Cg.h                   *
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


// Cg vertex shader source code
static const PLCore::String sDeferredGBufferDebug_Cg_VS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float4 VertexPosition : POSITION,	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
	   uniform int2	  TextureSize)					// Texture size in texel\n\
{\n\
	VS_OUTPUT OUT;\n\
\n\
	// Pass through the vertex position\n\
	OUT.Position = float4(VertexPosition.xy, 0, 1);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	OUT.TexCoord = VertexPosition.zw*TextureSize;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Cg fragment shader source code
static const PLCore::String sDeferredGBufferDebug_Cg_FS = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Fragment output\n\
struct FS_OUTPUT {\n\
	float4 Color0 : COLOR0;\n\
};\n\
\n\
// Programs\n\
// Decodes a 2 component normal vector to a 3 component normal vector\n\
float3 decodeNormalVector(float2 normal)\n\
{\n\
	float2 fenc = normal*4 - 2;\n\
	float f = dot(fenc, fenc);\n\
	float g = sqrt(1 - f/4);\n\
	float3 n;\n\
	n.xy = fenc*g;\n\
	n.z = 1 - f/2;\n\
	return n;\n\
}\n\
\n\
// Program entry point\n\
FS_OUTPUT main(VS_OUTPUT   IN			// Interpolated output from the vertex stage\n\
	 , uniform float	   NearPlane	// Camera near plane distance\n\
	 , uniform float	   Range		// Distance between camera far and new plane (never 0!)\n\
	 , uniform samplerRECT Map)			// Input texture containing the data to visualize\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// Fetch the required texel data and set the RGB output color\n\
#ifdef FS_SHOW_BLACK\n\
	OUT.Color0.rgb = 0;\n\
#elif defined FS_SHOW_ALBEDO\n\
	OUT.Color0.rgb = texRECT(Map, IN.TexCoord).rgb;\n\
#elif defined FS_SHOW_AMBIENTOCCLUSION\n\
	OUT.Color0.rgb = texRECT(Map, IN.TexCoord).a;\n\
#elif defined FS_SHOW_NORMALS\n\
	OUT.Color0.rgb = decodeNormalVector(texRECT(Map, IN.TexCoord).rg);\n\
#elif defined FS_SHOW_DEPTH\n\
	// Normalized the linear view space depth, else we can't inspect the depth properly\n\
	OUT.Color0.rgb = (texRECT(Map, IN.TexCoord).b - NearPlane)/Range;\n\
#elif defined FS_SHOW_SPECULAR_COLOR\n\
	OUT.Color0.rgb = texRECT(Map, IN.TexCoord).rgb;\n\
#elif defined FS_SHOW_SPECULAR_EXPONENT\n\
	OUT.Color0.rgb = texRECT(Map, IN.TexCoord).a/128;\n\
#elif defined FS_SHOW_SELFILLUMINATION\n\
	OUT.Color0.rgb = texRECT(Map, IN.TexCoord).rgb;\n\
#elif defined FS_SHOW_GLOW\n\
	OUT.Color0.rgb = texRECT(Map, IN.TexCoord).a;\n\
#else\n\
	// Invalid state, should never ever happen!\n\
	OUT.Color0.rgb = 0;\n\
#endif\n\
\n\
	// Alpha is always 1\n\
	OUT.Color0.a = 1;\n\
\n\
	// Done\n\
	return OUT;\n\
}";
