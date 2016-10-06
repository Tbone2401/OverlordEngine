#pragma once
#include "SceneGraph\GameObject.h"
#include "Components\Components.h"
class ClothPrefab : public GameObject
{
public:
	ClothPrefab(float width, float length, int resolution, XMFLOAT4 color);
	~ClothPrefab();
	virtual void Update(const GameContext& gameContext);



protected:
	virtual void Initialize(const GameContext& gameContext);
private:
	void SetEternalAcceleration(float x, float y, float z);

	float m_Width, m_Length;
	int m_Resolution, m_VertexCount, m_IndexCount;
	XMFLOAT4 m_Color;
	PxVec3 m_ExternalAcceleration;
	MeshDrawComponent *m_pMeshComp;
	std::vector<PxClothParticle> m_Particles;
	std::vector<PxU32> m_Primitives;
	std::vector<PxVec3> m_Normals;
	PxCloth *m_pCloth;

private:
	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	ClothPrefab(const ClothPrefab& yRef);
	ClothPrefab& operator=(const ClothPrefab& yRef);
};

