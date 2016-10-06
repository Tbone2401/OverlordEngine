//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Scenegraph/SceneManager.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Graphics/Material.h"
#include "../../Materials/ColorMaterial.h"
#include "../../Materials/DiffuseMaterial.h"
#include "Character.h"

ModelTestScene::ModelTestScene(void) :
	GameScene(L"ModelTestScene"),
	m_pChair(nullptr)
{
}


ModelTestScene::~ModelTestScene(void)
{
}

void ModelTestScene::Initialize(const GameContext& gameContext)
{
	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	//1. Attach a modelcomponent (chair.ovm)
	ModelComponent* modelComponent = new ModelComponent(L"./Resources/Meshes/chair.ovm");
	m_pChair->AddComponent(modelComponent);
	//2. Create a ColorMaterial and add it to the material manager
	ColorMaterial* colorMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);
	//3. Assign the material to the previous created modelcomponent
	modelComponent->SetMaterial(0);
	// Build and Run

	//4. Create a DiffuseMaterial (using PosNormTex3D.fx)
	DiffuseMaterial* diffuseMaterial = new DiffuseMaterial();
	//		Make sure you are able to set a texture (DiffuseMaterial::SetDiffuseTexture(const wstring& assetFile))
	diffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	//		Load the correct shadervariable and set it during the material variable update
	//5. Assign the material to the modelcomponent
	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 1);
	modelComponent->SetMaterial(1);
	// Build and Run

	//6. Attach a rigidbody component (pure-dynamic)
	RigidBodyComponent* rb = new RigidBodyComponent(false);
	m_pChair->AddComponent(rb);
	//7. Attach a collider component (Use a PxConvexMeshGeometry [chair.ovpc])
	PxMaterial * defaultMaterial = physX->createMaterial(0.0f, 0.0f, 1.0f);
	PxConvexMesh* convexMesh = ContentManager::Load<PxConvexMesh>(L"./Resources/Meshes/chair.ovpc");
	shared_ptr<PxGeometry> mesh(new PxConvexMeshGeometry(convexMesh)); 
	ColliderComponent* collider = new ColliderComponent(mesh, *defaultMaterial, PxTransform::createIdentity());
	m_pChair->AddComponent(collider);
	// Build and Run
	AddChild(m_pChair);
	rb->SetDensity(300);
	m_pChair->GetTransform()->Translate(5, 10, 0);
}

void ModelTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ModelTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}


/*//Testlevel
GameObject* level = new GameObject();
ModelComponent * levelModel = new ModelComponent(L"./Resources/Meshes/Level.ovm");
levelModel->SetMaterial(1);
rb = new RigidBodyComponent(true);
level->AddComponent(levelModel);
level->AddComponent(rb);
PxTriangleMesh* triangleMesh = ContentManager::Load<PxTriangleMesh>(L"./Resources/Meshes/Level.ovpt");
shared_ptr<PxGeometry> levelGeom(new PxTriangleMeshGeometry(triangleMesh));
collider = new ColliderComponent(levelGeom, *defaultMaterial, PxTransform::createIdentity());
level->AddComponent(collider);
level->GetTransform()->Translate(0, 0, 50);
AddChild(level);

Character* character = new Character();
AddChild(character);*/