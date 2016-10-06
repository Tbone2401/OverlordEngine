//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Character.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Physx/PhysxManager.h"
#include <SceneGraph/GameScene.h>
#include <Physx/PhysxProxy.h>
#include "Materials/SkinnedDiffuseMaterial.h"
#include "Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "../OverlordEngine/Components/ModelComponent.h"
#include "Graphics\ModelAnimator.h"
#include "Materials/DiffuseMaterial.h"

Character::Character(float radius, float height, float moveSpeed) :
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_pAnimMesh(nullptr),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f),
	m_TerminalVelocity(20),
	m_Gravity(20.0f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(0.8f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	m_Velocity(0, 0, 0),
	m_Points(0),
	m_HitPoints(2),
	m_pModel(nullptr)
{
}


Character::~Character(void)
{
	m_pScarf->release();
	delete[] m_pTriangleIndices;
	delete m_pMesh;
}

void Character::Initialize(const GameContext& gameContext)
{
	PxPhysics * physX = PhysxManager::GetInstance()->GetPhysics();

	// Create controller
	PxMaterial* defaultMaterial = physX->createMaterial(0, 0, 0);
	m_pController = new ControllerComponent(defaultMaterial);
	m_pCharacter = new GameObject();
	m_pCharacter->AddComponent(m_pController);
	m_pCharacter->SetTag(L"Player");
	AddChild(m_pCharacter);
	m_pCharacter->GetTransform()->Translate(0.0f, 35.0f, 0.0f);


	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial_Shadow();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/color.tga");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 250);

	m_pAnimMesh = new GameObject();
	m_pModel = new ModelComponent(L"./Resources/Meshes/Crash_Anim.ovm");
	m_pModel->SetMaterial(250);
	m_pAnimMesh->AddComponent(m_pModel);

	// Add a fixed camera as child
	m_pCameraObj = new GameObject();
	m_pCamera = new CameraComponent();
	m_pCameraFixed = new FreeCamera();
	m_pCameraFixed->AddComponent(m_pCamera);
	m_pCameraObj->AddChild(m_pCameraFixed);
	m_pCharacter->AddChild(m_pCameraObj);

	m_pController->GetCollisionFlags();
	

	// Register all Input Actions
	gameContext.pInput->AddInputAction(InputAction(LEFT, Down, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(RIGHT, Down, VK_RIGHT));
	gameContext.pInput->AddInputAction(InputAction(FORWARD, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(BACKWARD, Down, VK_DOWN));
	gameContext.pInput->AddInputAction(InputAction(JUMP, Down, VK_SPACE));


	//Adding anim mesh
	m_pCharacter->AddChild(m_pAnimMesh);
	CreateCloth();

}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_pCamera->SetActive();
	m_pCameraObj->GetTransform()->Rotate(30.0f, 0.0f, 0.0f);
	m_pCameraObj->GetTransform()->Translate(0.0f, 30.0f, -30.0f);
	m_pModel->GetAnimator()->SetAnimation(0);
	auto animator = m_pModel->GetAnimator();
	animator->Play();
	m_pAnimMesh->GetTransform()->Scale(.1f, .1f, .1f);
	m_pAnimMesh->GetTransform()->Rotate(0.0f, 180.0f, 0.0f);
	m_pCamera->SetActive();
}

XMFLOAT3 operator*(const float &a, const XMFLOAT3 &b)
{
	XMFLOAT3 o;
	o.x = a * b.x;
	o.y = a * b.y;
	o.z = a * b.z;
	return o;
}

XMFLOAT3 operator+(const XMFLOAT3 &a, const XMFLOAT3 &b)
{
	XMFLOAT3 o;
	o.x = a.x + b.x;
	o.y = a.y + b.y;
	o.z = a.z + b.z;
	return o;
}

float Clamp(const float i, const float min, const float max)
{
	if (i > max)
		return max;
	if (i < min)
		return min;
	return i;
}


void Character::Update(const GameContext& gameContext)
{
	m_timer += gameContext.pGameTime->GetElapsed();

	auto animator = m_pModel->GetAnimator();
	float deltaTime = gameContext.pGameTime->GetElapsed();
	XMFLOAT3 forward = m_pCamera->GetTransform()->GetForward();
	XMFLOAT3 right = m_pCamera->GetTransform()->GetRight();

	//Movement
	bool isMoving = false;

	if (gameContext.pInput->IsActionTriggered(FORWARD))
	{
		if (!animator->IsPlaying())
			animator->Play();
		animator->SetPlayReversed(false);
		isMoving = true;
		m_Velocity.z = Clamp(m_Velocity.z + m_RunAcceleration * deltaTime, -m_MaxRunVelocity, m_MaxRunVelocity);
	}
	else if (gameContext.pInput->IsActionTriggered(BACKWARD))
	{
		if (!animator->IsPlaying())
			animator->Play();
		if (!animator->IsReversed())
			animator->SetPlayReversed(true);
		isMoving = true;
		m_Velocity.z = Clamp(m_Velocity.z - m_RunAcceleration * deltaTime, -m_MaxRunVelocity, m_MaxRunVelocity);
	}
	if (gameContext.pInput->IsActionTriggered(RIGHT))
	{
		isMoving = true;
		m_Velocity.x = Clamp(m_Velocity.x + m_RunAcceleration * deltaTime, -m_MaxRunVelocity, m_MaxRunVelocity);
	}
	else if (gameContext.pInput->IsActionTriggered(LEFT))
	{
		isMoving = true;
		m_Velocity.x = Clamp(m_Velocity.x - m_RunAcceleration * deltaTime, -m_MaxRunVelocity, m_MaxRunVelocity);
	}

	if (isMoving == false)
		m_Velocity = { 0,0,0 };

	//Jumping
	if(m_pController->GetCollisionFlags() != PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		m_JumpVelocity = Clamp(m_JumpVelocity - m_JumpAcceleration * deltaTime, 0, m_MaxRunVelocity);
		m_FlyTime += deltaTime;
	}
	else
	{
		m_FlyTime = 0;
		if (gameContext.pInput->IsActionTriggered(JUMP))
		{
			animator->Reset(true);
			m_JumpVelocity = 40.0f;
		}
	}

	if (m_ShouldReactToBox)
	{
		m_FlyTime = 0;
		m_JumpVelocity += 40.0f;
	}

	m_Velocity.y = m_JumpVelocity - m_Gravity * m_FlyTime;
	if( abs(m_Velocity.x) <= 0.01f && abs(m_Velocity.z) <= 0.01f )
		animator->Reset(true);
	XMFLOAT3 deltaMove;
	right.y = 0;
	forward.y = 0;
	deltaMove = m_Velocity.x * right + m_Velocity.z * forward;
	deltaMove.y = m_Velocity.y;


	m_pController->Move(deltaTime * deltaMove);
	m_ShouldReactToBox = false;


	//*************************
	//HEALTH
	//*************************
	if(m_HitPoints == 1)
	{
		m_RegenTimer += gameContext.pGameTime->GetElapsed();
		if (m_RegenTimer >= 5.0f)
		{
			m_RegenTimer = 0.0f;
			++m_HitPoints;
		}
	}


	//*************************************
	//UPDATE CLOTH
	//*************************************
	XMFLOAT3 posMesh = m_pController->GetPosition();
	posMesh.y -= 0;
	m_pMesh->GetTransform()->Translate(posMesh);
	m_pMesh->GetTransform()->Rotate(0, m_TotalYaw, 0);

	auto pos = m_pController->GetPosition();
	PxVec3 pPos = PxVec3(pos.x, pos.y, pos.z);
	auto fw = m_pMesh->GetTransform()->GetForward();
	pPos.x = pPos.x - fw.x * m_Radius;
	pPos.y = pPos.y + fw.y * m_Radius;
	pPos.z = pPos.z - fw.z * m_Radius;

	PxQuat rot;
	auto r = m_pMesh->GetTransform()->GetRotation();
	rot.x = r.x;
	rot.y = r.y;
	rot.z = r.z;
	rot.w = r.w;
	PxTransform transform = PxTransform(pPos, rot);
	m_pScarf->setTargetPose(transform);

	//update cloth mesh
	XMFLOAT3 posMeshCloth = ToXMFLOAT3(pPos);
	PxClothParticleData *pData = m_pScarf->lockParticleData();
	auto meshDrawer = m_pScarfMesh->GetComponent<MeshDrawComponent>();
	meshDrawer->RemoveTriangles();
	for (PxU32 i = 0; i < (PxU32)m_nrOfIndices; i += 3)
	{
		PxVec3 vec1 = pData->particles[m_pTriangleIndices[i]].pos - pData->particles[m_pTriangleIndices[i + 1]].pos;
		PxVec3 vec2 = pData->particles[m_pTriangleIndices[i]].pos - pData->particles[m_pTriangleIndices[i + 2]].pos;
		XMFLOAT3 normal = ToXMFLOAT3(vec1.cross(vec2));

		meshDrawer->AddTriangle(
			TrianglePosNormCol(
				VertexPosNormCol(XMFLOAT3(ToXMFLOAT3(pData->particles[m_pTriangleIndices[i]].pos)), normal, XMFLOAT4(0.8f, 0.2f, 0.8f, 1)),
				VertexPosNormCol(XMFLOAT3(ToXMFLOAT3(pData->particles[m_pTriangleIndices[i + 1]].pos)), normal, XMFLOAT4(0.8f, 0.2f, 0.8f, 1)),
				VertexPosNormCol(XMFLOAT3(ToXMFLOAT3(pData->particles[m_pTriangleIndices[i + 2]].pos)), normal, XMFLOAT4(0.8f, 0.2f, 0.8f, 1))
				));
	}
	meshDrawer->UpdateBuffer();
	pData->unlock();
	m_pScarfMesh->GetTransform()->Translate(posMeshCloth);
	m_pScarfMesh->GetTransform()->Rotate(0, m_TotalYaw, 0);





	wstring string = to_wstring(m_pController->GetTransform()->GetPosition().x) + L" : " 
		+ to_wstring(m_pController->GetTransform()->GetPosition().y) + L" : "
		+ to_wstring(m_pController->GetTransform()->GetPosition().z);
	Logger::LogInfo(string);
}

void Character::GiveUpWardsImpulse()
{
	m_ShouldReactToBox = true;
}
const XMFLOAT3 Character::GetShadowCameraPosition() const
{
	return{ m_pController->GetPosition().x-50.0f,
		m_pController->GetPosition().y+30.0f,
		m_pController->GetPosition().z };
}

const void Character::SubtractHealth()
{
	if (m_timer < 1.0f) return;
	if (m_HitPoints == 1) m_IsDead = true;
	--m_HitPoints;
}
const bool Character::IsHurting() const
{
	if (m_HitPoints == 1) return true;
	
	
	return false;
}

void Character::CreateCloth()
{

	auto physX = PhysxManager::GetInstance()->GetPhysics();
	// create cloth
	PxTransform gPose = PxTransform(PxVec3(0, 0, 0));
	// create regular mesh
	PxU32 length = 4;
	PxU32 resolutiony = 10;

	PxU32 numParticles = length*resolutiony;
	PxU32 numTriangles = 2 * (length - 1)*(resolutiony - 1);

	// create cloth particles
	PxClothParticle* particles = new PxClothParticle[numParticles];
	PxVec3 center = PxVec3((float)length / 2, 0, -(float)resolutiony);
	//determines scale
	PxVec3 delta = PxVec3(1, 1, 1);
	PxClothParticle* pIt = particles;
	for (PxU32 i = 0; i<length; ++i)
	{
		for (PxU32 j = 0; j<resolutiony; ++j, ++pIt)
		{
			pIt->invWeight = j + 1<resolutiony ? 1.0f : 0.0f;
			pIt->pos = delta.multiply(PxVec3(PxReal(i), 0, -PxReal(j))) - center;
		}
	}
	// create triangles
	m_pTriangleIndices = new PxU32[3 * numTriangles];
	m_nrOfIndices = 3 * numTriangles;
	PxU32* iIt = m_pTriangleIndices;
	for (PxU32 i = 0; i<length - 1; ++i)
	{
		for (PxU32 j = 0; j<resolutiony - 1; ++j)
		{
			PxU32 odd = j & 1u, even = 1 - odd;
			*iIt++ = i*resolutiony + (j + odd);
			*iIt++ = (i + odd)*resolutiony + (j + 1);
			*iIt++ = (i + 1)*resolutiony + (j + even);
			*iIt++ = (i + 1)*resolutiony + (j + even);
			*iIt++ = (i + even)*resolutiony + j;
			*iIt++ = i*resolutiony + (j + odd);
		}
	}
	// create fabric from mesh
	PxClothMeshDesc meshDesc;
	meshDesc.points.count = numParticles;
	meshDesc.points.stride = sizeof(PxClothParticle);
	meshDesc.points.data = particles;

	meshDesc.invMasses.count = numParticles;
	meshDesc.invMasses.stride = sizeof(PxClothParticle);
	meshDesc.invMasses.data = &particles->invWeight;

	meshDesc.triangles.count = numTriangles;
	meshDesc.triangles.stride = 3 * sizeof(PxU32);
	meshDesc.triangles.data = m_pTriangleIndices;

	// cook fabric
	PxClothFabric* fabric = PxClothFabricCreate(*physX, meshDesc, PxVec3(0, 1, 0));

	// create cloth
	m_pScarf = physX->createCloth(gPose, *fabric, particles, PxClothFlags(0));

	fabric->release();
	delete[] particles;

	m_pScarf->setSolverFrequency(240.0f);

	auto gameScene = GetScene();
	gameScene->GetPhysxProxy()->GetPhysxScene()->addActor(*m_pScarf);
	gameScene->GetPhysxProxy()->GetPhysxScene()->setVisualizationParameter(PxVisualizationParameter::eCLOTH_HORIZONTAL, 1);
	gameScene->GetPhysxProxy()->GetPhysxScene()->setVisualizationParameter(PxVisualizationParameter::eCLOTH_VERTICAL, 1);
	gameScene->GetPhysxProxy()->GetPhysxScene()->setVisualizationParameter(PxVisualizationParameter::eCLOTH_BENDING, 1);
	m_pScarf->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);
	m_pScarf->setInertiaScale(0.5f);
	m_pScarf->setClothFlag(PxClothFlag::eSWEPT_CONTACT, true);

	m_pMesh = new GameObject();
	//make collision shperes
	auto fw = m_pMesh->GetTransform()->GetForward();
	XMFLOAT3 pos = GetTransform()->GetPosition();
	pos.x = pos.x - fw.x * m_Radius;
	pos.y = pos.y + fw.y * m_Radius;
	pos.z = pos.z - fw.z * m_Radius;
	PxClothCollisionSphere spheres[2] =
	{
		PxClothCollisionSphere(PxVec3(0, -m_Height / 2, 0 + m_Radius), m_Radius*1.2f),
		PxClothCollisionSphere(PxVec3(0, m_Height / 2, 0 + m_Radius), m_Radius*1.2f)
	};
	m_pScarf->setCollisionSpheres(spheres, 2);
	//cloth from mesh
	auto m_meshDrawer = new MeshDrawComponent(m_nrOfIndices);
	m_pScarfMesh = new GameObject();
	m_pScarfMesh->AddComponent(m_meshDrawer);

	GetScene()->AddChild(m_pScarfMesh);
}

float Character::GetHeight() const
{
	return m_pController->GetTransform()->GetPosition().y;
}

bool Character::IsDead() const
{
	return m_IsDead;
}
