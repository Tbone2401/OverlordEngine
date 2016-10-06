#pragma once
#include "..\Scenegraph\GameObject.h"

class QuadPrefab : public GameObject
{
public:
	QuadPrefab(float width = 1.0f, float height = 1.0f, XMFLOAT4 color = (XMFLOAT4)Colors::Red);
	~QuadPrefab();

	void Initialize(const GameContext& gameContext);
private:
	float m_Width, m_Height;
	XMFLOAT4 m_Color;
};

