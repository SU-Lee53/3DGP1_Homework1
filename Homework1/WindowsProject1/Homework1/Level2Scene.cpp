#include "stdafx.h"
#include "Level2Scene.h"
#include "FirstPersonPlayer.h"
#include "ExplosiveObject.h"

using namespace std;

void Level2Scene::BuildObjects()
{
	shared_ptr<Mesh> pTankMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pTankMesh, L"../Tank.obj");

	m_pObjects.resize(1);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RGB(0, 150, 0));
	m_pObjects[0]->SetMesh(pTankMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 5.f);
	m_pObjects[0]->GetTransform()->SetRotation(-90.f, 180.f, 0.f);

	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);
}

void Level2Scene::ReleaseObjects()
{
	Scene::ReleaseObjects();
}

void Level2Scene::Update(float fTimeElapsed)
{
	ProcessMouseInput();
	ProcessKeyboardInput();

	if (m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);

	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) { p->Update(fTimeElapsed); });
	}

}

void Level2Scene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"Level2Scene" };
	::TextOut(hDCFrameBuffer, 0, 0, wstrOutText.c_str(), wstrOutText.length());

	Scene::Render(hDCFrameBuffer);
}

void Level2Scene::ProcessMouseInput()
{
	m_pPlayer->ProcessMouseInput();
}

void Level2Scene::ProcessKeyboardInput()
{
	m_pPlayer->ProcessKeyboardInput();
}
