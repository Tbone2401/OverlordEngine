//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "UberShaderTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics\SpriteRenderer.h"

#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components\SpriteComponent.h"
#include "Components\TransformComponent.h"
#include "Graphics\MeshFilter.h"

#include "Components/Components.h"
#include <Materials/UberMaterial.h>

#define FPS_COUNTER 1

UberShaderTestScene::UberShaderTestScene(void) :
GameScene(L"UberShaderTestScene"),
m_FpsInterval(FPS_COUNTER),
m_pObj(nullptr)
{
}


UberShaderTestScene::~UberShaderTestScene(void)
{
}

void UberShaderTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GameObject* teapot = new GameObject();
	ModelComponent* model = new ModelComponent(L"./Resources/Meshes/teapot.ovm");
	UberMaterial* uberMaterial = new UberMaterial();
	uberMaterial->SetDiffuseTexture(L"./Resources/Textures/CobbleStone_DiffuseMap.dds");
	uberMaterial->SetEnvironmentCube(L"./Resources/Textures/Sunol_Cubemap.dds");
	uberMaterial->EnableDiffuseTexture(true);
	uberMaterial->EnableEnvironmentMapping(true);
	uberMaterial->EnableSpecularPhong(true);
	uberMaterial->SetReflectionStrength(0.5f);
	gameContext.pMaterialManager->AddMaterial(uberMaterial, 0);
	model->SetMaterial(0);
	teapot->AddComponent(model);
	AddChild(teapot);
}

void UberShaderTestScene::Update(const GameContext& gameContext)
{
	m_FpsInterval += gameContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	}
}

void UberShaderTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
