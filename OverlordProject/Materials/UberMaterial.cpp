#include "stdafx.h"
#include "UberMaterial.h"
#include "Graphics/TextureData.h"
#include "Content/ContentManager.h"
#include "Components/Components.h"

ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseDiffuseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable = nullptr;

UberMaterial::UberMaterial() : Material(L"./Resources/Effects/UberShader.fx")
{
}

UberMaterial::~UberMaterial()
{
}

void UberMaterial::SetLightDirection(XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}

void UberMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetDiffuseColor(XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

void UberMaterial::SetSpecularColor(XMFLOAT4 color)
{
	m_ColorSpecular = color;
}

void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}

void UberMaterial::SetSpecularLevelTexture(const wstring& assetFile)
{
	m_pSpecularLevelTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

void UberMaterial::SetAmbientColor(XMFLOAT4 color)
{
	m_ColorAmbient = color;
}

void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

void UberMaterial::FlipNormalGreenCHannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}

void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}

void UberMaterial::SetNormalMapTexture(const wstring& assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}

void UberMaterial::SetEnvironmentCube(const wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}

void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}

void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}

void UberMaterial::SetOpacityTexture(const wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}

void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

void UberMaterial::EnableFresnelFaloff(bool enable)
{
	m_bFresnelFaloff = enable;
}

void UberMaterial::SetFresnelColor(XMFLOAT4 color)
{
	m_ColorFresnel = color;
}

void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}

void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}

void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}

