//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SpriteComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Scenegraph/GameObject.h"
#include "../Graphics/TextureData.h"
#include "../Content/ContentManager.h"
#include "../Graphics/SpriteRenderer.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"

SpriteComponent::SpriteComponent(const wstring& spriteAsset, XMFLOAT2 pivot, XMFLOAT4 color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color),
	m_pTexture(nullptr)
{

}


SpriteComponent::~SpriteComponent(void)
{
}

void SpriteComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SpriteComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (!m_pTexture)
		return;

	// Here you need to draw the SpriteComponent using the Draw of the sprite renderer
	SpriteRenderer::GetInstance()->Draw(m_pTexture, XMFLOAT2(GetTransform()->GetPosition().x, GetTransform()->GetPosition().y), m_Color, m_Pivot, XMFLOAT2(GetTransform()->GetScale().x, GetTransform()->GetScale().y), QuaternionToEuler(GetTransform()->GetRotation()).z);
	// The sprite renderer is a singleton
	// you will need to position, the rotation and the scale
	// You can use the QuaternionToEuler function to help you with the z rotation 
}
