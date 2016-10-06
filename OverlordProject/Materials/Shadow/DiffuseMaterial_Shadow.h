#pragma once
#include "Graphics/TextureData.h"
#include <Graphics/Material.h>

class DiffuseMaterial_Shadow final: public Material
{
public:
	DiffuseMaterial_Shadow();
	~DiffuseMaterial_Shadow();

	void SetDiffuseTexture(const std::wstring &assetFile);
	void SetLightDirection(const XMFLOAT3 &dir) { m_LightDirection = dir; }

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	XMFLOAT3 m_LightDirection;
	TextureData* m_pDiffuseTexture;
	XMFLOAT4X4 m_LightVP;

	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;
	static ID3DX11EffectShaderResourceVariable* m_pShadowMapSRVvariable;
	static ID3DX11EffectMatrixVariable* m_pLightVPvariable;

	DiffuseMaterial_Shadow(const DiffuseMaterial_Shadow &obj) = delete;
	DiffuseMaterial_Shadow& operator=(const DiffuseMaterial_Shadow& obj) = delete;
};

