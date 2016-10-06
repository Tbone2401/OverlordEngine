#pragma once
#include "Scenegraph/GameScene.h"

class SpriteFont;

class TerrainScene : public GameScene
{
public:
	TerrainScene(void);
	virtual ~TerrainScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TerrainScene(const TerrainScene &obj);
	TerrainScene& operator=(const TerrainScene& obj);
};

