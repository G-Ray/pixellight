/*********************************************************\
 *  File: SRPDirectionalLightingShaders.h                *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
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


#ifndef __PLCOMPOSITING_GENERAL_DIRECTIONALLIGHTING_H__
#define __PLCOMPOSITING_GENERAL_DIRECTIONALLIGHTING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include <PLScene/Scene/SceneNodeHandler.h>
#include <PLScene/Compositing/General/SRPDirectionalLighting.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class RenderStates;
	class ProgramUniform;
	class ProgramAttribute;
}
namespace PLScene {
	class SNDirectionalLight;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Shaders based diffuse only scene renderer pass implementation
*
*  @remarks
*    This scene renderer pass can deal with the following material parameters:
*    - Opacity:                    Transparency, 0.0 is full transparent, 1.0 is solid, default: 1.0
*    - SrcBlendFunc:               If 'Opacity' is less '1', source blend function, default: 'SrcAlpha' (see 'PLRenderer::BlendFunc::Enum')
*    - DstBlendFunc:               If 'Opacity' is less '1', destination blend function, default: 'InvSrcAlpha' (see 'PLRenderer::BlendFunc::Enum')
*    - TwoSided:                   Backface culling active? If 1, both sides of polygons are visible, default: 0
*    - DiffuseColor:               Use this 3 floating point values to set the diffuse color, default: 1.0 1.0 1.0
*    - DiffuseMap:                 Diffuse map (2D, RGB, other usual name: 'decal map')
*      - AlphaReference:           Alpha reference 0.0-1.0, only used if the diffuse map has an alpha channel, if 0 no alpha test is performed at all
*    - DiffuseRampMap:             Can be used for 'cell shading' (1D, grayscale)
*    - AmbientOcclusionMap:        'Static global ambient occlusion', not influenced by any light but influcences the lighting (2D, grayscale)
*      - AmbientOcclusionFactor:   If there's a 'AmbientOcclusionMap', use this floating point value to set the ambient occlusion factor, default: 1.0
*    - LightMap:                   'Static lighting/shadowing', not influenced by any light (2D, RGB)
*      - LightMapColor:            If there's a 'LightMap', use this 3 floating point values to set the light map color, default: 1.0 1.0 1.0
*    - EmissiveMap:                'Emissive lighting', not influenced by any lighting (2D, RGB, other usual name: 'self illumination map'), emissive materials will not automatically glow, for glow you need to set 'Glow', too
*      - EmissiveMapColor:         If there's a 'EmissiveMap', use this 3 floating point values to set the emissive map color, default: 1.0 1.0 1.0
*    - IndexOfRefraction:          Index of refraction (IOR), used for Fresnel reflection, <= 0.0 means no Fresnel reflection, default: 0.0 (List of refractive indices: http://en.wikipedia.org/wiki/List_of_refractive_indices, the index of refaction of the "from" material is set to air=1.0)
*      - FresnelReflectionPower:   If 'IndexOfRefraction' is > 0.0, this is the Fresnel reflection power, default: 5.0
*    - ReflectionColor:            If 'IndexOfRefraction' is > 0.0 or if there's a 'ReflectionMap', use this 3 floating point values to set the reflection color, default: 1.0 1.0 1.0
*    - Reflectivity:               If 'IndexOfRefraction' is > 0.0 or if there's a 'ReflectionMap', reflectivity factor can be used, 0.0...1.0=no reflection...full reflection, default: 1.0
*    - ReflectivityMap:            Per fragment 'reflectivity' control (2D, grayscale)
*    - ReflectionMap:             'Reflections', not influenced by any light, 2D (spherical environment mapping) or cube map (cubic environment mapping), RGB
*    - NormalMap:                  Normal map (sometimes also called 'Dot3 bump map') required for 'per pixel lighting' (2D, RGB, tangent space)
*      - NormalMapBumpiness:       The 'bumpiness' of the normal map, normally values from 0-2, default: 1
*    - DetailNormalMap:            For more detailed 'per pixel lighting' (2D, RGB, tangent space)
*      - DetailNormalMapBumpiness: The 'bumpiness' of the detail normal map, normally values from 0-2, default: 1
*      - DetailNormalMapUVScale:   Texture coordinate scale factor for the detail normal map, default: 4.0 4.0
*    - HeightMap:                  Height map required for 'per pixel lighting' with "Parallax Mapping" or also called "Offset Normal Mapping" or "Virtual Displacement Mapping" (2D, grayscale, other usual name: 'bump map')
*      - Parallax:                 Controls the 'depth' of the material if parallax mapping is used, normally values from 0.01-0.04, default: 0.04. Parallax mapping can ONLY be used if a height map is given!
*    - SpecularColor:              Multiplied with the final specular term, normally values from 0-1, default: 1 for all components. If all 0, there's no specular lighting at all.
*      - SpecularExponent:         The 'power' of the specular lighting, normally values from 1-128, default: 45
*      - SpecularMap:              Specular control map (2D, RGB, if there's an alpha channel it's used for specular power control)
*      - SpecularRampMap:          Can be used for 'cell shading' (1D, grayscale)
*    - EdgeRampMap:                Can be used for cartoon silhouettes (1D, grayscale)
*    - Glow:                       Glow factor, only used if there's a 'SNMPostProcessGlow' post process effect, values 0.0..1.0=no glow...full glow, default: 0.0
*    - GlowMap:                    If 'Glow' is not 0, per fragment 'glow' control (2D, R grayscale)
*
*    Example:
*  @verbatim
*    <?xml version="1.0"?>
*    <Material Version="1">
*        <Float Name="Opacity">1.0</Float>
*        <String Name="SrcBlendFunc">SrcAlpha</String>
*        <String Name="DstBlendFunc">InvSrcAlpha</String>
*        <Float Name="TwoSided">0.0</Float>
*        <Float3 Name="DiffuseColor">1.0 1.0 1.0</Float3>
*        <Texture Name="DiffuseMap">Data/Textures/My_DiffuseMap.dds</Texture>
*        <Texture Name="DiffuseRampMap">Data/Textures/My_DiffuseRampMap.dds</Texture>
*        <Float Name="AlphaReference">0.5</Float>
*        <Texture Name="AmbientOcclusionMap">Data/Textures/My_AmbientOcclusionMap.dds</Texture>
*        <Float Name="AmbientOcclusionFactor">1.0</Float>
*        <Texture Name="LightMap">Data/Textures/My_LightMap.dds</Texture>
*        <Float3 Name="LightMapColor">1.0 1.0 1.0</Float3>
*        <Texture Name="EmissiveMap">Data/Textures/My_EmissiveMap.dds</Texture>
*        <Float3 Name="EmissiveMapColor">1.0 1.0 1.0</Float3>
*        <Float Name="IndexOfRefraction">0.0</Float>
*        <Float Name="FresnelReflectionPower">5.0</Float>
*        <Float3 Name="ReflectionColor">1.0 1.0 1.0</Float3>
*        <Float Name="Reflectivity">1.0</Float>
*        <Texture Name="ReflectivityMap">Data/Textures/My_ReflectivityMap.dds</Texture>
*        <Texture Name="ReflectionMap">Data/Textures/My_ReflectionMap.dds</Texture>
*        <Texture Name="NormalMap">Data/Textures/My_NormalMap.dds</Texture>
*        <Float Name="NormalMapBumpiness">1.0</Float>
*        <Texture Name="DetailNormalMap">Data/Textures/My_DetailNormalMap.dds</Texture>
*        <Float Name="DetailNormalMapBumpiness">1.0</Float>
*        <Float2 Name="DetailNormalMapUVScale">4.0 4.0</Float2>
*        <Texture Name="HeightMap">Data/Textures/My_HeightMap.dds</Texture>
*        <Float Name="Parallax">0.04</Float>
*        <Float3 Name="SpecularColor">1.0 1.0 1.0</Float3>
*        <Float Name="SpecularExponent">45.0</Float>
*        <Texture Name="SpecularMap">Data/Textures/My_SpecularMap.dds</Texture>
*        <Texture Name="SpecularRampMap">Data/Textures/My_SpecularRampMap.dds</Texture>
*        <Texture Name="EdgeRampMap">Data/Textures/My_EdgeRampMap.dds</Texture>
*        <Float Name="Glow">0.0</Float>
*        <Texture Name="GlowMap">Data/Textures/My_GlowMap.dds</Texture>
*    </Material>
*  @endverbatim
*
*  @note
*    - Normal map compression using swizzled DXT5 (xGxR), LATC2 and alternate XY swizzle LATC2 is supported
*    - Build in support for a glow post processing effect writing values into the alpha channel of the framebuffer
*    - Build in support for a DOF post processing effect writing values into the alpha channel of the framebuffer
*/
class SRPDirectionalLightingShaders : public PLScene::SRPDirectionalLighting {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene renderer pass flags (PLScene::SceneRendererPass flags extension)
		*/
		enum EFlags {
			NoGammaCorrection     = 1<<5,	/**< Do not perform gamma correction (in general gamma correction done for: DiffuseMap, LightMap, EmissiveMap, ReflectionMap) - if gamma correction is enabled, there should be a gamma correction at the end of the render pipeline */
			NoAmbientOcclusionMap = 1<<6,	/**< Ignore ambient occlusion maps */
			NoLightMap            = 1<<7,	/**< Ignore light maps */
			NoEmissiveMap         = 1<<8,	/**< Ignore emissive maps */
			NoFresnelReflection   = 1<<9,	/**< Ignore fresnel reflection */
			NoReflectivityMap     = 1<<10,	/**< Ignore reflectivity map */
			NoReflectionMap       = 1<<11,	/**< Ignore reflection map */
			NoNormalMap           = 1<<12,	/**< Ignore normal maps */
			NoDetailNormalMap     = 1<<13,	/**< Ignore detail normal maps */
			NoParallaxMapping     = 1<<14,	/**< No parallax mapping (also called 'Photonic Mapping', 'Offset Mapping' or 'Virtual Displacement Mapping') */
			NoSpecular            = 1<<15,	/**< No specular */
			NoSpecularMap         = 1<<16,	/**< Ignore specular maps */
			NoDiffuseRampMap      = 1<<17,	/**< Ignore diffuse ramp maps */
			NoSpecularRampMap     = 1<<18,	/**< Ignore specular ramp maps */
			NoEdgeRampMap         = 1<<19,	/**< Ignore edge ramp maps */
			NoGlow                = 1<<20,	/**< Ignore glow - please note, this only disables writing glow information into the alpha channel of the framebuffer */
			NoGlowMap             = 1<<21,	/**< Ignore glow map */
			NoDOF                 = 1<<22	/**< Ignore DOF - please note, this only disables writing DOF information into the alpha channel of the framebuffer */
		};
		pl_enum(EFlags)
			pl_enum_base(PLScene::SRPDirectionalLighting::EFlags)
			pl_enum_value(NoGammaCorrection,		"Do not perform gamma correction (in general gamma correction done for: DiffuseMap, LightMap, EmissiveMap, ReflectionMap) - if gamma correction is enabled, there should be a gamma correction at the end of the render pipeline")
			pl_enum_value(NoAmbientOcclusionMap,	"Ignore ambient occlusion maps")
			pl_enum_value(NoLightMap,				"Ignore light maps")
			pl_enum_value(NoEmissiveMap,			"Ignore emissive maps")
			pl_enum_value(NoFresnelReflection,		"Ignore fresnel reflection")
			pl_enum_value(NoReflectivityMap,		"Ignore reflectivity map")
			pl_enum_value(NoReflectionMap,			"Ignore reflection map")
			pl_enum_value(NoNormalMap,				"Ignore normal maps")
			pl_enum_value(NoDetailNormalMap,		"Inore detail normal maps")
			pl_enum_value(NoParallaxMapping,		"No parallax mapping (also called 'Photonic Mapping', 'Offset Mapping' or 'Virtual Displacement Mapping')")
			pl_enum_value(NoSpecular,				"No specular")
			pl_enum_value(NoSpecularMap,			"Ignore specular maps")
			pl_enum_value(NoDiffuseRampMap,			"Ignore diffuse ramp maps")
			pl_enum_value(NoSpecularRampMap,		"Ignore specular ramp maps")
			pl_enum_value(NoEdgeRampMap,			"Ignore edge ramp maps")
			pl_enum_value(NoGlow,					"Ignore glow - please note, this only disables writing glow information into the alpha channel of the framebuffer")
			pl_enum_value(NoGlowMap,				"Ignore glow map")
			pl_enum_value(NoDOF,					"Ignore DOF - please note, this only disables writing DOF information into the alpha channel of the framebuffer")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SRPDirectionalLightingShaders, "PLCompositing", PLScene::SRPDirectionalLighting, "Shaders based diffuse only scene renderer pass implementation")
		pl_constructor_0(DefaultConstructor, "Default constructor", "")
		pl_attribute(ShaderLanguage,	PLGeneral::String,		"",	ReadWrite,	DirectValue,	"Shader language to use (for example \"GLSL\" or \"Cg\"), if empty string, the default shader language of the renderer will be used",	"")
		// Overwritten PLScene::SceneRendererPass variables
		pl_attribute(Flags,				pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,			"Flags",																																"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCOM_API SRPDirectionalLightingShaders();

		/**
		*  @brief
		*    Destructur
		*/
		PLCOM_API virtual ~SRPDirectionalLightingShaders();

		/**
		*  @brief
		*    Returns the directional light scene node used for rendering the scene the last time
		*
		*  @note
		*    The directional light scene node used for rendering the scene the last time, NULL if no light was used
		*
		*  @note
		*    - This information can for example be used to skip the light rendering for the already rendered light
		*      within a following scene renderer pass
		*/
		PLCOM_API PLScene::SNDirectionalLight *GetUsedLight() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Vertex shader flags, flag names become to source code definitions
		*/
		enum EVertexShaderFlags {
			VS_TEXCOORD0				= 1<<0,	/**< Use texture coordinate 0 */
			VS_TEXCOORD1				= 1<<1,	/**< Use texture coordinate 1 */
			VS_NORMAL					= 1<<2,	/**< Use vertex normal */
				VS_TWOSIDEDLIGHTING		= 1<<3,	/**< Two sided lighting possible? (VS_NORMAL should be defined!) */
				VS_TANGENT_BINORMAL		= 1<<4,	/**< Use vertex tangent and binormal (VS_NORMAL should be defined!) */
					VS_PARALLAXMAPPING	= 1<<5,	/**< Perform parallax mapping (VS_NORMAL and VS_TANGENT_BINORMAL should be defined!) */
			VS_VIEWSPACEPOSITION		= 1<<6	/**< Calculate the view space position of the vertex (required for reflections and lighting) */
		};

		/**
		*  @brief
		*    Fragment shader flags, flag names become to source code definitions
		*/
		enum EFragmentShaderFlags {
			FS_TEXCOORD0						= 1<<0,		/**< Use texture coordinate 0 */
			FS_TEXCOORD1						= 1<<1,		/**< Use texture coordinate 1 */
			FS_NORMAL							= 1<<2,		/**< Use vertex normal */
				FS_TANGENT_BINORMAL				= 1<<3,		/**< Use vertex tangent and binormal (FS_NORMAL should be defined!) */
					FS_PARALLAXMAPPING			= 1<<4,		/**< Perform parallax mapping (FS_NORMAL and FS_TANGENT_BINORMAL should be defined!) */
			FS_GAMMACORRECTION					= 1<<5,		/**< Use gamma correction (sRGB to linear space) */
			FS_DIFFUSEMAP						= 1<<6,		/**< Take diffuse map into account */
				FS_ALPHATEST					= 1<<7,		/**< Use alpha test to discard fragments (FS_DIFFUSEMAP should be defined!) */
			FS_DIFFUSERAMPMAP					= 1<<8,		/**< Use diffuse ramp map */
			FS_AMBIENTOCCLUSIONMAP				= 1<<9,		/**< Use ambient occlusion map */
			FS_LIGHTMAP							= 1<<10,	/**< Use light map */
			FS_EMISSIVEMAP						= 1<<11,	/**< Use emissive map */
			FS_REFLECTION						= 1<<12,	/**< Use reflection */
				FS_FRESNELREFLECTION			= 1<<13,	/**< Use fresnel reflection (FS_REFLECTION should be defined!) */
				FS_REFLECTIVITYMAP				= 1<<14,	/**< Use reflectivity map (FS_REFLECTION and FS_FRESNELREFLECTION or FS_2DREFLECTIONMAP or FS_CUBEREFLECTIONMAP should be defined!) */
				FS_2DREFLECTIONMAP				= 1<<15,	/**< Use 2D reflection mapping (FS_REFLECTION should be defined, can't be set together with FS_CUBEREFLECTIONMAP!) */
				FS_CUBEREFLECTIONMAP			= 1<<16,	/**< Use cube reflection mapping (FS_REFLECTION should be defined, can't be set together with FS_2DREFLECTIONMAP!) */
			FS_NORMALMAP						= 1<<17,	/**< Take normal map into account */
				FS_NORMALMAP_DXT5_XGXR			= 1<<18,	/**< DXT5 XGXR normal map (FS_NORMALMAP should be defined and FS_NORMALMAP_LATC2 not!) */
				FS_NORMALMAP_LATC2				= 1<<19,	/**< LATC2 normal map (FS_NORMALMAP should be defined and FS_NORMALMAP_DXT5_XGXR not!) */
				FS_DETAILNORMALMAP				= 1<<20,	/**< Take detail normal map into account (FS_NORMALMAP should be defined!) */
				FS_DETAILNORMALMAP_DXT5_XGXR	= 1<<21,	/**< DXT5 XGXR compressed detail normal map (FS_NORMALMAP & FS_DETAILNORMALMAP should be defined and FS_DETAILNORMALMAP_LATC2 not!) */
				FS_DETAILNORMALMAP_LATC2		= 1<<22,	/**< LATC2 compressed detail normal map (FS_NORMALMAP & FS_DETAILNORMALMAP should be defined and FS_DETAILNORMALMAP_DXT5_XGXR not!) */
			FS_LIGHTING							= 1<<23,	/**< Perform lighting */
				FS_SPECULAR						= 1<<24,	/**< Use specular (FS_LIGHTING should be set, too) */
					FS_SPECULARMAP				= 1<<25,	/**< Take specular map into account (FS_LIGHTING and FS_SPECULAR should be set, too) */
					FS_SPECULARRAMPMAP			= 1<<26,	/**< Take specular ramp map into account (FS_LIGHTING and FS_SPECULAR should be set, too) */
				FS_EDGERAMPMAP					= 1<<27,	/**< Use edge ramp map (FS_LIGHTING should be set, too) */
			FS_GLOW								= 1<<28,	/**< Use glow (FS_DOF is not allowed to be set as well) */
				FS_GLOWMAP						= 1<<29,	/**< Use glow map (FS_GLOW should be set, too) */
			FS_DOF								= 1<<30		/**< Use depth of field (FS_GLOW is not allowed to be set as well) */
		};

		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			PLRenderer::ProgramAttribute *pVertexTexCoord0;
			PLRenderer::ProgramAttribute *pVertexTexCoord1;
			PLRenderer::ProgramAttribute *pVertexNormal;
			PLRenderer::ProgramAttribute *pVertexTangent;
			PLRenderer::ProgramAttribute *pVertexBinormal;
			// Vertex shader uniforms
			PLRenderer::ProgramUniform *pNormalScale;
			PLRenderer::ProgramUniform *pObjectSpaceToViewSpaceMatrix;
			PLRenderer::ProgramUniform *pObjectSpaceToClipSpaceMatrix;
			PLRenderer::ProgramUniform *pEyePos;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pAmbientColor;
			PLRenderer::ProgramUniform *pDiffuseColor;
			PLRenderer::ProgramUniform *pDiffuseMap;
			PLRenderer::ProgramUniform *pAlphaReference;
			PLRenderer::ProgramUniform *pDiffuseRampMap;
			PLRenderer::ProgramUniform *pAmbientOcclusionMap;
			PLRenderer::ProgramUniform *pAmbientOcclusionFactor;
			PLRenderer::ProgramUniform *pLightMap;
			PLRenderer::ProgramUniform *pLightMapColor;
			PLRenderer::ProgramUniform *pEmissiveMap;
			PLRenderer::ProgramUniform *pEmissiveMapColor;
			PLRenderer::ProgramUniform *pReflectionColor;
			PLRenderer::ProgramUniform *pReflectivity;
			PLRenderer::ProgramUniform *pReflectivityMap;
			PLRenderer::ProgramUniform *pFresnelConstants;
			PLRenderer::ProgramUniform *pReflectionMap;
			PLRenderer::ProgramUniform *pViewSpaceToWorldSpace;
			PLRenderer::ProgramUniform *pNormalMap;
			PLRenderer::ProgramUniform *pNormalMapBumpiness;
			PLRenderer::ProgramUniform *pDetailNormalMap;
			PLRenderer::ProgramUniform *pDetailNormalMapBumpiness;
			PLRenderer::ProgramUniform *pDetailNormalMapUVScale;
			PLRenderer::ProgramUniform *pHeightMap;
			PLRenderer::ProgramUniform *pParallaxScaleBias;
			PLRenderer::ProgramUniform *pLightDirection;
			PLRenderer::ProgramUniform *pLightColor;
			PLRenderer::ProgramUniform *pSpecularColor;
			PLRenderer::ProgramUniform *pSpecularExponent;
			PLRenderer::ProgramUniform *pSpecularMap;
			PLRenderer::ProgramUniform *pSpecularRampMap;
			PLRenderer::ProgramUniform *pEdgeRampMap;
			PLRenderer::ProgramUniform *pGlow;
			PLRenderer::ProgramUniform *pGlowMap;
			PLRenderer::ProgramUniform *pDOFParams;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be NULL */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */
		PLRenderer::RenderStates			*m_pRenderStates;		/**< Used to 'translate' render state strings, always valid! */
		PLMath::Vector3						 m_vLightDirection;		/**< Normalized view space light direction vector */
		PLGraphics::Color3					 m_cLightColor;			/**< Light color */
		PLScene::SceneNodeHandler			 m_cLightNodeHandler;	/**< Scene node handler keeping a reference to the directional light scene node used for rendering the last time */
		// Build in post processing features storing data within the alpha channel
		bool  m_bGlowEnabled;			/**< Do we need to take care of glow data? */
		float m_fDOFNearBlurDepth;
		float m_fDOFFocalPlaneDepth;
		float m_fDOFFarBlurDepth;
		float m_fDOFBlurrinessCutoff;


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SRPDirectionalLighting functions ]
	//[-------------------------------------------------------]
	private:
		void DrawMesh(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, PLScene::SceneNode &cSceneNode, const PLMesh::MeshHandler &cMeshHandler, const PLMesh::Mesh &cMesh, const PLMesh::MeshLODLevel &cMeshLODLevel, PLRenderer::VertexBuffer &cVertexBuffer);


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneRendererPass functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void Draw(PLRenderer::Renderer &cRenderer, const PLScene::SQCull &cCullQuery);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_GENERAL_DIRECTIONALLIGHTING_H__