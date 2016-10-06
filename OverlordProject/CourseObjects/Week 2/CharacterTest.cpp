//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CharacterTest.h"
#include "Scenegraph/GameObject.h"
#include "Scenegraph/SceneManager.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Physx/PhysxProxy.h"
#include "Graphics/Material.h"
#include "../../Materials/ColorMaterial.h"
#include "../../Materials/DiffuseMaterial.h"
#include "Character.h"

CharacterTest::CharacterTest(void) :
	GameScene(L"CharacterTest")
{
}


CharacterTest::~CharacterTest(void)
{
}

void CharacterTest::Initialize(const GameContext& gameContext)
{
	//Enable physX debugging
	//GameScene::GetPhysxProxy()->EnablePhysxDebugRendering(true);

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	PxMaterial* defaultMaterial = physX->createMaterial(0, 0, 0);

	//Ground Plane
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *defaultMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//Material
	DiffuseMaterial* diffuse = new DiffuseMaterial();
	diffuse->SetDiffuseTexture(L"./Resources/Textures/chair_dark.dds");
	gameContext.pMaterialManager->AddMaterial(diffuse, 0);

	//LEVEL
	//object
	GameObject* level = new GameObject();
	//mesh
	ModelComponent * levelModel = new ModelComponent(L"./Resources/Meshes/Level.ovm");
	levelModel->SetMaterial(0);
	level->AddComponent(levelModel);
	//rigidbody
	RigidBodyComponent * rb = new RigidBodyComponent(true);
	level->AddComponent(rb);
	//collider
	PxTriangleMesh* triangleMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Level.ovpt");
	shared_ptr<PxGeometry> levelGeom(new PxTriangleMeshGeometry(triangleMesh));
	ColliderComponent * collider = new ColliderComponent(levelGeom, *defaultMaterial, PxTransform::createIdentity());
	level->AddComponent(collider);
	level->GetTransform()->Translate(0, 0, 50);
	AddChild(level);

	//Character
	Character* character = new Character();
	AddChild(character);
}

void CharacterTest::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CharacterTest::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}