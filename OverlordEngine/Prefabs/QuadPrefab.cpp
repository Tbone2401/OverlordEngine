#include "stdafx.h"
#include "QuadPrefab.h"
#include "../Components/Components.h"


QuadPrefab::QuadPrefab(float width, float height, XMFLOAT4 color) :
	m_Width(width),
	m_Height(height),
	m_Color(color)
{
}


QuadPrefab::~QuadPrefab()
{
}

void QuadPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto mesh = new MeshDrawComponent(2);
	
	QuadPosNormCol quad;
	quad.Vertex1.Position.x = -m_Width / 2.0f;
	quad.Vertex1.Position.y = 0.0f;
	quad.Vertex1.Position.z = m_Height / 2.0f;
	quad.Vertex1.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	quad.Vertex1.Color = m_Color;

	quad.Vertex2.Position.x = m_Width / 2.0f;
	quad.Vertex2.Position.y = 0.0f;
	quad.Vertex2.Position.z = m_Height / 2.0f;
	quad.Vertex2.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	quad.Vertex2.Color = m_Color;

	quad.Vertex3.Position.x = m_Width / 2.0f;
	quad.Vertex3.Position.y = 0.0f;
	quad.Vertex3.Position.z = -m_Height / 2.0f;
	quad.Vertex3.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	quad.Vertex3.Color = m_Color;

	quad.Vertex4.Position.x = -m_Width / 2.0f;
	quad.Vertex4.Position.y = 0.0f;
	quad.Vertex4.Position.z = -m_Height / 2.0f;
	quad.Vertex4.Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	quad.Vertex4.Color = m_Color;

	mesh->AddQuad(quad);

	AddComponent(mesh);
}