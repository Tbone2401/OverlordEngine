#pragma once
#include "Graphics\Material.h"
#include <Graphics/TextureData.h>

class SkyboxMaterial: public Material
{
public:
	SkyboxMaterial();
	~SkyboxMaterial();

	void SetTexture(const wstring assetFile);

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	TextureData* m_pTexture = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pTextureVariable;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyboxMaterial(const SkyboxMaterial &obj);
	SkyboxMaterial& operator=(const SkyboxMaterial& obj);
};

