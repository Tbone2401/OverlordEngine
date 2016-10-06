//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TerrainScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components/Components.h"
#include <Physx/PhysxManager.h>
#include "Physx/PhysxProxy.h"
#include "SkyboxPrefab.h"
#include <Prefabs/SpherePrefab.h>
#include <CourseObjects/Week 2/Character.h>

#define FPS_COUNTER 1

TerrainScene::TerrainScene(void) :
GameScene(L"TerrainScene")
{
}


TerrainScene::~TerrainScene(void)
{
}

void TerrainScene::Initialize(const GameContext& gameContext)
{
	//GetPhysxProxy()->EnablePhysxDebugRendering(true);

	UNREFERENCED_PARAMETER(gameContext);
	GameObject *terrain = new GameObject();
	TerrainComponent* terrainComponent = new TerrainComponent(L"./Resources/Terrain/Hawai_HeightMap_64x64x16.raw", L"./Resources/Terrain/Hawai_TexMap.dds", 64, 64, 64.0f, 64.0f, 10.0f);
	terrain->AddComponent(terrainComponent);
	AddChild(terrain);

	SkyboxPrefab* sky = new SkyboxPrefab();
	AddChild(sky);

	PxPhysics* physX = PhysxManager::GetInstance()->GetPhysics();
	PxMaterial* defaultMaterial = physX->createMaterial(0.5f, 0.5f, 0.2f);

	SpherePrefab* sphere = new SpherePrefab();
	RigidBodyComponent* rb = new RigidBodyComponent();
	shared_ptr<PxGeometry> geom(new PxSphereGeometry(1.0f));
	ColliderComponent* collider = new ColliderComponent(geom, *defaultMaterial);
	sphere->AddComponent(rb);
	sphere->AddComponent(collider);
	AddChild(sphere);
	sphere->GetTransform()->Translate(0.0f, 50.0f, 0.0f);
}

void TerrainScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void TerrainScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
