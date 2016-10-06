#include "stdafx.h"

#include "ClothPrefab.h"
#include "Components\Components.h"
#include "Helpers\PhysxHelper.h"
#include "Physx\PhysxProxy.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "Scenegraph\GameScene.h"
#include "Scenegraph/GameObject.h"


ClothPrefab::ClothPrefab(float width, float length, int resolution, XMFLOAT4 color)
	:m_Width(width)
	, m_Length(length)
	, m_Resolution(resolution)
	, m_ExternalAcceleration(5.0f)
	, m_Color(color) {
}


ClothPrefab::~ClothPrefab(void) {
}

void ClothPrefab::Initialize(const GameContext& gameContext) {
	UNREFERENCED_PARAMETER(gameContext);

	auto& physX = PxGetPhysics();

	auto gameScene = GetScene();
	UNREFERENCED_PARAMETER(gameScene);

	//Amount of vertices on one edge
	int vertexRes = m_Resolution + 2;

	m_VertexCount = (vertexRes)* (vertexRes);
	m_IndexCount = (vertexRes - 1) * (vertexRes - 1) * 4;


	float halfWidth = m_Width / 2;

	for (int y = 0; y < vertexRes; y++) {
		for (int x = 0; x < vertexRes; x++) {
			PxClothParticle particle(PxVec3(-halfWidth + x * (m_Width / (vertexRes - 1)), -y * (m_Length / vertexRes), 0.0f), (rand() % 1 + 100) / 100.0f);
			m_Particles.push_back(particle);
		}
	}

	for (int i = 0; i < vertexRes; i++) {
		m_Particles[i].invWeight = 0.0f;
	}

	//indices
	for (int y = 0; y < vertexRes - 1; y++) {
		for (int x = 0; x < vertexRes - 1; x++) {
			m_Primitives.push_back((y * vertexRes) + x);
			m_Primitives.push_back((y * vertexRes) + x + 1);
			m_Primitives.push_back(((y + 1) * vertexRes) + x + 1);
			m_Primitives.push_back(((y + 1) * vertexRes) + x);
		}
	}

	PxClothMeshDesc meshDesc;

	meshDesc.points.data = m_Particles.data();
	meshDesc.points.count = m_VertexCount;
	meshDesc.points.stride = sizeof(PxClothParticle);

	meshDesc.invMasses.data = &m_Particles.data()->invWeight;
	meshDesc.invMasses.count = m_VertexCount;
	meshDesc.invMasses.stride = sizeof(PxClothParticle);

	meshDesc.quads.data = m_Primitives.data();
	meshDesc.quads.count = m_IndexCount / 4;
	meshDesc.quads.stride = sizeof(PxU32) * 4;


	PxClothFabric* fabric = PxClothFabricCreate(physX, meshDesc, PxVec3(0, -10, 0));


	PxTransform pose = PxTransform(ToPxVec3(GetTransform()->GetWorldPosition()));
	m_pCloth = physX.createCloth(pose, *fabric, m_Particles.data(), PxClothFlags());

	m_pCloth->setSolverFrequency(240.0f);

	gameScene->GetPhysxProxy()->GetPhysxScene()->addActor(*m_pCloth);
	gameScene->GetPhysxProxy()->GetPhysxScene()->setVisualizationParameter(PxVisualizationParameter::eCLOTH_HORIZONTAL, 1);
	gameScene->GetPhysxProxy()->GetPhysxScene()->setVisualizationParameter(PxVisualizationParameter::eCLOTH_VERTICAL, 1);
	gameScene->GetPhysxProxy()->GetPhysxScene()->setVisualizationParameter(PxVisualizationParameter::eCLOTH_BENDING, 1);
	m_pCloth->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);
	m_pCloth->setInertiaScale(0.5f);
	m_pCloth->setClothFlag(PxClothFlag::eSWEPT_CONTACT, true);

	m_pCloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, PxClothStretchConfig(1.0f));
	m_pCloth->setStretchConfig(PxClothFabricPhaseType::eHORIZONTAL, PxClothStretchConfig(0.9f));
	m_pCloth->setStretchConfig(PxClothFabricPhaseType::eSHEARING, PxClothStretchConfig(0.75f));
	m_pCloth->setStretchConfig(PxClothFabricPhaseType::eBENDING, PxClothStretchConfig(0.5f));

	PxClothStretchConfig stretchConfig;
	stretchConfig.stiffness = 0.8f;
	stretchConfig.stiffnessMultiplier = 0.5f;
	stretchConfig.compressionLimit = 0.6f;
	stretchConfig.stretchLimit = 1.2f;
	m_pCloth->setStretchConfig(PxClothFabricPhaseType::eVERTICAL, stretchConfig);

	m_pCloth->setDragCoefficient(1.0f);

	m_pCloth->setExternalAcceleration(m_ExternalAcceleration);
	m_pCloth->setSelfCollisionDistance(0.1f);
	//MeshObject 


	//Normals
	m_Normals.resize(m_VertexCount);
	for (size_t i = 0; i < (UINT)m_IndexCount; i += 4) {
		PxVec3 p1 = m_Particles[m_Primitives[i]].pos;
		PxVec3 p2 = m_Particles[m_Primitives[i + 1]].pos;
		PxVec3 p3 = m_Particles[m_Primitives[i + 2]].pos;
		PxVec3 n = (p2 - p1).cross(p3 - p1);

		m_Normals[m_Primitives[i]] += n / 3.0f;			//Smooth the cloth mesh
		m_Normals[m_Primitives[i + 1]] += n / 3.0f;
		m_Normals[m_Primitives[i + 2]] += n / 3.0f;
		m_Normals[m_Primitives[i + 3]] += n / 3.0f;
	}

	for (size_t i = 0; i < (UINT)m_VertexCount; i++) {
		m_Normals[i] = m_Normals[i].getNormalized();
	}



	m_pMeshComp = new MeshDrawComponent(m_IndexCount / 2);

	for (size_t i = 0; i < (UINT)m_IndexCount; i += 4) {
		m_pMeshComp->AddQuad(
			VertexPosNormCol(ToXMFLOAT3(m_Particles[m_Primitives[i]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i]]), m_Color),
			VertexPosNormCol(ToXMFLOAT3(m_Particles[m_Primitives[i + 1]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i + 1]]), m_Color),
			VertexPosNormCol(ToXMFLOAT3(m_Particles[m_Primitives[i + 2]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i + 2]]), m_Color),
			VertexPosNormCol(ToXMFLOAT3(m_Particles[m_Primitives[i + 3]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i + 3]]), m_Color)
			);
	}

	AddComponent(m_pMeshComp);
	fabric->release();

	GetScene()->AddChild(this);
}

