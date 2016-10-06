//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "Components.h"
#include "..\Base\GeneralStructs.h"
#include "../Content/ContentManager.h"
#include "../Helpers/EffectHelper.h"
#include "TransformComponent.h"
#include "../Graphics/TextureData.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include <limits>

TerrainComponent::TerrainComponent(const wstring& heightMapFile, const wstring& textureFile, unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth, float maxHeight) :
	m_HeightMapFile(heightMapFile),
	m_TextureFile(textureFile),
	m_NrOfRows(nrOfRows),
	m_NrOfColumns(nrOfColumns),
	m_NrOfVertices(nrOfRows*nrOfColumns),
	m_NrOfTriangles(2 * (nrOfRows - 1) * (nrOfColumns - 1)),
	m_Width(width),
	m_Depth(depth),
	m_MaxHeight(maxHeight)
{
	
}

TerrainComponent::~TerrainComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
}

void TerrainComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/PosNormTex3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	//TODO: Error handling
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	UINT ilSize = 0;
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, ilSize))
		Logger::LogError(L"TerrainComponent::Initialize >> BuildInputLayout failed!");

	//Texture
	m_pTextureData = ContentManager::Load<TextureData>(m_TextureFile);

	//Load Height Map
	ReadHeightMap();

	//Create Vertices & Triangles
	CalculateVerticesAndIndices();

	//Build Vertexbuffer
	BuildVertexBuffer(gameContext);

	//Build Indexbuffer
	BuildIndexBuffer(gameContext);

	//Create PhysX Heightfield
	CreatePxHeightField();

}

void TerrainComponent::Update(const GameContext& gameContext)
{ 
	UNREFERENCED_PARAMETER(gameContext);
}

void TerrainComponent::Draw(const GameContext& gameContext)
{ 
	XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMMATRIX viewProj = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	m_pDiffuseVariable->SetResource(m_pTextureData->GetShaderResourceView());

	// Set vertex buffer
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p< techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

//Exercise - Heightmap
void TerrainComponent::ReadHeightMap()
{
	m_VecHeightValues.resize(m_NrOfVertices, 0);
	std::ifstream inFile;
	inFile.open(m_HeightMapFile.c_str(), ios_base::binary);
	if(!inFile)
	{
		Logger::LogFormat(LogLevel::Error, L"Loading terrain \'%ls\' failed!", m_HeightMapFile.c_str());
		return;
	}
	inFile.read(reinterpret_cast<char*>(&m_VecHeightValues[0]), static_cast<std::streamsize>(m_NrOfVertices * sizeof(unsigned short)));
	inFile.close();
}

//Exercise - Flat Grid
void TerrainComponent::CalculateVerticesAndIndices()
{
	//**VERTICES
	//Reserve spots in the buffer
	m_VecVertices.resize(m_NrOfColumns * m_NrOfRows, VertexPosNormTex());
	//Calculate the Initial Position (Terrain centered on the origin)
	float cellWidth = m_Width / m_NrOfColumns;
	float cellDepth = m_Depth / m_NrOfRows;
	float cellXPos;
	float cellZPos = m_Depth / 2.0f;

	for (PxU32 row = 0; row < m_NrOfRows; row++)
	{
		cellXPos = -m_Width / 2.0f;
		for (PxU32 col = 0; col < m_NrOfColumns; col++)
		{
			int vertexId = row * m_NrOfColumns + col;
			m_VecVertices[vertexId].Position.x = cellXPos;
			m_VecVertices[vertexId].Position.y = (float)m_VecHeightValues[col + row * m_NrOfColumns] / std::numeric_limits<unsigned short>::max() * m_MaxHeight;
			m_VecVertices[vertexId].Position.z = cellZPos;

			m_VecVertices[vertexId].TexCoord.x = (float)col / m_NrOfColumns;
			m_VecVertices[vertexId].TexCoord.y = (float)row / m_NrOfColumns;

			cellXPos += cellWidth;
		}
		cellZPos -= cellDepth;
	}

	int nrQuadsRow = m_NrOfRows - 1;
	int nrQuadsColumn = m_NrOfColumns - 1;

	for (int row = 0; row < nrQuadsRow; ++row)
	{
		for (int col = 0; col < nrQuadsColumn; ++col)
		{
			int a = row * m_NrOfColumns + col;
			int b = a + 1;
			int c = a + m_NrOfColumns;
			int d = c + 1;
			AddQuad(a, b, c, d);
		}
	}

	vector<XMFLOAT3> faceNormals;
	for (unsigned int i = 0; i < m_VecIndices.size(); i+=6)
	{
		XMVECTOR a, b, c, d, e, f;
		a = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i)).Position);
		b = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 1)).Position);
		c = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 2)).Position);
		d = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 3)).Position);
		e = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 4)).Position);
		f = XMLoadFloat3(&m_VecVertices.at(m_VecIndices.at(i + 5)).Position);

		XMVECTOR v1, v2, normal;
		v1 = a - c;
		v2 = a - b;
		normal = XMVector3Cross(v2 ,v1);
		normal = XMVector3Normalize(normal);
		XMFLOAT3 normalFloat3;
		XMStoreFloat3(&normalFloat3, normal);
		faceNormals.push_back(normalFloat3);

		v1 = f - e;
		v2 = f - d;
		normal = XMVector3Cross(v2, v1);
		normal = XMVector3Normalize(normal);
		XMStoreFloat3(&normalFloat3, normal);
		faceNormals.push_back(normalFloat3);
	}

	int numFacesPerRow = (m_NrOfColumns - 1) * 2;
	int index[6];
	//from left front to right back
	for (unsigned int row = 0; row < m_NrOfRows; ++row)
	{
		for (unsigned int col = 0; col < m_NrOfColumns; ++col)
		{
			int centerindex = numFacesPerRow * row + col * 2;
			index[0] = centerindex - 1;
			index[1] = centerindex;
			index[2] = centerindex + 1;
			index[3] = centerindex - numFacesPerRow - 2;
			index[4] = centerindex - numFacesPerRow - 1;
			index[5] = centerindex - numFacesPerRow - 0;

			if (col == 0)
			{
				index[0] = -1;
				index[3] = -1;
				index[4] = -1;
			}
			if (col == m_NrOfColumns - 1)
			{
				index[1] = -1;
				index[2] = -1;
				index[5] = -1;
			}

			XMVECTOR sum;
			int it = 0;
			for (int i = 0; i < 6; ++i)
			{
				if ((index[i] >= 0) && (index[i] < (int)faceNormals.size()))
				{
					sum += XMLoadFloat3(&faceNormals.at(index[i]));
					it++;
				}
			}
			sum /= (float)it;
			XMVector3Normalize(sum);
			XMFLOAT3 normal;
			XMStoreFloat3(&normal, sum);
			m_VecVertices[col + row * m_NrOfColumns].Normal = normal;
		}
	}
}

