#pragma once
#include "Scenegraph/GameScene.h"

class SpriteFont;
class WorldButton;
class ParticleEmitterComponent;

class MenuSceneFlex : public GameScene
{
public:
	MenuSceneFlex(void);
	virtual ~MenuSceneFlex(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	FlexSolver* m_pSolver;
	FlexMemory* m_pFlexMem;
	FlexMemory* m_pFlexMemUpdate;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MenuSceneFlex(const MenuSceneFlex &obj);
	MenuSceneFlex& operator=(const MenuSceneFlex& obj);
};

