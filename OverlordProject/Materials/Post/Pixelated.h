#pragma once
#include "Graphics/PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class Pixelated : public PostProcessingMaterial
{
public:
	Pixelated();
	~Pixelated();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(RenderTarget* rendertarget);

	static ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Pixelated(const Pixelated &obj);
	Pixelated& operator=(const Pixelated& obj);
};
