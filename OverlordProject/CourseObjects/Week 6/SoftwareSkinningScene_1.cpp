//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SoftwareSkinningScene_1.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components/Components.h"
#include <Physx/PhysxManager.h>
#include "BoneObject.h"
#include "Materials/ColorMaterial.h"

#define FPS_COUNTER 1

SoftwareSkinningScene_1::SoftwareSkinningScene_1(void) :
GameScene(L"SoftwareSkinningScene_1"), m_pBone0(nullptr), m_pBone1(nullptr),
m_RotationSign(1), m_BoneRotation(0)
{
}


SoftwareSkinningScene_1::~SoftwareSkinningScene_1(void)
{
}

void SoftwareSkinningScene_1::Initialize(const GameContext& gameContext)
{
	ColorMaterial* material = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(material, 0);
	GameObject* root = new GameObject();
	AddChild(root);
	m_pBone0 = new BoneObject(0, 0, 15);
	m_pBone1 = new BoneObject(1, 0, 15);
	root->AddChild(m_pBone0);
	m_pBone0->AddBone(m_pBone1);
	root->GetTransform()->Rotate(0.0f, -90.0f, 0.0f);	
}

void SoftwareSkinningScene_1::Update(const GameContext& gameContext)
{
	m_BoneRotation += m_RotationSign * 45.0f * gameContext.pGameTime->GetElapsed();
	if (m_RotationSign < 0 && m_BoneRotation <= -45.0f)
		m_RotationSign = 1;
	else if (m_RotationSign > 0 && m_BoneRotation >= 45)
		m_RotationSign = -1;
	m_pBone0->GetTransform()->Rotate(m_BoneRotation, 0.0f, 0.0f);
	m_pBone1->GetTransform()->Rotate(-m_BoneRotation * 2.0f, 0.0f, 0.0f);
}

void SoftwareSkinningScene_1::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}