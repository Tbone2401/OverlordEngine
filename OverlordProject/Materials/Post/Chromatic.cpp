//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "Chromatic.h"
#include "Graphics/RenderTarget.h"

ID3DX11EffectShaderResourceVariable* Chromatic::m_pTextureMapVariabele = nullptr;
ID3DX11EffectScalarVariable* Chromatic::m_pTime = nullptr;

Chromatic::Chromatic()
	: PostProcessingMaterial(L"./Resources/Effects/Post/ChromaticAbberation.fx"),
	m_Time(0.0f)
{
}

Chromatic::~Chromatic(void)
{
}

void Chromatic::LoadEffectVariables()
{
	//Bind the 'gTexture' variable with 'm_pTextureMapVariable'
	//m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	m_pTextureMapVariabele = m_pEffect->GetVariableByName("gTexture")->AsShaderResource();
	//Check if valid!
	if (!m_pTextureMapVariabele->IsValid())
		Logger::LogWarning(L"Chromatic: GetVariableByName 'gTexture' not valid!");

	m_pTime = m_pEffect->GetVariableByName("gTime")->AsScalar();
	if (!m_pTime->IsValid())
		Logger::LogWarning(L"Chromatic: GetVariableByName 'gTime' not valid!");

}

void Chromatic::UpdateEffectVariables(RenderTarget* rendertarget)
{
	//Update the TextureMapVariable with the Color ShaderResourceView of the given RenderTarget
	m_pTextureMapVariabele->SetResource(rendertarget->GetShaderResourceView());
	rendertarget->GetDepthStencilView();
	m_pTime->SetFloat(m_Time);
}
void Chromatic::UpdateTime(const float delta)
{
	m_Time += delta;
}
void Chromatic::SetTime(const float delta)
{
	m_Time = delta;
}
