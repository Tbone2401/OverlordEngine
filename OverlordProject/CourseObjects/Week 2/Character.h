#pragma once
#include "Scenegraph/GameObject.h"

class ModelComponent;
class ControllerComponent;
class CameraComponent;
class FixedCamera;
class FreeCamera;

class Character : public GameObject
{
public:
	enum CharacterMovement : UINT
	{
		LEFT = 0,
		RIGHT,
		FORWARD,
		BACKWARD,
		JUMP
	};

	Character(float radius = 2, float height = 5, float moveSpeed = 100);
	virtual ~Character();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	void GiveUpWardsImpulse();
	const XMFLOAT3 GetShadowCameraPosition() const;
	const void SubtractHealth();
	const bool IsHurting() const;
	void CreateCloth();
	float GetHeight() const;
	bool IsDead() const;
protected:

	CameraComponent* m_pCamera;
	ControllerComponent* m_pController;
	ModelComponent* m_pModel;

	float m_TotalPitch, m_TotalYaw;
	float m_MoveSpeed, m_RotationSpeed;
	float m_Radius, m_Height;
	float m_FlyTime = 0.0f;
	float m_RegenTimer = 0.0f;

	//Running
	float m_MaxRunVelocity, 
		m_TerminalVelocity,
		m_Gravity, 
		m_RunAccelerationTime, 
		m_JumpAccelerationTime, 
		m_RunAcceleration, 
		m_JumpAcceleration, 
		m_RunVelocity, 
		m_JumpVelocity,
		m_timer;

	XMFLOAT3 m_Velocity;
	FreeCamera* m_pCameraFixed;
	
	bool m_ShouldReactToBox = false, m_IsDead = false;

	int m_Points = 0, m_HitPoints = 2;

	//Scarf
	PxU32* m_pTriangleIndices;
	int m_nrOfIndices;
	PxCloth* m_pScarf;
	GameObject* m_pMesh, *m_pScarfMesh, *m_pCharacter, *m_pAnimMesh, *m_pCameraObj;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Character(const Character& t);
	Character& operator=(const Character& t);
};


