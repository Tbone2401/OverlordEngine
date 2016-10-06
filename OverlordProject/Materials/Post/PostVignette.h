#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostVignette : public PostProcessingMaterial
{
public:
	PostVignette();
	~PostVignette();

	//STRENGTH
	//****
	void SetStrength(const float strength);
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	//STRENGTH
	//****
	static ID3DX11EffectScalarVariable *m_pStrength;
	float m_Strength;
	static ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	PostVignette(const PostVignette &obj);
	PostVignette& operator=(const PostVignette& obj);
};
