#include "stdafx.h"
#include "BoneObject.h"
#include "Components/Components.h"


BoneObject::BoneObject(int boneId, int materialId, float length) : 
	m_Length(length), m_BoneId(boneId), m_MaterialId(materialId)
{
}

BoneObject::~BoneObject()
{
}

XMFLOAT4X4 BoneObject::GetBindPose()
{
	return m_BindPose;
}

void BoneObject::CalculateBindPose()
{
	XMFLOAT4X4 world = GetTransform()->GetWorld();
	XMMATRIX worldMatrix = XMLoadFloat4x4(&world);
	XMVECTOR determinant = XMMatrixDeterminant(worldMatrix);
	XMMATRIX worldInverse = XMMatrixInverse(&determinant, worldMatrix);
	XMStoreFloat4x4(&m_BindPose, worldInverse);

	for (BoneObject* b : GetChildren<BoneObject>())
		b->CalculateBindPose();
}

void BoneObject::AddBone(GameObject* pBone)
{
	pBone->GetTransform()->Translate(0.0f, 0.0f, -m_Length);
	AddChild(pBone);
}

void BoneObject::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	GameObject* object = new GameObject();
	ModelComponent* modelComponent = new ModelComponent(L"./Resources/Meshes/Bone.ovm");
	modelComponent->SetMaterial(m_MaterialId);
	object->AddComponent(modelComponent);
	object->GetTransform()->Scale(m_Length, m_Length, m_Length);
	AddChild(object);
}