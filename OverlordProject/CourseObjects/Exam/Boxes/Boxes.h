#pragma once
#include "Scenegraph/GameObject.h"


class ColliderComponent;
class RigidBodyComponent;
class GameObject;

class Boxes : public GameObject
{
public:
	Boxes(PxVec3 pos, int index);
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	void SetTouched();
	const bool GetTouched();
	void SetDestroy(bool x);
	const bool GetDestroy() const;
	const void StopColliding();
	~Boxes();
protected:
	PxVec3 m_Pos;
	bool m_isDeleted, m_IsTouched, m_HasPlayerJumped;
	PxRigidActor *m_pTopTrigger = nullptr;
	GameObject* m_pBox = nullptr, *m_pTrigger = nullptr;
	ColliderComponent* m_pBoxCollider = nullptr;
	RigidBodyComponent* m_pRB = nullptr;
	float m_Timer = 0.0f;
private:
	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	int m_Index;
	float m_TimerToTrigger = 0.0f;

	Boxes(const Boxes& t);
	Boxes& operator=(const Boxes& t);
	bool m_shouldBeDestroyed = false;
};