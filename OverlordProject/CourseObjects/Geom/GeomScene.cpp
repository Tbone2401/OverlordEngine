//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GeomScene.h"
#include "Scenegraph\GameObject.h"
#include "Scenegraph\SceneManager.h"
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
#include <Materials\ColorMaterial.h>
#include <Prefabs/CubePrefab.h>
#include <Physx/PhysxManager.h>
#include "ExplosionMaterial.h"
#include "Graphics\TextRenderer.h"
#include "../OverlordEngine/Base/OverlordGame.h"


#define FPS_COUNTER 1

GeomScene::GeomScene(void) :
GameScene(L"GeomScene")
{
}


GeomScene::~GeomScene(void)
{
}

void GeomScene::Initialize(const GameContext& gameContext)
{
	m_pExplosionMat = new ExplosionMaterial();
	m_pExplosionMat->ReduceThickness(5.0f);
	m_pExplosionMat->SetDiffuseTexture(L"./Resources/Textures/Skulls_Diffusemap.tga");
	m_pExplosionMat->SetEffectLimits(0.0f, 0.0f);
	m_pExplosionMat->SetExplosionPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pExplosionMat->SetExplosionPower(5.0f);
	m_pExplosionMat->SetGravity(-9.81f);
	m_pExplosionMat->SetLimits(-2.5f, 2.5f);
	m_pExplosionMat->SetNoiseMap(L"./Resources/Textures/Skulls_Heightmap.tga");
	m_pExplosionMat->SetNoisemapInfluence(10.0f);
	m_pExplosionMat->SetNormalMap(L"./Resources/Textures/Skulls_Normalmap.tga");
	m_pExplosionMat->SetTime(0.0f);
	m_pExplosionMat->SetExplosionPos(XMFLOAT3(0.0f, 2.5f, 0.0f));
	gameContext.pMaterialManager->AddMaterial(m_pExplosionMat, 0);

	m_pTeapot = new GameObject();
	ModelComponent *comp = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	m_pTeapot->AddComponent(comp);

	comp->SetMaterial(0);

	AddChild(m_pTeapot);

	//*******************************
	//SpriteFont
	//*******************************
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"./Resources/Fonts/Consolas_32.fnt");


	UNREFERENCED_PARAMETER(gameContext);
	gameContext.pInput->AddInputAction(InputAction(0, InputTriggerState::Down, -1, 0x51));
	gameContext.pInput->AddInputAction(InputAction(1, InputTriggerState::Down, -1, 0x45));
}

void GeomScene::Update(const GameContext& gameContext)
{
	float elapsed = gameContext.pGameTime->GetElapsed();
	UNREFERENCED_PARAMETER(gameContext);
	if(gameContext.pInput->IsActionTriggered(0))
	{
		m_Timer -= elapsed;
		m_pExplosionMat->SetTime(m_Timer);
	}
	if (gameContext.pInput->IsActionTriggered(1))
	{
		m_Timer += elapsed;
		m_pExplosionMat->SetTime(m_Timer);
	}
	if(GetAsyncKeyState(0x45) & 0x8000)
	{
		m_Timer += elapsed;
		m_pExplosionMat->SetTime(m_Timer);
	}
	if (GetAsyncKeyState(0x51) & 0x8000)
	{
		m_Timer -= elapsed;
		if (m_Timer <= 0.0f) m_Timer = 0.0f;
		m_pExplosionMat->SetTime(m_Timer);
	}
	if (GetAsyncKeyState(0x52) & 0x8000)
	{
		m_Strength -= elapsed*5.0f;
		if (m_Strength <= 0.0f) m_Strength = 0.0f;
		m_pExplosionMat->SetExplosionPower(m_Strength);
	}	
	if (GetAsyncKeyState(0x54) & 0x8000)
	{
		m_Strength += elapsed*5.0f;
		m_pExplosionMat->SetExplosionPower(m_Strength);
	}
	if (GetAsyncKeyState(0x59) & 0x8000)
	{
		m_Gravity -= elapsed*10.0f;
		m_pExplosionMat->SetGravity(m_Gravity);
	}
	if (GetAsyncKeyState(0x55) & 0x8000)
	{
		m_Gravity += elapsed*10.0f;
		m_pExplosionMat->SetGravity(m_Gravity);
	}
}

void GeomScene::Draw(const GameContext& gameContext)
{
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Geometry Shader ", XMFLOAT2((OverlordGame::GetGameSettings().Window.Width / 2.0f) - 50.0f, 10), (XMFLOAT4)Colors::Crimson);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Q and E for time", XMFLOAT2(10, 10), (XMFLOAT4)Colors::Crimson);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"R and T for explosion strength", XMFLOAT2(10, 40), (XMFLOAT4)Colors::Crimson);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Y and U for gravity", XMFLOAT2(10, 70), (XMFLOAT4)Colors::Crimson);
	UNREFERENCED_PARAMETER(gameContext);
}