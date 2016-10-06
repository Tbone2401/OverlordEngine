//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "RaycastTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics\SpriteRenderer.h"
#include "Graphics\TextRenderer.h"

#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components\SpriteComponent.h"
#include "Components\TransformComponent.h"
#include "Graphics\MeshFilter.h"

#include "Components/Components.h"
#include <Materials/UberMaterial.h>
#include <Prefabs/CubePrefab.h>
#include <Physx/PhysxManager.h>

#define FPS_COUNTER 1

RaycastTestScene::RaycastTestScene(void) :
GameScene(L"RaycastTestScene")
{
}


RaycastTestScene::~RaycastTestScene(void)
{
}

void RaycastTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	PxPhysics* physX = PhysxManager::GetInstance()->GetPhysics();
	PxMaterial* defaultMaterial = physX->createMaterial(0, 0, 0);

	GameObject* cube = new CubePrefab(10,10,10);
	shared_ptr<PxGeometry> geom (new PxBoxGeometry(5, 5, 5));
	ColliderComponent* collider = new ColliderComponent(geom, *defaultMaterial, PxTransform::createIdentity());;
	cube->AddComponent(collider);

	RigidBodyComponent* rb = new RigidBodyComponent(true);
	cube->AddComponent(rb);
	AddChild(cube);

	gameContext.pInput->AddInputAction(InputAction(0, Pressed, -1, VK_RBUTTON));

	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"./Resources/Fonts/Consolas_32.fnt");
}

void RaycastTestScene::Update(const GameContext& gameContext)
{
	if(gameContext.pInput->IsActionTriggered(0))
	{
		if (gameContext.pCamera->Pick(gameContext) != nullptr)
			m_Picked = true;
		else
			m_Picked = false;
	}
}

void RaycastTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"TextRenderer:", XMFLOAT2(50, 50), (XMFLOAT4)Colors::White);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Hello World!", XMFLOAT2(50, 80), (XMFLOAT4)Colors::Blue);

	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Right click to pick.", XMFLOAT2(50, 600), (XMFLOAT4)Colors::White);
	if(m_Picked)
		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Object picked.", XMFLOAT2(50, 640), (XMFLOAT4)Colors::Red);
}
