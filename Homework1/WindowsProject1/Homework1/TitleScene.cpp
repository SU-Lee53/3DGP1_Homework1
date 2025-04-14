#include "stdafx.h"
#include "TitleScene.h"
#include "FirstPersonPlayer.h"

using namespace std;

void TitleScene::BuildObjects()
{
	shared_ptr<Mesh> pNameMesh = make_shared<Mesh>();
	MeshHelper::CreateMeshFromOBJFiles(pNameMesh, L"../name.obj");

	m_pObjects.resize(1);
	m_pObjects[0] = make_shared<GameObject>();
	m_pObjects[0]->SetColor(RGB(0, 0, 0));
	m_pObjects[0]->SetMesh(pNameMesh);
	m_pObjects[0]->GetTransform()->SetPosition(0.f, 0.f, 300.f);
	m_pObjects[0]->GetTransform()->SetRotation(90.f, 0.f, 0.f);

	m_pPlayer = make_shared<FirstPersonPlayer>();
	m_pPlayer->Initialize();
	m_pPlayer->GetTransform()->SetPosition(0.f, 0.f, 0.f);

}

void TitleScene::ReleaseObjects()
{
	Scene::ReleaseObjects();
}

void TitleScene::Update(float fTimeElapsed)
{
	ProcessMouseInput();
	ProcessKeyboardInput();

	if (m_pPlayer)
		m_pPlayer->Update(fTimeElapsed);

	if (!m_pObjects.empty()) {
		std::for_each(m_pObjects.begin(), m_pObjects.end(), [fTimeElapsed](std::shared_ptr<GameObject>& p) {p->Update(fTimeElapsed); });
	}
}

void TitleScene::Render(HDC hDCFrameBuffer)
{
	std::wstring wstrOutText{ L"TitleScene" };
	::TextOut(hDCFrameBuffer, 0, 0, wstrOutText.c_str(), wstrOutText.length());

	Scene::Render(hDCFrameBuffer);
}

void TitleScene::ProcessMouseInput()
{
	m_pPlayer->ProcessMouseInput();
}

void TitleScene::ProcessKeyboardInput()
{
	m_pPlayer->ProcessKeyboardInput();
}
