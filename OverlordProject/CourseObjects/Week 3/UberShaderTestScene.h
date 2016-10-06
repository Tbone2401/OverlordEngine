#pragma once
#include "Scenegraph/GameScene.h"

class TextureData;

class UberShaderTestScene : public GameScene
{
public:
	UberShaderTestScene(void);
	virtual ~UberShaderTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_FpsInterval;
	TextureData* m_pTexture;
	GameObject* m_pObj;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	UberShaderTestScene(const UberShaderTestScene &obj);
	UberShaderTestScene& operator=(const UberShaderTestScene& obj);
};

