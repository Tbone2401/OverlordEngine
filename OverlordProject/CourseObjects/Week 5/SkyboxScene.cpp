//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkyboxScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components/Components.h"
#include <Physx/PhysxManager.h>
#include "SkyboxPrefab.h"

#define FPS_COUNTER 1

SkyboxScene::SkyboxScene(void) :
GameScene(L"SkyboxScene")
{
}


SkyboxScene::~SkyboxScene(void)
{
}

void SkyboxScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	SkyboxPrefab* sky = new SkyboxPrefab();
	AddChild(sky);
}

void SkyboxScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SkyboxScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
