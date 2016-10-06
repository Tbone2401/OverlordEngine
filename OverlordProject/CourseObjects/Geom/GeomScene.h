#pragma once
#include "Scenegraph/GameScene.h"

class SpriteFont;
class WorldButton;
class ParticleEmitterComponent;
class GameObject;
class ExplosionMaterial;

class GeomScene : public GameScene
{
public:
	GeomScene(void);
	virtual ~GeomScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	GameObject * m_pTeapot = nullptr;
	SpriteFont* m_pSpriteFont = nullptr;
	ExplosionMaterial* m_pExplosionMat = nullptr;
	float m_Timer = 0.0f, m_Strength = 1.0f, m_Gravity = -9.81f;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GeomScene(const GeomScene &obj);
	GeomScene& operator=(const GeomScene& obj);
};

