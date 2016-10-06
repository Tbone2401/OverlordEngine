#include "stdafx.h"
#include "Enemy.h"
#include <Physx/PhysxManager.h>
#include <Components/ModelComponent.h>
#include <Components/RigidBodyComponent.h>
#include "Components/Components.h"
#include <Content/ContentManager.h>
#include "Materials/ColorMaterial.h"
#include "Materials/SkinnedDiffuseMaterial.h"
#include "Graphics\ModelAnimator.h"
#include "Content\ContentManager.h"
#include "Components\TransformComponent.h"
#include "Components\ModelComponent.h"
#include "Graphics\ModelAnimator.h"
#include "Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"


Enemy::Enemy(XMFLOAT3 pos, XMFLOAT3 left, XMFLOAT3 right) :
	m_pEnemy(nullptr),
	m_LeftPoint(left),
	m_RightPoint(right),
	m_Pos(pos),
	m_IsGoingLeft(true),
	m_IsTouched(false),
	m_CollisionGroups(PxFilterData(CollisionGroupFlag::Group0, 0, 0, 0))
{}
void Enemy::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	//********************
	//Material
	//********************
	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial_Shadow();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Vampire_Diffuse.dds");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 200);

	m_pModel = new ModelComponent(L"./Resources/Meshes/Crab_Enemy.ovm");
	m_pModel->SetMaterial(200);
	m_pEnemy = new GameObject();
	m_pEnemy->AddComponent(m_pModel);

	RigidBodyComponent* rb = new RigidBodyComponent();
	m_pEnemy->AddComponent(rb);
	auto defaultMaterial = physX->createMaterial(.5f, .5f, .01f);

	PxConvexMesh* convexMeshCrab = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Crab_Enemy.ovpc");
	shared_ptr<PxGeometry> enemyMesh(new PxConvexMeshGeometry(convexMeshCrab));
	m_pCollider = new ColliderComponent(enemyMesh, *defaultMaterial, PxTransform::createIdentity());
	m_pEnemy->AddComponent(m_pCollider);
	AddChild(m_pEnemy);

	rb->SetDensity(10000.0f);

	m_pTrigger = new GameObject();
	auto triggerRB = new RigidBodyComponent(false);
	triggerRB->SetKinematic(true);
	PxConvexMesh* convexMesh2 = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Crab_Trigger.ovpc");
	shared_ptr<PxGeometry> mesh3(new PxConvexMeshGeometry(convexMesh2));
	ColliderComponent* collider2 = new ColliderComponent(mesh3, *defaultMaterial, PxTransform::createIdentity());
	m_pTrigger->AddComponent(collider2);
	m_pTrigger->AddComponent(triggerRB);
	AddChild(m_pTrigger);
	collider2->EnableTrigger(true);

	m_pTrigger->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* receiver, TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		if (m_TimerToTrigger < 1.0f) { return; }
		if (action == TriggerAction::ENTER)
		{
			if (receiver->GetTag() == L"Player")
			{
				SetTouched();
				Logger::LogInfo(L"Triggered PLAYER");
			}
		}
	});

	//***********************
	//KILLBOX
	//***********************
	m_pTrigger2 = new GameObject();
	auto triggerRBKill = new RigidBodyComponent(false);
	triggerRBKill->SetKinematic(true);
	PxConvexMesh* convexMesh3 = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Crab_KillBox.ovpc");
	shared_ptr<PxGeometry> mesh4(new PxConvexMeshGeometry(convexMesh3));
	ColliderComponent* collider3 = new ColliderComponent(mesh4, *defaultMaterial, PxTransform::createIdentity());
	m_pTrigger2->AddComponent(collider3);
	m_pTrigger2->AddComponent(triggerRBKill);
	AddChild(m_pTrigger2);
	collider3->EnableTrigger(true);

	m_pTrigger2->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* receiver, TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		if (m_TimerToTrigger < 1.0f) { return; }
		if (action == TriggerAction::ENTER)
		{
			if (receiver->GetTag() == L"Player")
			{
				SetHit();
				Logger::LogInfo(L"Triggered PLAYER");
			}
		}
	});
	//rb->SetCollisionGroup((CollisionGroupFlag)m_CollisionGroups.word0);
	//rb->SetCollisionIgnoreGroups((CollisionGroupFlag)m_CollisionGroups.word1);
}

void Enemy::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pEnemy->GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);
	m_pTrigger->GetTransform()->Translate(m_Pos.x, m_Pos.y+4.0f, m_Pos.z);
	//m_pEnemy->GetTransform()->Scale(.1f, .1f, .1f);
	m_pModel->GetAnimator()->SetAnimation(0);
	m_pModel->GetAnimator()->Play();

}

void Enemy::Update(const GameContext& gameContext)
{
	m_IsTouched = false;
	m_TimerToTrigger += gameContext.pGameTime->GetElapsed();

	if(m_IsGoingLeft)
	{
		XMVECTOR pos{m_LeftPoint.x - m_pEnemy->GetTransform()->GetPosition().x,
			m_LeftPoint.y - m_pEnemy->GetTransform()->GetPosition().y ,
			m_LeftPoint.z - m_pEnemy->GetTransform()->GetPosition().z };
		pos = XMVector3Normalize(pos);
		XMFLOAT3 posss{};
		XMStoreFloat3(&posss, pos);
		m_Pos = {m_Pos.x-posss.x*gameContext.pGameTime->GetElapsed()*m_Speed
			,m_Pos.y - posss.y*gameContext.pGameTime->GetElapsed()*m_Speed,
			m_Pos.z - posss.z*gameContext.pGameTime->GetElapsed()*m_Speed };
		m_pEnemy->GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);
		m_pTrigger->GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);
		m_pTrigger2->GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);

	}
	//m_pEnemy->GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);
	/*wstring string = to_wstring(m_pEnemy->GetTransform()->GetPosition().x) + L" : "
		+ to_wstring(m_pEnemy->GetTransform()->GetPosition().y) + L" : "
		+ to_wstring(m_pEnemy->GetTransform()->GetPosition().z);
	Logger::LogInfo(string);*/

}
Enemy::~Enemy()
{
	Logger::LogInfo(L"Destroyed");
}

void Enemy::SetTouched()
{
	m_IsTouched = true;
}

void Enemy::SetUntouched()
{
	m_IsTouched = false;
}

const bool Enemy::GetTouched() const
{
	return m_IsTouched;
}
XMFLOAT3 operator-(const XMFLOAT3 &a, const XMFLOAT3 &b)
{
	XMFLOAT3 o;
	o.x = a.x - b.x;
	o.y = a.y - b.y;
	o.z = a.z - b.z;
	return o;
}
void Enemy::SetHit()
{
	m_HitPlayer = true;
}
bool Enemy::DidDamage()
{
	if(m_HitPlayer)
	{
		m_HitPlayer = false;
		return true;
	}
	return false;
}