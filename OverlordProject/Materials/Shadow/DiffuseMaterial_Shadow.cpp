#include "stdafx.h"
#include "DiffuseMaterial_Shadow.h"
#include "Base/GeneralStructs.h"
#include "Content/ContentManager.h"
#include "Graphics/ShadowMapRenderer.h"

ID3DX11EffectShaderResourceVariable * DiffuseMaterial_Shadow::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable * DiffuseMaterial_Shadow::m_pShadowMapSRVvariable = nullptr;
ID3DX11EffectVectorVariable* DiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* DiffuseMaterial_Shadow::m_pLightVPvariable = nullptr;

DiffuseMaterial_Shadow::DiffuseMaterial_Shadow() : Material(L"./Resources/Effects/Shadow/PosNormTex3D_Shadow.fx", L"Default"), m_pDiffuseTexture(nullptr)
{
}


DiffuseMaterial_Shadow::~DiffuseMaterial_Shadow()
{
}

void DiffuseMaterial_Shadow::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void DiffuseMaterial_Shadow::LoadEffectVariables()
{
	m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if(!m_pDiffuseSRVvariable->IsValid())
		Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
	m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
	if (!m_pLightDirectionVariable->IsValid())
		Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightDirection\' variable not found!");
	m_pShadowMapSRVvariable = m_pEffect->GetVariableByName("gShadowMap")->AsShaderResource();
		if (!m_pShadowMapSRVvariable->IsValid())
			Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gShadowMap\' variable not found!");
	m_pLightVPvariable = m_pEffect->GetVariableByName("gLightVP")->AsMatrix();
	if (!m_pLightVPvariable->IsValid())
		Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightVP\' variable not found!");
}

void DiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	m_pLightDirectionVariable->SetFloatVector(reinterpret_cast<float*>(&m_LightDirection));

	m_pShadowMapSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());

	m_LightVP = gameContext.pShadowMapper->GetLightVP();
	m_pLightVPvariable->SetMatrix(reinterpret_cast<float*>(&m_LightVP));
}