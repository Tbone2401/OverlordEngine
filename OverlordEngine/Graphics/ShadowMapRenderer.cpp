#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "RenderTarget.h"
#include <OverlordGame.h>
#include "../Scenegraph/SceneManager.h"
#include "MeshFilter.h"
#include "ShadowMapMaterial.h"
#include "../Components/ModelComponent.h"
#include "ModelAnimator.h"


ShadowMapRenderer::ShadowMapRenderer()
{
	m_pShadowMat = new ShadowMapMaterial();
}


ShadowMapRenderer::~ShadowMapRenderer()
{
	SafeDelete(m_pShadowMat);
	SafeDelete(m_pShadowRT);
}

void ShadowMapRenderer::Initialize(const GameContext& gameContext)
{
	if (!m_IsInitialized)
	{
		//set target
		RENDERTARGET_DESC desc{};

		desc.Height = OverlordGame::GetGameSettings().Window.Height;
		desc.Width = OverlordGame::GetGameSettings().Window.Width;
		desc.EnableDepthBuffer = true;
		desc.EnableDepthSRV = true;
		desc.GenerateMipMaps_Color = false;
		m_pShadowRT = new RenderTarget(gameContext.pDevice);
		m_pShadowRT->Create(desc);

		m_pShadowMat->Initialize(gameContext);
		m_IsInitialized = true;
	}
}

void ShadowMapRenderer::Begin(const GameContext& gameContext)
{
	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	gameContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV);

	SceneManager::GetInstance()->GetGame()->SetRenderTarget(m_pShadowRT);
	m_pShadowRT->Clear(gameContext, Colors::Yellow);

	m_pShadowMat->SetLight(m_LightVP);
}

void ShadowMapRenderer::Draw(const GameContext& gameContext, MeshFilter* pMeshFilter, ModelComponent* pModelComponent, const XMFLOAT4X4& worldMatrix)
{
	m_pShadowMat->SetWorld(worldMatrix);

	if(pModelComponent->HasAnimator())
		m_pShadowMat->EnableBones(true);
	else
		m_pShadowMat->EnableBones(false);

	m_pShadowMat->UpdateVariables(gameContext, pModelComponent);

	gameContext.pDeviceContext->IASetInputLayout(m_pShadowMat->GetInputLayout());
	UINT offset = 0;
	VertexBufferData vertexBufferData = pMeshFilter->GetVertexBufferData(gameContext, m_pShadowMat);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride, &offset);
	gameContext.pDeviceContext->IASetIndexBuffer(pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto tech = m_pShadowMat->GetTechnique();
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(pMeshFilter->m_IndexCount, 0, 0);
	}
}

void ShadowMapRenderer::End(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(nullptr);
}

ID3D11ShaderResourceView* ShadowMapRenderer::GetShadowMap()
{
	return m_pShadowRT->GetDepthShaderResourceView();
}

void ShadowMapRenderer::SetLight(const XMFLOAT3& position, const XMFLOAT3& direction)
{
	m_LightPosition = position;
	m_LightDirection = direction;

	XMStoreFloat4x4(&m_LightVP, XMMatrixIdentity());
	auto windowSettings = OverlordGame::GetGameSettings().Window;
	XMMATRIX projection, view;

	float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio*2.0f : windowSettings.Width;
	float viewHeight = (m_Size>0) ? m_Size*2.0f : windowSettings.Height;
	projection = XMMatrixOrthographicLH(viewWidth, viewHeight, 20.0f, 500.0f);

	XMVECTOR worldPosition = XMLoadFloat3(&m_LightPosition);
	XMVECTOR lookAt = XMLoadFloat3(&m_LightDirection);
	XMFLOAT3 up = XMFLOAT3(0, 1, 0); //y up???
	XMVECTOR upVec = XMLoadFloat3(&up);

	view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);

	XMStoreFloat4x4(&m_LightVP, view * projection);
}