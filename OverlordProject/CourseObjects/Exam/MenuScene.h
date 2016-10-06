#pragma once
#include "Scenegraph/GameScene.h"

class SpriteFont;
class WorldButton;
class ParticleEmitterComponent;

class MenuScene : public GameScene
{
public:
	MenuScene(void);
	virtual ~MenuScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	bool m_Picked = false, m_IsOldSchool = false, m_DidChange = true;
	SpriteFont* m_pSpriteFont = nullptr;
	WorldButton *m_pStart = nullptr,
		*m_pQuit = nullptr,
		*m_pArrow1 = nullptr,
		*m_pArrow2 = nullptr,
		*m_pModern = nullptr,
		*m_pOldschool = nullptr;
	ParticleEmitterComponent* m_pParticleEmitterRight = nullptr, *m_pParticleEmitterLeft = nullptr;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MenuScene(const MenuScene &obj);
	MenuScene& operator=(const MenuScene& obj);
};

