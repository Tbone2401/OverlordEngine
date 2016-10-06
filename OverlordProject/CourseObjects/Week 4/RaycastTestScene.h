#pragma once
#include "Scenegraph/GameScene.h"

class SpriteFont;

class RaycastTestScene : public GameScene
{
public:
	RaycastTestScene(void);
	virtual ~RaycastTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	bool m_Picked = false;
	SpriteFont* m_pSpriteFont = nullptr;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	RaycastTestScene(const RaycastTestScene &obj);
	RaycastTestScene& operator=(const RaycastTestScene& obj);
};

