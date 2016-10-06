//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "PostProcessingMaterial.h"
#include "RenderTarget.h"
#include "../Base/OverlordGame.h"
#include "../Content/ContentManager.h"

ID3D11Buffer* PostProcessingMaterial::m_pVertexBuffer = nullptr;
unsigned int PostProcessingMaterial::m_VertexBufferStride = sizeof(VertexPosTex);

ID3D11InputLayout *PostProcessingMaterial::m_pInputLayout = nullptr;
vector<ILDescription> PostProcessingMaterial::m_pInputLayoutDescriptions = vector<ILDescription>();
UINT PostProcessingMaterial::m_pInputLayoutSize = 0;
UINT PostProcessingMaterial::m_InputLayoutID = 0;

PostProcessingMaterial::PostProcessingMaterial(wstring effectFile, const wstring& technique)
	: m_IsInitialized(false),
	m_effectFile(effectFile),
	m_TechniqueName(technique),
	m_pEffect(nullptr),
	m_pTechnique(nullptr),
	m_pRenderTarget(nullptr)
{
}


PostProcessingMaterial::~PostProcessingMaterial()
{
	//Delete/Release NON-STATIC objects
	//SafeDelete(m_pRenderTarget);
	m_pInputLayoutDescriptions.clear();
	SafeRelease(m_pInputLayout);
	//SafeRelease(m_pVertexBuffer);
	//m_pInputLayoutDescriptions.clear();
	delete m_pRenderTarget;
}

void PostProcessingMaterial::Destroy()
{
	//Delete/Release/Clear STATIC objects/lists
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pInputLayout);
}

void PostProcessingMaterial::Initialize(const GameContext& gameContext)
{
	if(!m_IsInitialized)
	{
		//1. LoadEffect (LoadEffect(...))
		LoadEffect(gameContext, m_effectFile);
		//2. CreateInputLaytout (CreateInputLayout(...))
		CreateInputLayout(gameContext, m_pTechnique);
		//   CreateVertexBuffer (CreateVertexBuffer(...)) > As a TriangleStrip (FullScreen Quad)
		CreateVertexBuffer(gameContext);
		//3. Create RenderTarget (m_pRenderTarget)
		m_pRenderTarget = new RenderTarget(gameContext.pDevice);
		RENDERTARGET_DESC desc;
		desc.EnableColorSRV = true;
		desc.EnableDepthSRV = true;
		desc.Height = OverlordGame::GetGameSettings().Window.Height;
		desc.Width = OverlordGame::GetGameSettings().Window.Width;
		auto hr = m_pRenderTarget->Create(desc);
		Logger::LogHResult(hr, L"PostProcessingMaterial::Initialize()");
		m_IsInitialized = true;
	}
}

bool PostProcessingMaterial::LoadEffect(const GameContext& gameContext, const wstring& effectFile)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Load Effect through ContentManager
	m_pEffect = ContentManager::Load<ID3DX11Effect>(effectFile);
	//Check if m_TechniqueName (default constructor parameter) is set
	// If SET > Use this Technique (+ check if valid)
	if (m_TechniqueName.size() > 0)
	{
		string techString = string(m_TechniqueName.begin(), m_TechniqueName.end());
		m_pTechnique = m_pEffect->GetTechniqueByName(techString.c_str());
	}
	else
		m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	// If !SET > Use Technique with index 0
	//Call LoadEffectVariables
	LoadEffectVariables();

	return true;
}

//Static Function
void PostProcessingMaterial::Draw(const GameContext& gameContext, RenderTarget* previousRendertarget)
{
	//1. Clear the object's RenderTarget (m_pRenderTarget) [Check RenderTarget Class]
	m_pRenderTarget->Clear(gameContext, Colors::CornflowerBlue);
	//2. Call UpdateEffectVariables(...)
	UpdateEffectVariables(previousRendertarget);
	//3. Set InputLayout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	//4. Set VertexBuffer
	UINT strides = sizeof(VertexPosTex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &strides, &offset);
	//5. Set PrimitiveTopology (TRIANGLESTRIP!!)
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//6. Draw (NOT Indexed!)
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p) {
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(4, 0);
	}

	//7. Unbind the PreviousRendertarget's ShaderResource view from the pipeline. Because we'll need to use it as RenderTarget later on. 
	//   A resource can't be bound as RenderTarget & ShaderResource at the same time. See what happens when you comment these last lines.
	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	gameContext.pDeviceContext->PSSetShaderResources(0, 1, pSRV);
}

//Static Function
void PostProcessingMaterial::CreateVertexBuffer(const GameContext& gameContext)
{

	if (m_pVertexBuffer)
		return;
	// create a vertex buffer for a full screen quad using VertexPosTex
	vector<VertexPosTex> vertices = 
	{
		VertexPosTex(XMFLOAT3(-1, -1, 0), XMFLOAT2(0, 1)),
		VertexPosTex(XMFLOAT3(-1, 1, 0), XMFLOAT2(0, 0)),
		VertexPosTex(XMFLOAT3(1, -1, 0), XMFLOAT2(1, 1)),
		VertexPosTex(XMFLOAT3(1, 1, 0), XMFLOAT2(1, 0)),
	};

	// Create buffer
	D3D11_BUFFER_DESC buffDesc;
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = sizeof(VertexPosTex) * 4;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices.data();

	auto hr = gameContext.pDevice->CreateBuffer(&buffDesc, &initData, &m_pVertexBuffer);
	if (Logger::LogHResult(hr, L"PostProcessingMaterial::CreateVertexBuffer"))
		return;
}

void PostProcessingMaterial::CreateInputLayout(const GameContext& gameContext, ID3DX11EffectTechnique* pTechnique)
{
	//Static InputLayout Object (Created once, shared between other PPMaterials)
	if (m_pInputLayout)
		return;

	auto hr = EffectHelper::BuildInputLayout(gameContext.pDevice, pTechnique, &m_pInputLayout, m_pInputLayoutDescriptions, m_pInputLayoutSize, m_InputLayoutID);
	if (Logger::LogHResult(hr, L"PostProcessingMaterial::BuildInputLayout"))
		return;
	//+ Check for errors (HRESULT)
}

RenderTarget* PostProcessingMaterial::GetRenderTarget() const
{
	return m_pRenderTarget;
}