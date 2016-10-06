//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ColorMaterial.h"
#include "Base/GeneralStructs.h"


ColorMaterial::ColorMaterial(bool enableTransparency) : 
	Material(L"./Resources/Effects/PosNormCol3D.fx", enableTransparency?L"TransparencyTech":L"")
{
}


ColorMaterial::~ColorMaterial()
{
}

void ColorMaterial::ChangeColor(XMFLOAT3 col)
{
	m_color = col;
}

void ColorMaterial::DisableRender()
{
	m_Alpha = 0.0f;
}

void ColorMaterial::EnableRender()
{
	m_Alpha = 1.0f;
}

void ColorMaterial::LoadEffectVariables()
{
	m_pColor = m_pEffect->GetVariableByName("gColor")->AsVector();
	if (!m_pColor->IsValid())
	{
		Logger::LogError(L"ColorMAterial::Initialize() > Shader variable \'m_pColor\' not valid!");
		return;
	}
	m_pAlpha = m_pEffect->GetVariableByName("gOpacity")->AsScalar();
	if (!m_pAlpha->IsValid())
	{
		Logger::LogError(L"ColorMAterial::Initialize() > Shader variable \'m_pAlpha\' not valid!");
		return;
	}
}

void ColorMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	m_pColor->SetFloatVector(&m_color.x);
	m_pAlpha->SetFloat(m_Alpha);
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
}
