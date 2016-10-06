//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MenuSceneFlex.h"
#include "Scenegraph\GameObject.h"
#include "Scenegraph\SceneManager.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics\SpriteRenderer.h"
#include "Graphics\TextRenderer.h"

#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components\SpriteComponent.h"
#include "Components\TransformComponent.h"
#include "Graphics\MeshFilter.h"

#include "Components/Components.h"
#include <Materials\ColorMaterial.h>
#include <Prefabs/CubePrefab.h>
#include <Physx/PhysxManager.h>

#include "..\CourseObjects\Exam\ButtonClass\WorldButton.h"
//#include "..\..\ButtonClass\WorldButton.h"
#include "../OverlordEngine/Components/ParticleEmitterComponent.h"

#define FPS_COUNTER 1
const int maxParticles = 1000;
const int maxDiffuse = 0;
struct FlexParticles
{
	float x = 0;
	float y = 0;
	float z = 0;
	float m = 0;
};
struct velocity
{
	float x = 0;
	float y = 0;
	float z = 0;
};

MenuSceneFlex::MenuSceneFlex(void) :
	GameScene(L"MenuSceneFlex")
{
}


MenuSceneFlex::~MenuSceneFlex(void)
{
	flexDestroySolver(m_pSolver);
	flexShutdown();
}

void MenuSceneFlex::Initialize(const GameContext& gameContext)
{
	m_pFlexMem = new FlexMemory();
	UNREFERENCED_PARAMETER(gameContext);
	flexInit();
	m_pSolver = flexCreateSolver(maxParticles, maxDiffuse);

	vector<FlexParticles> particles;
	vector<velocity> velocities;
	for (int i = 0; i < 100; ++i)
	{
		FlexParticles p;
		p.x = rand() % 50;
		p.y = rand() % 50;
		p.z = rand() % 50;
		p.m = 1;

		particles.push_back(p);

		velocity v;
		v.x = rand() % 10;
		v.y = rand() % 10;
		v.z = rand() % 10;

		velocities.push_back(v);
	}
	int fluidPhase = flexMakePhase(0, eFlexPhaseSelfCollide | eFlexPhaseFluid);
	int* ind = new int[100];
	for (int i = 0; i < 100; ++i)
	{
		ind[i] = i;
	}

	flexSetActive(m_pSolver, ind, 100, *m_pFlexMem);

	flexSetParticles(m_pSolver, reinterpret_cast<float*>(particles.data()), 100, *m_pFlexMem);
	flexSetVelocities(m_pSolver, reinterpret_cast<float*>(velocities.data()), 100, *m_pFlexMem);
	flexSetPhases(m_pSolver, &fluidPhase, 100, *m_pFlexMem);
}

void MenuSceneFlex::Update(const GameContext& gameContext)
{
	vector<FlexParticles> particlesss;
	particlesss.resize(100);
	UNREFERENCED_PARAMETER(gameContext);
	flexUpdateSolver(m_pSolver, gameContext.pGameTime->GetElapsed(), 1, nullptr);
	flexGetParticles(m_pSolver, reinterpret_cast<float*>(particlesss.data()), particlesss.size(), *m_pFlexMem);
	cout.setf(ios::fixed);
	//for (int i = 0; i < 1; ++i)
	{
		std::cout << "Position " << particlesss[0].x << " " << particlesss[0].y << " " << particlesss[0].z << std::endl;
	}
}

void MenuSceneFlex::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}