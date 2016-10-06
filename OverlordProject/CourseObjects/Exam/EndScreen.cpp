//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "EndScreen.h"
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

#include "ButtonClass\WorldButton.h"
#include "../OverlordEngine/Components/ParticleEmitterComponent.h"
#include "ExamScene.h"
#include "Score.h"
#include "../OverlordEngine/Base/OverlordGame.h"


#define FPS_COUNTER 1

EndScreen::EndScreen(void) :
GameScene(L"EndScreen")
{
}


EndScreen::~EndScreen(void)
{
}

void EndScreen::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//*************************
	//START QUIT MESHES
	//*************************
	m_pStart = new WorldButton(XMFLOAT3(0.0f,20.0f,0.0f),L"Start");
	m_pQuit = new WorldButton(XMFLOAT3(0.0f,-20.0f,0.0f),L"Quit");
	m_pArrow1 = new WorldButton(XMFLOAT3(-40.0f, 0.0f, 0.0f), L"Arrow",XMFLOAT3(0.0f,180.0f,0.0f));
	m_pArrow2 = new WorldButton(XMFLOAT3(40.0f, 0.0f, 0.0f), L"Arrow");
	m_pModern = new WorldButton(XMFLOAT3(0.0f, 0.0f, 0.0f), L"Modern");
	m_pOldschool = new WorldButton(XMFLOAT3(0.0f, 0.0f, 0.0f), L"Oldschool");

	AddChild(m_pStart);
	AddChild(m_pQuit);
	AddChild(m_pArrow1);
	AddChild(m_pArrow2);
	AddChild(m_pOldschool);
	AddChild(m_pModern);
	m_pStart->SetActiveColor(XMFLOAT3(1.0f, .5f, 0.0f));
	m_pStart->SetInactiveColor(XMFLOAT3(1.0f, 1.0f, .5f));
	m_pQuit->SetActiveColor(XMFLOAT3(1.0f, .5f, 0.0f));
	m_pQuit->SetInactiveColor(XMFLOAT3(1.0f, 1.0f, .5f));
	m_pArrow1->SetActiveColor(XMFLOAT3(1.0f, .5f, 0.0f));
	m_pArrow1->SetInactiveColor(XMFLOAT3(1.0f, 1.0f, .5f));
	m_pArrow2->SetActiveColor(XMFLOAT3(1.0f, .5f, 0.0f));
	m_pArrow2->SetInactiveColor(XMFLOAT3(1.0f, 1.0f, .5f));
	m_pModern->SetActiveColor(XMFLOAT3(1.0f, .5f, 0.0f));
	m_pModern->SetInactiveColor(XMFLOAT3(1.0f, 1.0f, .5f));
	m_pOldschool->SetActiveColor(XMFLOAT3(1.0f, .5f, 0.0f));
	m_pOldschool->SetInactiveColor(XMFLOAT3(1.0f, 1.0f, .5f));
	//PARTICLE-EMITTER
	//****************

	auto pObjRight = new GameObject();
	auto pObjLeft = new GameObject();

	m_pParticleEmitterRight = new ParticleEmitterComponent(L"./Resources/Textures/Smoke.png", 60);
	m_pParticleEmitterRight->SetVelocity(XMFLOAT3(0, 6.0f, 0));
	m_pParticleEmitterRight->SetMinSize(1.0f);
	m_pParticleEmitterRight->SetMaxSize(3.0f);
	m_pParticleEmitterRight->SetMinEnergy(4.0f);
	m_pParticleEmitterRight->SetMaxEnergy(5.0f);
	m_pParticleEmitterRight->SetMinSizeGrow(6.5f);
	m_pParticleEmitterRight->SetMaxSizeGrow(9.5f);
	m_pParticleEmitterRight->SetMinEmitterRange(0.2f);
	m_pParticleEmitterRight->SetMaxEmitterRange(0.5f);
	m_pParticleEmitterRight->SetColor(XMFLOAT4(1.f, 1.f, 0.9f, 0.6f));
	pObjRight->AddComponent(m_pParticleEmitterRight);
	AddChild(pObjRight);
	pObjRight->GetTransform()->Translate(20.0f, 0.0f, 0.0f);

	m_pParticleEmitterLeft = new ParticleEmitterComponent(L"./Resources/Textures/Smoke.png", 60);
	m_pParticleEmitterLeft->SetVelocity(XMFLOAT3(0, 6.0f, 0));
	m_pParticleEmitterLeft->SetMinSize(1.0f);
	m_pParticleEmitterLeft->SetMaxSize(3.0f);
	m_pParticleEmitterLeft->SetMinEnergy(4.0f);
	m_pParticleEmitterLeft->SetMaxEnergy(5.0f);
	m_pParticleEmitterLeft->SetMinSizeGrow(6.5f);
	m_pParticleEmitterLeft->SetMaxSizeGrow(9.5f);
	m_pParticleEmitterLeft->SetMinEmitterRange(0.2f);
	m_pParticleEmitterLeft->SetMaxEmitterRange(0.5f);
	m_pParticleEmitterLeft->SetColor(XMFLOAT4(1.f, .9f, 0.6f, 0.6f));
	pObjLeft->AddComponent(m_pParticleEmitterLeft);
	AddChild(pObjLeft);
	pObjLeft->GetTransform()->Translate(-20.0f, 0.0f, 0.0f);

	gameContext.pInput->AddInputAction(InputAction(0, Pressed, -1, VK_LBUTTON));

	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"./Resources/Fonts/Consolas_32.fnt");
}

void EndScreen::Update(const GameContext& gameContext)
{
	if(gameContext.pInput->IsActionTriggered(0))
	{
		if (gameContext.pCamera->Pick(gameContext) == m_pStart->ReturnGameObj())
		{
			Score::GetInstance()->ResetScore();
			SceneManager::GetInstance()->AddGameScene(new ExamScene(m_IsOldSchool));
			SceneManager::GetInstance()->SetActiveGameScene(L"ExamScene");
		}
		if(gameContext.pCamera->Pick(gameContext) == (m_pArrow1->ReturnGameObj()) ||
			gameContext.pCamera->Pick(gameContext) == (m_pArrow2->ReturnGameObj()))
		{
			m_IsOldSchool = !m_IsOldSchool;
			m_DidChange = true;
		}
		if (gameContext.pCamera->Pick(gameContext) == (m_pQuit->ReturnGameObj()))
		{
			PostQuitMessage(0);
		}
	}
	if (m_IsOldSchool && m_DidChange)
	{
		m_pOldschool->MoveAndEnable();
		m_pModern->MoveAndDisable();
		m_DidChange = false;
	}
	else if(m_DidChange)
	{
		m_pModern->MoveAndEnable();
		m_pOldschool->MoveAndDisable();
		m_DidChange = false;
	}
}

void EndScreen::Draw(const GameContext& gameContext)
{
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Game over! ", XMFLOAT2((OverlordGame::GetGameSettings().Window.Width / 2.0f) - 50.0f, 10), (XMFLOAT4)Colors::Crimson);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Score: " + to_wstring(Score::GetInstance()->GetScore()), XMFLOAT2((OverlordGame::GetGameSettings().Window.Width/2.0f)-50.0f, 30), (XMFLOAT4)Colors::Crimson);
	UNREFERENCED_PARAMETER(gameContext);
}