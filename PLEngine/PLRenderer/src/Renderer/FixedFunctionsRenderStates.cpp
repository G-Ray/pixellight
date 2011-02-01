/*********************************************************\
 *  File: FixedFunctionsRenderStates.cpp                 *
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
#include <PLGeneral/Tools/Tools.h>
#include "PLRenderer/Renderer/Types.h"
#include "PLRenderer/Renderer/FixedFunctionsRenderStates.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLGeneral;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(FixedFunctionsRenderStates)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
// Fog
bool FixedFunctionsRenderStates::GetFogEnable() const
{
	return m_nRS[FixedFunctions::RenderState::FogEnable] != 0;
}

void FixedFunctionsRenderStates::SetFogEnable(bool bValue)
{
	m_nRS[FixedFunctions::RenderState::FogEnable] = bValue;
}

Color4 FixedFunctionsRenderStates::GetFogColor() const
{
	const uint32 nColor = m_nRS[FixedFunctions::RenderState::FogColor];
	return Color4(Color4::RedFromUInt32(nColor), Color4::GreenFromUInt32(nColor), Color4::BlueFromUInt32(nColor), Color4::AlphaFromUInt32(nColor));
}

void FixedFunctionsRenderStates::SetFogColor(const Color4 &cValue)
{
	m_nRS[FixedFunctions::RenderState::FogColor] = Color4::ToUInt32(cValue.r, cValue.g, cValue.b, cValue.a);
}

float FixedFunctionsRenderStates::GetFogDensity() const
{
	return Tools::UInt32ToFloat(m_nRS[FixedFunctions::RenderState::FogDensity]);
}

void FixedFunctionsRenderStates::SetFogDensity(float fValue)
{
	m_nRS[FixedFunctions::RenderState::FogDensity] = Tools::FloatToUInt32(fValue);
}

float FixedFunctionsRenderStates::GetFogStart() const
{
	return Tools::UInt32ToFloat(m_nRS[FixedFunctions::RenderState::FogStart]);
}

void FixedFunctionsRenderStates::SetFogStart(float fValue)
{
	m_nRS[FixedFunctions::RenderState::FogStart] = Tools::FloatToUInt32(fValue);
}

float FixedFunctionsRenderStates::GetFogEnd() const
{
	return Tools::UInt32ToFloat(m_nRS[FixedFunctions::RenderState::FogEnd]);
}

void FixedFunctionsRenderStates::SetFogEnd(float fValue)
{
	m_nRS[FixedFunctions::RenderState::FogEnd] = Tools::FloatToUInt32(fValue);
}

FixedFunctions::Fog::Enum FixedFunctionsRenderStates::GetFogMode() const
{
	return static_cast<FixedFunctions::Fog::Enum>(m_nRS[FixedFunctions::RenderState::FogMode]);
}

void FixedFunctionsRenderStates::SetFogMode(FixedFunctions::Fog::Enum nValue)
{
	m_nRS[FixedFunctions::RenderState::FogMode] = nValue;
}

// Alpha test
bool FixedFunctionsRenderStates::GetAlphaTestEnable() const
{
	return m_nRS[FixedFunctions::RenderState::AlphaTestEnable] != 0;
}

void FixedFunctionsRenderStates::SetAlphaTestEnable(bool bValue)
{
	m_nRS[FixedFunctions::RenderState::AlphaTestEnable] = bValue;
}

Compare::Enum FixedFunctionsRenderStates::GetAlphaTestFunction() const
{
	return static_cast<Compare::Enum>(m_nRS[FixedFunctions::RenderState::AlphaTestFunction]);
}

void FixedFunctionsRenderStates::SetAlphaTestFunction(Compare::Enum nValue)
{
	m_nRS[FixedFunctions::RenderState::AlphaTestFunction] = nValue;
}

float FixedFunctionsRenderStates::GetAlphaTestReference() const
{
	return Tools::UInt32ToFloat(m_nRS[FixedFunctions::RenderState::AlphaTestReference]);
}

void FixedFunctionsRenderStates::SetAlphaTestReference(float fValue)
{
	m_nRS[FixedFunctions::RenderState::AlphaTestReference] = Tools::FloatToUInt32(fValue);
}

// Misc
bool FixedFunctionsRenderStates::GetLighting() const
{
	return m_nRS[FixedFunctions::RenderState::Lighting] != 0;
}

void FixedFunctionsRenderStates::SetLighting(bool bValue)
{
	m_nRS[FixedFunctions::RenderState::Lighting] = bValue;
}

Color4 FixedFunctionsRenderStates::GetAmbient() const
{
	const uint32 nColor = m_nRS[FixedFunctions::RenderState::Ambient];
	return Color4(Color4::RedFromUInt32(nColor), Color4::GreenFromUInt32(nColor), Color4::BlueFromUInt32(nColor), Color4::AlphaFromUInt32(nColor));
}

void FixedFunctionsRenderStates::SetAmbient(const Color4 &cValue)
{
	m_nRS[FixedFunctions::RenderState::Ambient] = Color4::ToUInt32(cValue.r, cValue.g, cValue.b, cValue.a);
}

bool FixedFunctionsRenderStates::GetNormalizeNormals() const
{
	return m_nRS[FixedFunctions::RenderState::NormalizeNormals] != 0;
}

void FixedFunctionsRenderStates::SetNormalizeNormals(bool bValue)
{
	m_nRS[FixedFunctions::RenderState::NormalizeNormals] = bValue;
}

FixedFunctions::Shade::Enum FixedFunctionsRenderStates::GetShadeMode() const
{
	return static_cast<FixedFunctions::Shade::Enum>(m_nRS[FixedFunctions::RenderState::ShadeMode]);
}

void FixedFunctionsRenderStates::SetShadeMode(FixedFunctions::Shade::Enum nValue)
{
	m_nRS[FixedFunctions::RenderState::ShadeMode] = nValue;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FixedFunctionsRenderStates::FixedFunctionsRenderStates() :
	FogEnable(this),
	FogColor(this),
	FogDensity(this),
	FogStart(this),
	FogEnd(this),
	FogMode(this),
	AlphaTestEnable(this),
	AlphaTestFunction(this),
	AlphaTestReference(this),
	Lighting(this),
	Ambient(this),
	NormalizeNormals(this),
	ShadeMode(this)
{
	// Fog
	m_nRS[FixedFunctions::RenderState::FogEnable]  = false;
	m_nRS[FixedFunctions::RenderState::FogColor]   = 0;
	m_nRS[FixedFunctions::RenderState::FogDensity] = Tools::FloatToUInt32(1.0f);
	m_nRS[FixedFunctions::RenderState::FogStart]   = Tools::FloatToUInt32(0.0f);
	m_nRS[FixedFunctions::RenderState::FogEnd]     = Tools::FloatToUInt32(1.0f);
	m_nRS[FixedFunctions::RenderState::FogMode]    = FixedFunctions::Fog::Exp;
	// Alpha test
	m_nRS[FixedFunctions::RenderState::AlphaTestEnable]    = false;
	m_nRS[FixedFunctions::RenderState::AlphaTestFunction]  = Compare::GreaterEqual;
	m_nRS[FixedFunctions::RenderState::AlphaTestReference] = Tools::FloatToUInt32(0.5f);
	// Misc
	m_nRS[FixedFunctions::RenderState::Lighting]         = false;
	m_nRS[FixedFunctions::RenderState::Ambient]          = 0;
	m_nRS[FixedFunctions::RenderState::NormalizeNormals] = true;
	m_nRS[FixedFunctions::RenderState::ShadeMode]        = FixedFunctions::Shade::Smooth;
}

/**
*  @brief
*    Copy constructor
*/
FixedFunctionsRenderStates::FixedFunctionsRenderStates(const FixedFunctionsRenderStates &cSource) :
	FogEnable(this),
	FogColor(this),
	FogDensity(this),
	FogStart(this),
	FogEnd(this),
	FogMode(this),
	AlphaTestEnable(this),
	AlphaTestFunction(this),
	AlphaTestReference(this),
	Lighting(this),
	Ambient(this),
	NormalizeNormals(this),
	ShadeMode(this)
{
	// Copy states
	for (uint32 i=0; i<FixedFunctions::RenderState::Number; i++)
		m_nRS[i] = cSource.m_nRS[i];
}

/**
*  @brief
*    Destructor
*/
FixedFunctionsRenderStates::~FixedFunctionsRenderStates()
{
}

/**
*  @brief
*    Retrieves a render-state value
*/
uint32 FixedFunctionsRenderStates::Get(FixedFunctions::RenderState::Enum nState) const
{
	// Check whether the state is a valid render state member
	return (nState < FixedFunctions::RenderState::Number) ? m_nRS[nState] : 0;
}

/**
*  @brief
*    Sets a single render-state parameter
*/
bool FixedFunctionsRenderStates::Set(FixedFunctions::RenderState::Enum nState, uint32 nValue)
{
	// Check whether the state is a valid render state member
	if (nState >= FixedFunctions::RenderState::Number)
		return false; // Error!

	// Set render state value
	m_nRS[nState] = nValue;

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
FixedFunctionsRenderStates &FixedFunctionsRenderStates::operator =(const FixedFunctionsRenderStates &cSource)
{
	// Copy states
	for (uint32 i=0; i<FixedFunctions::RenderState::Number; i++)
		m_nRS[i] = cSource.m_nRS[i];

	// Return this
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
