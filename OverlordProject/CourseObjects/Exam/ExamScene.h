#pragma once
#include "Scenegraph/GameScene.h"

class EnemiesManager;
class BoxManager;
class Character;
class SpriteFont;
class ClothPrefab;
class Enemy;
class Chromatic;
class PostVignette;
class Pixelated;
class PostGrayscale;
class FileReader;
class Timer;

class ExamScene : public GameScene
{
public:
	ExamScene(bool isOldSchool);
	virtual ~ExamScene(void);


protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);
private:
	ClothPrefab *m_pCloth;
	GameObject *m_pLevel;
	EnemiesManager *m_pEnemiesManager;
	BoxManager *m_pBoxManager;
	Character* m_pCharacter;
	SpriteFont* m_pSpriteFont;
	Chromatic* m_pChromaticPP;
	PostVignette* m_pPostVignettePP;
	Pixelated * m_pPixelPP = nullptr;
	PostGrayscale * m_pGrayPP = nullptr;
	FileReader * m_pFileReader = nullptr;
	Timer * m_pTimer = nullptr;
	bool m_IsOldSchool = false, m_IsRead = false;
};

