#include "stdafx.h"
#include "WorldButton.h"
#include <Physx/PhysxManager.h>
#include "Components/Components.h"

#include <Components/RigidBodyComponent.h>

#include "Prefabs/CubePrefab.h"
#include <Content/ContentManager.h>
#include "Materials/ColorMaterial.h"

int WorldButton::m_sIndex = 0;

WorldButton::WorldButton(XMFLOAT3 pos, wstring name, XMFLOAT3 rot)
	: m_Pos(pos),
	m_Rot(rot),
	m_Name(name),
	m_pButton(nullptr),
	m_pMat(nullptr),
	m_ActiveCol(1.0f,0.0f,0.0f),
	m_InactiveCol(0.0f,0.0f,0.0f)
{
}

void WorldButton::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	PxPhysics* physX = PhysxManager::GetInstance()->GetPhysics();
	PxMaterial* defaultMaterial = physX->createMaterial(0, 0, 0);

	m_pMat = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(m_pMat, 150 + m_sIndex);


	m_pButton = new GameObject();
	ModelComponent *startComp = new ModelComponent(L"./Resources/Meshes/" + m_Name + L".ovm");
	m_pButton->AddComponent(startComp);
	startComp->SetMaterial(150 + m_sIndex);

	m_PrivateIndex = 150 + m_sIndex;
	++m_sIndex;

	PxConvexMesh* convMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/" + m_Name + L".ovpc");
	shared_ptr<PxGeometry> mesh(new PxConvexMeshGeometry(convMesh));
	ColliderComponent* collider = new ColliderComponent(mesh, *defaultMaterial, PxTransform::createIdentity());
	m_pButton->AddComponent(collider);

	RigidBodyComponent * rbStart = new RigidBodyComponent();
	rbStart->SetKinematic(true);
	rbStart->SetDensity(5000.0f);
	m_pButton->AddComponent(rbStart);

	AddChild(m_pButton);
}

void WorldButton::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pButton->GetTransform()->Rotate(m_Rot);
	m_pButton->GetTransform()->Translate(m_Pos);
}

void WorldButton::Update(const GameContext& gameContext)
{
	if (gameContext.pCamera->Pick(gameContext) == m_pButton)
	{
		m_pMat->ChangeColor(m_ActiveCol);
	}
	else
	{
		m_pMat->ChangeColor(m_InactiveCol);
	}
}

void WorldButton::SetActiveColor(const XMFLOAT3 col)
{
	m_ActiveCol = col;
}

void WorldButton::SetInactiveColor(const XMFLOAT3 col)
{
	m_InactiveCol = col;
}

void WorldButton::SetActive(bool active)
{
	m_IsActive = active;
}

bool WorldButton::GetActive()
{
	return m_IsActive;
}

void WorldButton::MoveAndDisable()
{
	m_pButton->GetTransform()->Translate(m_Pos.x, m_Pos.y + 100.0f, m_Pos.z);
	m_IsActive = false;
	m_pMat->DisableRender();
}

void WorldButton::MoveAndEnable()
{
	m_pButton->GetTransform()->Translate(m_Pos.x, m_Pos.y, m_Pos.z);
	m_IsActive = true;
	m_pMat->EnableRender();
}

GameObject * WorldButton::ReturnGameObj()
{
	return m_pButton;
}

WorldButton::~WorldButton()
{
}
