//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "PostBlur.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* PostBlur::m_pTextureMapVariabele = nullptr;

PostBlur::PostBlur()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Blur.fx")
{
}

PostBlur::~PostBlur(void)
{

}

void PostBlur::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	//Check if valid!
	if (!m_pTextureMapVariabele->IsValid())
		Logger::LogWarning(L"PostBlur: GetVariableByName 'gTexture' not valid!");
}

void PostBlur::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	rendertarget->GetDepthStencilView();
}