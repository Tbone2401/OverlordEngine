//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkinnedDiffuseMaterial_Shadow.h"
#include "Base/GeneralStructs.h"
#include "Diagnostics/Logger.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"
#include "Components/ModelComponent.h"
#include "Graphics/ModelAnimator.h"
#include "Graphics/ShadowMapRenderer.h"

ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial_Shadow::m_pShadowMapSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pBoneTransformsVariable = nullptr;
ID3DX11EffectVectorVariable* SkinnedDiffuseMaterial_Shadow::m_pLightDirectionVariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial_Shadow::m_pLightVPvariable = nullptr;

SkinnedDiffuseMaterial_Shadow::SkinnedDiffuseMaterial_Shadow() : Material(L"./Resources/Effects/Shadow/PosNormTex3D_Skinned_Shadow.fx"),
m_pDiffuseTexture(nullptr)
{
}


SkinnedDiffuseMaterial_Shadow::~SkinnedDiffuseMaterial_Shadow()
{

}

void SkinnedDiffuseMaterial_Shadow::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkinnedDiffuseMaterial_Shadow::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	//Create a link to the gBones variable
	if (!m_pBoneTransformsVariable)
	{
		m_pBoneTransformsVariable = m_pEffect->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransformsVariable->IsValid())
		{
			if (!m_pDiffuseSRVvariable->IsValid())
			{
				Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gBones\' variable not found!");
				m_pDiffuseSRVvariable = nullptr;
			}
		}
	}

	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pBoneTransformsVariable->IsValid())
				Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightDirection\' variable not found!");
	}

	if (!m_pShadowMapSRVvariable)
	{
		m_pShadowMapSRVvariable = m_pEffect->GetVariableByName("gShadowMap")->AsShaderResource();
		if (!m_pShadowMapSRVvariable->IsValid())
			Logger::LogWarning(L"SkinnedDiffuseMaterial_Shadow::LoadEffectVariables() > \'gShadowMap\' variable not found!");
	}
	m_pLightVPvariable = m_pEffect->GetVariableByName("gLightVP")->AsMatrix();
	if (!m_pLightVPvariable->IsValid())
		Logger::LogWarning(L"DiffuseMaterial_Shadow::LoadEffectVariables() > \'gLightVP\' variable not found!");
}

void SkinnedDiffuseMaterial_Shadow::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	//Set the matrix array (BoneTransforms of the ModelAnimator)
	if (m_pBoneTransformsVariable)
	{
		vector<XMFLOAT4X4> boneTransforms = pModelComponent->GetAnimator()->GetBoneTransforms();
		m_pBoneTransformsVariable->SetMatrixArray(reinterpret_cast<float*>(&boneTransforms[0]), 0, boneTransforms.size());
	}

	m_pLightDirectionVariable->SetFloatVector(reinterpret_cast<float*>(&m_LightDirection));

	m_pShadowMapSRVvariable->SetResource(gameContext.pShadowMapper->GetShadowMap());

	m_LightVP = gameContext.pShadowMapper->GetLightVP();
	m_pLightVPvariable->SetMatrix(reinterpret_cast<float*>(&m_LightVP));
}
