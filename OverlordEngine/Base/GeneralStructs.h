#pragma once
#include "GameTime.h"
#include "../Components/CameraComponent.h"
#include "InputManager.h"
#include "MaterialManager.h"

class CameraComponent;
class ShadowMapRenderer;

struct GameSettings
{
public:
	GameSettings():
		Window(WindowSettings()),
		DirectX(DirectXSettings())
	{}

#pragma region
	struct WindowSettings
	{
		WindowSettings():
			Width(1240),
			Height(720),
			AspectRatio(Width/(float)Height),
			Title(L"Overlord Engine (DX11)"),
			WindowHandle(nullptr)
		{
		}

		int Width;
		int Height;
		float AspectRatio;
		wstring Title;
		HWND WindowHandle;
	}Window;
#pragma endregion WINDOW_SETTINGS

#pragma region
	struct DirectXSettings
	{
		DirectXSettings():
			pAdapter(nullptr),
			pOutput(nullptr),
			m_RefreshColor(Colors::CornflowerBlue)
		{}

		IDXGIAdapter* pAdapter;
		IDXGIOutput* pOutput;
		XMFLOAT4 m_RefreshColor;
	}DirectX;
#pragma endregion DIRECTX_SETTINGS
};

struct GameContext
{
public:
	GameTime* pGameTime;
	CameraComponent* pCamera;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	InputManager* pInput;
	MaterialManager* pMaterialManager;
	ShadowMapRenderer* pShadowMapper;
};