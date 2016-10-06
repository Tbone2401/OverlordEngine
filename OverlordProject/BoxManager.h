#pragma once

#include "Scenegraph/GameObject.h"

class Boxes;

class BoxManager : public GameObject
{
public:
	BoxManager();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	~BoxManager();
	const bool DidPlayerTouchBox();

	void AddBox(PxVec3 pos, bool isRand, const GameContext& gameContext);
private:
	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	BoxManager(const BoxManager& t);
	BoxManager& operator=(const BoxManager& t);
	void AddMaterial(const GameContext& gameContext);

	std::vector<Boxes*> m_BoxArray;
	bool m_DidPlayerTouchBox = false, m_HasAsked = false;
	int m_Index;
};
