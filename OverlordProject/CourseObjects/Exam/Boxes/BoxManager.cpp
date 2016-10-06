#include "stdafx.h"
#include "BoxManager.h"
//#include "RandBox.h"
#include "Boxes.h"
#include "ExplosionMaterial.h"
#include "Score.h"


BoxManager::BoxManager() 
	: m_DidPlayerTouchBox(false),
	m_HasAsked(false),
	m_Index(100)
{
}

void BoxManager::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BoxManager::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void BoxManager::Update(const GameContext& gameContext)
{
	m_DidPlayerTouchBox = false;
	for (size_t i = 0; i < m_BoxArray.size(); ++i )
	{
		if (m_BoxArray[i] == nullptr){}
		else if (m_BoxArray[i]->GetDestroy())
		{
			RemoveChild(m_BoxArray[i]);
			m_BoxArray[i] = nullptr;
			m_HasAsked = false;
		}
		else if(m_HasAsked){}
		else if (m_BoxArray[i]->GetTouched())
		{
			Score::GetInstance()->DestroyedBox();
			reinterpret_cast<ExplosionMaterial*>(gameContext.pMaterialManager->GetMaterial(i + 100))->StartExplosion();
			m_DidPlayerTouchBox = true;
			m_BoxArray[i]->StopColliding();
		}
	}
	return;
}

BoxManager::~BoxManager()
{
}

const bool BoxManager::DidPlayerTouchBox()
{
	if (m_DidPlayerTouchBox)
	{
		m_HasAsked = true;
	}
	return m_DidPlayerTouchBox;
}

void BoxManager::AddBox(PxVec3 pos, bool isRand, const GameContext& gamecontext)
{
	if(isRand)
	{
		//m_BoxArray.push_back(new RandBox(pos));
	}
	else
	{
		AddMaterial(gamecontext);
		m_BoxArray.push_back(new Boxes(pos, m_Index));
		AddChild(m_BoxArray[m_BoxArray.size()-1]);
		++m_Index;
	}
}

void BoxManager::AddMaterial(const GameContext& gameContext)
{
	if (m_Index > 200) Logger::LogError(L"Error, more than a 100 boxes");
	ExplosionMaterial* explMat = new ExplosionMaterial();
	explMat->ReduceThickness(10.0f);
	explMat->SetDiffuseTexture(L"./Resources/Textures/Skulls_Diffusemap.tga");
	explMat->SetEffectLimits(0.0f, 0.0f);
	explMat->SetExplosionPos(XMFLOAT3(0.0f, 0.0f, 0.0f));
	explMat->SetExplosionPower(5.0f);
	explMat->SetGravity(-9.81f);
	explMat->SetLimits(-2.5f, 2.5f);
	explMat->SetNoiseMap(L"./Resources/Textures/Skulls_Heightmap.tga");
	explMat->SetNoisemapInfluence(5.0f);
	explMat->SetNormalMap(L"./Resources/Textures/Skulls_Normalmap.tga");
	explMat->SetTime(0.0f);
	explMat->SetExplosionPos(XMFLOAT3(0.0f, 2.5f, 0.0f));
	gameContext.pMaterialManager->AddMaterial(explMat, m_Index);
}
