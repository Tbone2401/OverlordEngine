//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ParticleEmitterComponent.h"
#include "../Helpers/EffectHelper.h"
#include "../Content/ContentManager.h"
#include "../Content/TextureDataLoader.h"
#include "../Graphics/Particle.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"


ParticleEmitterComponent::ParticleEmitterComponent(const wstring& assetFile, int particleCount) :
	m_ParticleCount(particleCount),
	m_AssetFile(assetFile)
{
	for (int i = 0; i < m_ParticleCount; ++i)
	{
		m_Particles.push_back(new Particle(m_Settings));
	}
}


ParticleEmitterComponent::~ParticleEmitterComponent(void)
{
	for (int i = 0; i < m_ParticleCount; ++i)
	{
		delete m_Particles[i];
	}
	m_Particles.clear();
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ParticleRenderer.fx");
	m_pDefaultTechnique = m_pEffect->GetTechniqueByIndex(0);

	m_pWvpVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pWvpVariable->IsValid())
	{
		Logger::LogError(L"ParticleEmitterComponent::LoadEffect() > Shader variable \'gWorldViewProj\' not valid!");
		return;
	}

	m_pViewInverseVariable = m_pEffect->GetVariableByName("gViewInverse")->AsMatrix();
	if (!m_pViewInverseVariable->IsValid())
	{
		Logger::LogError(L"ParticleEmitterComponent::LoadEffect() > Shader variable \'gViewInverse\' not valid!");
		return;
	}

	m_pTextureVariable = m_pEffect->GetVariableByName("gParticleTexture")->AsShaderResource();
	if (!m_pTextureVariable->IsValid())
	{
		Logger::LogError(L"ParticleEmitterComponent::Initialize() > Shader variable \'gParticleTexture\' not valid!");
		return;
	}
	EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize);
}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	if (m_pVertexBuffer != nullptr) m_pVertexBuffer->Release();

	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ParticleVertex) * m_ParticleCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = m_Particles.data();

	hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(hr)) Logger::LogHResult(hr, L"ParticleEmitterComponent::CreateVertexBuffer");
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{

	float particleInterval = ((m_Settings.MaxEnergy + m_Settings.MinEnergy) / 2) / m_ParticleCount;
	m_LastParticleInit += gameContext.pGameTime->GetElapsed();
	m_ActiveParticles = 0;
	
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ParticleVertex* pBuffer = (ParticleVertex*)mappedResource.pData;
	for (Particle* p : m_Particles)
	{
		p->Update(gameContext);
		if (p->IsActive())
		{
			pBuffer[m_ActiveParticles] = p->GetVertexInfo();
			++m_ActiveParticles;
		}
		else if (m_LastParticleInit >= particleInterval)
		{
			p->Init(GetTransform()->GetWorldPosition());
			pBuffer[m_ActiveParticles] = p->GetVertexInfo();
			++m_ActiveParticles;
			m_LastParticleInit = 0.0f;
		}
	}
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	m_pWvpVariable->SetMatrix((float*)&gameContext.pCamera->GetViewProjection());
	m_pViewInverseVariable->SetMatrix((float*)&gameContext.pCamera->GetViewInverse());

	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	//Set Inputlayout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	//Set Primitive Topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	//Set Vertex Buffer
	UINT offset = 0;
	UINT stride = sizeof(ParticleVertex);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	////DRAW
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pDefaultTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}
