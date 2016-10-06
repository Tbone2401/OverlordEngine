#pragma once
#include "stdafx.h"
#include "Graphics\Material.h"

class TextureData;

class ExplosionMaterial : public Material
{
public:
	ExplosionMaterial();
	~ExplosionMaterial();

	//LIGHT
	//*****
	void SetLightDirection(const XMFLOAT3 direction);

	//DIFFUSE
	//*******
	void SetDiffuseTexture(const wstring& assetFile);

	//NOISEMAP
	//********
	void SetNoiseMap(const wstring& assetFile);

	//NORMALMAP
	//*********
	void SetNormalMap(const wstring& assetFile);

	//EXPLOSIONPARAMETERS
	//*******************
	void SetExplosionPower(const float power);
	void SetLimits(const float lower, const float upper);
	void SetEffectLimits(const float percentage, const float interpolation);

	//TIME
	//****
	void SetTime(const float time);

	//GRAVITY
	//*******
	void SetGravity(const float gravity);
	
	//NOISEMAPINFLUENCE
	//*****************
	void SetNoisemapInfluence(const float noise);
	
	//THICKNESSREDUCTION
	//******************
	void ReduceThickness(const float thickness);
	
	//EXPLOSIONPOS
	//************
	void SetExplosionPos(const XMFLOAT3 pos);

	//START EXPLODING
	//***************
	const void StartExplosion();


protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	//LIGHT
	//*****
	XMFLOAT3 m_LightDirection = { 0.577f, 0.577f, 0.577f };
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;

	//DIFFUSE
	//*******
	static ID3DX11EffectShaderResourceVariable* m_pUseDiffuseTextureVariable;
	TextureData* m_pDiffuseTexture = nullptr;

	//NOISEMAP
	//********
	static ID3DX11EffectShaderResourceVariable* m_pUseNoiseTextureVariable;
	TextureData* m_pNoiseTexture = nullptr;

	//NORMALMAP
	//*********
	static ID3DX11EffectShaderResourceVariable* m_pUseNormalTextureVariable;
	TextureData* m_pNormalTexture = nullptr;

	//EXPLOSIONPARAMETERS
	//*******************
	static ID3DX11EffectScalarVariable* m_pExplosionPower;
	float m_ExplosionPower;

	static ID3DX11EffectScalarVariable *m_pLowerLimit;
	float m_LowerLimit;

	static ID3DX11EffectScalarVariable *m_pUpperLimit;
	float m_UpperLimit;

	static ID3DX11EffectScalarVariable *m_pCompletionPercentage;
	float m_CompletionPercentage;

	static ID3DX11EffectScalarVariable *m_pSoftCompletion;
	float m_SoftCompletion;

	//TIME
	//****
	static ID3DX11EffectScalarVariable *m_pTime;
	float m_Time;

	//GRAVITY
	//*******
	static ID3DX11EffectScalarVariable* m_pGravity;
	float m_Gravity;

	//NOISEMAPINFLUENCE
	//*****************
	static ID3DX11EffectScalarVariable* m_pNoiseMapInfluence;
	float m_NoiseMapInfluence;

	//THICKNESSREDUCTION
	//******************
	static ID3DX11EffectScalarVariable* m_pThicknessReduction;
	float m_ThicknessReduction;

	//EXPLOSIONPOS
	//************
	static ID3DX11EffectVectorVariable* m_pUseCustomExplosionPos;
	XMFLOAT3 m_ExplosionPos;
	bool m_UseCustomExplosionPos;
	bool m_IsExploding;
};

