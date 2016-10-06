#pragma once
class MeshFilter;
class RenderTarget;
class ModelComponent;
class ShadowMapMaterial;

class ShadowMapRenderer
{
public:
	ShadowMapRenderer();
	~ShadowMapRenderer();

	void Begin(const GameContext& gameContext);
	void Draw(const GameContext& gameContext, MeshFilter* pMeshFilter, ModelComponent* pModelComponent, const XMFLOAT4X4& worldMatrix);
	void End(const GameContext& gameContext);

	XMFLOAT3 GetLightDirection() { return m_LightDirection; }
	XMFLOAT4X4 GetLightVP() { return m_LightVP; }
	ShadowMapMaterial* GetMaterial() { return m_pShadowMat; }
	ID3D11ShaderResourceView* GetShadowMap();

	void Initialize(const GameContext &gameContext);
	void SetLight(const XMFLOAT3& position, const XMFLOAT3 &direction);
private:
	bool m_IsInitialized = false;
	XMFLOAT3 m_LightDirection;
	XMFLOAT3 m_LightPosition;
	XMFLOAT4X4 m_LightVP;
	ShadowMapMaterial* m_pShadowMat = nullptr;
	RenderTarget* m_pShadowRT = nullptr;
	RenderTarget* m_pDefaultRT = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
	const float m_Size = 100;
	float m_NearPlane = 2.5f;
	float m_FarPlane = 50.0f;

	//Remove assignment and copy constructor
	ShadowMapRenderer& operator=(const ShadowMapRenderer& other);
	ShadowMapRenderer(const ShadowMapRenderer& other);
};

