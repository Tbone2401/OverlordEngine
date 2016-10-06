//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkyboxMaterial.h"
#include "Base/GeneralStructs.h"
#include <Content/ContentManager.h>

ID3DX11EffectShaderResourceVariable * SkyboxMaterial::m_pTextureVariable = nullptr;

SkyboxMaterial::SkyboxMaterial() : 
	Material(L"./Resources/Effects/Skybox.fx", L"Render")
{
}


SkyboxMaterial::~SkyboxMaterial()
{
}

void SkyboxMaterial::SetTexture(const wstring assetFile)
{
	m_pTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkyboxMaterial::LoadEffectVariables()
{
	m_pTextureVariable = m_pEffect->GetVariableByName("m_CubeMap")->AsShaderResource();
}

void SkyboxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent); 
	m_pTextureVariable->SetResource(m_pTexture->GetShaderResourceView());
}