void UberMaterial::LoadEffectVariables()
{
	m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();
	if (!m_pLightDirectionVariable->IsValid())
		Logger::LogError(L"gLightDirection: Get variable failed!");
	m_pUseDiffuseTextureVariable = m_pEffect->GetVariableByName("gUseTextureDiffuse")->AsScalar();
	if (!m_pUseDiffuseTextureVariable->IsValid())
		Logger::LogError(L"gUseTextureDiffuse: Get variable failed!");
	m_pDiffuseColorVariable = m_pEffect->GetVariableByName("gColorDiffuse")->AsVector();
	if (!m_pDiffuseColorVariable->IsValid())
		Logger::LogError(L"gColorDiffuse: Get variable failed!");
	m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gTextureDiffuse")->AsShaderResource();
	if (!m_pDiffuseSRVvariable->IsValid())
		Logger::LogError(L"gTextureDiffuse: Get variable failed!");
	m_pSpecularColorVariable = m_pEffect->GetVariableByName("gColorSpecular")->AsVector();
	if (!m_pSpecularColorVariable->IsValid())
		Logger::LogError(L"gColorSpecular: Get variable failed!");
	m_pSpecularLevelSRVvariable = m_pEffect->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();
	if (!m_pSpecularLevelSRVvariable->IsValid())
		Logger::LogError(L"gTextureSpecularIntensity: Get variable failed!");
	m_pUseSpecularLevelTextureVariable = m_pEffect->GetVariableByName("gUseTextureSpecularIntensity")->AsScalar();
	if (!m_pUseSpecularLevelTextureVariable->IsValid())
		Logger::LogError(L"gUseTextureSpecularIntensity: Get variable failed!");
	m_pShininessVariable = m_pEffect->GetVariableByName("gShininess")->AsScalar();
	if (!m_pShininessVariable->IsValid())
		Logger::LogError(L"gShininess: Get variable failed!");
	m_pAmbientColorVariable = m_pEffect->GetVariableByName("gColorAmbient")->AsVector();
	if (!m_pAmbientColorVariable->IsValid())
		Logger::LogError(L"gColorAmbient: Get variable failed!");
	m_pAmbientIntensityVariable = m_pEffect->GetVariableByName("gAmbientIntensity")->AsScalar();
	if (!m_pAmbientIntensityVariable->IsValid())
		Logger::LogError(L"gAmbientIntensity: Get variable failed!");
	m_pFlipGreenChannelVariable = m_pEffect->GetVariableByName("gFlipGreenChannel")->AsScalar();
	if (!m_pFlipGreenChannelVariable->IsValid())
		Logger::LogError(L"gFlipGreenChannel: Get variable failed!");
	m_pUseNormalMappingVariable = m_pEffect->GetVariableByName("gUseTextureNormal")->AsScalar();
	if (!m_pUseNormalMappingVariable->IsValid())
		Logger::LogError(L"gUseTextureNormal: Get variable failed!");
	m_pNormalMappingSRVvariable = m_pEffect->GetVariableByName("gTextureNormal")->AsShaderResource();
	if (!m_pNormalMappingSRVvariable->IsValid())
		Logger::LogError(L"gTextureNormal: Get variable failed!");
	m_pEnvironmentSRVvariable = m_pEffect->GetVariableByName("gCubeEnvironment")->AsShaderResource();
	if (!m_pEnvironmentSRVvariable->IsValid())
		Logger::LogError(L"gCubeEnvironment: Get variable failed!");
	m_pUseEnvironmentMappingVariable = m_pEffect->GetVariableByName("gUseTextureEnvironment")->AsScalar();
	if (!m_pUseEnvironmentMappingVariable->IsValid())
		Logger::LogError(L"gUseTextureEnvironment: Get variable failed!");
	m_pReflectionStrengthVariable = m_pEffect->GetVariableByName("gReflectionStrength")->AsScalar();
	if (!m_pReflectionStrengthVariable->IsValid())
		Logger::LogError(L"gReflectionStrength: Get variable failed!");
	m_pRefractionStrengthVariable = m_pEffect->GetVariableByName("gRefractionStrength")->AsScalar();
	if (!m_pRefractionStrengthVariable->IsValid())
		Logger::LogError(L"gRefractionStrength: Get variable failed!");
	m_pFresnelMultiplierVariable = m_pEffect->GetVariableByName("gFresnelMultiplier")->AsScalar();
	if (!m_pFresnelMultiplierVariable->IsValid())
		Logger::LogError(L"gFresnelMultiplier: Get variable failed!");
	m_pRefractionIndexVariable = m_pEffect->GetVariableByName("gRefractionIndex")->AsScalar();
	if (!m_pRefractionIndexVariable->IsValid())
		Logger::LogError(L"gRefractionIndex: Get variable failed!");
	m_pUseFresnelFalloffVariable = m_pEffect->GetVariableByName("gUseFresnelFalloff")->AsScalar();
	if (!m_pUseFresnelFalloffVariable->IsValid())
		Logger::LogError(L"gUseFresnelFalloff: Get variable failed!");
	m_pFresnelColorVariable = m_pEffect->GetVariableByName("gColorFresnel")->AsVector();
	if (!m_pFresnelColorVariable->IsValid())
		Logger::LogError(L"gColorFresnel: Get variable failed!");
	m_pFresnelPowerVariable = m_pEffect->GetVariableByName("gFresnelPower")->AsScalar();
	if (!m_pFresnelPowerVariable->IsValid())
		Logger::LogError(L"gFresnelPower: Get variable failed!");
	m_pFresnelPowerVariable = m_pEffect->GetVariableByName("gFresnelMultiplier")->AsScalar();
	if (!m_pFresnelPowerVariable->IsValid())
		Logger::LogError(L"gFresnelMultiplier: Get variable failed!");
	m_pFresnelHardnessVariable = m_pEffect->GetVariableByName("gFresnelHardness")->AsScalar();
	if (!m_pFresnelHardnessVariable->IsValid())
		Logger::LogError(L"gFresnelHardness: Get variable failed!");
	m_pOpacityVariable = m_pEffect->GetVariableByName("gOpacityIntensity")->AsScalar();
	if (!m_pOpacityVariable->IsValid())
		Logger::LogError(L"gOpacityIntensity: Get variable failed!");
	m_pUseOpacityMapVariable = m_pEffect->GetVariableByName("gUseTextureOpacity")->AsScalar();
	if (!m_pUseOpacityMapVariable->IsValid())
		Logger::LogError(L"gUseTextureOpacity: Get variable failed!");
	m_pOpacitySRVvariable = m_pEffect->GetVariableByName("gTextureOpacity")->AsShaderResource();
	if (!m_pOpacitySRVvariable->IsValid())
		Logger::LogError(L"gTextureOpacity: Get variable failed!");
	m_pUseBlinnVariable = m_pEffect->GetVariableByName("gUseSpecularBlinn")->AsScalar();
	if (!m_pUseBlinnVariable->IsValid())
		Logger::LogError(L"gUseSpecularBlinn: Get variable failed!");
	m_pUsePhongVariable = m_pEffect->GetVariableByName("gUseSpecularPhong")->AsScalar();
	if (!m_pUsePhongVariable->IsValid())
		Logger::LogError(L"gUseSpecularPhong: Get variable failed!");
}

void UberMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	m_pLightDirectionVariable->SetFloatVector(reinterpret_cast<float*>(&m_LightDirection));
	m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);
	if (m_pDiffuseTexture != nullptr)
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	m_pDiffuseColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorDiffuse));
	m_pSpecularColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorSpecular));
	m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);
	if (m_pSpecularLevelTexture != nullptr)
		m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());
	m_pShininessVariable->SetInt(m_Shininess);
	m_pAmbientColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorAmbient));
	m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);
	m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);
	m_pUseNormalMappingVariable->SetBool(m_bNormalMapping);
	if(m_pNormalMappingTexture != nullptr)
		m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());
	m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);
	if (m_pEnvironmentCube != nullptr)
		m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());
	m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	m_pRefractionStrengthVariable->SetFloat(m_RefractionStrength);
	m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);
	m_pRefractionIndexVariable->SetFloat(m_Opacity);
	m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);
	if(m_pOpacityMap != nullptr)
		m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());
	m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);
	m_pUsePhongVariable->SetBool(m_bSpecularPhong);
	m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);
	m_pFresnelColorVariable->SetFloatVector(reinterpret_cast<float*>(&m_ColorFresnel));
	m_pFresnelPowerVariable->SetFloat(m_FresnelPower);
	m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);
	m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);
}