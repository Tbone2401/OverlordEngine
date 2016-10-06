#pragma once
#include "Scenegraph/GameScene.h"
#include <Components/Components.h>

class BoneObject;
struct SkinnedVertex;

class SoftwareSkinningScene_3 : public GameScene
{
public:
	SoftwareSkinningScene_3(void);
	virtual ~SoftwareSkinningScene_3(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	void CreateMesh(float length);
	MeshDrawComponent* m_pMeshDrawer = nullptr;
	vector<SkinnedVertex> m_SkinnedVertices;
	BoneObject* m_pBone0, *m_pBone1;
	float m_BoneRotation;
	int m_RotationSign;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SoftwareSkinningScene_3(const SoftwareSkinningScene_3 &obj);
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& obj);
};

