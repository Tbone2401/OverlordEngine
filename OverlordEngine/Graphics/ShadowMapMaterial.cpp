#include "stdafx.h"
#include "ShadowMapMaterial.h"
#include "../Components/ModelComponent.h"
#include "ModelAnimator.h"

ID3DX11EffectMatrixVariable* ShadowMapMaterial::m_pLightVPMatrixVariable = nullptr;
ID3DX11EffectMatrixVariable* ShadowMapMaterial::m_pWorldMatrixVariable = nullptr;
ID3DX11EffectScalarVariable* ShadowMapMaterial::m_pHasBonesVariable = nullptr;
ID3DX11EffectMatrixVariable* ShadowMapMaterial::m_pBoneTransformsVariable = nullptr;

ShadowMapMaterial::ShadowMapMaterial():
	Material(L"./Resources/Effects/Shadow/ShadowMap.fx", L"Default")
{
}


ShadowMapMaterial::~ShadowMapMaterial()
{
}

void ShadowMapMaterial::SetLight(const XMFLOAT4X4& lightVP)
{
	m_LightVP = lightVP;
}

void ShadowMapMaterial::SetWorld(const XMFLOAT4X4& world)
{
	m_WorldMatrix = world;
}

void ShadowMapMaterial::EnableBones(const bool enabled)
{
	m_HasBones = enabled;
}

void ShadowMapMaterial::LoadEffectVariables()
{
	m_pLightVPMatrixVariable = m_pEffect->GetVariableByName("gLightVP")->AsMatrix();
	if(!m_pLightVPMatrixVariable->IsValid())
		Logger::LogWarning(L"ShadowMapMaterial: GetVariableByName 'gLightVP' not valid!");

	m_pWorldMatrixVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	if (!m_pWorldMatrixVariable->IsValid())
		Logger::LogWarning(L"ShadowMapMaterial: GetVariableByName 'gWorld' not valid!");

	m_pBoneTransformsVariable = m_pEffect->GetVariableByName("gBones")->AsMatrix();
	if (!m_pBoneTransformsVariable->IsValid())
		Logger::LogWarning(L"ShadowMapMaterial: GetVariableByName 'gBones' not valid!");

	m_pHasBonesVariable = m_pEffect->GetVariableByName("gHasBones")->AsScalar();
	if (!m_pBoneTransformsVariable->IsValid())
		Logger::LogWarning(L"ShadowMapMaterial: GetVariableByName 'gHasBones' not valid!");
}

void ShadowMapMaterial::UpdateVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(pModelComponent);
	UNREFERENCED_PARAMETER(gameContext);
	m_pWorldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&m_WorldMatrix));
	m_pLightVPMatrixVariable->SetMatrix(reinterpret_cast<float*>(&m_LightVP));

	if (m_HasBones)
	{
		vector<XMFLOAT4X4> boneTransforms = pModelComponent->GetAnimator()->GetBoneTransforms();
		m_pBoneTransformsVariable->SetMatrixArray(reinterpret_cast<float*>(&boneTransforms[0]), 0, boneTransforms.size());
	}
	m_pHasBonesVariable->SetBool(m_HasBones);
}