#pragma once
#include <Graphics/Material.h>

class TextureData;

class SkinnedDiffuseMaterial_Shadow : public Material
{
public:
	SkinnedDiffuseMaterial_Shadow();
	~SkinnedDiffuseMaterial_Shadow();

	void SetDiffuseTexture(const wstring& assetFile);
	void SetLightDirection(const XMFLOAT3 &dir) { m_LightDirection = dir; }

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:

	XMFLOAT3 m_LightDirection;
	XMFLOAT4X4 m_LightVP;

	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	static ID3DX11EffectMatrixVariable* m_pBoneTransformsVariable;
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;
	static ID3DX11EffectShaderResourceVariable* m_pShadowMapSRVvariable;
	static ID3DX11EffectMatrixVariable* m_pLightVPvariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkinnedDiffuseMaterial_Shadow(const SkinnedDiffuseMaterial_Shadow &obj);
	SkinnedDiffuseMaterial_Shadow& operator=(const SkinnedDiffuseMaterial_Shadow& obj);
};

