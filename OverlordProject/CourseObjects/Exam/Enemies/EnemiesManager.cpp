#include "stdafx.h"
#include "EnemiesManager.h"
#include "Enemy.h"
#include "Score.h"
#include "Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"


EnemiesManager::EnemiesManager() 
	:m_DidPlayerKill(false),
	m_Index(0),
	m_State(0)
{
}

void EnemiesManager::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//********************
	//Material
	//********************
	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial_Shadow();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/color.tga");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 200);
}

void EnemiesManager::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void EnemiesManager::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	m_DidPlayerKill = false;
	m_State = 0;
	for (size_t i = 0; i < m_EnemyVector.size(); ++i)
	{
		if(m_EnemyVector[i] == nullptr)
		{
			continue;
		}
		if (m_EnemyVector[i]->IsKilled())
		{
			Score::GetInstance()->KilledEnemy();
			RemoveChild(m_EnemyVector[i]);
			m_EnemyVector[i] = nullptr;
			m_State = 1;
			return;
		}
		if(m_EnemyVector[i]->DidDamage())
		{
			m_State = 2;
			return;
		}
	}
}
void EnemiesManager::AddEnemy(XMFLOAT3 pos, XMFLOAT3 left, XMFLOAT3 right)
{
	m_EnemyVector.push_back(new Enemy(pos, left, right));
	AddChild(m_EnemyVector[m_EnemyVector.size()-1]);
	++m_Index;
}
int EnemiesManager::PlayerInteraction()
{
	return m_State;
}
EnemiesManager::~EnemiesManager()
{
}



