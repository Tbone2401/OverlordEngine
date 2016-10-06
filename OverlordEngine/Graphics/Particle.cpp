//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Particle.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"


Particle::Particle(const ParticleEmitterSettings& emitterSettings) :
	m_EmitterSettings(emitterSettings)
{
}


Particle::~Particle(void)
{
}

void Particle::Update(const GameContext& gameContext)
{
	if (!m_IsActive) return;
	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if (m_CurrentEnergy < 0)
	{
		m_IsActive = false;
		return;
	}
	m_VertexInfo.Position.x += m_EmitterSettings.Velocity.x*gameContext.pGameTime->GetElapsed();
	m_VertexInfo.Position.y += m_EmitterSettings.Velocity.y*gameContext.pGameTime->GetElapsed();
	m_VertexInfo.Position.z += m_EmitterSettings.Velocity.z*gameContext.pGameTime->GetElapsed();

	m_VertexInfo.Color = m_EmitterSettings.Color;
	float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;
	m_VertexInfo.Color.w = particleLifePercent * 2.0f;

	if (m_SizeGrow > 1.0f)
		m_VertexInfo.Size = m_InitSize * m_SizeGrow * (1 - particleLifePercent);
	if (m_SizeGrow < 1.0f)
		m_VertexInfo.Size = m_InitSize * m_SizeGrow * (particleLifePercent);
}

void Particle::Init(XMFLOAT3 initPosition)
{
	m_IsActive = true;

	//Energy
	m_TotalEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_CurrentEnergy = m_TotalEnergy;

	//Position
	XMFLOAT3 temp = XMFLOAT3(1,0,0);
	XMVECTOR randomDirection = XMLoadFloat3(&temp);
	XMMATRIX randRotation = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	randomDirection = XMVector3Transform(randomDirection, randRotation);

	float distance = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);

	XMVECTOR pos = distance* randomDirection;
	pos += XMLoadFloat3(&initPosition);

	XMStoreFloat3(&m_VertexInfo.Position, pos);

	//Size
	m_InitSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_VertexInfo.Size = m_InitSize;

	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);

	//Rotation
	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);
}