void ClothPrefab::Update(const GameContext& gameContext) {
	UNREFERENCED_PARAMETER(gameContext);

	XMFLOAT4 rotation = GetTransform()->GetWorldRotation();

	m_pCloth->setTargetPose(PxTransform(ToPxVec3(GetTransform()->GetWorldPosition()), PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));

	// Get updated cloth data
	PxClothParticleData* pData = m_pCloth->lockParticleData();
	PxClothParticle* pVertices = pData->particles;

	// Calculate new normals for mesh
	m_Normals.resize(m_VertexCount);
	for (size_t i = 0; i < (UINT)m_IndexCount; i += 4) {
		PxVec3 p1 = m_Particles[m_Primitives[i]].pos;
		PxVec3 p2 = m_Particles[m_Primitives[i + 1]].pos;
		PxVec3 p3 = m_Particles[m_Primitives[i + 2]].pos;
		PxVec3 n = (p2 - p1).cross(p3 - p1);
		n = n.getNormalized();

		m_Normals[m_Primitives[i]] = n;
		m_Normals[m_Primitives[i + 1]] = n;
		m_Normals[m_Primitives[i + 2]] = n;
		m_Normals[m_Primitives[i + 3]] = n;
	}



	m_pMeshComp->RemoveTriangles();
	for (size_t i = 0; i < (UINT)m_IndexCount; i += 4) {
		m_pMeshComp->AddQuad(
			VertexPosNormCol(ToXMFLOAT3(pVertices[m_Primitives[i]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i]]), m_Color),
			VertexPosNormCol(ToXMFLOAT3(pVertices[m_Primitives[i + 1]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i + 1]]), m_Color),
			VertexPosNormCol(ToXMFLOAT3(pVertices[m_Primitives[i + 2]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i + 2]]), m_Color),
			VertexPosNormCol(ToXMFLOAT3(pVertices[m_Primitives[i + 3]].pos), ToXMFLOAT3(m_Normals[m_Primitives[i + 3]]), m_Color)
			);
	}

	m_pMeshComp->UpdateBuffer();
	pData->unlock();
}

void ClothPrefab::SetEternalAcceleration(float x, float y, float z) {
	m_ExternalAcceleration.x = x;
	m_ExternalAcceleration.y = y;
	m_ExternalAcceleration.z = z;
	m_pCloth->setExternalAcceleration(m_ExternalAcceleration);
}