//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Scenegraph\GameObject.h"

#include "Content\ContentManager.h"
#include "Graphics\ModelAnimator.h"

#include "Materials\SkinnedDiffuseMaterial.h"
#include "Materials\SkyboxMaterial.h"
#include "Materials\UberMaterial.h"

#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "CourseObjects/Week 5/SkyboxPrefab.h"
#include "Materials\ColorMaterial.h"
#include "CourseObjects\Prefabs\ClothPrefab.h"
#include "CourseObjects\Week 2\Character.h"

#include "../Exam/Boxes/BoxManager.h"
#include "../Exam/Enemies/EnemiesManager.h"
#include "ExamScene.h"
#include <ExplosionMaterial.h>
#include <Score.h>
#include <Graphics/SpriteFont.h>

#include "Graphics/TextRenderer.h"
#include "../Exam/Enemies/Enemy.h"
#include <Materials/Shadow/DiffuseMaterial_Shadow.h>
#include <Graphics/ShadowMapRenderer.h>

#include "../Exam/LevelBuilder/FileReader.h"

#include "Base/OverlordGame.h"
#include "Materials/Post/Chromatic.h"
#include "Materials/Post/PostVignette.h"
#include "Materials/Post/Pixelated.h"
#include "Materials/Post/PostGrayscale.h"
#include "Timer/Timer.h"
#include "EndScreen.h"
#include "Scenegraph\SceneManager.h"


ExamScene::ExamScene(bool isOldSchool) :
	GameScene(L"ExamScene"),
	m_IsOldSchool(isOldSchool)
{
	m_pTimer = new Timer(300.0f);
	m_pCloth = nullptr;
}


ExamScene::~ExamScene()
{
	delete m_pFileReader;
	delete m_pTimer;
}

