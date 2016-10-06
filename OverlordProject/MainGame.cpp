//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MainGame.h"
#include "Base\GeneralStructs.h"
#include "Scenegraph\SceneManager.h"
#include "Physx\PhysxProxy.h"
#include "Diagnostics\DebugRenderer.h"

#define FLEX

#ifdef W1
#include "CourseObjects/Week 1/TestScene.h"
#include "CourseObjects/Week 1/ComponentTestScene.h"
#endif

#ifdef W2
#include "CourseObjects/Week 2/ModelTestScene.h"
#include "CourseObjects/Week 2/CharacterTest.h"
#endif

#ifdef W3
#include "CourseObjects/Week 3/SpikeyScene.h"
#include "CourseObjects/Week 3/SpriteTestScene.h"
#include "CourseObjects/Week 3/UberShaderTestScene.h"
#endif

#ifdef W4
#include "CourseObjects/Week 4/RaycastTestScene.h"
#endif

#ifdef W5
#include "CourseObjects/Week 5/TerrainScene.h"
#include "CourseObjects/Week 5/SkyboxScene.h"
#endif

#ifdef W6
#include "CourseObjects/Week 6/SoftwareSkinningScene_1.h"
#include "CourseObjects/Week 6/SoftwareSkinningScene_2.h"
#include "CourseObjects/Week 6/SoftwareSkinningScene_3.h"
#endif W6

#ifdef W7
#include "CourseObjects/Week 7/HardwareSkinningScene.h"
#endif

#ifdef W8
#include "CourseObjects/Week 8/PostProcessingScene.h"
#endif

#ifdef W9
#include "CourseObjects/Week 9/ShadowMappingScene.h"
#endif

#ifdef EXAM
#include "CourseObjects/Exam/MenuScene.h"
#include "CourseObjects/Exam/ExamScene.h"
#endif

#ifdef GEOM
#include "CourseObjects/Geom/GeomScene.h"
#endif

#ifdef FLEX
#include "FlexObjects\MenuSceneFlex.h"
#endif

MainGame::MainGame(void)
{
}


MainGame::~MainGame(void)
{
}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
}

void MainGame::Initialize()
{

#ifdef W1
	SceneManager::GetInstance()->AddGameScene(new TestScene());
	SceneManager::GetInstance()->AddGameScene(new ComponentTestScene());
#endif

#ifdef W2
	SceneManager::GetInstance()->AddGameScene(new ModelTestScene());
	SceneManager::GetInstance()->AddGameScene(new CharacterTest());

	SceneManager::GetInstance()->SetActiveGameScene(L"CharacterTest");
#endif

#ifdef W3
	SceneManager::GetInstance()->AddGameScene(new SpikeyScene());
	SceneManager::GetInstance()->AddGameScene(new SpriteTestScene());
	SceneManager::GetInstance()->AddGameScene(new UberShaderTestScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"SpriteTestScene");
#endif

#ifdef W4
	SceneManager::GetInstance()->AddGameScene(new RaycastTestScene());
#endif

#ifdef W5
	SceneManager::GetInstance()->AddGameScene(new TerrainScene());
	SceneManager::GetInstance()->AddGameScene(new SkyboxScene());
#endif

#ifdef W6
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_1());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_2());
	SceneManager::GetInstance()->AddGameScene(new SoftwareSkinningScene_3());
	SceneManager::GetInstance()->SetActiveGameScene(L"SoftwareSkinningScene_3");
#endif

#ifdef W7
	SceneManager::GetInstance()->AddGameScene(new HardwareSkinningScene());
#endif

#ifdef W8
	SceneManager::GetInstance()->AddGameScene(new PostProcessingScene());
#endif

#ifdef W9
	SceneManager::GetInstance()->AddGameScene(new ShadowMappingScene());
#endif

#ifdef EXAM
	SceneManager::GetInstance()->AddGameScene(new MenuScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"MenuScene");
#endif
#ifdef GEOM
	SceneManager::GetInstance()->AddGameScene(new GeomScene());
	SceneManager::GetInstance()->SetActiveGameScene(L"GeomScene");
#endif
#ifdef FLEX
	SceneManager::GetInstance()->AddGameScene(new MenuSceneFlex());
	SceneManager::GetInstance()->SetActiveGameScene(L"MenuSceneFlex");
#endif

}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
			else if (wParam == VK_F4)
			{
				DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
			else if (wParam == VK_F6)
			{
				auto activeScene = SceneManager::GetInstance()->GetActiveScene();
				activeScene->GetPhysxProxy()->NextPhysXFrame();
			}
		}
	}

	return -1;
}
