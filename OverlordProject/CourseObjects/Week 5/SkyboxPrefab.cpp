#include "stdafx.h"
#include "SkyboxPrefab.h"
#include <Components/Components.h>
#include <Materials/SkyboxMaterial.h>
#include <Materials/UberMaterial.h>

SkyboxPrefab::SkyboxPrefab()
{
}


SkyboxPrefab::~SkyboxPrefab()
{
}

void SkyboxPrefab::Initialize(const GameContext& gameContext)
{
	ModelComponent* mesh = new ModelComponent(L"./Resources/Meshes/box.ovm");
	SkyboxMaterial* pMaterial = new SkyboxMaterial();
	pMaterial->SetTexture(L"./Resources/Textures/skybox.dds");
	gameContext.pMaterialManager->AddMaterial(pMaterial, 0);
	mesh->SetMaterial(0);
	AddComponent(mesh);
}