void ExamScene::Initialize(const GameContext& gameContext)
{
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	gameContext.pShadowMapper->SetLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });
	//GetPhysxProxy()->EnablePhysxDebugRendering(true);
	//******************************
	//FONTS
	//******************************
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"./Resources/Fonts/Consolas_32.fnt");
	//******************************
	//CHARACTER
	//******************************
	m_pCharacter = new Character(2.0, 5.0, 100.0f);
	m_pCharacter->SetTag(L"Player");
	AddChild(m_pCharacter);

	//******************************
	//MATERIALS
	//******************************
	UberMaterial* uberMat = new UberMaterial();
	gameContext.pMaterialManager->AddMaterial(uberMat, 1);

	DiffuseMaterial_Shadow* difMat = new DiffuseMaterial_Shadow();
	difMat->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	difMat->SetDiffuseTexture(L"./Resources/Textures/Level_Texture.png");
	gameContext.pMaterialManager->AddMaterial(difMat, 2);
	//******************************
	//LEVEL
	//******************************
	m_pLevel = new GameObject();
	ModelComponent *comp = new ModelComponent(L"./Resources/Meshes/Level1.ovm");
	m_pLevel->AddComponent(comp);
	comp->SetMaterial(2);

	PxMaterial * defaultMaterial = physX->createMaterial(.5f, .5f, .0f);
	PxTriangleMesh* triMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Level1.ovpt");
	shared_ptr<PxGeometry> mesh(new PxTriangleMeshGeometry(triMesh));
	ColliderComponent* collider = new ColliderComponent(mesh, *defaultMaterial, PxTransform::createIdentity());
	m_pLevel->AddComponent(collider);

	RigidBodyComponent * rb = new RigidBodyComponent();
	rb->SetKinematic(true);
	rb->SetDensity(5000.0f);
	m_pLevel->AddComponent(rb);

	AddChild(m_pLevel);
	m_pLevel->GetTransform()->Translate(0.0f, 0.0f, 2400.0f);

	//******************************
	//Bounds
	//******************************
	GameObject* bounds = new GameObject();
	ModelComponent *compBounds = new ModelComponent(L"./Resources/Meshes/Level1_Bounds.ovm");
	bounds->AddComponent(compBounds);

	PxTriangleMesh* triMeshBounds = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Level1_Bounds.ovpt");
	shared_ptr<PxGeometry> meshBounds(new PxTriangleMeshGeometry(triMeshBounds));
	ColliderComponent* colliderBounds = new ColliderComponent(meshBounds, *defaultMaterial, PxTransform::createIdentity());
	bounds->AddComponent(colliderBounds);

	RigidBodyComponent * rbBounds = new RigidBodyComponent();
	rbBounds->SetKinematic(true);
	rbBounds->SetDensity(5000.0f);
	bounds->AddComponent(rbBounds);

	AddChild(bounds);
	bounds->GetTransform()->Translate(0.0f, 0.0f, 2400.0f);

	//*******************
	//BOXES
	//*******************
	m_pBoxManager = new BoxManager();
	AddChild(m_pBoxManager);
	//******************************
	//ENEMIES
	//******************************
	m_pEnemiesManager = new EnemiesManager();
	AddChild(m_pEnemiesManager);

	//******************************
	//FILEREADER
	//******************************
	m_pFileReader = new FileReader("./Resources/StageInfo/level1.txt");
	//*****************************
	//POST-PROCESSING
	//*****************************
	m_pChromaticPP = new Chromatic();	
	m_pChromaticPP->SetTime(gameContext.pGameTime->GetElapsed());
	gameContext.pMaterialManager->AddMaterial_PP(m_pChromaticPP, 0);
	AddPostProcessingMaterial(0);
	//******************************
	//SKYBOX
	//******************************
	auto skb = new SkyboxPrefab();
	AddChild(skb);

	if(m_IsOldSchool)
	{
		m_pPixelPP = new Pixelated();
		gameContext.pMaterialManager->AddMaterial_PP(m_pPixelPP, 1);
		AddPostProcessingMaterial(1);
		m_pGrayPP = new PostGrayscale();
		gameContext.pMaterialManager->AddMaterial_PP(m_pGrayPP, 2);
		AddPostProcessingMaterial(2);
	}
}
void ExamScene::Update(const GameContext& gameContext)
{
	if (!m_IsRead)
	{
		m_pFileReader->ReadFile(m_pBoxManager, m_pEnemiesManager, gameContext);
		m_IsRead = true;
	}
	//**************************
	//POST-PROCESSING
	//**************************
	if (m_pCharacter->IsHurting())
	{
		m_pChromaticPP->UpdateTime(gameContext.pGameTime->GetElapsed());
	}
	else
	{
		m_pChromaticPP->SetTime(3.14f);
	}
	//*******************
	//BOXES
	//*******************
	if(m_pBoxManager->DidPlayerTouchBox())
	{
		m_pCharacter->GiveUpWardsImpulse();
	}
	gameContext.pShadowMapper->SetLight({ m_pCharacter->GetShadowCameraPosition() }, { 0.740129888f, -0.597205281f, 0.309117377f });
	
	//*****************************
	//TIMER
	//*****************************
	if(m_pTimer->UpdateTimer(gameContext.pGameTime->GetElapsed()) ||
		m_pCharacter->GetHeight() <= -300.0f ||
		m_pCharacter->IsDead())
	{
		SceneManager::GetInstance()->AddGameScene(new EndScreen());
		SceneManager::GetInstance()->SetActiveGameScene(L"EndScreen");
	}

	if(m_pEnemiesManager->PlayerInteraction() == 2)
	{
		m_pCharacter->SubtractHealth();
	}

}

void ExamScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Score: " + to_wstring(Score::GetInstance()->GetScore()), XMFLOAT2(50, 10), (XMFLOAT4)Colors::Crimson);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Time: " + to_wstring(m_pTimer->GetTime()), XMFLOAT2((OverlordGame::GetGameSettings().Window.Width/2.0f) - 50, 10), (XMFLOAT4)Colors::Crimson);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"FPS: " + to_wstring(gameContext.pGameTime->GetFPS()), XMFLOAT2((int)OverlordGame::GetGameSettings().Window.Width - 120.0f, 10), (XMFLOAT4)Colors::Crimson);
}