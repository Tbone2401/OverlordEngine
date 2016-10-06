//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SoftwareSkinningScene_3.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Content\ContentManager.h"
#include "Components\Components.h"
#include "BoneObject.h"
#include "Materials/ColorMaterial.h"

#define FPS_COUNTER 1

SoftwareSkinningScene_3::SoftwareSkinningScene_3(void) :
	GameScene(L"SoftwareSkinningScene_3"), m_pBone0(nullptr), m_pBone1(nullptr),
	m_RotationSign(1), m_BoneRotation(0)
{
}


SoftwareSkinningScene_3::~SoftwareSkinningScene_3(void)
{
}

void SoftwareSkinningScene_3::Initialize(const GameContext& gameContext)
{
	ColorMaterial* material = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(material, 0);
	GameObject* root = new GameObject();

	m_pBone0 = new BoneObject(0, 0, 15);
	m_pBone1 = new BoneObject(1, 0, 15);
	root->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);
	root->GetTransform()->Rotate(0.0f, -90.0f, 0.0f);
	AddChild(root);
	m_pBone0->CalculateBindPose();

	m_pMeshDrawer = new MeshDrawComponent();
	m_pMeshDrawer->SetTriangleCapacity(24);
	CreateMesh(15.0f);
	GameObject* drawer = new GameObject();
	drawer->AddComponent(m_pMeshDrawer);
	AddChild(drawer);
}

void SoftwareSkinningScene_3::Update(const GameContext& gameContext)
{
	XMFLOAT4X4 bindPose0 = m_pBone0->GetBindPose();
	XMFLOAT4X4 bindPose1 = m_pBone1->GetBindPose();
	XMMATRIX boneTransform0 = XMMatrixMultiply(XMLoadFloat4x4(&bindPose0), XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld()));
	XMMATRIX boneTransform1 = XMMatrixMultiply(XMLoadFloat4x4(&bindPose1), XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld()));

	for (size_t i = 0; i < m_SkinnedVertices.size(); i++)
	{
		XMVECTOR vertex = XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position);
		XMVECTOR vertex1 = XMVector3Transform(vertex, boneTransform0);
		XMVECTOR vertex2 = XMVector3Transform(vertex, boneTransform1);
		vertex = vertex1 * m_SkinnedVertices[i].m_Weight0 + vertex2 * m_SkinnedVertices[i].m_Weight1;
		XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, vertex);
	}

	m_pMeshDrawer->RemoveTriangles();
	for (size_t i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		m_pMeshDrawer->AddQuad(QuadPosNormCol(
			m_SkinnedVertices[i].TransformedVertex,
			m_SkinnedVertices[i + 1].TransformedVertex,
			m_SkinnedVertices[i + 2].TransformedVertex,
			m_SkinnedVertices[i + 3].TransformedVertex),
			true);
	}
	m_pMeshDrawer->UpdateBuffer();

	m_BoneRotation += m_RotationSign * 45.0f * gameContext.pGameTime->GetElapsed();
	if (m_RotationSign < 0 && m_BoneRotation <= -45.0f)
		m_RotationSign = 1;
	else if (m_RotationSign > 0 && m_BoneRotation >= 45)
		m_RotationSign = -1;
	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0.0f, 0.0f);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2.0f, 0.0f, 0.0f);
}

void SoftwareSkinningScene_3::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SoftwareSkinningScene_3::CreateMesh(float length)
{
	auto pos = XMFLOAT3(length / 2, 0, 0);
	auto offset = XMFLOAT3(length / 2, 2.5f, 2.5f); auto col = XMFLOAT4(1, 0, 0, 0.5f);

	//******
	//*BOX1*
	//******

	//FRONT
	auto norm = XMFLOAT3(0, 0, -1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	//BACK
	norm = XMFLOAT3(0, 0, 1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//TOP
	norm = XMFLOAT3(0, 1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	//BOTTOM
	norm = XMFLOAT3(0, -1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col));
	//LEFT
	norm = XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 1.0f, 0.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 1.0f, 0.0f));
	//RIGHT
	norm = XMFLOAT3(1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));

	//******
	//*BOX2*
	//******

	col = XMFLOAT4(0, 1, 0, 0.5f);
	pos = XMFLOAT3(22.5f, 0, 0);
	//FRONT
	norm = XMFLOAT3(0, 0, -1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col,0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//BACK
	norm = XMFLOAT3(0, 0, 1);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	//TOP
	norm = XMFLOAT3(0, 1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//BOTTOM
	norm = XMFLOAT3(0, -1, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//LEFT
	norm = XMFLOAT3(-1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.5f, 0.5f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(-offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.5f, 0.5f));
	//RIGHT
	norm = XMFLOAT3(1, 0, 0);
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z), norm, col, 0.0f, 1.0f));
	m_SkinnedVertices.push_back(SkinnedVertex(XMFLOAT3(offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z), norm, col, 0.0f, 1.0f));
}