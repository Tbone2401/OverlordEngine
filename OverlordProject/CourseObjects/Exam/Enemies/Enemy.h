#pragma once

#include "Scenegraph/GameObject.h"

class ColliderComponent;
class ModelComponent;

class Enemy : public GameObject
{
public:
	Enemy(XMFLOAT3 pos, XMFLOAT3 left, XMFLOAT3 right);
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	~Enemy();
	void SetTouched();
	void SetUntouched();
	const bool IsKilled() const;
	void SetDamageToPlayer();
	bool DidDamage();
	float FastDistance(const XMFLOAT3 point1, const XMFLOAT3 point2) const;

protected:
	GameObject* m_pEnemy, *m_pTrigger, *m_pTrigger2;
	XMFLOAT3 m_LeftPoint, m_RightPoint, m_Pos;
	ColliderComponent* m_pCollider;
	ModelComponent* m_pModel;
	bool m_IsGoingLeft, m_IsTouched, m_DamagedPlayer;
	PxFilterData m_CollisionGroups;
	float m_TimerToTrigger = 0.0f, m_Speed = 4.0f;
	int m_Sign = 1;
private:
	Enemy(const Enemy& t);
	Enemy& operator=(const Enemy& t);
};

