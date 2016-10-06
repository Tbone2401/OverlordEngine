//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "PostGrayscale.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* PostGrayscale::m_pTextureMapVariabele = nullptr;

PostGrayscale::PostGrayscale()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Grayscale.fx")
{
}

PostGrayscale::~PostGrayscale(void)
{
}

void PostGrayscale::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//Check if valid!
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele->IsValid())
		Logger::LogWarning(L"PostGrayscale: GetVariableByName 'gTexture' not valid!");
}

void PostGrayscale::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
}