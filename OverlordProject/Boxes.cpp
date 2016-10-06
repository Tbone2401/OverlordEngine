#include "stdafx.h"
#include "Boxes.h"
#include <Physx/PhysxManager.h>
#include "Components/Components.h"

#include <Components/RigidBodyComponent.h>

#include "Prefabs/CubePrefab.h"
#include <Content/ContentManager.h>
#include "Materials/ColorMaterial.h"
#include "ExplosionMaterial.h"


Boxes::Boxes(PxVec3 pos, int index) : m_Pos(pos), m_isDeleted(false), m_IsTouched(false), m_HasPlayerJumped(false), m_Timer(0.0f), m_Index(index)
{

}

void Boxes::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	m_pBox = new GameObject();
	ModelComponent *comp = new ModelComponent(L"./Resources/Meshes/Crate.ovm");
	m_pBox->AddComponent(comp);

	comp->SetMaterial(m_Index);


	m_pRB = new RigidBodyComponent();
	m_pRB->SetKinematic(true);
	m_pBox->AddComponent(m_pRB);
	auto defaultMaterial = physX->createMaterial(.5f, .5f, .01f);

	PxConvexMesh* convexMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Crate.ovpc");
	shared_ptr<PxGeometry> boxMesh(new PxConvexMeshGeometry(convexMesh));
	m_pBoxCollider = new ColliderComponent(boxMesh, *defaultMaterial, PxTransform::createIdentity());

	m_pBox->AddComponent(m_pBoxCollider);

	m_pRB->SetDensity(5000.0f);


	m_pTrigger = new GameObject();
	auto triggerRB = new RigidBodyComponent(false);
	triggerRB->SetKinematic(true);
	PxConvexMesh* convexMesh2 = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/Crate_Trigger.ovpc");
	shared_ptr<PxGeometry> mesh2(new PxConvexMeshGeometry(convexMesh2));
	ColliderComponent* collider2 = new ColliderComponent(mesh2, *defaultMaterial, PxTransform::createIdentity());
	m_pTrigger->AddComponent(collider2);
	m_pTrigger->AddComponent(triggerRB);
	AddChild(m_pTrigger);

	m_pTrigger->SetOnTriggerCallBack([this](GameObject* trigger, GameObject* receiver, TriggerAction action)
	{
		UNREFERENCED_PARAMETER(trigger);
		if (m_TimerToTrigger < 1.0f) { return; }
		if (action == TriggerAction::ENTER)
		{
			if(receiver->GetTag() == L"Player")
			{
				SetTouched();
				Logger::LogInfo(L"Triggered PLAYER");
			}
		}
	});

	//m_pTrigger->AddComponent(comp2);

	AddChild(m_pBox);
	
	collider2->EnableTrigger(true);

}

void Boxes::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pBox->GetTransform()->Translate(m_Pos.x,m_Pos.y, m_Pos.z);
	m_pTrigger->GetTransform()->Translate(m_Pos.x, m_Pos.y + 1.0f, m_Pos.z); //plus 1.0f otherwise no trigger happens
}

void Boxes::Update(const GameContext& gameContext)
{
	if(m_IsTouched)
	m_Timer += gameContext.pGameTime->GetElapsed();
	
	m_TimerToTrigger += gameContext.pGameTime->GetElapsed();
	if (m_Timer >= 5.0f)
	{
		m_isDeleted = true;
	}
}

void Boxes::SetTouched()
{
	m_IsTouched = true;
}

const bool Boxes::GetTouched()
{
	return m_IsTouched;
}

void Boxes::SetDestroy(bool x)
{
	if (m_Timer > 1.0f)
	{
		Logger::LogInfo(L"Will be destroyed");
		m_isDeleted = x;
	}
}

const bool Boxes::GetDestroy() const
{
	return m_isDeleted;
}

const void Boxes::StopColliding()
{
	m_pBoxCollider->EnableTrigger(true);
}

Boxes::~Boxes()
{
}
