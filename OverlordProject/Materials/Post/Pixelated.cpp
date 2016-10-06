//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "Pixelated.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* Pixelated::m_pTextureMapVariabele = nullptr;

Pixelated::Pixelated()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Pixelated.fx")
{
}

Pixelated::~Pixelated(void)
{

}

void Pixelated::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	//Check if valid!
	if (!m_pTextureMapVariabele->IsValid())
		Logger::LogWarning(L"Pixelated: GetVariableByName 'gTexture' not valid!");
}

void Pixelated::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	rendertarget->GetDepthStencilView();
}