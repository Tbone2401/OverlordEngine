//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "PostVignette.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* PostVignette::m_pTextureMapVariabele = nullptr;
ID3DX11EffectScalarVariable* PostVignette::m_pStrength = nullptr;

PostVignette::PostVignette()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Vignette.fx")
{
}

PostVignette::~PostVignette(void)
{
}

void PostVignette::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	//Check if valid!
	if (!m_pTextureMapVariabele->IsValid())
		Logger::LogWarning(L"PostVignette: GetVariableByName 'gTexture' not valid!");

	m_pStrength = m_pEffect->GetVariableByName("gFalloff")->AsScalar();
	if (!m_pStrength->IsValid())
		Logger::LogWarning(L"PostVignette: GetVariableByName 'gFalloff' not valid!");

}

void PostVignette::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	rendertarget->GetDepthStencilView();
	m_pStrength->SetFloat(m_Strength);
}

void PostVignette::SetStrength(const float strength)
{
	m_Strength = strength;
}
