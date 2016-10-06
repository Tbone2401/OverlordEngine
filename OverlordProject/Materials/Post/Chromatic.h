#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class Chromatic : public PostProcessingMaterial
{
public:
	Chromatic();
	~Chromatic();


	//TIME
	//****
	void SetTime(const float delta);
	void UpdateTime(const float delta);
protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);


	//TIME
	//****
	static ID3DX11EffectScalarVariable *m_pTime;
	float m_Time;
	static ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Chromatic(const Chromatic &obj);
	Chromatic& operator=(const Chromatic& obj);
};
