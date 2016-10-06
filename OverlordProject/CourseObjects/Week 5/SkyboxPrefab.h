#pragma once
#include "Scenegraph\GameObject.h"
class SkyboxPrefab :
	public GameObject
{
public:
	SkyboxPrefab();
	virtual ~SkyboxPrefab();

	virtual void Initialize(const GameContext& gameContext);
};