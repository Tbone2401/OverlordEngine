#pragma once
#include "Scenegraph/GameObject.h"

class ColliderComponent;
class RigidBodyComponent;
class ColorMaterial;

class WorldButton : public GameObject
{
public:
	WorldButton(XMFLOAT3 pos, wstring name, XMFLOAT3 rot = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	void SetActiveColor(const XMFLOAT3 col);
	void SetInactiveColor(const XMFLOAT3 col);
	void SetActive(bool active);
	bool GetActive();
	void MoveAndDisable();
	void MoveAndEnable();

	GameObject * ReturnGameObj();
	~WorldButton();
private:
	XMFLOAT3 m_Pos, m_Rot;
	std::wstring m_Name;
	GameObject *m_pButton;
	ColorMaterial * m_pMat;
	XMFLOAT3 m_ActiveCol, m_InactiveCol;
	UINT m_PrivateIndex = 0;
	
	static int m_sIndex;
	bool m_IsActive = true;

	WorldButton(const WorldButton& t);
	WorldButton& operator=(const WorldButton& t);
};

