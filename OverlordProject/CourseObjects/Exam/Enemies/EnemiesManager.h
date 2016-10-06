#pragma once
#include "Scenegraph/GameObject.h"

class Enemy;

class EnemiesManager : public GameObject
{
public:
	EnemiesManager();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	~EnemiesManager();

	void AddEnemy(XMFLOAT3 pos, XMFLOAT3 left, XMFLOAT3 right);
	int PlayerInteraction();
private:
	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	EnemiesManager(const EnemiesManager& t);
	EnemiesManager& operator=(const EnemiesManager& t);
	void AddMaterial(const GameContext& gameContext);

	std::vector<Enemy*> m_EnemyVector;
	bool m_DidPlayerTouch = false, m_DidPlayerKill = false;
	int m_Index, m_State;
};

