/*********************************************************\
 *  File: HDRAverageLuminance_Cg.h                       *
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
static const PLCore::String sHDRAverageLuminance_Cg_VS = "\
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


// Downsample Cg vertex shader source code
static const PLCore::String sHDRAverageLuminance_Cg_VS_Downsample = "\
// Vertex output\n\
struct VS_OUTPUT {\n\
	float4 Position : POSITION;		// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
	float2 TexCoord	: TEXCOORD0;	// Vertex texture coordinate, lower/left is (0,0) and upper/right is (<TextureWidth>,<TextureHeight>)\n\
};\n\
\n\
// Programs\n\
VS_OUTPUT main(float4 VertexPosition : POSITION,	// Clip space vertex position, lower/left is (-1,-1) and upper/right is (1,1)\n\
													// zw = Vertex texture coordinate, lower/left is (0,0) and upper/right is (1,1)\n\
	   uniform int2	  TextureSize,					// Texture size in texel\n\
	   uniform float2 Size)							// Size [0..1]\n\
{\n\
	VS_OUTPUT OUT;\n\
\n\
	// Convert the incoming clip space vertex position from [-1..1] to [0..1]\n\
	float2 position = (VertexPosition.xy + 1)*0.5f;\n\
\n\
	// Apply size\n\
	position *= Size;\n\
\n\
	// Write out the vertex position in clip space\n\
	OUT.Position = float4(position*2 - 1, 0, 1);\n\
\n\
	// Pass through the scaled vertex texture coordinate\n\
	OUT.TexCoord = VertexPosition.zw*TextureSize;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Common Cg fragment shader source code
static const PLCore::String sHDRAverageLuminance_Cg_FS_Common = "\
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
// Neighbor offset table\n\
const static float2 Offsets[4] = {\n\
	float2( 0,  0), // Center       0\n\
	float2( 1,  0), // Right        1\n\
	float2( 1,  1), // Bottom right 2\n\
	float2( 0,  1)  // Bottom       3\n\
};\n";


// Downsample log Cg fragment shader source code
static const PLCore::String sHDRAverageLuminance_Cg_FS_DownsampleLog = "\
// Downsample log main function\n\
FS_OUTPUT main(VS_OUTPUT   IN				// Interpolated output from the vertex stage\n\
	 , uniform float3      LuminanceConvert	// Convert RGB to luminance\n\
	 , uniform float       Epsilon			// A small value to avoid undefined log(0)\n\
	 , uniform samplerRECT Texture)			// HDR texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	// 'Photographic Tone Reproduction for Digital Images': Formula 1\n\
	float luminance = 0;\n\
	for (int i=0; i<4; i++) {\n\
		// If, for any reason, there's an invalid value it would mess up everything... so, security is really a must have in here!\n\
		float value = log(dot(texRECT(Texture, IN.TexCoord + Offsets[i]).rgb, LuminanceConvert) + Epsilon);\n\
		if (!isnan(value))\n\
			luminance += value;\n\
	}\n\
\n\
	// Write down the result\n\
	OUT.Color0 = luminance*0.25f;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Downsample Cg fragment shader source code
static const PLCore::String sHDRAverageLuminance_Cg_FS_Downsample = "\
// Downsample main function\n\
FS_OUTPUT main(VS_OUTPUT   IN		// Interpolated output from the vertex stage\n\
	 , uniform samplerRECT Texture)	// HDR texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	float luminance = 0;\n\
	for (int i=0; i<4; i++)\n\
		luminance += texRECT(Texture, IN.TexCoord + Offsets[i]).r;\n\
	OUT.Color0 = luminance*0.25f;\n\
\n\
	// Done\n\
	return OUT;\n\
}";


// Downsample exp Cg fragment shader source code
static const PLCore::String sHDRAverageLuminance_Cg_FS_DownsampleExp = "\
// Downsample exp main function\n\
FS_OUTPUT main(VS_OUTPUT   IN		// Interpolated output from the vertex stage\n\
	 , uniform samplerRECT Texture)	// HDR texture\n\
{\n\
	FS_OUTPUT OUT;\n\
\n\
	float luminance = 0;\n\
	for (int i=0; i<4; i++)\n\
		luminance += texRECT(Texture, IN.TexCoord + Offsets[i]).r;\n\
	OUT.Color0 = exp(luminance*0.25f);\n\
\n\
	// Done\n\
	return OUT;\n\
}";
