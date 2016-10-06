#pragma once
#include "Graphics/Material.h"
#include "Graphics/TextureData.h"

class SelfIlluminatedMaterial final: public Material
{
public:
	SelfIlluminatedMaterial();
	~SelfIlluminatedMaterial();

	void SetDiffuseTexture(const std::wstring &assetFile);

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	TextureData* m_pDiffuseTexture;

	SelfIlluminatedMaterial(const SelfIlluminatedMaterial &obj) = delete;
	SelfIlluminatedMaterial& operator=(const SelfIlluminatedMaterial& obj) = delete;
};

