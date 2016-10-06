#pragma once
#include "Graphics/Material.h"
#include "Graphics/TextureData.h"

class DiffuseMaterial final: public Material
{
public:
	DiffuseMaterial();
	~DiffuseMaterial();

	void SetDiffuseTexture(const std::wstring &assetFile);

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	TextureData* m_pDiffuseTexture;

	DiffuseMaterial(const DiffuseMaterial &obj) = delete;
	DiffuseMaterial& operator=(const DiffuseMaterial& obj) = delete;
};

