#pragma once
#include "Graphics\Material.h"

class ColorMaterial: public Material
{
public:
	ColorMaterial(bool enableTransparency = false);
	~ColorMaterial();
	void ChangeColor(XMFLOAT3 col);
	void DisableRender();
	void EnableRender();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	ID3DX11EffectVectorVariable * m_pColor;
	ID3DX11EffectScalarVariable * m_pAlpha;
	XMFLOAT3 m_color;
	float m_Alpha = 1.0f;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ColorMaterial(const ColorMaterial &obj);
	ColorMaterial& operator=(const ColorMaterial& obj);
};