//Exercise - Flat Grid
void TerrainComponent::AddTriangle(unsigned int a, unsigned int b, unsigned c)
{
	m_VecIndices.push_back(a);
	m_VecIndices.push_back(b);
	m_VecIndices.push_back(c);
}

//Exercise - Flat Grid
void TerrainComponent::AddQuad(unsigned int a, unsigned int b, unsigned c, unsigned d)
{
	AddTriangle(a, b, d);
	AddTriangle(a, d, c);
}

void TerrainComponent::BuildVertexBuffer(const GameContext& gameContext)
{
	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * m_NrOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecVertices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void TerrainComponent::BuildIndexBuffer(const GameContext& gameContext)
{
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecIndices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Indexbuffer");
	m_NumIndices = m_VecIndices.size();
}

void TerrainComponent::CreatePxHeightField()
{
	PxPhysics* physX = PhysxManager::GetInstance()->GetPhysics();
	PxRigidStatic* actor = physX->createRigidStatic(PxTransform::createIdentity());
	if(!actor)
		Logger::LogError(L"TerrainComponent: Creating the terrain actor failed");

	PxMaterial* defaultMaterial = physX->createMaterial(0.5f, 0.5f, 0.2f);

	PxReal minHeight = PX_MAX_F32;
	PxReal maxHeight = -PX_MAX_F32;
	for (PxU32 i = 0; i < m_NrOfColumns * m_NrOfRows; i++)
	{
		minHeight = PxMin(minHeight, m_VecVertices[i].Position.y);
		maxHeight = PxMax(maxHeight, m_VecVertices[i].Position.y);
	}

	PxReal deltaHeight = maxHeight - minHeight;
	PxReal quantization = (PxReal)0x7fff;
	PxReal heightScale = PxMax(deltaHeight / quantization, PX_MIN_HEIGHTFIELD_Y_SCALE);

	PxHeightFieldSample* hfSamples = new PxHeightFieldSample[m_NrOfColumns * m_NrOfRows];

	for (PxU32 row = 0; row < m_NrOfRows; row++)
	{
		for (PxU32 col = 0; col < m_NrOfColumns; col++)
		{
			PxI32 height = PxI16(quantization * ((m_VecVertices[row * m_NrOfColumns + col].Position.y - minHeight) / deltaHeight));
			assert(height <= 0xffff);
			PxHeightFieldSample& smp = (PxHeightFieldSample&)(hfSamples[row * m_NrOfColumns + col]);
			smp.height = (PxI16)(height);
			smp.setTessFlag();
			smp.materialIndex0 = 0;
			smp.materialIndex1 = 0;
		}
	}

	PxHeightFieldDesc terrainDesc;
	terrainDesc.format = PxHeightFieldFormat::eS16_TM;
	terrainDesc.nbColumns = m_NrOfColumns;
	terrainDesc.nbRows = m_NrOfRows;
	terrainDesc.samples.data = hfSamples;
	terrainDesc.samples.stride = sizeof(PxHeightFieldSample);

	PxHeightFieldGeometry hfGeom;
	hfGeom.heightFieldFlags = PxMeshGeometryFlags();
	hfGeom.columnScale = m_Width / (m_NrOfColumns);
	hfGeom.rowScale = m_Depth / (m_NrOfRows );
	hfGeom.heightScale = deltaHeight != 0.0f ? heightScale : 1.0f;
	hfGeom.heightField = physX->createHeightField(terrainDesc);
	if(!hfGeom.heightField)
		Logger::LogError(L"TerrainComponent: Creating the heightfield failed");

	delete[] hfSamples;

	PxTransform localPose;
	localPose.p = PxVec3(-(m_NrOfColumns * 0.5f), minHeight, (m_NrOfRows * 0.5f));
	localPose.q = PxQuat(PxPiDivTwo, PxVec3(0.0f, 1.0f, 0.0f));
	PxShape* shape = physX->createShape(hfGeom, &defaultMaterial, 1);
	if(!shape)
		Logger::LogError(L"TerrainComponent: Creating the terrain shape failed");
	shape->setLocalPose(localPose);

	PxFilterData CollisionGroups;
	CollisionGroups.word0 = CollisionGroupFlag::Group0;
	CollisionGroups.word1 = static_cast<CollisionGroupFlag>(Group1 | Group2);
	shape->setSimulationFilterData(CollisionGroups);
	shape->setQueryFilterData(CollisionGroups);

	actor->attachShape(*shape);
	GetGameObject()->GetScene()->GetPhysxProxy()->GetPhysxScene()->addActor(*actor);
}