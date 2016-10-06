#pragma once
#include "Material.h"
class ShadowMapMaterial : public Material
{
public:
	ShadowMapMaterial();
	~ShadowMapMaterial();

	void SetLight(const XMFLOAT4X4& lightVP);
	void SetWorld(const XMFLOAT4X4& world);
	void EnableBones(const bool enabled);
	void UpdateVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

protected:
	void LoadEffectVariables();
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent){
		UNREFERENCED_PARAMETER(gameContext);
		UNREFERENCED_PARAMETER(pModelComponent);
	}

private:
	static ID3DX11EffectMatrixVariable* m_pLightVPMatrixVariable;
	static ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable;
	static ID3DX11EffectScalarVariable* m_pHasBonesVariable;
	static ID3DX11EffectMatrixVariable* m_pBoneTransformsVariable;

	XMFLOAT4X4 m_LightVP;
	XMFLOAT4X4 m_WorldMatrix;
	bool m_HasBones;

	ShadowMapMaterial& operator=(const ShadowMapMaterial& other);
	ShadowMapMaterial(const ShadowMapMaterial& other);
};