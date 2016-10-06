#pragma once
#include "..\OverlordEngine\Scenegraph\GameObject.h"
class BoneObject :
	public GameObject
{
public:
	BoneObject(const BoneObject& other) = delete;
	BoneObject & operator=(const BoneObject& other) = delete;
	BoneObject(int boneId, int materialId, float length = 5.0f);
	~BoneObject();

	void AddBone(GameObject* pBone);
	XMFLOAT4X4 GetBindPose();
	void CalculateBindPose();
protected:
	virtual void Initialize(const GameContext& gameContext);

private:
	float m_Length;
	int m_BoneId;
	int m_MaterialId;
	XMFLOAT4X4 m_BindPose;
};

struct SkinnedVertex
{
	SkinnedVertex(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float weight0 = 0.0f, float weight1 = 0.0f):
	TransformedVertex(position, normal, color),
		OriginalVertex(position, normal, color),
		m_Weight0(weight0), m_Weight1(weight1)
		{}

	VertexPosNormCol TransformedVertex;
	VertexPosNormCol OriginalVertex;
	float m_Weight0, m_Weight1;
};