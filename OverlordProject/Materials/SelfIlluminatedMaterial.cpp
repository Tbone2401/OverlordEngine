#include "stdafx.h"
#include "SelfIlluminatedMaterial.h"
#include "Base/GeneralStructs.h"
#include "Content/ContentManager.h"

ID3DX11EffectShaderResourceVariable * SelfIlluminatedMaterial::m_pDiffuseSRVvariable = nullptr;

SelfIlluminatedMaterial::SelfIlluminatedMaterial() : Material(L"./Resources/Effects/2Sided_SelfIllum.fx", L"Default"), m_pDiffuseTexture(nullptr)
{
}


SelfIlluminatedMaterial::~SelfIlluminatedMaterial()
{
}

void SelfIlluminatedMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void SelfIlluminatedMaterial::LoadEffectVariables()
{
	m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

void SelfIlluminatedMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
}
