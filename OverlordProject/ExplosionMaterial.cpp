#include "stdafx.h"
#include "ExplosionMaterial.h"
#include <Content/ContentManager.h>
#include "Graphics/TextureData.h"

ID3DX11EffectVectorVariable* ExplosionMaterial::m_pLightDirectionVariable = nullptr;
ID3DX11EffectShaderResourceVariable* ExplosionMaterial::m_pUseDiffuseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* ExplosionMaterial::m_pUseNoiseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* ExplosionMaterial::m_pUseNormalTextureVariable = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pExplosionPower = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pLowerLimit = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pUpperLimit = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pCompletionPercentage = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pSoftCompletion = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pTime = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pGravity = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pNoiseMapInfluence = nullptr;
ID3DX11EffectScalarVariable* ExplosionMaterial::m_pThicknessReduction = nullptr;
ID3DX11EffectVectorVariable* ExplosionMaterial::m_pUseCustomExplosionPos = nullptr;

ExplosionMaterial::ExplosionMaterial() : Material(L"./Resources/Effects/Explosion.fx"),
	m_IsExploding(false)
{
}


ExplosionMaterial::~ExplosionMaterial()
{

}

void ExplosionMaterial::LoadEffectVariables()
{
	m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
	m_pUseDiffuseTextureVariable = m_pEffect->GetVariableByName("gTextureDiffuse")->AsShaderResource();
	m_pUseNoiseTextureVariable = m_pEffect->GetVariableByName("gNoiseMap")->AsShaderResource();
	m_pUseNormalTextureVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
	m_pExplosionPower = m_pEffect->GetVariableByName("gExplosionPower")->AsScalar();
	m_pLowerLimit = m_pEffect->GetVariableByName("gLowerLimit")->AsScalar();
	m_pUpperLimit = m_pEffect->GetVariableByName("gUpperLimit")->AsScalar();
	m_pCompletionPercentage = m_pEffect->GetVariableByName("gCompletionPercentage")->AsScalar();
	m_pSoftCompletion = m_pEffect->GetVariableByName("gSoftCompletion")->AsScalar();
	m_pTime = m_pEffect->GetVariableByName("gTime")->AsScalar();
	m_pGravity = m_pEffect->GetVariableByName("gGravity")->AsScalar();
	m_pNoiseMapInfluence = m_pEffect->GetVariableByName("gNoiseMapInfluence")->AsScalar();
	m_pThicknessReduction = m_pEffect->GetVariableByName("gchunkThickness")->AsScalar();
	m_pUseCustomExplosionPos = m_pEffect->GetVariableByName("gCustomExplosionPosition")->AsVector();
}
void ExplosionMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	m_pLightDirectionVariable->SetFloatVector(reinterpret_cast<float*>(&m_LightDirection));
	m_pUseDiffuseTextureVariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	m_pUseNoiseTextureVariable->SetResource(m_pNoiseTexture->GetShaderResourceView());
	m_pUseNormalTextureVariable->SetResource(m_pNormalTexture->GetShaderResourceView());
	m_pExplosionPower->SetFloat(m_ExplosionPower);
	m_pLowerLimit->SetFloat(m_LowerLimit);
	m_pUpperLimit->SetFloat(m_UpperLimit);
	m_pCompletionPercentage->SetFloat(m_CompletionPercentage);
	m_pSoftCompletion->SetFloat(m_SoftCompletion);
	if(m_IsExploding)
		m_Time += gameContext.pGameTime->GetElapsed();
	
	m_pTime->SetFloat(m_Time);
	m_pGravity->SetFloat(m_Gravity);
	m_pNoiseMapInfluence->SetFloat(m_NoiseMapInfluence);
	m_pThicknessReduction->SetFloat(m_ThicknessReduction);
	m_pUseCustomExplosionPos->SetFloatVector(reinterpret_cast<float*>(&m_UseCustomExplosionPos));

}


void ExplosionMaterial::SetLightDirection(const XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

void ExplosionMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void ExplosionMaterial::SetNoiseMap(const wstring& assetFile)
{
	m_pNoiseTexture = ContentManager::Load<TextureData>(assetFile);
}

void ExplosionMaterial::SetNormalMap(const wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);
}

void ExplosionMaterial::SetExplosionPower(const float power)
{
	m_ExplosionPower = power;
}

void ExplosionMaterial::SetLimits(const float lower, const float upper)
{
	m_LowerLimit = lower;
	m_UpperLimit = upper;
}

void ExplosionMaterial::SetEffectLimits(const float percentage, const float interpolation)
{
	m_CompletionPercentage = percentage;
	m_SoftCompletion = interpolation;
}

void ExplosionMaterial::SetTime(const float time)
{
	m_Time = time;
}

void ExplosionMaterial::SetGravity(const float gravity)
{
	m_Gravity = gravity;
}

void ExplosionMaterial::SetNoisemapInfluence(const float noise)
{
	m_NoiseMapInfluence = noise;
}

void ExplosionMaterial::ReduceThickness(const float thickness)
{
	m_ThicknessReduction = thickness;
}

void ExplosionMaterial::SetExplosionPos(const XMFLOAT3 pos)
{
	m_ExplosionPos = pos;
}

const void ExplosionMaterial::StartExplosion()
{
	m_IsExploding = true;
}
